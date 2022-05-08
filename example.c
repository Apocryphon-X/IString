#define print(x) fputs((x), stdout)
#define println(x) \
    print(x);      \
    print("\n")

#include "i_string.h"
#include <stdio.h>

void show_data(i_string src, const char* name)
{
    printf(
        "[%s]\nSize: %zu\nCapacity: %zu\n\n",
        name, src -> size, src -> capacity
    );
}

int main()
{
    i_string str = IString("Hello", " friend", "!");
    i_string my_copy = sm.copy(str);

    println(str -> content);
    println(my_copy -> content);

    int arbitrary_value = 5;
    i_string fstr = sm.format(
        "The number %d is now inside of `fstr`",
         arbitrary_value
    );

    sm.s_append(fstr, ". Nice!");
    println(fstr -> content);

    println("");

    show_data(str, "str");
    show_data(my_copy, "my_copy");
    show_data(fstr, "fstr");

    sm.destroy(my_copy);
    sm.destroy(fstr);
    sm.destroy(str);
    return EXIT_SUCCESS;
}