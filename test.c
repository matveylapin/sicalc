#include <stdio.h>

#define DEBUG
#include <sicalc/sicalc.h>
#include <math.h>

int test_count = 0;
int pass_test_count = 0;

void TESTCASE(const char* eq, sicalc_real eq_ans, sicalc_info_t ret)
{
    test_count++;

    sicalc_real ans = sicalc_solve_string(eq, ret);

    if (fabs(ans - eq_ans) < 0.001)
    {
        pass_test_count++;
        printf("%s: PASS\n", eq);
    } else {
        printf("%s: FAIL\n\t%lf != %lf\n", eq, ans, eq_ans);
    }
}

int main(int argc, char **argv)
{
    struct sicalc_info_s ret;

    TESTCASE("- 3", -3, &ret);
    TESTCASE("5 + 3", 8, &ret);
    TESTCASE("10 - 7", 3, &ret);
    TESTCASE("4 * 6", 24, &ret);
    TESTCASE("12 / 3", 4, &ret);
    TESTCASE("2 ^ 3", 8, &ret);
    TESTCASE("(5 + 3) * 2", 16, &ret);
    TESTCASE("10 - (4 * 2)", 2, &ret);
    TESTCASE("(12 / 3) ^ 2", 16, &ret);
    TESTCASE("-5 + 3", -2, &ret);
    TESTCASE("10 - (-7)", 17, &ret);
    TESTCASE("(-4) * 6", -24, &ret);
    TESTCASE("12 / (-3)", -4, &ret);
    TESTCASE("2.5 + 3.7", 6.2, &ret);
    TESTCASE("5.2 - 1.8", 3.4, &ret);
    TESTCASE("2.4 * 1.5", 3.6, &ret);
    TESTCASE("10.8 / 3.6", 3, &ret);
    TESTCASE("(5 + 3) * 2 - 10", 6, &ret);
    TESTCASE("12 / 3 + 4 * 5 - 2", 22, &ret);
    TESTCASE("(10 - 2) ^ 2 / (2 + 3)", 12.8, &ret);
    TESTCASE("sqrt(16)", 4, &ret);
    TESTCASE("sqrt(25)", 5, &ret);
    TESTCASE("sqrt(0.25)", 0.5, &ret);
    TESTCASE("log10(100)", 2, &ret);
    TESTCASE("log10(1000)", 3, &ret);
    TESTCASE("log10(0.1)", -1, &ret);
    TESTCASE("ln(2.71828)", 1, &ret);
    TESTCASE("ln(10)", 2.30259, &ret); 
    TESTCASE("log2(8)", 3, &ret);
    TESTCASE("log2(16)", 4, &ret);
    TESTCASE("sin(30)", 0.5, &ret);
    TESTCASE("cos(60)", 0.5, &ret);
    TESTCASE("tan(45)", 1, &ret);
    TESTCASE("exp(1)", 2.71828, &ret);
    TESTCASE("exp(2)", 7.38906, &ret);
    TESTCASE("exp(0)", 1, &ret);
    TESTCASE("log10(100) * 2", 4, &ret);
    TESTCASE("ln(10) - 1", 1.30259, &ret);
    TESTCASE("sin(30) * 2", 1, &ret);
    TESTCASE("cos(60) + exp(1)", 3.21827, &ret);
    TESTCASE("sqrt(25) / 5", 1, &ret);
    TESTCASE("log10(1000) - log2(8)", 0, &ret);
    TESTCASE("ln(10) / 2", 1.15129, &ret);
    TESTCASE("sin(60) + cos(30)", 1.73205, &ret);
    TESTCASE("exp(2) - log10(100)", 5.38906, &ret);
    TESTCASE("(sqrt(16) + 5) * 2", 18, &ret);
    TESTCASE("log10(100) * (2 + 3)", 10, &ret);
    TESTCASE("ln(10) - 1 / 2", 1.8025850, &ret);
    TESTCASE("sin(30) * 2 + cos(60)", 1.5, &ret);
    TESTCASE("exp(1) + cos(60) - sin(30)", 2.71828, &ret);
    TESTCASE("sqrt(25) * (5 - 2)", 15, &ret);
    TESTCASE("log10(1000) - log2(8) * 2", -3, &ret);
    TESTCASE("ln(10) / 2 + sin(30)", 1.65129, &ret);
    TESTCASE("sin(60) + cos(30) - tan(45)", 0.732052246, &ret);
    TESTCASE("exp(2) - log10(100) / 2", 6.3890560, &ret);
    TESTCASE("sqrt(16 + 9) * (2^3 - 5)", 15, &ret);
    TESTCASE("log10(1000 / 10) + ln(exp(2))", 4, &ret);
    TESTCASE("(sin(60) + cos(30)) * tan(45)", 1.73205, &ret);
    TESTCASE("exp(1) / (log2(8) + 1)", 2.71828 / 4, &ret);
    TESTCASE("sqrt(100) - (log10(100) + ln(exp(1)))", 7, &ret);
    TESTCASE("(sin(45) + cos(45)) * sqrt(2)", 2, &ret);
    TESTCASE("log2(16) / (ln(exp(2)) - 1)", 4, &ret);
    TESTCASE("exp(1) + (sin(30) + cos(30))", 4.0843070, &ret);
    TESTCASE("sqrt(16 + 9 * 4) / (log10(100) + 1)", 2.40370085, &ret);
    TESTCASE("ln(exp(2)) - log2(8) + sin(45)", -0.29289368, &ret);
    TESTCASE("exp(1) + sqrt(16) * (log10(100) - 1)", 6.71828182, &ret);
    TESTCASE("cos(60) / (sin(30) * tan(45))", 1, &ret);
    TESTCASE("log2(16) + log10(100) * ln(exp(1))", 6, &ret);
    TESTCASE("sqrt(100 - 36) / (log10(100) - 1)", 8, &ret);
    TESTCASE("ln(exp(2)) + sin(30) - cos(60)", 1.999998, &ret);
    TESTCASE("exp(1) + (sqrt(16) - log10(100))", 4.71828, &ret);
    TESTCASE("cos(60) * (sin(30) + tan(45))", 0.75, &ret);
    TESTCASE("log2(16) + log10(100) + ln(exp(1))", 7, &ret);
    TESTCASE("sqrt(100 - 36) / (log10(100) + 1)", 2.666666, &ret);
    TESTCASE("ln(exp(2)) - sin(30) + cos(60)", 2.00000, &ret);

    printf("\n\n Tests: %d Failed: %d\n\tPassed: %.02f%%\n\0", test_count, test_count - pass_test_count, (float)pass_test_count / (float)test_count * 100.0);

    return 0;
}