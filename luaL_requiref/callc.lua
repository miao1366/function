dylib = require "dylib_test"  --my_math 是一个dll或so
local c = dylib.add(1,2)
print(c)
dylib.start()
dylib = require "dylib2.test"
local c = dylib.add(1,2)
print(c)