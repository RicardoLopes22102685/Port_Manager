/* THIS CODE WAS MADE BY RICARDO LOPES A22102685 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSG_ERRO "ERROR: invalid command"
#define MSG_SCS "SUCCESS: operation concluded"
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

#define MAX_DOCKS 10

typedef struct Container {
    char id[50];
    int weight;
    struct Container *next;
} Container;

typedef struct {
    int dock_Number;
    char ship_Name[15];
    Container *piles;
} Ship;

typedef struct {
    int dock_Number;
    short occupied;
    Ship ship;
} Dock;

short check_Ship_name(char *name);

short check_Container(Container);

int count_Words(char *string);

void printDockStatus(Dock docks[]);

void addContainer(Container **pile, char *container_ID);

void removeContainer(Container **pile);

void navigate(Dock docks[], int num_Docks, char *ship_ID, int destination_Dock);

Ship *find_Ship(Dock docks[], int num_Docks, char *ship_ID);

int main(int argc, char *argv[]) {
    const char *known_commands[] = {"quit", "move", "show", "where", "navigate", "load", "weight", "help", "save"};
    char command[50], command_name[10];
    short is_command_valid;

    Dock docks[MAX_DOCKS];
    for (int i = 0; i < MAX_DOCKS; i++) { //Initialize docks
        docks[i].dock_Number = i;
        docks[i].occupied = 0;
    }

    printf(MENU);
    do {
        is_command_valid = 0;
        /*struct container temp = {"FF3", 500};
        printf("%d", check_Container(temp)); */
        fgets(command, sizeof(command), stdin); //Verificar null no Pandora
        sscanf(command, "%s", command_name);
        for (int i = 0;
             i < (int) (sizeof(known_commands) / sizeof(known_commands[0])); i++) { //Checks if main command exists
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
            if (count_Words(command) == 5) {
                char ship_ID[10], dock[5];
                /* sscanf(command, "%*s %s", command_one);
                 sscanf(command, "%*s %*s %*s %s", command_two);
                 if (strcmp(command_one, "-e") == 0){
                     sscanf(command, "%*s %*s %s", ship_ID);
                     printf("Valor de -e: %s", ship_ID);
                }
                 */
                char *e_position = strstr(command, "-e");
                char *d_position = strstr(command, "-d");
                if (e_position != NULL && d_position != NULL) {
                    if (e_position < d_position) {
                        sscanf(e_position, "-e %s -d %s", ship_ID, dock);
                    } else {
                        sscanf(d_position, "-d %s -e %s", dock, ship_ID);
                    }
                    navigate(docks, MAX_DOCKS, ship_ID,
                             atoi(dock)); //Verificar Pandora se atoi devolve 0 por falha de conversão
                } else {
                    printf(MSG_ERRO);
                    continue;
                }
            } else {
                printf(MSG_ERRO);
            }
        } else if (strcmp(command_name, "where") == 0) {
            if (count_Words(command) == 2) {
                char ship_ID[10];
                Ship *ship = NULL;
                sscanf(command, "%*s %s", ship_ID);
                ship = find_Ship(docks, MAX_DOCKS, ship_ID);
                if (ship != NULL && check_Ship_name(ship_ID)) {
                    printf("d%d %s", ship->dock_Number, ship->ship_Name);
                }
            } else printf(MSG_ERRO);
        }
        if ((strcmp(command_name, "quit") != 0)) printf("\n>");
    } while (
            strcmp(command_name,
                   "quit") != 0);

    return 0;
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

short check_Container(Container container_to_check) {
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

void printDockStatus(Dock docks[]) {
    printf("Dock Status:\n");
    for (int i = 0; i < MAX_DOCKS; i++) {
        printf("Dock %d: ", docks[i].dock_Number);
        if (docks[i].occupied) {
            printf("Ship %s is docked\n", docks[i].ship.ship_Name);
        } else {
            printf("No ship is docked\n");
        }
    }
}

void addContainer(Container **pile, char *container_ID) {
    Container *newContainer = (Container *) malloc(sizeof(Container));
    strcpy(newContainer->id, container_ID);
    newContainer->next = NULL;
    if (*pile == NULL) {
        *pile = newContainer;
    } else {
        Container *current = *pile;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newContainer;
    }
}

void removeContainer(Container **pile) {
    Container *temp;
    if (*pile == NULL) {
        printf("The pile is already empty.\n");
        return;
    }
    temp = *pile;
    *pile = (*pile)->next;
    free(temp);
}

int count_Words(char *string) {
    int count = 0;
    int len = (int) strlen(string);
    int i = 0;

    // Skip leading white spaces
    while (string[i] == ' ')
        i++;
    for (; i < len; i++) {
        // If current character is a whitespace
        // and the next character is not a whitespace or end of the string
        if (string[i] == ' ' && (string[i + 1] != ' ' && string[i + 1] != '\0')) {
            count++;
        }
    }
    // Add 1 for the last word
    count++;
    return count;
}

void navigate(Dock docks[], int num_Docks, char *ship_ID, int destination_Dock) {
    Ship *existingShip = NULL;
    int currentDock = -1;
    if (!check_Ship_name(ship_ID)) {
        printf(MSG_ERRO);
        return;
    }
    for (int i = 0; i < num_Docks; i++) {
        if (docks[i].occupied && strcmp(docks[i].ship.ship_Name, ship_ID) == 0) {
            existingShip = &(docks[i].ship);
            currentDock = i;
            break;
        }
    }

    if (existingShip != NULL) {//Ship existe
        if (destination_Dock < 0 || destination_Dock > 9) {
            printf(MSG_ERRO);
            return;
        }
        if (currentDock == destination_Dock) {
            printf(MSG_ERRO);
            return;
        }
        if (docks[destination_Dock].occupied) {
            printf(MSG_ERRO);
            return;
        }
        docks[currentDock].occupied = 0;
        docks[destination_Dock].occupied = 1;
        existingShip->dock_Number = destination_Dock;
        docks[destination_Dock].ship = *existingShip;
        printf(MSG_SCS);
    } else { // Ship não existe, criar Ship
        Ship newShip;
        if (destination_Dock < 0 || destination_Dock > 9) {
            printf(MSG_ERRO);
            return;
        }
        if (docks[destination_Dock].occupied) {
            printf(MSG_ERRO);
            return;
        }
        newShip.dock_Number = destination_Dock;
        strncpy(newShip.ship_Name, ship_ID, sizeof(newShip.ship_Name) - 1);
        newShip.ship_Name[sizeof(newShip.ship_Name) - 1] = '\0'; // Ensure null-termination
        newShip.piles = NULL;
        docks[destination_Dock].occupied = 1;
        docks[destination_Dock].ship = newShip;
        printf(MSG_SCS);
    }
}

Ship *find_Ship(Dock docks[], int num_Docks, char *ship_ID) {
    for (int i = 0; i < num_Docks; i++) {
        if (docks[i].occupied && strcmp(docks[i].ship.ship_Name, ship_ID) == 0) {
            return &(docks[i].ship);
        }
    }
    return NULL;
}


