/* THIS CODE WAS MADE BY RICARDO LOPES A22102685 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MSG_ERRO "ERROR: invalid command\n"
#define MSG_SCS "SUCCESS: operation concluded\n"
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
             "+----\n"

#define MAX_DOCKS 10
#define MAX_PILES 6

typedef struct Container {
    char id[50];
    int weight;
    struct Container *next;
} Container;

typedef struct {
    int dock_Number;
    char ship_Name[15];
    Container *piles[MAX_PILES];
} Ship;

typedef struct {
    int dock_Number;
    short occupied;
    Ship ship;
} Dock;

short check_Ship_name(char *name);

short check_Container(Container);

int count_Words(char *string);

int loadContainer(Dock docks[], int num_Docks, char *ship_ID, int pile_Number, char *container_string);

int navigate(Dock docks[], int num_Docks, char *ship_ID, int destination_Dock);

Ship *find_Ship(Dock docks[], int num_Docks, char *ship_ID);

int weight(Ship);

void show(Dock docks[], int numDocks, char *command);

int main(void /*int argc, char *argv[]*/) {
    const char *known_commands[] = {"quit", "move", "show", "where", "navigate", "load", "weight", "help", "save"};
    char command[100], command_name[10];
    Dock docks[MAX_DOCKS];

    for (int i = 0; i < MAX_DOCKS; i++) { //Initialize docks
        docks[i].dock_Number = i;
        docks[i].occupied = 0;
    }
    printf(MENU);
    printf(">");
    do {
        short is_command_valid = 0;
        fgets(command, sizeof(command), stdin);
        sscanf(command, "%9s", command_name);
        for (int i = 0;
             i < (int) (sizeof(known_commands) / sizeof(known_commands[0])); i++) { //Checks if main command exists
            if (strcmp(command_name, known_commands[i]) == 0) {
                is_command_valid = 1;
                break;
            }
        }
        if (!is_command_valid) {
            printf(MSG_ERRO);
        } else {
            if (strcmp(command_name, "navigate") == 0) {
                if (count_Words(command) == 5) {
                    char *e_position = strstr(command, "-e");
                    char *d_position = strstr(command, "-d");
                    if (e_position != NULL && d_position != NULL) {
                        char ship_ID[10], dock[5];
                        if (e_position < d_position) {
                            sscanf(e_position, "-e %9s -d %4s", ship_ID, dock);
                        } else {
                            sscanf(d_position, "-d %4s -e %9s", dock, ship_ID);
                        }
                        if (strlen(dock) == 1) {
                            isdigit(dock[0]) ? navigate(docks, MAX_DOCKS, ship_ID, dock[0] - '0') : printf(MSG_ERRO);
                        } else printf(MSG_ERRO);
                    } else {
                        printf(MSG_ERRO);
                    }
                } else printf(MSG_ERRO);

            } else if (strcmp(command_name, "where") == 0) {
                if (count_Words(command) == 2) {
                    char ship_ID[10];
                    Ship *ship = NULL;
                    sscanf(command, "%*s %9s", ship_ID);
                    ship = find_Ship(docks, MAX_DOCKS, ship_ID);
                    if (ship != NULL && check_Ship_name(ship_ID)) {
                        printf("%d %s", ship->dock_Number, ship->ship_Name);
                    } else printf(MSG_ERRO);
                } else printf(MSG_ERRO);
            } else if (strcmp(command_name, "load") == 0) {
                if (count_Words(command) == 7) {
                    char *e_position = strstr(command, "-e");
                    char *p_position = strstr(command, "-p");
                    char *c_position = strstr(command, "-c");
                    if (e_position != NULL && p_position != NULL && c_position != NULL) {
                        char e_value[10], p_value[10], c_value[10];
                        sscanf(e_position, "-e %9s", e_value);
                        sscanf(p_position, "-p %9s", p_value);
                        sscanf(c_position, "-c %9s", c_value);
                        !isalpha(p_value[0]) ? loadContainer(docks, MAX_DOCKS, e_value, p_value[0] - '0', c_value)
                                             : printf(MSG_ERRO);
                    }

                } else printf(MSG_ERRO);
            } else if (strcmp(command_name, "help") == 0) {
                printf(MENU);
            } else if (strcmp(command_name, "show") == 0) {
                if (count_Words(command) <= 3) {
                    show(docks, MAX_DOCKS, command);
                } else printf(MSG_ERRO);

            } else if (strcmp(command_name, "weight") == 0) {
                if (count_Words(command) == 2) {
                    char ship_ID[10];
                    Ship *ship = NULL;
                    sscanf(command, "weight %9s", ship_ID);
                    ship = find_Ship(docks, MAX_DOCKS, ship_ID);
                    if (ship != NULL) {
                        int ship_weight = weight(*ship);
                        printf("%s %d\n", ship->ship_Name, ship_weight);
                    } else printf(MSG_ERRO);

                } else printf(MSG_ERRO);
            }
        }
        if ((strcmp(command_name, "quit") != 0)) printf(">");
    } while (
            strcmp(command_name, "quit") != 0);

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
        container_to_check.weight <
        500) { // Ao subtrair '0' permite obter o numérico literal quando o char é um número
        return 0;
    }
    return 1;
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

int navigate(Dock docks[], int num_Docks, char *ship_ID, int destination_Dock) {
    Ship *existingShip = NULL;
    int currentDock = -1;
    if (!check_Ship_name(ship_ID)) {
        printf(MSG_ERRO);
        return 0;
    }
    for (int i = 0; i < num_Docks; i++) {
        if (docks[i].occupied && strcmp(docks[i].ship.ship_Name, ship_ID) == 0) {
            existingShip = &(docks[i].ship);
            currentDock = i;
            break;
        }
    }
    if (destination_Dock < 0 || destination_Dock > 9 || currentDock == destination_Dock ||
        docks[destination_Dock].occupied) {
        printf(MSG_ERRO);
        return 0;
    } else if (existingShip != NULL) {//Ship existe
        docks[currentDock].occupied = 0;
        docks[destination_Dock].occupied = 1;
        existingShip->dock_Number = destination_Dock;
        docks[destination_Dock].ship = *existingShip;
        printf(MSG_SCS);
    } else { // Ship não existe, criar Ship
        Ship newShip;
        newShip.dock_Number = destination_Dock;
        strncpy(newShip.ship_Name, ship_ID, sizeof(newShip.ship_Name) - 1);
        newShip.ship_Name[sizeof(newShip.ship_Name) - 1] = '\0'; // Garante null-termination
        for (int i = 0; i < MAX_PILES; i++) {
            newShip.piles[i] = NULL;
        }
        docks[destination_Dock].occupied = 1;
        docks[destination_Dock].ship = newShip;
        printf(MSG_SCS);
    }
    return 1;
}

Ship *find_Ship(Dock docks[], int num_Docks, char *ship_ID) {
    for (int i = 0; i < num_Docks; i++) {
        if (docks[i].occupied && strcmp(docks[i].ship.ship_Name, ship_ID) == 0) {
            return &(docks[i].ship);
        }
    }
    return NULL; //Ship não existe
}

Container *find_Container(Dock docks[], int num_Docks, char *container_ID) {
    for (int i = 0; i < num_Docks; i++) {
        if (docks[i].occupied) {
            Ship ship = docks[i].ship;
            for (int j = 0; j < MAX_PILES; j++) {
                Container *current = ship.piles[j]; //Current é o primeiro Container de cada Pile
                while (current != NULL) {
                    if (strcmp(current->id, container_ID) == 0) {
                        return current; //Container encontrado
                    }
                    current = current->next;
                }
            }
        }
    }
    return NULL; // Container não existe
}

int loadContainer(Dock docks[], int num_Docks, char *ship_ID, int pile_Number, char *container_string) {
    char container_ID[10];
    char container_weight_string[7];
    int container_weight;
    Container *container_to_add = (Container *) malloc(sizeof(Container));
    sscanf(container_string, "%3[^:]", container_ID);
    sscanf(container_string, "%*[^:]:%6s", container_weight_string);
    strncpy(container_to_add->id, container_ID, sizeof(container_to_add->id) - 1);
    container_to_add->id[sizeof(container_to_add->id) - 1] = '\0';
    for(int i=0; i< (int) strlen(container_weight_string); i++){
        if(isdigit(container_weight_string[i])){
            continue;
        } else {
            printf(MSG_ERRO);
            free(container_to_add);
            return 0;
        }
    } //container_weight_string aqui é só digitos
    container_weight = atoi(container_weight_string);
    container_to_add->weight = container_weight;
    container_to_add->next = NULL;
    if (check_Ship_name(ship_ID) && check_Container(*container_to_add) && pile_Number >= 0 && pile_Number < 6 &&
        find_Container(docks, num_Docks, container_ID) == NULL && find_Ship(docks, num_Docks, ship_ID) != NULL) {
        Ship *ship_to_load = find_Ship(docks, num_Docks, ship_ID);
        Container *current = ship_to_load->piles[pile_Number];
        if (current == NULL) {
            ship_to_load->piles[pile_Number] = container_to_add;
        } else {
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = container_to_add;
        }
        printf(MSG_SCS);
        return 1;
    } else {
        printf(MSG_ERRO);
        free(container_to_add);
    }
    return 0;
}

void show(Dock docks[], int numDocks, char *command) {
    char clean_command[10];
    sscanf(command, "%9[^\n]", clean_command);
    if (strcmp(clean_command, "show") == 0) {
        for (int i = 0; i < numDocks; i++) {
            if (docks[i].occupied) {
                Ship *ship = &(docks[i].ship);
                printf("d%d %s\n", ship->dock_Number, ship->ship_Name);
                for (int j = 0; j < MAX_PILES; j++) {
                    if (ship->piles[j] != NULL) {
                        Container *container = ship->piles[j];
                        int count = 0;
                        while (container != NULL) {
                            container = container->next;
                            count++;
                        }
                        printf("\tp%d %d", j, count);
                        //int count = 0;
                        while (container != NULL) {
                            printf(" %s:%d", container->id, container->weight);
                            container = container->next;
                        }
                        container = ship->piles[j];
                        while (container != NULL) {
                            printf(" %s:%d", container->id, container->weight);
                            container = container->next;
                        }
                        printf("\n");
                    }
                }

            }
        }
    } else if (strncmp(clean_command, "show -d", 7) == 0) {
        int dock_Number;
        sscanf(command, "show -d %d", &dock_Number);
        if (dock_Number >= 0 && dock_Number <= 9 && docks[dock_Number].occupied) {
            Ship *ship = &(docks[dock_Number].ship);
            printf("d%d %s\n", ship->dock_Number, ship->ship_Name);
            for (int i = 0; i < MAX_PILES; i++) {
                if (ship->piles[i] != NULL) {
                    Container *container = ship->piles[i];
                    int count = 0;
                    while (container != NULL) {
                        container = container->next;
                        count++;
                    }
                    printf("\tp%d %d", i, count);
                    while (container != NULL) {
                        printf(" %s:%d", container->id, container->weight);
                        container = container->next;
                    }
                    container = ship->piles[i];
                    while (container != NULL) {
                        printf(" %s:%d", container->id, container->weight);
                        container = container->next;
                    }
                    printf("\n");
                }
            }
            return;
        }
        printf(MSG_ERRO);
    } else if (strncmp(clean_command, "show -e", 7) == 0) {
        char shipName[15];
        Ship *ship = NULL;
        sscanf(command, "show -e %14s", shipName);
        ship = find_Ship(docks, numDocks, shipName);
        if (ship != NULL) {
            printf("d%d %s\n", ship->dock_Number, ship->ship_Name);
            for (int j = 0; j < MAX_PILES; j++) {
                if (ship->piles[j] != NULL) {
                    Container *container = ship->piles[j];
                    int count = 0;
                    while (container != NULL) {
                        container = container->next;
                        count++;
                    }
                    printf("\tp%d %d", j, count);
                    while (container != NULL) {
                        printf(" %s:%d", container->id, container->weight);
                        container = container->next;
                    }
                    container = ship->piles[j];
                    while (container != NULL) {
                        printf(" %s:%d", container->id, container->weight);
                        container = container->next;
                    }
                    printf("\n");
                }
            }
            return;
        }
        printf(MSG_ERRO);
    } else {
        printf(MSG_ERRO);
    }
}

int weight(Ship ship) {
    int count = 0;
    for (int i = 0; i < MAX_PILES; i++) {
        Container *container = ship.piles[i];
        while (container != NULL) {
            count += container->weight;
            container = container->next;
        }
    }
    return count;
}

/* THIS CODE WAS MADE BY RICARDO LOPES A22102685 */
