local lpeg = require "lpeg"

local match = lpeg.match
local R = lpeg.R

local r1 = R"09" -- 匹配任意数字
print(match(r1, "12345"))  ---> 2 注：只要匹配到1个字符就返回

local r2 = R("az", "AZ")  -- 可以传多个2个字符组成的字符串
print(match(r2, "11abcd"))  ---> nil
print(match(r2, "Abcd"))  ---> 2

-- 特别注意
local r3 = R()
local str = "aaa"
print(match(r3, str))  ---> nil
-- 当R没有参数时，不管str为什么字符串，总返回nil