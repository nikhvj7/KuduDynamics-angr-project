#include <stdio.h>
#include <stdlib.h>

void print_menu() 
{
    printf("Simplest r0p challenge...\n");
    printf("3) do the thing\n");
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


int vuln()
{
    int choice;
    char buffer[40] = { 0 };
    return read_input(buffer, sizeof(buffer) * 2);
}

int main(int argc, char **argv)
{
    int choice;
    
    print_menu();
    choice = get_choice();
    if (choice == 3) {
        vuln();
    } else {
        printf("you only had one choice...\n");
    }

    
    
}
