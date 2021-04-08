local lpeg = require "lpeg"
local P = lpeg.P
local match = lpeg.match
local R = lpeg.R
local C = lpeg.C
local S = lpeg.S
local Ct = lpeg.Ct
local Cs= lpeg.Cs

-- local sp = P(' ')^0  -- zero or more spaces (like '%s*')
-- function Space(pat) -- surrond a pattern with optional space
--     return sp * pat * sp
-- end

-- function List(pat)
--     pat = Space(pat)
--     return C(pat) * (',' * C(pat))^0
-- end
-- -- 所以，一个列表的识别码可以这样写 (按平常的写法):
-- local idenchar = R('AZ','az') + P'_'
-- local iden = idenchar * (idenchar+R'09')^0
-- local list = match(Ct(List(iden)), ' hello  ,  dolly , _x, s23')
-- print(#list[1], #list[2], #list[3], #list[4])
-- print(list[1], list[2], list[3], list[4])

local sp = P(' ')^0  -- zero or more spaces (like '%s*')
function Space(pat) -- surrond a pattern with optional space
    return sp * C(pat) * sp
end

function List(pat)
    pat = Space(pat)
    return pat * (',' * pat)^0
end
-- 所以，一个列表的识别码可以这样写 (按平常的写法):
local idenchar = R('AZ','az') + P'_'
local iden = idenchar * (idenchar+R'09')^0
local list = match(Ct(List(iden)), ' hello  ,  dolly , _x, s23')
print(#list[1], #list[2], #list[3], #list[4])
print(list[1], list[2], list[3], list[4])