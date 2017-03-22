
#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>


// le nombre de char max pour un nom de fonction
// le nombre max de fonction dans une commande 

#define STARTING_PORT 1000 
#define NB_DEFAULT_CLIENT 25 
#define CODE_EMPTY  10
#define CODE_CLOSE  20
#define DEFAULT_LST 5
#define END_CMD "@"

typedef struct Function Function;
typedef struct Commande Commande;
// permet de simplifier l'ecriture et l'appel des fonctions
typedef int (*Func)(int, char **);



typedef struct Client Client;


struct Client {
    int etat ;
    int socket ;
    pid_t pid;
};




void closeServer();
int getSlotToken(int sock);
int terminateChild(int id);
int launchServ();


#endif