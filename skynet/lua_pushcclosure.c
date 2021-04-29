#include "./lua/lualib.h"
#include "./lua/lauxlib.h"
#include "./lua/lua.h"
#include <stdio.h>

int counter(lua_State* L);

int newCounter(lua_State* L){
   lua_pushnumber(L, 0);
   lua_pushcclosure(L, counter, 1);
   return 1;
}

int counter (lua_State *L) {
    int val = lua_tonumber(L, lua_upvalueindex(1));
    printf("hello world upvalue: %d\n", val);
    // lua_pushnumber(L, ++val); 
    // lua_pushvalue(L, -1);        /* duplicate it */
    // lua_replace(L, lua_upvalueindex(1));
    return 1;
}

int main(void) {
    struct lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    newCounter(L);
    return 0;
}
// gcc ./lua_pushcclosure.c -I./lua -L./lua -llua -ldl -lm

  