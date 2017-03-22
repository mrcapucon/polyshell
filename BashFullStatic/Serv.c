#include "Serv.h"
#include "Kernel.h"



static  Client DefaultClient = {CODE_EMPTY,CODE_EMPTY,CODE_EMPTY};

Client  * liClient ;
int nbSocketCl;
int sockserv = CODE_EMPTY ;
int port = STARTING_PORT ;


void closeServer(){
    int i;
    for(i=0;i<NB_DEFAULT_CLIENT;i++)
        if(liClient[i].etat==CODE_CLOSE)
            terminateChild(i);
    sockserv = CODE_CLOSE;
    close(sockserv);
    exit(EXIT_SUCCESS);
}

int getSlotToken(int sock){
    int i ;
    for(i=0;i<NB_DEFAULT_CLIENT;i++){
    
        if(liClient[i].etat==CODE_EMPTY){
            liClient[i].etat=CODE_CLOSE;
            liClient[i].socket=sock;
            return i;
        }
        
    }
    return -1 ;
    
}
void exitChild(){
    printf("Child Closed \n");
    exit(EXIT_SUCCESS);
}
int terminateChild(int id){
    liClient[id].etat=CODE_EMPTY;
    close(liClient[id].socket);
    kill(liClient[id].pid,SIGUSR1);
    nbSocketCl--;
    return 0;
}

int launchServ(){
    int pid = fork();
    if(pid==0){
        // Handler servant à éteindre le serveur
        struct sigaction act, oldact;
        struct sockaddr_in servIN;
        struct sockaddr_in cliIN;
        socklen_t cliINSZ = sizeof(struct sockaddr_in);
    
        
        sigaction(SIGUSR1, NULL, &oldact);
        act.sa_handler = closeServer;
        act.sa_flags = SA_RESTART;
        act.sa_mask = oldact.sa_mask;
        sigaction(SIGUSR1, &act, NULL);
        
        
        // l'instance de Client utilsé dans chaque processus fils
        int i ,idToken,nbcmd,nbgroup;
        char ** tokens ;
        char ** test ;

        char *name = malloc (MAX_NAME_SZ*sizeof(char));
        char buffer[MAX_NAME_SZ];
        char currentDir[100] ;
        char hostName[100] ;
        
        GroupCommande * group ;
        Commande * cmd;
    
        // liste des clients la tailles max est la constante NB_DEFAULT_CLIENT
        liClient = malloc(sizeof( Client)*NB_DEFAULT_CLIENT);
        
        // Initialisation du tableau
        for(i=0;i<NB_DEFAULT_CLIENT;i++)
            liClient[i]=DefaultClient;
    

        // Socket servant pour le serveur
        sockserv = socket(AF_INET, SOCK_STREAM, 0);
        
        if (sockserv < 0) {
            perror("ERROR opening socket");
            exit(1);
        }

        // La  structure socket
        bzero((char *) &servIN, sizeof(struct sockaddr_in));
        
        servIN.sin_family = AF_INET;
        servIN.sin_addr.s_addr = INADDR_ANY;
        
        //  on essaye les ports un par un on arrete des qu'un est libre
        do {
            port ++;
            servIN.sin_port = htons(port);
        }while((bind(sockserv, (struct sockaddr *) &servIN, sizeof(struct sockaddr_in)) < 0) );
        printf("SERVER STARTED ON PORT %d \n",port);
        
        

        
        // DEFAUT_LST est le nombre max d'ecoute faite en même temps
        if(listen(sockserv,DEFAULT_LST)==-1){
            perror("IMPOSSIBLE DE LANCER LE SERVEUR BUG : LISTEN");
            exit(-1);
        }
        
        
        // tant que le signal d'arret du serveur n'est pas donné
        while (sockserv!=CODE_CLOSE) {
            
            // tant que le nombre max de clients n'est pas atteint 
            // on peutt continuer à rendre des connections
            if(nbSocketCl!=NB_DEFAULT_CLIENT){
                
                // on rentre le client dans le tableau
                idToken=getSlotToken(accept(sockserv, (struct sockaddr *) &cliIN, &cliINSZ));
                
                // on recupere l'instance de Client
                
                
                // si le accept a échoué 
                if (liClient[idToken].socket < 0) {
                    perror("IMPOSSIBLE DE SE CONNECTER AU CLIENT");
                    exit(1);
                }
               
                // on incrémente le nombre de clients actifs
                nbSocketCl++;
                
                // on fork pour le traitement du fils
                liClient[idToken].pid = fork();
                if (liClient[idToken].pid < 0) {
                    perror("IMPOSSIBLE DE FORK");
                    exit(1);
                }
                
                // l'etat du slot est close
                // plus personne ne peut l'utiliser

                if (liClient[idToken].pid == 0) {

                    // on evite que la fermeture du serveur soit faites deux fois

                    sigaction(SIGUSR1, NULL, &oldact);
                    act.sa_handler = exitChild;
                    act.sa_flags = SA_RESTART;
                    act.sa_mask = oldact.sa_mask;
                    sigaction(SIGUSR1, &act, NULL);
                
                    // on redirige la sortie standard et d'erreur vers le socket
    	            dup2(liClient[idToken].socket, STDOUT_FILENO);
    	            dup2(liClient[idToken].socket, STDERR_FILENO);
    	            // tant que le slot n'est pas close 


                        // on lis la taille du message qui va arriver
    
                        
                    printf("Interpreteur de commande v1.0 \nTaper \"quit\" pour quitter\n");
                    while(1){
                        prompt(currentDir,hostName);
                        /* Get the command, with size limit. */
                        recv(liClient[idToken].socket,buffer,MAX_NAME_SZ,0);
                        test=str_split(buffer,'#');
                        name=strdup(*(test));
                        if ((strlen(name)>0) && (name[strlen (name) - 1] == '\n'))
                            name[strlen (name) - 1] = '\0';
                        //printf("vous avez ecris %s taille %d \n",name,(int)strlen(name));
                        if(strcmp(name,"quit")==0){
                            close(liClient[idToken].socket);
                            free(test);
                            break;
                        }
                        else if(strcmp(name,"")==0)
                            continue ;
                        else{
                            tokens= str_split(name, ' ');
                            cmd=parseCmd(tokens,&nbcmd);
                            if(cmd==NULL)
                                continue ;
                            if(fileListeFun(cmd,nbcmd)!=0)
                                perror("CMD UNKNOWN IN THE MEMORY");
                            else{
                                group=processingGroup(cmd,nbcmd,&nbgroup);
                                exectGroup(group);
                                fflush(stdout);
                        }
                        
                        }
                    
                    
                    }
                
                    close(liClient[idToken].socket);
                    liClient[idToken].etat=CODE_EMPTY;
                    nbSocketCl--;
                    exit(0);
                }
                else {
                close(liClient[idToken].socket);
                }
            }
        } /* end of while */
        
    }
    return pid;
}