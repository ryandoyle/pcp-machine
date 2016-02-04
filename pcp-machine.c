#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <pcp/pmapi.h>
#include "pcp_ez.h"

#define LUA_EXEC_FAILED_EXIT_CODE 125

static char *default_host = "local:";
static char *host_context;

const static char usage_text[] = "Usage: pcp-machine [OPTIONS] SCRIPT\n"
"LUA scripting engine for Performance Co-Pilot\n"
"\n"
"Options:\n"
"  -H HOST          Run LUA scripts in the context of HOST. Defaults to localhost\n"
"                   if not specified\n"
"  -h               This help screen\n"
"\n"
"For more information see: https://github.com/ryandoyle/pcp-machine\n\n"
;

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
    const char *host;

    /* Check if we have a hostname pushed onto the stack */
    if(lua_gettop(L) == 2) {
        if (lua_type(L, 2) == LUA_TTABLE) {
            lua_pushstring(L, "host");
            lua_gettable(L, 2);
            host = luaL_checkstring(L, -1);
            lua_pop(L, 1);
        } else {
            luaL_error(L, "Invalid argument: %s", luaL_checkstring(L, 2));
        }
    } else {
        host = host_context;
    }

    int error;
    pcp_ez_query_t *pcp_ez;
    if((error = pcp_ez_query_for(metric, host, &pcp_ez)) < 0) {
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

typedef struct {
    char *host_context;
    char *script_location;
    int has_parsing_error;
} PcpMachineOptions;

static PcpMachineOptions parse_options(int argc, char **argv) {
    int c;
    PcpMachineOptions options = {
            .host_context = default_host,
            .script_location = NULL,
            .has_parsing_error = 1,
    };

    if(argc == 1) {
        fprintf(stderr, usage_text);
        return options;
    }

    while((c = getopt(argc, argv, "hH:")) != -1) {
        switch (c) {
            case 'H':
                options.host_context = optarg;
                break;
            case 'h':
            default:
                fprintf(stderr, usage_text);
                return options;
        }
    }

    options.script_location = argv[argc-1];

    /* If we got here, options have parsed as they should */
    options.has_parsing_error = 0;
    return options;
}

static int evaluate_lua(char *script_location) {
    int error;
    int return_code = 0;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, l_metric);
    lua_setglobal(L, "metric");

    error = luaL_loadfile(L, script_location) || lua_pcall(L,0,1,0);

    if(error) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return_code = LUA_EXEC_FAILED_EXIT_CODE;
    } else {
        return_code = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    lua_close(L);
    return return_code;
}

int main(int argc, char **argv) {

    PcpMachineOptions opts = parse_options(argc, argv);
    if(opts.has_parsing_error) {
        exit(1);
    }

    host_context = opts.host_context;

    return evaluate_lua(opts.script_location);
}
