#include <stdio.h>
#include <string.h>

struct container {
    char id[50];
    int weight;
};

void menu(void);

short check_Ship_name(char *name);

short check_Container(struct container);

int main(int argc, char *argv[]) {
    menu();
    struct container temp = {"FF4", 500};
    printf("%d", check_Container(temp));
    return 0;
}

void menu(void) {
    printf("+---- MENU\n"
           "| move\t\t[-g grua] [-d ponto] [-p pilha] [-D ponto] [-P pilha] [-n numero_de_contentores]\n"
           "| show\t\t[-d ponto] [-e embarc]\n"
           "| where\t\t[embarc]\n"
           "| navigate\t[-e embarc] [-d ponto]\n"
           "| load\t\t[-e embarc] [-p pilha] [-c contentor:peso]\n"
           "| weight\t[embarc]\n"
           "| save\t\t[filename]\n"
           "| help\n"
           "| quit\n"
           "+----\n"
           ">");
}

short check_Ship_name(char *name) {
    if (strlen(name) != 4) {
        return 0;
    }
    for (int i = 0; i < (int) strlen(name); i++) {
        if (name[i] < 65 || name[i] > 90) {
            return 0;
        }
    }
    return 1;
}

short check_Container(struct container container_to_check) {
    if (strlen(container_to_check.id) != 3) {
        return 0;
    }
    for (int i = 0; i < (int) strlen(container_to_check.id); i++) {
        if (i == (int) strlen(container_to_check.id) - 1) {
            if (container_to_check.id[i] >= 0 && container_to_check.id[i] <= 9)
                return 1;
        }
        if (container_to_check.id[i] < 65 || container_to_check.id[i] > 90) {
            return 0;
        }
    }
    return 1;
}
