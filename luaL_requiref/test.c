#include "lua.h"
#include "lauxlib.h"
 
#include <stdio.h>
 
int
dylib_add(lua_State* L) {
    int a = lua_tonumber(L,1);
    int b = lua_tonumber(L,2);
    int c = a+b;
    lua_pop(L,2);
    lua_pushnumber(L,c);
    return 1;
}
 
int dylib2_test(lua_State* L);
int start(lua_State* L) {
    luaL_requiref(L, "dylib2.test", dylib2_test, 0);
    return 0;
}
 
int
luaopen_dylib_test(lua_State* L) {
    luaL_Reg l[] = {
       { "add", dylib_add },
       { "start", start },
       { NULL, NULL },
    };
    luaL_checkversion(L);
    luaL_newlib(L,l);
 
    return 1;
}
 

 int
dylib_add2(lua_State* L) {
    int a = lua_tonumber(L,1);
    int b = lua_tonumber(L,2);
    int c = a+b+b;
    lua_pop(L,2);
    lua_pushnumber(L,c);
    return 1;
}


int
dylib2_test(lua_State* L) {
    luaL_Reg l[] = {
       { "add", dylib_add2},
       { NULL, NULL },
    };
    luaL_checkversion(L);
    luaL_newlib(L,l);
 
    return 1;
}

// gcc -I./ -shared -fpic -o my_math.so test.c 
// lua ./callc.lua