#include <dlfcn.h>
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


int do_system(char *cmd)
{
    return system(cmd);
}

void *get_libc_base()
{
    if (libc_base == NULL) {
        libc_base = dlopen("libc.so.6", RTLD_LAZY);
    }
    return libc_base;
}

void *print_address()
{
    void *address = NULL;
    char  funcname[256] = { 0 };

    printf("function name:\n");
    read_input(funcname, sizeof(funcname) - 1);

    address = dlsym(get_libc_base(), funcname);
    printf("%s: %p\n", funcname, address);
}


int doit()
{
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
            printf("libc: %p\n", get_libc_base());
            printf("system: %p\n", &system);
            break;
        case 2: 
            print_address();
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
