#ifndef __SICALC_H__
#define __SICALC_H__

#include <stdbool.h>

#if !defined(SICALC_FLOAT) && !defined(SICALC_DOUBLE)
#define SICALC_DOUBLE
#endif

#if defined(SICALC_FLOAT)
#define sicalc_real float
#else
#define sicalc_real double
#endif

#if defined(DEBUG) || defined(SICALC_DEBUG)
#define DMSG(str, ...) printf("DEBUG: " str "\n", __VA_ARGS__)
#else
#define DMSG(str, ...)
#endif

#define SICALC_TOKEN_ARGS_COUNT (int)2

#define SICALC_TOKEN_DEFINE(token) \
struct sicalc_token_s token; \
sicalc_init_token(&token);

typedef enum {
    SICALC_STATUS_OK = 0,
    SICALC_STATUS_ZERO_DEVISION,
    SICALC_STATUS_NEGATINE_ARGUMENT,
    SICALC_STATUS_SKIP,
    SICALC_STATUS_MISSING_ARGUMENT,
    SICALC_STATUS_UNKNOWN_ACTION,
    SICALC_STATUS_BRACKETS_ERROR
} sicalc_error_t;

typedef enum {
    SICALC_ACTION_ARGS0 = 0,
    SICALC_ACTION_ARGS1 = 1,
    SICALC_ACTION_ARGS2 = 2,
    SICALC_ACTION_FUNCTION = (1 << 3),
    SICALC_ACTION_OPERATOR = (1 << 4),
} sicalc_action_info_t;

typedef struct sicalc_info_s {
    // const char* msg; // not used
    sicalc_error_t error;
    int index;
} *sicalc_info_t;

typedef sicalc_real (*sicalc_action_fn)(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret);

typedef struct sicalc_action_s
{
    const char* id;
    sicalc_action_info_t type;
    sicalc_action_fn function;
} *sicalc_action_t;

typedef struct sicalc_token_s
{
    char* raw;
    sicalc_real result;
    struct sicalc_action_s* action;
    struct sicalc_token_s* args[SICALC_TOKEN_ARGS_COUNT];
    struct sicalc_info_s info;
} *sicalc_token_t;

void sicalc_init_token(sicalc_token_t token);
void sicalc_erase_token(sicalc_token_t token);

sicalc_real sicalc_solve_string(const char* eq, sicalc_token_t token);
sicalc_real sicalc_solve_token(sicalc_token_t token);

#endif // __SICALC_H__