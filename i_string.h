#ifndef I_STRING_H
#define I_STRING_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IString(...) __IString(__VA_ARGS__, NULL)

typedef struct i_string *i_string;

struct i_string
{
    char *content;
    size_t size;
    size_t length;
    size_t capacity;
};

static size_t __next_capacity(size_t __capacity)
{
    size_t __new_capacity = 1;
    if (__capacity && !(__capacity & (__capacity - 1)))
        return __capacity;
    while (__new_capacity < __capacity)
        __new_capacity <<= 1;
    return __new_capacity;
}

static void __cond_realloc_z(i_string __dest, size_t __new_size)
{
    if ((__dest->capacity) < __new_size)
    {
        size_t __d_new_capacity = __next_capacity(__new_size);
        __dest->content = (char *)realloc(__dest->content, __d_new_capacity);
        memset(__dest->content + __dest->size, 0, __d_new_capacity - __dest->size);
        __dest->capacity = __d_new_capacity;
    }
}

char *strmanip_s_format(const char *fmt, ...)
{
    char *formatted_str;
    size_t size = 0;

    va_list args;
    va_start(args, fmt);

    size = (size_t)vsnprintf(NULL, 0, fmt, args) + 1;

    va_start(args, fmt);

    formatted_str = (char *)malloc(size * 2);
    vsprintf(formatted_str, fmt, args);

    va_end(args);

    return formatted_str;
}

void strmanip_append(i_string dest, i_string src)
{
    size_t new_size = (dest->size) + (src->size) + 1;
    __cond_realloc_z(dest, new_size);

    strcat(dest->content, src->content);

    dest->length = new_size;
    dest->size = new_size;
}

void strmanip_s_append(i_string dest, const char *src)
{
    size_t new_size = (dest->size) + strlen(src) + 1;
    __cond_realloc_z(dest, new_size);

    strcat(dest->content, src);

    dest->length = new_size - 1;
    dest->size = new_size - 1;
}

void strmanip_reverse(i_string src)
{
    size_t idx;
    for (idx = 0; idx < (src->size / 2); idx++)
    {
        char tmp = src->content[idx];
        src->content[idx] = src->content[(src->size) - idx - 1];
        src->content[(src->size) - idx - 1] = tmp;
    }
}

void strmanip_destroy(i_string istr)
{
    free(istr->content);
    free(istr);
}

i_string __IString(void *str, ...)
{
    va_list args;
    va_start(args, str);

    i_string ptr = NULL;

    ptr = (i_string)malloc(sizeof *ptr);
    ptr->content = (char *)malloc(sizeof(char));
    ptr->capacity = sizeof(char);
    ptr->length = 0;
    ptr->size = 0;

    strcpy(ptr->content, "");
    while (str != NULL)
    {
        strmanip_s_append(ptr, (char *)str);
        str = va_arg(args, char *);
    }

    va_end(args);
    return ptr;
}

i_string strmanip_copy(i_string src)
{
    i_string ptr = IString(NULL);
    __cond_realloc_z(ptr, src->size + 1);
    strcpy(ptr->content, src->content);

    ptr->length = src->length;
    ptr->size = src->size;
    return ptr;
}

i_string strmanip_format(const char *fmt, ...)
{
    i_string formatted_str = IString(NULL);
    size_t size = 0;

    va_list args;
    va_start(args, fmt);

    size = (size_t)vsnprintf(NULL, 0, fmt, args) + 1;

    va_start(args, fmt);

    formatted_str->content = (char *)realloc(formatted_str->content, size);
    vsprintf(formatted_str->content, fmt, args);

    formatted_str->capacity = size;
    formatted_str->length = size - 1;
    formatted_str->size = size - 1;

    va_end(args);

    return formatted_str;
}

const struct
{
    i_string (*format)(const char *, ...);
    i_string (*copy)(i_string);
    void (*reverse)(i_string);
    char *(*s_format)(const char *, ...);
    void (*append)(i_string, i_string);
    void (*s_append)(i_string, const char *);
    void (*destroy)(i_string);
} strmanip = {
    strmanip_format,
    strmanip_copy,
    strmanip_reverse,
    strmanip_s_format,
    strmanip_append,
    strmanip_s_append,
    strmanip_destroy},
  sm = strmanip;

#endif /* I_STRING_H */
