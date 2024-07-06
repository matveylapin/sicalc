#ifndef __SICALC_TOOLS_H__
#define __SICALC_TOOLS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define CHECK_MASK(val, mask)

typedef struct stack_item_s {
    char data;
    struct stack_item_s* next;
} *stack_item_t;

stack_item_t stack_new(char data)
{
    stack_item_t stack_item = (struct stack_item_s*)malloc(sizeof(struct stack_item_s));

    stack_item->data = data;
    stack_item->next = NULL;

    return stack_item;
}

int stack_empty(stack_item_t root)
{
    return !root;
}

void stack_push(stack_item_t* root, char data)
{
    stack_item_t stack_item = stack_new(data);
    stack_item->next = *root;
    *root = stack_item;
}

char stack_pop(stack_item_t* root)
{
    if (stack_empty(*root)) return 0;
    
    struct stack_item_s* temp = *root;
    *root = (*root)->next;
    char popped = temp->data;
    free(temp);

    return popped;
}

void stack_erase(stack_item_t* root)
{
    while (stack_empty(*root))
        stack_pop(root);
}

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

/**
 * @brief Try to convert string to real number
 * 
 * @param str 
 * @param r 
 * @return int 
 */
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

char inverse_bracket(char c) {
    switch (c)
    {
    case '}':
        return '{';
    case '{':
        return '}';
    case ')':
        return '(';
    case '(':
        return ')';
    default:
        break;
    }

    return c;
}

bool in_brackets(const char* src, char brecket)
{
    int counter = 1;
    bool ret = false;

    if (src[0] != brecket) return ret;

    for (int i = 1; i < strlen(src); i++)
    {
        if (src[i] == brecket)
        {
            if (ret)
                return false;
            
            counter++;
        }

        if (src[i] == inverse_bracket(brecket))
        {
            counter--;
            if (!counter && i == (strlen(src) - 1)) ret = true;
        }
    }

    return ret;
}

bool remove_side_brackets(char* str, char brecket)
{
    bool in = in_brackets(str, brecket);
    if (in)
    {
        memmove(str, str+1, strlen(str));
        str[strlen(str)-1] = '\0';
    }

    return in;
}

#endif // __SICALC_TOOLS_H__
