local lpeg = require "lpeg"
local match = lpeg.match
local P = lpeg.P
local Cs = lpeg.Cs
local ps = P'ab'

print(match(ps, "a"))    -->nil
print(match(ps, "abcd")) -->3

-- value是非负整数
-- 注：完全匹配任意value个字符
local pn1 = P(3)
-- local pn1 = P(3.0)  -- 这样也是可以的
print(match(pn1, "12"))  ---> nil  注：只有2个字符
print(match(pn1, "abcd"))  ---> 4

-- 注：value不能是小数，但是小数部分为0是可以的
-- 如果是小数，总是返回1
local pn2 = P(3.1)
print(match(pn2, ""))  ---> 1

-- value是负整数
-- 被匹配的字符串长度要小于value
local pn3 = P(-3)  -- 等价于  local pn3 = - P(3)
print(match(pn3, "abcd"))  ---> nil  注：字符串长度>=3了
print(match(pn3, "ab"))  ---> 1
print(match(pn3, ""))  ---> 1

-- value是boolean
local pb1 = P(true)
local str1 = "aaa"  -- str1为任意字符串
print(match(pb1, str1))  ---> 1  注：当value为true时，不管str1是什么字符串，始终返回1

local pb2 = P(false)
local str2 = "aaa"  -- str2为任意字符串
print(match(pb2, str2))  ---> nil  注：当value为false时，不管str2是什么字符串，始终返回nil

-- value是table        看不懂哦
local m1 = Cs((P(1) / { a = 1, b = 2, c = 3 }) ^ 0)
print(match(m1, "abc"))  ---> 123

-- value是function
-- 请跳到match-time capture(lpeg.Cmt)

--要匹配超过1个子串，可以用 ^ 操作符。 在这种情况下，匹配与Lua的 '^a+' 等价- 匹配一个或多个'a':
print(match(P'a'^1,'aaa')) --->4

--要按顺序合并模式（pattern），可以通过 * 操作符。 这等价于 '^ab*' - 'a' 后跟着0个或多个 'b':
print(match(P'a'*P'b'^0,'abbc'))  ---->4
