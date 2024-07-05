#include <sicalc/sicalc.h>
#include <sicalc/sicalc_tools.h>

#include "operators.h"

#include <stdio.h>
#include <stdlib.h>

#define ACTION_ARG_COUNT_MASK 0b11
#define ACTION_ARG_COUNT(t) (int)(t & ACTION_ARG_COUNT_MASK)

/**
 * @brief Validate a token raw brackets
 * 
 * @param token 
 * @return bool true valid token
 * @return bool false invalid token
 */
static bool validate_brackets(sicalc_token_t token)
{
    stack_item_t stack = NULL;

    for (int i = 0; i < strlen(token->raw); i++)
    {
        if(strchr("{(", token->raw[i]))
        {
            stack_push(&stack, token->raw[i]);
        }
        else {
            if(
                strchr("})", token->raw[i]) &&
                (stack_empty(stack) || inverse_bracket(token->raw[i]) != stack_pop(&stack))
            )
            {

                token->info.index = i;
                token->info.error = SICALC_STATUS_BRACKETS_ERROR;
                
                return false;
            }
        }
    }

    if (!stack_empty(stack))
    {
        token->info.index = strlen(token->raw);
        token->info.error = SICALC_STATUS_BRACKETS_ERROR;
        stack_erase(&stack);

        return false;
    }

    return true;
}

/**
 * @brief Create a new token object
 * 
 * @param token 
 * @param raw 
 * @param start 
 * @param len 
 */
static void create_new_token(sicalc_token_t token, const char* raw, int start, int len)
{
    sicalc_init_token(token);

    token->raw = malloc(len + 1);
    strncpy(token->raw, &raw[start], len);
    token->raw[len] = '\0';

    if (!remove_side_brackets(token->raw, '(')) remove_side_brackets(token->raw, '{');
}

/**
 * @brief 
 * 
 * @param token 
 * @param action 
 * @return int 1 - found and append
 * @return int 0 - not found
 * @return int -1 - parse error
 */
static int parse_function_args(sicalc_token_t token, sicalc_action_t action)
{
    int len = strlen(token->raw) - strlen(action->id) + 1;

    if (len <= 0) return 0;

    if(strncmp(token->raw, action->id, strlen(action->id)) != 0)
    {
        return 0;
    }

    switch (ACTION_ARG_COUNT(action->type))
    {
    case SICALC_ACTION_ARGS0:
    {
        if (len != 1)
        {
            token->info.error = SICALC_STATUS_INVALID_TOKEN_FORMAT;
            return -1;
        }
        return 1;
    }
    case SICALC_ACTION_ARGS1:
    {   
        if (!in_brackets(&token->raw[strlen(action->id)], '('))
        {
            if (action->type & SICALC_ACTION_MAYBE_NO_BRACKETS)
            {
                sicalc_real result = 0.0;

                if (!strreal(&token->raw[strlen(action->id)], &result))
                {
                    token->info.error = SICALC_STATUS_INVALID_TOKEN_FORMAT;
                    return -1;
                }
            } else {
                token->info.error = SICALC_STATUS_INVALID_TOKEN_FORMAT;
                return -1;
            }
        }

        token->args[0] = malloc(sizeof(struct sicalc_token_s));
        create_new_token(token->args[0], token->raw, strlen(action->id), strlen(token->raw) - strlen(action->id));
        
        return 1;
    }
    default:
        break;
    }

    return 0;
}

/**
 * @brief Parsing operator arguments
 * 
 * @param token 
 * @param action 
 * @return int 1 - found and append
 * @return int 0 - not found
 * @return int -1 - parse error
 */
static int parse_operator_args(sicalc_token_t token, sicalc_action_t action)
{
    int braces_counter = 0, brackets_counter = 0;
    int len = strlen(token->raw) - strlen(action->id) + 1;

    if (len < 0) return 0;

    for (int i = 0; i < len; i++)
    {
        char c = token->raw[i];

        // take only out of brackets action
        if (c == '(') brackets_counter++;
        if (c == '{') braces_counter++;
        if (c == ')') brackets_counter--;
        if (c == '}') braces_counter--;

        if (brackets_counter == 0 && braces_counter == 0 && !strncmp(&token->raw[i], action->id, strlen(action->id)))
        {
            token->args[0] = malloc(sizeof(struct sicalc_token_s));
            create_new_token(token->args[0], token->raw, 0, i);

            token->args[1] = malloc(sizeof(struct sicalc_token_s));
            create_new_token(token->args[1], token->raw, i + strlen(action->id), strlen(token->raw) - i - strlen(action->id));
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief Function to parse action arguments and create new tokens
 * 
 * @param token 
 * @param action 
 * @return int 1 - found and append
 * @return int 0 - not found
 * @return int -1 - parse error
 */
static int parse_args(sicalc_token_t token, sicalc_action_t action)
{
    bool find = false;
    if (action->type & SICALC_ACTION_FUNCTION)
    {
        find = parse_function_args(token, action);
    }

    if (action->type & SICALC_ACTION_OPERATOR)
    {
        find |= parse_operator_args(token, action);
    }

    if (find == 1) {
        token->action = action;
        DMSG("Find %s. Id: %s", token->raw, token->action->id);
    } else {
        return find;
    }

    if (ACTION_ARG_COUNT(action->type) > 0 && token->args[0] == NULL)
        token->info.error = SICALC_STATUS_MISSING_ARGUMENT;
    
    if (ACTION_ARG_COUNT(action->type) == 2 && token->args[1] == NULL)
        token->info.error = SICALC_STATUS_MISSING_ARGUMENT;

    if (token->info.error != SICALC_STATUS_OK)
    {
        DMSG("Error: %d", token->info.error);
        return -1;
    }

    return 1;
}

void parse_token(sicalc_token_t token, sicalc_actions_list_t extra_actions)
{
    sicalc_real result;
    if (strreal(token->raw, &result))
    {
        token->result = result;
        return;
    }

    int actions_count = std_actions.count;

    if (extra_actions)
        actions_count += extra_actions->count;

    for (int i = 0; i < actions_count; i++)
    {
        sicalc_action_t action = NULL;

        if (i < std_actions.count)
            action = &std_actions.actions[i];
        else 
            action = &extra_actions->actions[i - std_actions.count];
        
        int parse_ret = parse_args(token, action);
        if (parse_ret == -1)
            return;
        else if (parse_ret == 0)
            continue;

        for (int j = 0; j < SICALC_TOKEN_ARGS_COUNT; j++)
        {
            if (token->args[j] != NULL)
            {
                DMSG("\tARG%d: %s", j, token->args[j]->raw);
                parse_token(token->args[j], extra_actions);

                if (token->args[j]->info.error != SICALC_STATUS_OK)
                {
                    token->info.error = SICALC_STATUS_SKIP;
                }
            }
        }

        return;
    }

    token->info.error = SICALC_STATUS_UNKNOWN_ACTION;
}

void sicalc_init_token(sicalc_token_t token)
{
    token->raw = NULL;
    token->args[0] = NULL;
    token->args[1] = NULL;
    token->action = NULL;
    token->info.error = SICALC_STATUS_OK;
    token->info.index = -1;
}

void sicalc_erase_token(sicalc_token_t token)
{
    free(token->raw);

    for (int i = 0; i < SICALC_TOKEN_ARGS_COUNT; i++)
    {
        if (token->args[i] != NULL)
        {
            sicalc_erase_token(token->args[i]);
            free(token->args[i]);
        }
    }
}

void sicalc_init_actions_list(sicalc_actions_list_t action_list)
{
    action_list->actions = NULL;
    action_list->count = 0;
}

int sicalc_add_action(sicalc_actions_list_t action_list,
                        const char* id,
                        sicalc_action_info_t type,
                        sicalc_real value,
                        sicalc_action_fn function)
{

    if ((type & SICALC_ACTION_ARGS1 || type & SICALC_ACTION_ARGS2) && !function)
        return -1;

    sicalc_action_t new_action = NULL;
    if (action_list->actions == NULL)
    {
        new_action = (struct sicalc_action_s*)
            malloc(sizeof(struct sicalc_action_s));
    } else {
        new_action = (struct sicalc_action_s*)
            realloc(action_list->actions, sizeof(struct sicalc_action_s) * (action_list->count + 1));
    }

    if (!new_action)
        return -1;

    new_action->id = strdup(id);
    new_action->type = type;
    new_action->value = value;
    new_action->function = function;

    action_list->actions = new_action;
    action_list->count++;

    return 0;
}

void sicalc_erase_actions_list(sicalc_actions_list_t action_list)
{
    for (int i = 0; i < action_list->count; i++)
    {
        free(action_list->actions[i].id);
    }

    free(action_list->actions);
    action_list->count = 0;
}

sicalc_real sicalc_solve_token(sicalc_token_t token)
{
    if (token->action != NULL)
    {
        sicalc_real args_results[SICALC_TOKEN_ARGS_COUNT];

        for (int i = 0; i < SICALC_TOKEN_ARGS_COUNT; i++)
        {
            if (token->args[i] != NULL)
            {
                args_results[i] = sicalc_solve_token(token->args[i]);
            }
        }
        
        if (ACTION_ARG_COUNT(token->action->type) == 0)
            token->result = token->action->value;
        else
            token->result = token->action->function(args_results[0], args_results[1], &token->info);
    }

    DMSG("Token %s result: %f", token->raw, token->result);
    return token->result;
}

sicalc_real sicalc_solve_string(const char* eq,
                                sicalc_token_t token,
                                sicalc_actions_list_t extra_actions)
{
    if (!token) return 0.0;

    char* str = (char*)malloc(strlen(eq) + 1);
    strcpy(str, eq);

    remove_whitespaces(str, eq);
    token->raw = str;

    if (!validate_brackets(token)) return 0.0;

    parse_token(token, extra_actions);

    if (token->info.error == SICALC_STATUS_OK)
    {
        sicalc_solve_token(token);
    }

    return token->result;
}

void sicalc_print_error(sicalc_token_t token)
{
    switch (token->info.error)
    {
    case SICALC_STATUS_OK:
        return;
    case SICALC_STATUS_SKIP:
    {
        for (int i = 0; i < SICALC_TOKEN_ARGS_COUNT; i++)
        {
            if (token->args[i] != NULL) sicalc_print_error(token->args[i]);
        }
        return;
    }
    case SICALC_STATUS_ZERO_DEVISION:
    {
        printf("Zero devision in: %s\n", token->action->id);
        return;
    }
    case SICALC_STATUS_NEGATINE_ARGUMENT:
    {
        printf("Negative argument in: %s\n", token->action->id);
        return;
    }
    case SICALC_STATUS_MISSING_ARGUMENT:
    {
        printf("Missing argument for: %s\n", token->action->id);
        return;
    }
    case SICALC_STATUS_UNKNOWN_ACTION:
    {
        printf("Unknown argument: %s\n", token->raw);
        return;
    }
    case SICALC_STATUS_BRACKETS_ERROR:
    {
        char* underscore = malloc(strlen(token->raw) + 1);
        memset(underscore, '_', strlen(token->raw));
        underscore[token->info.index] = '^';
        printf("Error while parsing brackets:\n\t%s\n\t%s\n", token->raw, underscore);
        free(underscore);
        return;
    }
    case SICALC_STATUS_INVALID_TOKEN_FORMAT:
    {
        printf("Invalid token %s format: %s\n", token->action->id, token->raw);
        return;
    }
    default:
        break;
    }
}