/*
*   ChangeLog
*           - Le fichier reconnait les fichiers du "premier niveau" comme des dossiers, mise à part le tout premier fichier rencontrer.
*               Mise à part ce soucis non négligeable, CP est fonctionnel.
*
*/


#define _GNU_SOURCE

#define debug   //  Define perso qui affiche les messages de Debug !

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include <libgen.h>
#include <getopt.h> // Pour les arguments
#include <string.h>



int copie(char * pathSrc, char * pathDest)
{
    FILE *pFileSrc;         // Fichier Source
    FILE *pFileDest;        // Fichier Destination
    DIR *dirp;
  	struct dirent *dptr;
    int etatLecture;        //Lors de la copie, l'état lecture renvera un chiffre != 0 pour indiquer la fin de la lecture du fichier source.


    char buffer1[PATH_MAX+1];       //Buffer
    char buffer2[PATH_MAX+1];       //Buffer

    char * nameCurrentDir;
    // Récupération du type du fichier analysé
    struct stat sb; //buffer


    pathSrc = realpath(pathSrc, buffer1); //Get absolute path de la source


    #ifdef debug
    printf("\n ----------- \nDEBUG : Bienvenue dans copie ! :) le pathSrc = %s et le pathDest = %s \n", pathSrc, pathDest);
    #endif
    // debug


    stat(pathSrc, &sb); //récup info sur le path source

    if (S_ISDIR(sb.st_mode)) // SI DOSSIER
    {
        #ifdef debug
        printf("DEBUG : Il semble que %s soit un dossier... \n", pathSrc);
        #endif // debug
        pathDest = realpath(pathDest, buffer2); //Get absolute path dest.
        // On ouvre le dossier source
        if ((dirp=opendir(pathSrc)) == NULL)
            {
                printf("Erreur, impossible d'ouvrir le répertoire");
                return 1;
            }

        // On entre dedans
        if (chdir(pathSrc) != 0)
        {
                printf("Erreur, impossible d'accéder au répertoire source");
                return 1;
        }


        nameCurrentDir = basename(pathSrc); //Récup du nom du dossier courant à copier (sans le path)
        strcat(pathDest, "/"); //On ajoute / au pathDest
        strcat(pathDest, nameCurrentDir ); // On ajoute le nom du dossier à ajouter.

        #ifdef debug
        printf("DEBUG : On souhaite créer le dossier %s vers %s ... \n", pathSrc, pathDest);
        #endif // debug

        if (mkdir(pathDest, sb.st_mode) == 0)   //Création du dossier
        {
            #ifdef debug
            printf("DEBUG : Le dossier %s a bien été créé ! \n", nameCurrentDir);
            #endif
            while(dptr = readdir(dirp)) // On parcours le dossier, pour voir son contenu.
            {
            char pathSrcTmp[strlen(pathSrc)+strlen(dptr->d_name)+2];
            char pathDestTmp[strlen(pathSrc)+strlen(dptr->d_name)+2];
                stat(dptr->d_name, &sb); //Pour chaque entrée, on récupère les informations.

                strcpy(pathSrcTmp, pathSrc);
                strcpy(pathDestTmp, pathDest);


                #ifdef debug
                    printf("DEBUG : WHILE \n\t pathSrc = %s \n\t pathSrcTmp = %s \n\t pathDest= %s \n\t pathDestTmp = %s \n ", pathSrc, pathSrcTmp, pathDest, pathDestTmp);
                #endif

                if((strcmp(".",dptr->d_name) != 0) && (strcmp("..",dptr->d_name) != 0)) // On ne veut pas du . et du .. !
                {
                    strcat(pathSrcTmp, "/");
                    strcat(pathSrcTmp, dptr->d_name); // On update le pathSrc


                    if (!S_ISDIR(sb.st_mode)) // SI Fichier
                    {
                        #ifdef debug
                        printf("DEBUG : Askip, %s est effectivement un fichier. \n", dptr->d_name);
                        #endif
                        strcat(pathDestTmp, "/");
                        strcat(pathDestTmp, dptr->d_name); // On update le pathDest
                    }


                    #ifdef debug
                    printf("DEBUG : Analyse de %s en cours. Ce fichier doit être copié de %s vers %s \n", dptr->d_name, pathSrcTmp, pathDestTmp);
                    #endif
                    copie(pathSrcTmp, pathDestTmp);
                    //return 0;
                }

            }
        }
        else
        {
            printf("ERREUR : La création du dossier n'a plus aboutir.");
            return 1;
        }

    }
    else // Si fichier
    {
    int blocksize = sb.st_blksize;
    char *buffer = malloc(blocksize*sizeof(char));       //Buffer

        #ifdef debug
        printf("DEBUG : Il semble que %s soit un fichier... \n", pathSrc);
        #endif // debug

        //Ouverture du fichier source
        if ((pFileSrc = fopen(pathSrc,"rb")) == NULL)
        {
            printf("ERREUR : Le fichier source que vous avez spécifié n'a plus être ouvert. Vérifier que vous avez les droits nécessaires, et que ce fichier existe. Saisir cp -h pour obtenir de l'aide \n");
            return 1;
        }

        //Ouverture du fichier de destination
        if ((pFileDest = fopen(pathDest,"wb")) == NULL)
        {
            fclose(pFileSrc);
            printf("ERREUR : Le fichier destination que vous avez spécifié n'a plus être ouvert ou créé. Vérifier que vous avez les droits nécessaires. Saisir cp -h pour obtenir de l'aide \n");
            #ifdef debug
            printf("DEBUG : pathSrc : %s pathDest %s \n", pathSrc, pathDest);
            #endif
            return 1;
        }
        //Copie du fichier.
        while ((etatLecture = fread(buffer, 1, blocksize, pFileSrc)) != 0)
        {
            printf("Copie du répertoire \"%s\" vers \"%s\" \n", pathSrc, pathDest);
            printf("%s", buffer);
            fwrite(buffer, 1, etatLecture, pFileDest);
        }
        #ifdef debug
            printf("DEBUG : Il semble que le fichier a bien été créé... La suite maestro !\n");
        #endif
        //Fermeture Propre des fichiers.
        fclose(pFileDest);
        fclose(pFileSrc);
    }
}

int MyCp(int argc, char * argv[])
{
    // Déclaration variable pour arguments

    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "h" ; // Les options disponibles

    int help = 0; // -h

    char * pathSrc = NULL; // Stock le chemin source
    char * pathDest = NULL; // Stock le chemin destination



 // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser cp -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
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
        printf("Pour utiliser la fonction cp taper : cp [-options] [chemin source] [chemin destination] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        return 0;
    }

    /* ******** Récupération du fichier source et du chemin de destination ***************/
    if (argc > 1)
    {
        int i;
        for (i = 1; i<argc; i++)
        {
            if(argv[i][0] != '-')
            {
                if(pathSrc != NULL)
                {
                    if (pathDest != NULL)
                    {
                        printf("ERREUR : Vous ne pouvez spécifier qu'une seule destination. Saisir cp -h pour obtenir de l'aide \n");
                        return 1;
                    }
                    else
                    {
                        pathDest = argv[i];
                    }
                }
                else
                {
                    pathSrc = argv[i];
                }
            }
        }
    }
    /* ******** Verification que la commande contient bien une source et une destination ***************/
    if (pathSrc == NULL)
    {
        printf("ERREUR : Veuillez spécifier le nom du chemin source. Saisir cp -h pour obtenir de l'aide. \n");
        return 1;
    }
    if (pathDest == NULL)
    {
        printf("ERREUR : Veuillez spécifier le nom du chemin de destination. Saisir cp -h pour obtenir de l'aide. \n");
        return 1;
    }


    /* ******** Coeur de la fonction ***************/
    int retour = copie(pathSrc, pathDest);
    return retour;
}
