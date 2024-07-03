#include <stdio.h>
#include <string.h>

#define SICALC_DEBUG
#include <sicalc/sicalc.h>

int main(int argc, char **argv)
{
    struct sicalc_info_s ret;
    char* eq = "(5 + 3) * 2 - 10";
    sicalc_real a = sicalc_solve_string(eq, &ret);

    printf("%lf\n", a);

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%s\n", argv[i]);
    // }
    return 0;
}