local lpeg = require "lpeg"
-- matches a numeral and captures its numerical value
local number = lpeg.R"09"^1 / tonumber
 
-- matches a list of numbers, capturing their values
local list = number * ("," * number)^0
 
-- auxiliary function to add two numbers
-- 创建一个折叠的捕获，假设patt有n个返回值,C1,C2,C3,那么Cf返回 f(f( f(C1),C2), C3)。
function add (acc, newvalue) return acc + newvalue end
 
-- folds the list of numbers adding them
local sum = lpeg.Cf(list, add)
 
-- example of use
print(sum:match("10,30,43"))  --> 83