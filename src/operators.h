#include <sicalc/sicalc.h>

#include <math.h>

// #define DEG2RAD(a) a * (sicalc_real)M_PI / (sicalc_real)180.0
#define DEG2RAD(a) a

sicalc_real sum(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
    return arg1 + arg2;
}

sicalc_real difference(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
    return arg1 - arg2;
}

sicalc_real multiplication(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
    return arg1 * arg2;
}

sicalc_real division(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
    sicalc_real result = arg1 / arg2;
    if (!isfinite(result))
        ret->error = SICALC_STATUS_ZERO_DEVISION;
    return result;
}

sicalc_real si_pow(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return powf(arg1, arg2);
#else
    return pow(arg1, arg2);
#endif
}

sicalc_real si_sqrt(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
    if (arg1 < 0.0)
    {
        ret->error = SICALC_STATUS_NEGATINE_ARGUMENT;
        return 0.0;
    }
#ifdef SICALC_FLOAT
    return sqrtf(arg1);
#else
    return sqrt(arg1);
#endif
}

sicalc_real si_ln(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return logf(arg1);
#else
    return log(arg1);
#endif
}

sicalc_real si_log2(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return log2f(arg1);
#else
    return log2(arg1);
#endif
}

sicalc_real si_log10(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return log10f(arg1);
#else
    return log10(arg1);
#endif
}

sicalc_real si_sin(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return sinf(arg1);
#else
    return sin(arg1);
#endif
}

sicalc_real si_cos(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return cosf(arg1);
#else
    return cos(arg1);
#endif
}

sicalc_real si_tan(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return tanf(arg1);
#else
    return tan(arg1);
#endif
}

sicalc_real si_exp(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return expf(arg1);
#else
    return exp(arg1);
#endif
}

sicalc_real factorial(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    int arg_int = roundf(arg1);
#else
    int arg_int = round(arg1);
#endif

    if (arg_int < 1)
    {
        ret->error = SICALC_STATUS_NEGATINE_ARGUMENT;
        return 0.0;
    }

    sicalc_real result = 1.0;

    for (int i = 1; i <= arg_int; i++)
    {
        result *= i;
    }

    return result;
}

struct sicalc_action_s actions_table[] = {
    {"+",     SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, 0.0, sum},
    {"-",     SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, 0.0, difference},
    {"*",     SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, 0.0, multiplication},
    {"/",     SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, 0.0, division},
    {"^",     SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, 0.0, si_pow},
    {"sqrt",  SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_sqrt},
    {"log2",  SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_log2},
    {"log10", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_log10},
    {"ln",    SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_ln},
    {"sin",   SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_sin},
    {"cos",   SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_cos},
    {"tan",   SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_tan},
    {"exp",   SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, 0.0, si_exp},
    {"!",     SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION | SICALC_ACTION_MAYBE_NO_BRACKETS, 0.0, factorial},
    {"pi",    SICALC_ACTION_ARGS0 | SICALC_ACTION_FUNCTION, (sicalc_real)M_PI, NULL}
};

struct sicalc_actions_list_s std_actions = {
    .count =  sizeof(actions_table) / sizeof(struct sicalc_action_s),
    .actions = actions_table
};