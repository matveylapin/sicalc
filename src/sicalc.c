#include <sicalc/sicalc.h>

#include "sicalc_tools.h"
#include "operators.h"

#include <stdio.h>
#include <stdlib.h>

#define ACTION_ARG_COUNT_MASK 0b11
#define ACTION_ARG_COUNT(t) (int)(t & ACTION_ARG_COUNT_MASK)

static int get_token_start(const char* eq, const char* token_id)
{
    int braces_counter = 0, brackets_counter = 0;
    int len = strlen(eq) - strlen(token_id) + 1;

    if (len < 0) return -1;

    for (int i = 0; i < len; i++)
    {
        char c = eq[i];

        // take only out of brackets action
        if (c == '(') brackets_counter++;
        if (c == '{') braces_counter++;
        if (c == ')') brackets_counter--;
        if (c == '}') braces_counter--;

        if (brackets_counter == 0 && braces_counter == 0 && !strncmp(&eq[i], token_id, strlen(token_id)))
        {
            return i;
        }
    }
    return -1;
}

static void create_new_token(sicalc_token_t token, const char* raw, int start, int len)
{
    sicalc_init_token(token);

    token->raw = malloc(len + 1);
    strncpy(token->raw, &raw[start], len);
    token->raw[len] = '\0';

    remove_side_brackets(token->raw);
}

void parse_token(sicalc_token_t token)
{
    sicalc_real result;
    if (strreal(token->raw, &result))
    {
        token->result = result;
        return;
    }

    for (int i = 0; i < ARRAY_SIZE(operators_table); i++)
    {
        sicalc_action_t op = &operators_table[i];

        int index = get_token_start(token->raw, op->id);

        if (index >= 0)
        {
            token->action = op;
            DMSG("Find %s. Id: %s", token->raw, token->action->id);
            
            if (ACTION_ARG_COUNT(op->type) == 0)
            {
                return;
            }

            if (op->type & SICALC_ACTION_OPERATOR && ACTION_ARG_COUNT(op->type) == 2)
            {
                token->args[0] = malloc(sizeof(struct sicalc_token_s));
                create_new_token(token->args[0], token->raw, 0, index);

                token->args[1] = malloc(sizeof(struct sicalc_token_s));
                create_new_token(token->args[1], token->raw, index + strlen(op->id), strlen(token->raw) - index - strlen(op->id));
            }

            if (op->type & SICALC_ACTION_FUNCTION && ACTION_ARG_COUNT(op->type) == 1)
            {
                token->args[0] = malloc(sizeof(struct sicalc_token_s));
                create_new_token(token->args[0], token->raw, strlen(op->id), strlen(token->raw) - strlen(op->id));
            }

            if (ACTION_ARG_COUNT(op->type) > 0 && token->args[0] == NULL)
                    token->info.error = SICALC_STATUS_MISSING_ARGUMENT;
            
            if (ACTION_ARG_COUNT(op->type) == 2 && token->args[1] == NULL)
                    token->info.error = SICALC_STATUS_MISSING_ARGUMENT;

            if (token->info.error != SICALC_STATUS_OK)
            {
                DMSG("Error: %d", token->info.error);
                return;
            }

            if (token->args[0] != NULL)
            {
                DMSG("\tARG0: %s", token->args[0]->raw);
                parse_token(token->args[0]);
            }

            if (token->args[1] != NULL)
            {
                DMSG("\tARG1: %s", token->args[1]->raw);
                parse_token(token->args[1]);
            }

            return;
        }
    }

    token->info.error = SICALC_STATUS_MISSING_ACTION;
}

void sicalc_init_token(sicalc_token_t token)
{
    token->raw = NULL;
    token->args[0] = NULL;
    token->args[1] = NULL;
    token->action = NULL;
    token->info.error = SICALC_STATUS_OK;
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

        token->result = token->action->function(args_results[0], args_results[1], &token->info);
    }

    DMSG("Token %s result: %f", token->raw, token->result);
    return token->result;
}

sicalc_real sicalc_solve_string(const char* eq, sicalc_info_t ret)
{
    char* str = (char*)malloc(strlen(eq) + 1);
    strcpy(str, eq);
    remove_whitespaces(str, eq);

    SICALC_TOKEN_DEFINE(root_token);

    root_token.raw = str;
    parse_token(&root_token);

    sicalc_solve_token(&root_token);

    return root_token.result;
}