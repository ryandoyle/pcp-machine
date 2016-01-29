#include <stdio.h>
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include <pcp/pmapi.h>
#include "pcp_ez.h"

#define LUA_EXEC_FAILED_EXIT_CODE 125

static void push_pmvalue_result(lua_State *L, pmValueSet *pm_value_set, int pm_type, int vlist_index) {
    pmAtomValue pm_atom_value;

    pmExtractValue(pm_value_set->valfmt, &pm_value_set->vlist[vlist_index], pm_type, &pm_atom_value, pm_type);

    switch (pm_type) {
        case PM_TYPE_32:
            lua_pushnumber(L, pm_atom_value.l);
            break;
        case PM_TYPE_U32:
            lua_pushnumber(L, pm_atom_value.ul);
            break;
        case PM_TYPE_64:
            lua_pushnumber(L, pm_atom_value.ll);
            break;
        case PM_TYPE_U64:
            lua_pushnumber(L, pm_atom_value.ull);
            break;
        case PM_TYPE_FLOAT:
            lua_pushnumber(L, pm_atom_value.f);
            break;
        case PM_TYPE_DOUBLE:
            lua_pushnumber(L, pm_atom_value.d);
            break;
        case PM_TYPE_STRING:
            lua_pushstring(L, pm_atom_value.cp);
            free(pm_atom_value.cp);
            break;
        case PM_TYPE_AGGREGATE:
            free(pm_atom_value.vbp);
        default:
            luaL_error(L, "PM_TYPE %d not supported", pm_type);
    }
}

static int l_metric(lua_State *L) {

    const char *metric = luaL_checkstring(L, 1);
    const char *host = "localhost";

    /* Check if we have a hostname pushed */
    if(lua_type(L, 2) == LUA_TSTRING) {
        host = luaL_checkstring(L, 2);
    }

    int error;
    pcp_ez_query_t *pcp_ez;
    if((error = pcp_ez_query_for(metric, host, &pcp_ez))) {
        return pcp_ez_safe_raise_error(pcp_ez, L, error);
    }

    /* Convert the result. Just push the value if its a NULL indom */
    pmValueSet *pm_value_set = pcp_ez_get_pmvalueset(pcp_ez);
    if(pcp_ez_get_indom(pcp_ez) == PM_INDOM_NULL) {
        push_pmvalue_result(L, pm_value_set, pcp_ez_get_pm_type(pcp_ez), 0);
    } else {
        lua_newtable(L);
        int i;
        for(i=0; i < pm_value_set->numval; i++) {
            lua_pushstring(L, pcp_ez_get_external_instance_name_for(pm_value_set->vlist[i].inst, pcp_ez));
            push_pmvalue_result(L, pm_value_set, pcp_ez_get_pm_type(pcp_ez), i);
            lua_settable(L, -3);
        }
    }

    pcp_ez_free(pcp_ez);

    return 1;
}

int main() {
    int error;
    int return_code = 0;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, l_metric);
    lua_setglobal(L, "metric");

    error = luaL_loadfile(L, "/home/ryan/code/pcp-machine/program.lua") || lua_pcall(L,0,1,0);

    if(error) {
        fprintf(stderr, "%s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return_code = LUA_EXEC_FAILED_EXIT_CODE;
    } else {
        return_code = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    lua_close(L);
    return return_code;
}