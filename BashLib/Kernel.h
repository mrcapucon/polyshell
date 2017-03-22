
#ifndef KERN_H
#define KERN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dlfcn.h>



// le nombre de char max pour un nom de fonction
#define MAX_NAME_SZ 256
// le nombre max de fonction dans une commande 
#define MAX_NB_FUNC 20

// les flux standards
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LIB_COMMANDS "LibFunc.so"



typedef struct Function Function;
typedef struct Commande Commande;
typedef struct GroupCommande GroupCommande;

// permet de simplifier l'ecriture et l'appel des fonctions
typedef int (*Func)(int, char **);


void *lib;


char** str_split(char* a_str, const char a_delim);
int lclFunction(char * cmd);
int getStructFunct(char ** cmd);
int getType(char * partCmd);
Commande * parseCmd(char ** tokens ,int * retnbcmd);
void prompt(char *currentDir, char *hostName);
GroupCommande * processingGroup(Commande * cmd,int nbCmd,int * retnb);
int exectGroup (GroupCommande * ligrp);
int chainExec(Commande cmd);
void clear();
void initialize();
int exect(Commande cmd );
int  fileListeFun(Commande *  liCmd , int nbCmd);



// les etats de l'automates d'analyse de la commande
enum State{ NOCMDSTART,CMDSTART,NEEDCMDNEXT,NEEDNOTCMDNEXT,NEWTHREAD};

// les differents composantes d'une commande 
enum Type{UNDEFINED,CMD,COMPLEMENTCMD,REDIRECTIONLEFTERROR,REDIRECTIONLEFT,REDIRECTIONRIGHTERROR,REDIRECTIONRIGHT,DOUBLECMD,LOGIC};



// structure à remplir pour l'execution d'une commande
struct Commande {
    char * name;
    char * option;
    char * redirectionin;
    char * redirectionout;
    char * redirectionerror;
    char * redirectionkeyboard ;

    Func pfunc;
    
    // thread indique un detachement du terminal
    int thread ;
    
    // logique indique un lien logique dans le chainage 
    char logic ;
    int nboption;
    
    // nextCmd est un pointeur vers la fonction suivante dans la commande 
    struct Commande * nextCmd;

};

struct GroupCommande{
    Commande * cmd;
    GroupCommande * nextgroup;
    char logic ;
    
};

// structure faisant le lien entre les noms de commandes et les fonctions 
struct Function {
    char * name ;
    Func pfunc;
};


#endif