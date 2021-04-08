local lpeg = require "lpeg"

local match = lpeg.match
local S = lpeg.S

local s1 = S"+-*/"
print(match(s1, "+"))  ---> 2
print(match(s1, "-"))  ---> 2
print(match(s1, "*"))  ---> 2
print(match(s1, "/"))  ---> 2

-- 特别注意
local s2 = S""
local str = "aaa"
print(match(s2, str))  ---> nil
-- 注：当S的参数为空字符串的时候，不管str是什么字符串，总返回nil