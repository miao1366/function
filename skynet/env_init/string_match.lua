local str = "08-10:10"
local mat1 ,mat2, mat3, mat4 = string.match(str, "(%d+)-(%d+)?(%d+)")
print(mat1 ,mat2, mat3, mat4)
local mat11 ,mat12, mat13, mat14 = string.match(str, "(%d+)-(%d+):(%d+)")
print(mat11, mat12, mat13, mat14)

local mat21 ,mat22, mat23, mat24 = string.match(str, "%d+-%d+:%d+")
print(mat21, mat22, mat23, mat24)