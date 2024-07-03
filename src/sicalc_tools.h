#ifndef __SICALC_TOOLS_H__
#define __SICALC_TOOLS_H__

#include <string.h>
#include <stdio.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define CHECK_MASK(val, mask)

void remove_whitespaces(char* dest, const char* src)
{
    int shift = 0;
    for (int i = 0; i < strlen(src); i++)
    {
        if (src[i] != ' ')
        {
            dest[shift] = src[i];
            shift++;
        }
    }

    dest[shift] = '\0';
}

int strreal(const char* str, sicalc_real* r)
{
    int len;

    int ret = sscanf(str,
#ifdef SICALC_FLOAT
        "%f %n",
#else
        "%lf %n",
#endif
        r, &len);

    return ret && !str[len];
}

bool in_brackets(const char* src)
{
    int counter = 1;
    bool ret = false;

    if (src[0] != '(') return ret;

    for (int i = 1; i < strlen(src); i++)
    {
        if (src[i] == '(')
        {
            if (ret)
                return false;
            
            counter++;
        }

        if (src[i] == ')')
        {
            counter--;
            if (!counter && i == (strlen(src) - 1)) ret = true;
        }
    }

    return ret;
}

bool remove_side_brackets(char* str)
{
    bool in = in_brackets(str);
    
    if (in)
    {
        memmove(str, str+1, strlen(str));
        str[strlen(str)-1] = '\0';
    }

    return in;
}

#endif // __SICALC_TOOLS_H__