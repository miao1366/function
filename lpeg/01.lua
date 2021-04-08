-- https://www.cnblogs.com/cnxkey/articles/5377061.html
local lpeg = require "lpeg"
local P = lpeg.P
local match = lpeg.match
local R = lpeg.R
local C = lpeg.C
local S = lpeg.S
local Ct = lpeg.Ct
local Cs= lpeg.Cs

local Q = P'"'
local str = Q * (P(1) - Q)^0 * Q
print(C(str):match '"hello"')

-- 或者，你需要的是捕获字符串里的内容，不包括引号。在这种情况下，只要使用1来代替P(1)就好，而且这是一个比较常见的表达'所有不是P的x'模式：

local str2 = Q * C((1 - Q)^0) * Q
print(str2:match '"hello"')
-- 这种模式可以简单的普及到其他情形里去；结束符（结束模式）不一定是最终的模式：
function Extract_quote(openp,endp)
    openp = P(openp)
    endp = P(endp) - openp
    local upto_endp = (1 - endp)^1
    return openp * C(upto_endp) * endp
end
print(Extract_quote('(',')'):match '(and more)')
print(Extract_quote('[[',']]'):match '[[long string]]')
-- 现在，我们来尝试下把Markdown 代码块 (反斜杠包裹的文本) 转换为 Lua wiki (双花括号包裹的文本)。 最显浅的办法是抽取出字符串并连接成最终结果，
-- 但是这看起来很蠢，而且极大的限制了我们的选项（下文将解析这点）。
function Subst(openp,repl,endp)
    openp = P(openp)
    endp = endp and P(endp) or openp
    local upto_endp = (1 - endp)^1
    return openp * C(upto_endp)/repl * endp
end
print(Subst('`','{{%1}}'):match '`code`')
print(Subst('_',"''%1''"):match '_italics_')

-- 我们之前曾经用过 / 操作符，使用 tonumber 函数来转换number。 它也有跟 string.gsub极为相似的格式，比如 %n 表示第n个匹配。
-- 这个操作可以被表达为：
print(string.gsub('_italics_','^_([^_]+)_',"''%1''"))

-- lpeg.Cs 是一个 替换捕获，并提供了一个更一般化的全局字符串匹配模块。 在lpeg手册里， string.gsub有这样一个等价的替换：
-- 这里的C 捕获了整个匹配，每一个 '/' 增加了一个新的捕获，捕获的值用替换字符串代替。
-- 而使用Cs, 所有匹配都被捕获，最后组成一个字符串。其中一些捕获会被 '/'所修改，所以我们能够获得替换过的字符串。
function Gsub (s, patt, repl)
    patt = P(patt)
    local p = Cs ((patt / repl + 1)^0)
    return p:match(s)
end
print(Gsub('hello dog, dog!','dog','cat'))
local p = C((P'dog'/'cat' + 1)^0)
print(p:match 'hello dog, dog!')

-- 这是最终用来转换Markdown文档到Lua维基格式的代码:
-- local lpeg = require 'lpeg'
-- local P,S,C,Cs,Cg = lpeg.P,lpeg.S,lpeg.C,lpeg.Cs,lpeg.Cg
-- local test = [[
-- ## A title
-- here _we go_ and `a:bonzo()`:
--     one line
--     two line
--     three line
-- and `more_or_less_something`
-- [A reference](http://bonzo.dog)
-- > quoted
-- > lines
-- ]]
-- function subst(openp,repl,endp)
--     openp = P(openp)  -- make sure it's a pattern
--     endp = endp and P(endp) or openp
--     -- pattern is 'bracket followed by any number of non-bracket followed by bracket'
--     local contents = C((1 - endp)^1)
--     local patt = openp * contents * endp    
--     if repl then patt = patt/repl end
--     return patt
-- end
-- function empty(p)
--     return C(p)/''
-- end
-- lf = P'\n'
-- rest_of_line = C((1 - lf)^1)
-- rest_of_line_nl = C((1 - lf)^0*lf)
-- -- indented code block
-- indent = P'\t' + P'    '
-- indented = empty(indent)*rest_of_line_nl
-- -- which we'll assume are Lua code
-- block = Cs(indented^1)/'    [[[!Lua\n%1]]]\n'
-- -- use > to get simple quoted block
-- quoted_line = empty('> ')*rest_of_line_nl 
-- quote = Cs (quoted_line^1)/"[[[\n%1]]]\n"
-- code = subst('`','{{%1}}')
-- italic = subst('_',"''%1''")
-- bold = subst('**',"'''%1'''")
-- rest_of_line = C((1 - lf)^1)
-- title1 = P'##' * rest_of_line/'=== %1 ==='
-- title2 = P'###' * rest_of_line/'== %1 =='
-- url = (subst('[',nil,']')*subst('(',nil,')'))/'[%2 %1]'
-- item = block + title1 + title2 + code + italic + bold + quote + url + 1
-- text = Cs(item^1)
-- if arg[1] then
--     local f = io.open(arg[1])
--     test = f:read '*a'
--     f:close()
-- end
-- print(text:match(test))

-- 组捕获有两种： 命名式及匿名式。
-- Cg(C"baz" * C"qux", "name") -- 命名组。
-- Cg(C"foo" * C"bar")         -- 匿名组。

