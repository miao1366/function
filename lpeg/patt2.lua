-- patt
-- 返回一个pattern，只有patt没有被匹配到的时候始终返回1，否则返回nil
-- 这个pattern从不产生任何捕获
-- 这个pattern等价于patt的补集
-- 个人理解：这里始终返回1是根据结果猜的，如果有其他情况后面再补上

local lpeg = require "lpeg"

local match = lpeg.match
local P = lpeg.P

local p1 = P"aa"
local p2 = - p1

print(match(p1, "aa"))  ---> 3
print(match(p2, "aa"))  ---> nil

print(match(p1, "a"))  ---> nil
print(match(p2, "a"))  ---> 1