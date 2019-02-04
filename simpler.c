//#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void *libc_base = NULL;
void *func_addr = NULL;

void print_menu() 
{
    printf("Simpler r0p challenge...\n");
    printf("1) get libc address\n");
    printf("2) get address of libc function\n");
    printf("3) do the thing\n");
    printf("4) exit\n");
}

int read_input(char *buffer, int length)
{
    char c = '\0';
    int i = 0;

    for (i = 0; i < length; i++) {
        c = getc(_IO_stdin);
        if (c == '\n') {
            break;
        }
        buffer[i] = c;
    }
    return i;
}

int get_choice()
{
    int choice;
    char buffer[2] = { 0 };
    read_input(buffer, sizeof(buffer));
    choice = atoi(buffer);
    return choice;
}


int doit()
{
    //char buffer[1024] = { 0 };
    char buffer[48] = { 0 };
    return read_input(buffer, sizeof(buffer) * 2);
}

int main(int argc, char **argv)
{
    int choice;

    while (1) {
        print_menu();
        choice = get_choice();
        switch(choice) {
        case 1:
            printf("not implemented\n");
            break;
        case 2: 
            printf("not implemented\n");
            break;
        case 3:
            doit();
            break;
        case 4:
            return 0;
        default:
            printf("Bad\n");
        }
    }
}
