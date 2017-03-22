#include <unistd.h>
#include <stdlib.h>
#include <getopt.h> //option
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>   //path
#include <stdio.h>  //printf




int MyCd(int argc, char *argv[]){
    int help = 0; //option h
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "h" ; // Les options disponibles
    // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("Un des paramètres n'est pas reconnu. Utiliser -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
                break;
            default:
                printf("Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }

    // Si -h
    if (help == 1)
    {
        printf("Pour utiliser la fonction cd taper : cd [-option] [chemin] \n");
        printf("Pour se déplacer \n");
        printf("\t -h : Affiche l'aide. \n");
        return 0;
    }

    // Déplacement
    chdir(argv[1]);
    //Pour écrire le chemin
    char buff[PATH_MAX + 1];    // On stock la taille du chemin max + 1
    char* path = getcwd( buff, PATH_MAX + 1 );
        if( path != NULL )  //Si le chemin existe alors on l'écrit et on return
        {
            printf("Déplacement dans : %s \n", path);
            return 0;
        }
        else
        {
            printf("Erreur le chemin n'existe pas\n");
            return 1;
        }
    return 0;
}



