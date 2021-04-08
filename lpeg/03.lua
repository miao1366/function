local lpeg = require "lpeg"
local P = lpeg.P
local match = lpeg.match
local R = lpeg.R
local C = lpeg.C
local S = lpeg.S
local Ct = lpeg.Ct
local Cs= lpeg.Cs

function Maybe(p)
    return p^-1
end
local digit = R'09' -- anything from '0' to '9'
local digits = digit^1
local mpm = Maybe(S'+-')
local dot = '.'
local exp = S'eE'
local float = mpm * digits * Maybe(dot*digits) * Maybe(exp*mpm*digits)
local sp = P(' ')^0  -- zero or more spaces (like '%s*')
function Space(pat) -- surrond a pattern with optional space
    return sp * C(pat) * sp
end

function List(pat)
    pat = Space(pat)
    return pat * (',' * pat)^0
end

local rlistf = List(float)

local csv = Ct( (Ct(rlistf)+'\n')^1 )
local list = csv:match ' 1, 2.3, 3\n 10, 20 , 30\n'
print(#list[1][1], #list[1][2], #list[1][3], #list[2][1], #list[2][2], #list[2][3])
print(list[1][1], list[1][2], list[1][3], list[2][1], list[2][2], list[2][3])
