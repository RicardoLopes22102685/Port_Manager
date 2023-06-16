/* THIS CODE WAS MADE BY RICARDO LOPES A22102685 */

#include <stdio.h>
#include <string.h>

#define MSG_ERRO "ERROR: invalid command\n>"
#define MSG_SCS "SUCCESS: operation concluded\n>"
#define MENU "+---- MENU\n" \
             "| move\t\t[-g grua] [-d ponto] [-p pilha] [-D ponto] [-P pilha] [-n numero_de_contentores]\n" \
             "| show\t\t[-d ponto] [-e embarc]\n" \
             "| where\t\t[embarc]\n" \
             "| navigate\t[-e embarc] [-d ponto]\n" \
             "| load\t\t[-e embarc] [-p pilha] [-c contentor:peso]\n" \
             "| weight\t[embarc]\n" \
             "| save\t\t[filename]\n" \
             "| help\n" \
             "| quit\n" \
             "+----\n>"

struct container {
    char id[50];
    int weight;
};

void menu(void);

short check_Ship_name(char *name);

short check_Container(struct container);

int main(int argc, char *argv[]) {
    const char *known_commands[] = {"quit", "move", "show", "where", "navigate", "load", "weight", "help", "save"};
    char command[50], command_name[10];
    short is_command_valid;

    printf(MENU);
    do {
        is_command_valid = 0;
        /*struct container temp = {"FF3", 500};
        printf("%d", check_Container(temp)); */
        fgets(command, sizeof(command), stdin); //Verificar null no Pandora
        sscanf(command, "%s %*s", command_name);
        for (int i = 0; i < (int) (sizeof(known_commands) / sizeof(known_commands[0])); i++) {
            if (strcmp(command_name, known_commands[i]) == 0) {
                is_command_valid = 1;
                break;
            }
        }
        if (!is_command_valid) {
            printf(MSG_ERRO);
            continue;
        }
        if (strcmp(command_name, "navigate") == 0) {
            printf("GOOD\n>");
        }

    } while (strcmp(command_name, "quit") != 0);

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
    for (int i = 0; i < (int) strlen(container_to_check.id) - 1; i++) { //Checks first 2 letters
        if (container_to_check.id[i] < 65 || container_to_check.id[i] > 90) {
            return 0;
        }
    }
    if (!(container_to_check.id[2] - '0' >= 0 && container_to_check.id[2] - '0' <= 9) ||
        container_to_check.weight < 500) { // Ao subtrair '0' permite obter o numérico literal quando o char é um número
        return 0;
    }
    return 1;
}
