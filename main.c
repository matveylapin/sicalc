#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <sicalc/sicalc.h>
#include <sicalc/sicalc_tools.h>

static struct option options_list[] =
{
    {"key",  required_argument, 0, 'k'},
    {"help", no_argument,       0, 'h'},
    {0, 0, 0, 0}
};

void print_help(void)
{
    printf("Usage: cli_sicalc <expression> [options]\n");
    printf("Options:\n");
    printf("  -k, --key <key>    Set the key\n");
    printf("  -h, --help         Print this help\n");
}

int parse_key(sicalc_actions_list_t actions_list, const char* key)
{
    sicalc_real value;
    char* action_id = strndup(key, strchr(key, '=') - key);
    char* value_str = strchr(key, '=') + 1;
    
    if (!strreal(value_str, &value))
    {
        printf("\tInvalid value for: %s\n", key);
        return -1;
    }


    sicalc_add_action(actions_list,
                action_id,
                SICALC_ACTION_ARGS0 | SICALC_ACTION_FUNCTION,
                value,
                NULL);
    
    free(action_id);

    return 0;
}

int parse_actions(sicalc_actions_list_t actions_list, int argc, char **argv)
{
    int opt;
    int opt_index = 0;
    
    while ((opt = getopt_long(argc, argv, "k:h", options_list, &opt_index)) != -1)
    {
        switch (opt)
        {
            case 'k':
            {
                if (parse_key(actions_list, optarg))
                    return -1;
                break;
            }
            case 'h':
                print_help();
                return -1;
            default:
                break;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_help();
        return -1;
    }

    char* expression = strdup(argv[1]);

    SICALC_ACTION_LIST_DEFINE(extra_actions);

    if (parse_actions(&extra_actions, argc, argv))
        return -1;

    SICALC_TOKEN_DEFINE(root_token);
    sicalc_real a = sicalc_solve_string(expression, &root_token, &extra_actions);

    if (root_token.info.error == SICALC_STATUS_OK)
        printf("Result: %lf\n", a);
    else
        sicalc_print_error(&root_token);

    return 0;
}
