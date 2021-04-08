local lpeg = require "lpeg"
local P = lpeg.P
local match = lpeg.match
local R = lpeg.R
local C = lpeg.C
local S = lpeg.S
local Ct = lpeg.Ct
local Cs= lpeg.Cs

-- lpeg.Cs 是一个 替换捕获，并提供了一个更一般化的全局字符串匹配模块。
-- 这里的C 捕获了整个匹配，每一个 '/' 增加了一个新的捕获，捕获的值用替换字符串代替。
-- 而使用Cs, 所有匹配都被捕获，最后组成一个字符串。其中一些捕获会被 '/'所修改，所以我们能够获得替换过的字符串。
function Gsub (s, patt, repl)
    patt = P(patt)
    local p = Cs ((patt / repl + 1)^0)
    return p:match(s)
end
print(Gsub('hello dog, dog!','dog','cat'))
-- local p = C((P'dog'/'cat' + 1)^0)
-- local p = C((P('dog'/'cat') + 1)^0)
local p = C((P('dog')/'cat' + 1)^0)
print(p:match 'hello dog, dog!')

-- local p1 = (R('az' ,'AZ') + S(' _!'))^0 * C((P('dog')/'cat')^1) + (R('az' , 'AZ') + S(' _!'))^0
-- print(p1:match 'hello dog, dog!')
-- print(Ct(match(p1, 'hello dog, dog!')))
-- local list =Ct(match(p1, 'hello dog, dog!'))
-- print(list[1], list[2])

-- local sp = (R('az' ,'AZ') + S(' _!'))^0  -- zero or more spaces (like '%s*')
-- function Space(pat) -- surrond a pattern with optional space
--     return sp * C(pat) * sp
-- end


-- function List(pat)
--     pat = Space(pat)
--     return pat * ( pat)^0
-- end

-- list = match(Ct(List(P("dog"))), 'hello dog, dog!')
-- -- print(#list[1], #list[2], #list[3], #list[4])
-- print(list[1], list[2], list[3], list[4])