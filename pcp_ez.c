#include <pcp/pmapi.h>
#include <lua.h>
#include <lauxlib.h>

#ifndef PM_MAXERRMSGLEN
#define PM_MAXERRMSGLEN		128
#endif

typedef struct pcp_ez_result {
    pmID pmid;
    pmDesc pm_desc;
    pmResult *pm_result;
    int *instlist;
    char **namelist;
    int pcp_context;
} pcp_ez_query_t;

static pcp_ez_query_t * pcp_ez_new() {
    pcp_ez_query_t * result;
    result = (pcp_ez_query_t *)malloc(sizeof(pcp_ez_query_t));
    if(result == NULL) {
        return NULL;
    }
    result->instlist = NULL;
    result->namelist = NULL;
    result->pm_result = NULL;
    return result;
};

static int pcp_ez_create_context(pcp_ez_query_t *query, const char *host) {
    int pcp_context_or_error = pmNewContext(PM_CONTEXT_HOST, host);
    if(pcp_context_or_error < 0) {
        return pcp_context_or_error;
    }
    query->pcp_context = pcp_context_or_error;
    return 0;
}


static int pcp_ez_lookup_metric(pcp_ez_query_t *query, const char *metric) {
    int error;
    pmID *pmid_list;
    const char **metric_list;

    pmid_list = malloc(sizeof(pmID));
    metric_list = malloc(sizeof(char*));
    metric_list[0] = metric;

    if((error = pmLookupName(1, (char**)metric_list, pmid_list)) < 0) {
        free(pmid_list);
        free(metric_list);
        return error;
    }

    query->pmid = pmid_list[0];

    free(metric_list);
    free(pmid_list);

    return 0;
}

static int pcp_ez_lookup_metric_description(pcp_ez_query_t *query) {
    pmDesc pm_desc;
    int error;

    if((error = pmLookupDesc(query->pmid, &pm_desc)) < 0) {
        return error;
    }
    query->pm_desc = pm_desc;

    return 0;
}

static int pcp_ez_lookup_instances(pcp_ez_query_t *result) {
    int error;
    int *instlist;
    char **namelist;

    if(result->pm_desc.indom != PM_INDOM_NULL) {
        if((error = pmGetInDom(result->pm_desc.indom, &instlist, &namelist)) < 0) {
            return error;
        }
        result->instlist = instlist;
        result->namelist = namelist;
    }
    return 0;
}

static int pcp_ez_fetch(pcp_ez_query_t *query) {
    pmResult *pm_result;
    pmID *pmids_to_lookup;
    int error;

    pmids_to_lookup = malloc(sizeof(pmID));
    pmids_to_lookup[0] = query->pmid;
    if((error = pmFetch(1, pmids_to_lookup, &pm_result)) < 0) {
        free(pmids_to_lookup);
        return error;
    }
    free(pmids_to_lookup);

    query->pm_result = pm_result;
    return 0;
}


int pcp_ez_get_pm_type(pcp_ez_query_t *query) {
    return query->pm_desc.type;
}

pmInDom	pcp_ez_get_indom(pcp_ez_query_t *query) {
    return query->pm_desc.indom;
}

pmValueSet* pcp_ez_get_pmvalueset(pcp_ez_query_t *query) {
    return query->pm_result->vset[0];
}

char* pcp_ez_get_external_instance_name_for(int inst, pcp_ez_query_t *query) {
    int i;
    for(i=0; i < query->pm_result->vset[0]->numval; i++) {
        if(inst == query->instlist[i]) {
            return query->namelist[i];
        }
    }
    return NULL;
}


void pcp_ez_free(pcp_ez_query_t *query) {
    if(query->pm_result) {
        pmFreeResult(query->pm_result);
    }
    if(query->namelist) {
        free(query->namelist);
    }
    if(query->instlist) {
        free(query->instlist);
    }
    if(query->pcp_context >= 0) {
        pmDestroyContext(query->pcp_context);
    }
    free(query);
};

int pcp_ez_safe_raise_error(pcp_ez_query_t *query, lua_State *L, int error) {
    /* We use this to return early as luaL_error does a longjmp. Free before raising the error
     * otherwise we can't do it later */
    if(query) {
        pcp_ez_free(query);
    }
    char error_string[PM_MAXERRMSGLEN];
    return luaL_error(L, "%s\n", pmErrStr_r(error, error_string, sizeof(error_string)));
}


int pcp_ez_query_for(const char *metric, const char *host, pcp_ez_query_t **query_ptr) {
    int error;

    pcp_ez_query_t *query = pcp_ez_new();
    *query_ptr = query;
    if(query == NULL) {
        return -ENOMEM;
    }

    if((error = pcp_ez_create_context(query, host)) < 0) {
        return error;
    }

    /* Populate the pmID */
    if((error = pcp_ez_lookup_metric(query, metric)) < 0) {
        return error;
    }

    /* Lookup the metric description */
    if((error = pcp_ez_lookup_metric_description(query)) < 0) {
        return error;
    }

    /* Lookup instances if they exist */
    if((error = pcp_ez_lookup_instances(query)) < 0) {
        return error;
    }

    /* Fetch the metric */
    if((error = pcp_ez_fetch(query)) < 0) {
        return error;
    }

    return 0;

}