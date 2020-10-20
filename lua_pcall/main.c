#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdio.h>


static int 
traceback (lua_State *L) {
    printf("traceback error ");
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

static int 
traceback1 (lua_State *L) {
    printf("traceback1 error ");
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

static int 
traceback2 (lua_State *L) {
    printf("traceback2 error ");
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}
int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, traceback);
    lua_pushcfunction(L, traceback1);
    lua_pushcfunction(L, traceback2);

    int r = luaL_loadfile(L, "./test.lua");      
	if (r != LUA_OK) {
        printf("loadfile error ");
		return 1;
	}
    lua_pushboolean(L, 1);
	lua_pushlstring(L, "hello", 5);
	r = lua_pcall(L,1,0,3);             
	if (r != LUA_OK) {
        printf("pcall   error");
		
	}
    printf("lua pop %d", lua_gettop(L));
    return 1;
} 

//gcc main.c -L./ -llua -lm -ldl