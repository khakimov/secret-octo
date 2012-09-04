/*
./a.out
[in main] static_var = 1337
        [in function] var = 5
        [in function] static_var = 5
[in main] static_var = 1337
        [in function] var = 5
        [in function] static_var = 6
[in main] static_var = 1337
        [in function] var = 5
        [in function] static_var = 7
[in main] static_var = 1337
        [in function] var = 5
        [in function] static_var = 8
[in main] static_var = 1337
        [in function] var = 5
        [in function] static_var = 9
*/

#include <stdio.h>

void function() {
    int var = 5;
    static int static_var = 5;
    printf("\t[in function] var = %d\n", var);
    printf("\t[in function] static_var = %d\n", static_var);

    var++;
    static_var++;
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
    printf("[in main] static_var = %d\n", static_var);
    function();
    }
}
