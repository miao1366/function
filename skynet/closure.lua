local function test()
    local i=0
    return function()--//尾调用
        i = i + 1
        return i
    end
end
local c1 = test()
local c2 = test()   --//c1,c2是建立在同一个函数，同一个局部变量的不同实例上面的两个不同的闭包
                    --//闭包中的upvalue各自独立，调用一次test（）就会产生一个新的闭包
print(c1())         -->1
print(c1())         -->2//重复调用时每一个调用都会记住上一次调用后的值，就是说i=1了已经
print(c2())         -->1//闭包不同所以upvalue不同    
print(c2())         -->2