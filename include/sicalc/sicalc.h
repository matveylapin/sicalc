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

#define SICALC_ACTION_LIST_DEFINE(action_list) \
struct sicalc_actions_list_s action_list; \
sicalc_init_actions_list(&action_list);

typedef enum {
    SICALC_STATUS_OK = 0,
    SICALC_STATUS_SKIP,
    SICALC_STATUS_ZERO_DEVISION,
    SICALC_STATUS_NEGATINE_ARGUMENT,
    SICALC_STATUS_MISSING_ARGUMENT,
    SICALC_STATUS_UNKNOWN_ACTION,
    SICALC_STATUS_BRACKETS_ERROR,
    SICALC_STATUS_INVALID_TOKEN_FORMAT
} sicalc_error_t;

typedef enum {
    SICALC_ACTION_ARGS0 = 0,
    SICALC_ACTION_ARGS1 = 1,
    SICALC_ACTION_ARGS2 = 2,
    SICALC_ACTION_FUNCTION = (1 << 3),
    SICALC_ACTION_OPERATOR = (1 << 4),
    SICALC_ACTION_MAYBE_NO_BRACKETS = (1 << 5)
} sicalc_action_info_t;

/**
 * @brief Info about token errors
 * 
 */
typedef struct sicalc_info_s {
    int index;
    sicalc_error_t error;
} *sicalc_info_t;

/**
 * @brief Action function
 * 
 * @param arg1 First argument
 * @param arg2 Second argument
 * @param ret sicalc_info_s pointer for notify of internal errors
 */
typedef sicalc_real (*sicalc_action_fn)(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret);

/**
 * @brief Action structure
 * 
 */
typedef struct sicalc_action_s
{
    // Name of the action
    char* id;
    // Type of the action
    sicalc_action_info_t type;
    // Value for 0 args variant
    sicalc_real value;
    // Action's function for 1 or 2 args variant
    sicalc_action_fn function;
} *sicalc_action_t;

/**
 * @brief List of actions
 * 
 */
typedef struct sicalc_actions_list_s
{
    int count;
    sicalc_action_t actions;
} *sicalc_actions_list_t;

typedef struct sicalc_token_s
{
    char* raw;
    sicalc_real result;
    struct sicalc_action_s* action;
    struct sicalc_token_s* args[SICALC_TOKEN_ARGS_COUNT];
    struct sicalc_info_s info;
} *sicalc_token_t;

/**
 * @brief Initialize token
 * 
 * @param token 
 */
void sicalc_init_token(sicalc_token_t token);

/**
 * @brief Free memory of the token
 * 
 * @param token 
 */
void sicalc_erase_token(sicalc_token_t token);

/**
 * @brief Initialize actions list
 * 
 * @param actions_list 
 */
void sicalc_init_actions_list(sicalc_actions_list_t actions_list);

/**
 * @brief Append action to the list
 * 
 * @param actions_list 
 * @param id 
 * @param type 
 * @param value 
 * @param function 
 * @return int 0 success
 * @return int -1 unsuccessful
 */
int sicalc_add_action(sicalc_actions_list_t actions_list,
                        const char* id,
                        sicalc_action_info_t type,
                        sicalc_real value,
                        sicalc_action_fn function);

/**
 * @brief Free memory of the actions list
 * 
 * @param actions_list 
 */
void sicalc_erase_actions_list(sicalc_actions_list_t actions_list);

/**
 * @brief Solve equation in string
 * 
 * @param eq 
 * @param token result and errors of each tokens
 * @param extra_actions 
 * @return sicalc_real 
 */
sicalc_real sicalc_solve_string(const char* eq,
                                sicalc_token_t token,
                                sicalc_actions_list_t extra_actions);

/**
 * @brief Solve equation in token
 * 
 * @param token result and errors of each tokens
 * @return sicalc_real 
 */
sicalc_real sicalc_solve_token(sicalc_token_t token);

void sicalc_print_error(sicalc_token_t token);

#endif // __SICALC_H__
