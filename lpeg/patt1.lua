-- 返回一个pattern。如果输入的字符串被patt匹配到，这个pattern才会被匹配。
-- 这个pattern被叫做，and谓语，等价于原生PEG中的&patt
-- 这个pattern从不产生任何捕获

local lpeg = require "lpeg"
local match = lpeg.match
local P = lpeg.P
local C = lpeg.C
local V= lpeg.V

local p1 = P"aa"
local p2 = #p1

print(match(p1, "a"))  ---> nil
print(match(p2, "a"))  ---> nil

print(match(p1, "aaaa"))  ---> 3
print(match(p2, "aaaa"))  ---> 1

-- 进阶
local pat = P{
    "S";
    S1 = C("abc") + 3,
    S = #V("S1")     -- 这里捕获到了结果，但是#必须忽略这个结果
}

print(match(pat, "abc"))  ---> 1

-- 调整长度(fixed length)
local m1 = #("a" * (P"bd" + "cd"))
local m2 = C(m1 * 2)
local m3 = C(m1 * 3)
local m4 = C(m1 * 4)
print(match(m1, "acd"))  ---> 1
print(match(m2, "acd"))  ---> ac
print(match(m3, "acd"))  ---> acd
print(match(m4, "acd"))  ---> nil

-- 解析：
-- 1. #patt后面跟着 * number时，是对捕获到的值的长度进行调整
-- 2. number的值不能超过捕获到的值的长度