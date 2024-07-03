#include <sicalc/sicalc.h>

#include <math.h>

#define DEG2RAD(a) a * (sicalc_real)M_PI / (sicalc_real)180.0

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
    return arg1 / arg2;
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
    return sinf(DEG2RAD(arg1));
#else
    return sin(DEG2RAD(arg1));
#endif
}

sicalc_real si_cos(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return cosf(DEG2RAD(arg1));
#else
    return cos(DEG2RAD(arg1));
#endif
}

sicalc_real si_tan(sicalc_real arg1, sicalc_real arg2, sicalc_info_t ret)
{
#ifdef SICALC_FLOAT
    return tanf(DEG2RAD(arg1));
#else
    return tan(DEG2RAD(arg1));
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

sicalc_action_t operators_table[] = {
    {"+", SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, sum},
    {"-", SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, difference},
    {"*", SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, multiplication},
    {"/", SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, division},
    {"^", SICALC_ACTION_ARGS2 | SICALC_ACTION_OPERATOR, si_pow},
    {"sqrt", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_sqrt},
    {"log2", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_log2},
    {"log10", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_log10},
    {"ln", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_ln},
    {"sin", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_sin},
    {"cos", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_cos},
    {"tan", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_tan},
    {"exp", SICALC_ACTION_ARGS1 | SICALC_ACTION_FUNCTION, si_exp}
};