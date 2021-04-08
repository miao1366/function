-- 创建一个简单的捕获，捕获匹配到patt的子字符串。
-- 捕获的值是一个字符串。
-- 如果patt有其他捕获，他们的值将在这个值之后返回
local lpeg = require "lpeg"
local match = lpeg.match
local P = lpeg.P
local R = lpeg.R
local S = lpeg.S
local C = lpeg.C
local V = lpeg.V
local p1 = P"aa" ^ 3
local cp1 = C(p1)
print(match(cp1, "aaaaaa"))  ---> aaaaaa
print(match(cp1, "aa"))  ---> nil
local r1 = R"09" ^ 0
local cr1 = C(r1)
print(match(cr1, "1234aaaa"))  ---> 1234
print(#match(cr1, "aaaa"))  ---> 0  注：这里返回的是空字符串，跟lpeg.P不同
local s1 = S"+-*/" ^ 0
local cs1 = C(s1)
print(match(cs1, "++++aaaa"))  ---> ++++
print(#match(cs1, "aaaa"))  ---> 0  注：这里返回的是空字符串，跟lpeg.P不同 
-- 进阶
local m1 = { [1] = C(C(1) * V(1) + -1) }
print(match(m1, "abc"))  ---> "abc" "a" "bc" "c" "c" ""  注：这里的值添加了引号，最后一个是空字符串

-- 解析：
-- 过程：
-- ① (1)匹配到a，然后匹配V(1)，由于C(1)，产生结果a
-- ② (1)匹配到b，然后匹配V(1)，由于C(1)，产生结果b
-- ③ (1)匹配到c，然后匹配V(1)，由于C(1)，产生结果c
-- ④ (1)C(1) * V(1)没有匹配到，匹配到-1，返回到③，由于-1，产生""
-- ③ (2)匹配到C(1) * V(1)，返回到②，由于C(1) * V(1)，产生c和④的组合，就是c
-- ② (2)匹配到C(1) * V(1)，返回到①，由于C(1) * V(1)，产生b和③的组合，就是bc
-- ① (2)匹配到C(1) * V(1)，返回结果，由于C(1) * V(1)，产生a和②的组合，就是abc
-- 这个过程是个递归的过程，看似有点复杂，根据结果，仔细分析一下就能得出结论啦