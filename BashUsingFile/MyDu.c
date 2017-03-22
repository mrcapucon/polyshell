#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>


unsigned int duRecusive(char *fichier)  //Fonction recursive de Du pour calculer la taille de chaque fichier
{
	struct stat st;
	char path[PATH_MAX];
	unsigned int taille;   //unsigned pour doublé la taille du int
	DIR *dossier;
	struct dirent *nomDos;

	if (lstat(fichier,&st)==-1)    //Si on a un probleme avec le fichier visé alors :
	{
		fprintf(stderr,"Probleme (%d) %s\n",errno,strerror(errno));
		return 1;
	}
	taille=st.st_size; //variable qui va servir a incrementer la taille

	if (S_ISDIR(st.st_mode))
	{
		if ((dossier=opendir(fichier))==NULL){
			fprintf(stderr,"Probleme (%d) %s\n",errno,strerror(errno));
            return 1;
        }
		else
		{
			while ((nomDos=readdir(dossier)))  //Tant qu'on peut voir le dossier alors on calcule la taille du dossier
			{
				if (!strcmp(nomDos->d_name,".") || !strcmp(nomDos->d_name,"..")) continue;  // si . et ..  alors on passe

				if (fichier[strlen(fichier)-1]=='/') sprintf(path,"%s%s",fichier,nomDos->d_name);
				else sprintf(path,"%s/%s",fichier,nomDos->d_name);

				taille+=duRecusive(path);
			}
			closedir(dossier);
		}
	}
	return(taille);    // On retourne la taille du dossier
}

int MyDu(int argc, char *argv[])
{
	unsigned int taille;

    //Partie getopt
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "ha" ; // Les options disponibles
    int help = 0; //option h
	int a = 0;	// Pour afficher aussi les fichiers

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
			case 'a':
				a = 1;
				break;
            default:
                printf("Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }

    if (help == 1)
    {
        printf("Pour utiliser la fonction 'du' taper : du [-options]\n");
        printf("Du : Statistiques sur l'utilisation du disque. \n");
        printf("\t -h : Affiche l'aide. \n");
		printf("\t -a : Affiche les dossiers et les fichiers \n");
        return 0;
    }

	struct stat sb; //fichier
	char * path = NULL; // Stock le chemin demandé

	// Récupérer le répertoire
	if (argc > 1)
	{
		int i;
		for (i=1; i<argc ;i++)
		{
			if(argv[i][0] != '-')
			{
				if(path != NULL)
				{
					printf("ERREUR : Vous ne pouvez spécifier qu'un unique chemin. Saisir -h pour obtenir de l'aide. \n");
					return 1;
				}
				else
				{
					path = argv[i];
				}
			}
		}
	}

	// Mise par défaut à "./" si pas de spécifications de chemin.
	if (path == NULL)
		path = ".";

	//Définition Directory
	DIR *dirp;
	struct dirent *dptr;

	// Test l'ouverture du chemin et ouverture si OK, erreur sinon.
	if ((dirp=opendir(path)) == NULL)
	{
		printf("Erreur, impossible d'ouvrir le répertoire");
		return 1;
	}

	//Pour chaque entrée du répertoire...
	while((dptr=readdir(dirp)))
	{
		stat(dptr->d_name, &sb); // Obtenir des informations sur le fichier analysé

		if ((dptr->d_name[0] != '.') && (opendir(dptr->d_name)!= NULL)) // Inverse de ( fichier commençant par . et si fichier )
		{
				// Nom de fichier
				printf("./%-20.19s", dptr->d_name);

				// Taille
				taille=0;
				taille=duRecusive(dptr->d_name)/515;	// Pour avoir la meme unité que le 'du' du bash
				printf("%u\n",taille);
		}

		if ( a == 1 && (dptr->d_name[0] != '.')){
			// Nom de fichier
			printf("%-20.19s", dptr->d_name);

			// Taille
			taille=0;
			taille=duRecusive(dptr->d_name)/515;	// Pour avoir la meme unité que le 'du' du bash
			printf("%u\n",taille);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
    return MyDu(argc, argv);
}
