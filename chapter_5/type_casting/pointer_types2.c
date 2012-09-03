#include <stdio.h>

int main() {
        int i;

        char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
        int int_array[5] = {1, 2, 3, 4, 5};
        // on 64bit system memory address stores not in 4 bytes, but 8 bytes.
        // so we have to provide samesize cell to store memory address
        // long unsigned int is 8 bytes

        long unsigned int hacky_nonpointer;

        hacky_nonpointer = (long unsigned int) char_array;

        for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
            // format ‘%p’ expects type ‘void *’, but argument 2 has type ‘long unsigned int’
            // so we have to typecaste this also
                printf("[hacky_nonpointer] points to %p, which contains the char '%c'\n",
            (void *) hacky_nonpointer, *((char *) hacky_nonpointer));
                hacky_nonpointer = hacky_nonpointer + sizeof(char);
        }

        hacky_nonpointer = (long unsigned int) int_array;

        for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
                printf("[hacky_nonpointer] points to %p, which contains the integer %d\n",
            (void *)hacky_nonpointer, *((int *) hacky_nonpointer));
                hacky_nonpointer = hacky_nonpointer + sizeof(int);
        }
}
