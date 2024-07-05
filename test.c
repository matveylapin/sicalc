#include <stdio.h>

#include <sicalc/sicalc.h>
#include <math.h>

int test_count = 0;
int pass_test_count = 0;

void TESTCASE(const char* eq, sicalc_real eq_ans)
{
    test_count++;

    SICALC_TOKEN_DEFINE(root_token);
    sicalc_real ans = sicalc_solve_string(eq, &root_token);
    (void)root_token;
    
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
    // struct sicalc_token_t ret;

    TESTCASE("- 3", -3);
    TESTCASE("5 + 3", 8);
    TESTCASE("10 - 7", 3);
    TESTCASE("4 * 6", 24);
    TESTCASE("12 / 3", 4);
    TESTCASE("2 ^ 3", 8);
    TESTCASE("(5 + 3) * 2", 16);
    TESTCASE("10 - (4 * 2)", 2);
    TESTCASE("(12 / 3) ^ 2", 16);
    TESTCASE("-5 + 3", -2);
    TESTCASE("10 - (-7)", 17);
    TESTCASE("(-4) * 6", -24);
    TESTCASE("12 / (-3)", -4);
    TESTCASE("2.5 + 3.7", 6.2);
    TESTCASE("5.2 - 1.8", 3.4);
    TESTCASE("2.4 * 1.5", 3.6);
    TESTCASE("10.8 / 3.6", 3);
    TESTCASE("(5 + 3) * 2 - 10", 6);
    TESTCASE("12 / 3 + 4 * 5 - 2", 22);
    TESTCASE("(10 - 2) ^ 2 / (2 + 3)", 12.8);
    TESTCASE("!3", 6);
    TESTCASE("!(3)", 6);
    TESTCASE("sqrt(16)", 4);
    TESTCASE("sqrt(25)", 5);
    TESTCASE("sqrt(0.25)", 0.5);
    TESTCASE("log10(100)", 2);
    TESTCASE("log10(1000)", 3);
    TESTCASE("log10(0.1)", -1);
    TESTCASE("ln(2.71828)", 1);
    TESTCASE("ln(10)", 2.30259); 
    TESTCASE("log2(8)", 3);
    TESTCASE("log2(16)", 4);
    TESTCASE("sin(pi/6)", 0.5);
    TESTCASE("cos(pi/3)", 0.5);
    TESTCASE("tan(pi/4)", 1);
    TESTCASE("exp(1)", 2.71828);
    TESTCASE("exp(2)", 7.38906);
    TESTCASE("exp(0)", 1);
    TESTCASE("log10(100) * 2", 4);
    TESTCASE("ln(10) - 1", 1.30259);
    TESTCASE("sin(pi/6) * 2", 1);
    TESTCASE("cos(pi/3) + exp(1)", 3.21827);
    TESTCASE("sqrt(25) / 5", 1);
    TESTCASE("log10(1000) - log2(8)", 0);
    TESTCASE("ln(10) / 2", 1.15129);
    TESTCASE("sin(pi/3) + cos(pi/6)", 1.73205);
    TESTCASE("exp(2) - log10(100)", 5.38906);
    TESTCASE("(sqrt(16) + 5) * 2", 18);
    TESTCASE("log10(100) * (2 + 3)", 10);
    TESTCASE("ln(10) - 1 / 2", 1.8025850);
    TESTCASE("sin(pi/6) * 2 + cos(pi/3)", 1.5);
    TESTCASE("exp(1) + cos(pi/3) - sin(pi/6)", 2.71828);
    TESTCASE("sqrt(25) * (5 - 2)", 15);
    TESTCASE("log10(1000) - log2(8) * 2", -3);
    TESTCASE("ln(10) / 2 + sin(pi/6)", 1.65129);
    TESTCASE("sin(pi/3) + cos(pi/6) - tan(pi/4)", 0.732052246);
    TESTCASE("exp(2) - log10(100) / 2", 6.3890560);
    TESTCASE("sqrt(16 + 9) * (2^3 - 5)", 15);
    TESTCASE("log10(1000 / 10) + ln(exp(2))", 4);
    TESTCASE("(sin(pi/3) + cos(pi/6)) * tan(pi/4)", 1.73205);
    TESTCASE("exp(1) / (log2(8) + 1)", 2.71828 / 4);
    TESTCASE("sqrt(100) - (log10(100) + ln(exp(1)))", 7);
    TESTCASE("(sin(pi/4) + cos(pi/4)) * sqrt(2)", 2);
    TESTCASE("log2(16) / (ln(exp(2)) - 1)", 4);
    TESTCASE("exp(1) + (sin(pi/6) + cos(pi/6))", 4.0843070);
    TESTCASE("sqrt(16 + 9 * 4) / (log10(100) + 1)", 2.40370085);
    TESTCASE("ln(exp(2)) - log2(8) + sin(pi/4)", -0.29289368);
    TESTCASE("exp(1) + sqrt(16) * (log10(100) - 1)", 6.71828182);
    TESTCASE("cos(pi/3) / (sin(pi/6) * tan(pi/4))", 1);
    TESTCASE("log2(16) + log10(100) * ln(exp(1))", 6);
    TESTCASE("sqrt(100 - 36) / (log10(100) - 1)", 8);
    TESTCASE("ln(exp(2)) + sin(pi/6) - cos(pi/3)", 1.999998);
    TESTCASE("exp(1) + (sqrt(16) - log10(100))", 4.71828);
    TESTCASE("cos(pi/3) * (sin(pi/6) + tan(pi/4))", 0.75);
    TESTCASE("log2(16) + log10(100) + ln(exp(1))", 7);
    TESTCASE("sqrt(100 - 36) / (log10(100) + 1)", 2.666666);
    TESTCASE("ln(exp(2)) - sin(pi/6) + cos(pi/3)", 2.00000);

    printf("\n\n Tests: %d Failed: %d\n\tPassed: %.02f%%\n", test_count, test_count - pass_test_count, (float)pass_test_count / (float)test_count * 100.0);

    return 0;
}