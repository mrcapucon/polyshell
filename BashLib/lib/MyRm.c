#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> // Pour les arguments

#include <sys/types.h>  //Partie rmdir
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>



int supprDossier(char const *nomdufic){ //Fonction qui supprime le dossier fais en recursive qui parcours un dossier et supprime les fics et si trouve un dossier et refais de meme.
   DIR *repertoire;
   struct dirent *entrerep;     // Entrée dans un répertoire.
   struct stat infoFic;    // Info sur le fic
   char tab[1024] = {0}; //Tableau qui sert a stocker le chemin d'accès complet des fic et dos

   repertoire = opendir(nomdufic);   //On ouvre le dossier demandé
   if ( repertoire == NULL ) {   //Si on peut pas ouvrir alors le dossier est suppr on return et on affiche un msg
       fprintf(stderr, "fichier suppr \n");
       return 0;
   }

   while ( (entrerep = readdir(repertoire)) != NULL ) {
       if ( strcmp(entrerep->d_name, ".") == 0 || strcmp(entrerep->d_name, "..") == 0 ) { // On fais attention de ne pas suprimer les fichiers cachés
           continue;
       }
       snprintf(tab, 1024, "%s/%s", nomdufic, entrerep->d_name);    // Construction du chemin, avec comme séparateur '/'

       stat(tab, &infoFic);    //informations sur le fichier
       if ( S_ISREG(infoFic.st_mode) ) {  //Si c'est un fichier alors on le supprime
           remove(tab);
       }
       else if ( S_ISDIR(infoFic.st_mode) ) { //Si c'est un dossier alors on rappel la fonction
           supprDossier(tab);
       }
   }
   closedir(repertoire);
   remove(nomdufic);    //Le dossier est vide alors on le suppr
   return 0;
}

int MyRm(int argc, char *argv[])
{

	//Partie getopt
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "hcd" ; // Les options disponibles
    int i;
    int help = 0; //option h
    int c = 0; // option c : Demande une confirmation avant de supprimer, faire attention peut etre dangereux si on ne l'uilise pas.
    int d = 0; // option d : Permet de supprimer tout les fichiers d'un dossier. Faire attention peut etre dangereux.
    //int n = 0; //suppression normal mais mis en arg pour pas tout le temps suppr c'est relou

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
            case 'c':
                c = 1;
                break;
            case 'd':
                d = 1;
                break;
            default:
                printf("Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }


    if (help == 1)
    {
        printf("Pour utiliser la fonction rm taper : rm [-options] [nom_du_fichier] [nom_du_fichier_2] ...\n");
        printf("Echo : Supprime des fichiers. \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t -c : Demande la confirmation avant de supprimer. \n");
        printf("\t -d : Pour supprimer un dossier et tout ses fichiers. \n");
        return 0;
    }

    else if (c == 1)	//confirmation
    {
        printf("Souhaitez vous vraiment supprimer ce fichier ? Entrez oui pour supprimer et non pour annuler.\n");
		char yon[10];
        char valide[]="oui\n";
		fgets(yon, 10, stdin);
        printf("[%s]\n",yon );

		if(strcmp(yon,valide)==0)
		{
			printf("Vous avez selectionnez oui. Suppresion.\n");
    		for(i=1;i<argc;i++)
			{
    		    remove(argv[i]);
    		}
    		return 0;
		}
		else
		{
			printf("Vous avez selectionnez non. Annulation de la suppression.\n");
            return 1;
		}
    }
    else if (d == 1)	//confirmation
    {
        for(i=1;i<argc;i++)
        {
            supprDossier(argv[i]);
        }
        printf("Suppresion du dossier effectué ! \n");
    }
    else{  //Sinon il n'y a pas d'option alors on suprime le fic
		for( i=1;i<argc;i++)
		{
		    remove(argv[i]);
		}
		printf("Suppresion effectué.\n");
		return 0;
	}

	return 0;
}

