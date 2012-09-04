#include <stdio.h>

void function() {
    int var = 5;
    static int static_var = 5;
    printf("\t[in function] var @ %p = %d\n", &var, var);
    printf("\t[in function] static_var @ %p = %d\n", &static_var, static_var);

    var++; // add 1 to var
    static_var++; // add 1 to static_var
}

int main()
{
    int i;
    static int static_var = 1337;
    // even if you init new one with same name as in function()
    // it will not change it
    // in addtion every call function() will increase
    // local static_var and get everytime new value
    // because static int inits only once when you first time
    // call function()
    for(i = 0; i < 5; i++) {
    printf("[in main] static_var @ %p = %d\n", &static_var, static_var);
    function();
    }
}
