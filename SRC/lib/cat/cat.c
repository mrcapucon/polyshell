#include <stdlib.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <getopt.h> // Pour les arguments

int cat(int argc, char * argv[])
{
    char c; // Pour l'affichage caractère à caractère.
    char * path = NULL; //Stockage du path.
    FILE *pFile; // Initialise la variable Fichier.

     // Déclaration variable pour arguments
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "hn" ; // Les options disponibles
    int help = 0; // -h
    int number = 0;
// Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser cat -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
                break;
            case 'n':
                number = 1;
                break;
            default:
                printf("ERREUR : Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }

// Rubrique AIDE
 if (help == 1)
        // Si -h
    {
        printf("Pour utiliser la fonction cat taper : cat [-options] [fichier] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t -n : Affiche le nombre de chaque ligne. \n");
        return 0;
    }

  // Récupérer le PATH en argument
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
	{
        printf("ERREUR : Vous devez spécifier un chemin vers un fichier. Exemple: cat monfichier . Saisir -h pour obtenir de l'aide. \n");
		return 1;
    }
    // Vérifier que le path est bien un fichier régulier.
    struct stat sb; //buffer
    stat(path, &sb); //récup info sur le path
    if ( (sb.st_mode & S_IFMT) != S_IFREG)
    {
        printf("ERREUR : Le fichier ciblé n'est pas un fichier régulier. Saisir -h pour obtenir de l'aide. \n");
        return 1;
    }


    // Affichage du fichier

    int n = 1; // Nb Ligne : pour option -n
    printf("%d \t", n);
    if (pFile=fopen(path,"r"))
    {
        while ( ((c=fgetc(pFile)) !=EOF) || (!feof(pFile)) )
        {
            printf("%c", c);
            if (number == 1)
            {
                if (c == '\n')
                {
                    n++;
                    printf("%d \t", n);
                }
            }
        }
    }
    printf("\n");
    return 0;
}
int main(int argc, char * argv[])
{
    cat(argc, argv);
    return 0;
}
