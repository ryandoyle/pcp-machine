#ifndef PMIE_LUA_PCP_EZ_H
#define PMIE_LUA_PCP_EZ_H 1

#include <pcp/pmapi.h>
#include <lua.h>

typedef struct pcp_ez_result pcp_ez_query_t;

int pcp_ez_get_pm_type(pcp_ez_query_t *query);
pmInDom	pcp_ez_get_indom(pcp_ez_query_t *query);
pmValueSet* pcp_ez_get_pmvalueset(pcp_ez_query_t *query);
char* pcp_ez_get_external_instance_name_for(int inst, pcp_ez_query_t *query);

int pcp_ez_safe_raise_error(pcp_ez_query_t *L, lua_State *error, int i);
void pcp_ez_free(pcp_ez_query_t *query);

int pcp_ez_query_for(const char *metric, const char *host, pcp_ez_query_t **query_ptr);

#endif
