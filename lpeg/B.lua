local lpeg = require "lpeg"
local match = lpeg.match
local B = lpeg.B
local m1 = B"a"
local m2 = 1 * m1
local m3 = 1 * B"b"

print(match(1, "a"))       ---> 2
print(match(1 * m1, "a"))  ---> 2
print(match(m1, "a"))      ---> nil
print(match(m2, "a"))      ---> 2
print(match(m3, "a"))      ---> nil
-- 注：这里的m3和m2的区别就是一个是a，一个是b，但得到的结果不一样

local m4 = B(1)
local m5 = 1 * m4
print(match(m4, "a"))  ---> nil
print(match(m5, "a"))  ---> 2
print(match(-m4, "a"))  ---> nil
local m6 = B(250)
local m7 = 250 * m6
print(match(m6, string.rep("a", 250)))  ---> nil
print(match(m7, string.rep("a", 250)))  ---> 251
--以上实例为在位置5之前匹配str， 匹配成功返回当前 index 5 ， 若改为从位置6开始匹配，则匹配失败，返回nil。
local b = lpeg.B("str")
print (lpeg.match(b, "istrim", 5))  -->5
print (lpeg.match(b, "istrim", 6))  -->nil
