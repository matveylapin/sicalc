#include <stdio.h>
#include <string.h>

#define SICALC_DEBUG
#include <sicalc/sicalc.h>

int main(int argc, char **argv)
{
    char* eq = "(5 + 3) * 2 - 10";

    SICALC_TOKEN_DEFINE(root_token);
    sicalc_real a = sicalc_solve_string(argv[1], &root_token, NULL);

    printf("%lf\n", a);
    printf("Error:%d\n", root_token.info.error);

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%s\n", argv[i]);
    // }
    return 0;
}