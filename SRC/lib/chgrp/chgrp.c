#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h> // isdigit ....

#include <getopt.h> // Pour les arguments
#include <pwd.h>  // Pour la fonction getpwnam qui change le nom;


// Pour le parsing de la commande
int myChgrp(int argc, char * argv[])
{
   // Déclaration variable pour arguments
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "h" ; // Les options disponibles

    int help = 0; // -h
    char * group = NULL; // Stock de nouveau groupe
    char * path = NULL; // Stock le chemin demandé


 // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser chgrp -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
                break;
            /*default:
                printf("ERREUR : Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;*/
        }
    }
    if (help == 1) // Si -h
    {
        printf("Pour utiliser la fonction chgrp taper : chgrp [nomNouveauGroupe] [cheminDuFichier] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t \n");
        printf("Exemple : \n");
        printf("chown root test \t Le nouveau groupe de \"test\" est celui de root\n");
        return 0;
    }

    // Récupérer le répertoire et le mode
    if (argc > 1)
    {
        int i;
        for (i = 1; i<argc; i++)
        {

             switch(i)
             {
                case 1:
                    group = argv[i];
                    break;
                case 2:
                    path = argv[i];
                    break;
             }

        }
    }


    if (path == NULL)
    {
        printf("ERREUR : Veuillez spécifier le nom d'un répertoire ou d'un fichier. Saisir chgrp -h pour obtenir de l'aide. \n");
        return 1;
    }

    /* Coeur de la commande chgrp */

    // Récupération des uid et gid du fichier

    struct stat sb;
    stat(path, &sb);

    uid_t uid; // déclaration uid
    gid_t gid; // déclaration gid

    uid = sb.st_uid;    // récupération de l'actuelle uid
    gid = sb.st_gid;    // récupération de l'actuelle gid

        // Récupération du gId;
        struct passwd* grp;

        if (group != NULL)
        {
            gid = atoi(group);
            if (!(grp = getpwnam(group)))
            {
                printf("ERREUR : Impossible d'obtenir le gId du groupe : \"%s\" \n", group);
                return 1;
            }
            gid = grp->pw_gid;
        }

        // Changement du fichier
        if (chown(path, uid, gid) != 0)
        {
            printf("ERREUR : Les modifications demandé sur le fichier n'ont pu être effectué.\n Assurez-vous d'avoir les droits nécéssaires. \n");
            return 1;
        }

    printf("Le groupe propriétaire du fichier \"%s\" a correctement été changé.\n", path);
    return 0;
}

int main(int argc, char * argv[])
{
    myChgrp(argc, argv);
    return 0;
}
