#include <stdlib.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h> // isdigit ....

#include <getopt.h> // Pour les arguments

int mymkdir(int argc, char * argv[])
{
    // Déclaration variable pour arguments

    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "hm" ; // Les options disponibles

    int help = 0; // -h
    short int m = 0; // -m spécifier le mode
    char * modeArgv; // mode récupéré en argv
    char * path = NULL; // Stock le chemin demandé
    mode_t mode = (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); // Stock le masque (droits donnés aux répertoires)

 // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser mkdir -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
                break;
            case 'm':
                m = 1;
                break;
            default:
                printf("ERREUR : Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }

    if (help == 1)
        // Si -h
    {
        printf("Pour utiliser la fonction mkdir taper : mkdir [-options] [(Nom Du Nouveau Répertoire)] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t -m [masque en octet] affecte des droits au dossier. Cette option évite d'avoir recours à un chdir \n");
        return 0;
    }

    // Récupérer le répertoire et le mode
    if (argc > 1)
    {
        int i;
        for (i = 1; i<argc; i++)
        {
             if(argv[i][0] != '-')
             {
                if(path != NULL)
                {
                    printf("ERREUR : Vous ne pouvez spécifier qu'un unique chemin. Saisir mkdir -h pour obtenir de l'aide. \n");
                    return 1;
                }
                else
                {
                    path = argv[i];

                }
             }
             else
             {
                modeArgv = argv[i+1];
                i++;
             }
        }
    }

    if (path == NULL)
    {
        printf("ERREUR : Veuillez spécifier le nom du répertoire. Saisir mkdir -h pour obtenir de l'aide. \n");
        return 1;
    }

    if (m == 1) {
        char* end;
        mode = (mode_t) strtol(modeArgv, &end, 10); // Convertis String en int (par strtol) puis en mode_t (par cast)
        if (mode > 777 || mode<0) {
            printf("ERREUR : Le mode doit etre compris entre 0 et 777");
            return 1;
        }
        if  (*end != '\0')
        {
            printf("ERREUR : Fermeture du mode incorrect");
            return 1;
        }
        mode = (mode_t) strtol(modeArgv, &end, 8); // Conversion en base 8
    }


    if (mkdir(path,mode) != -1)
    {
        printf("Le répertoire \"%s\" vient d'être créé avec succés ! \n", path);
    }

    return 0;
}
int main(int argc, char * argv[])
{
    mymkdir(argc, argv);


    return 0;
}
