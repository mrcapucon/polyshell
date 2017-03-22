/*
Fonction LS
Projet C - MiniShell - Polytech Nantes 2016

Suggestion amélioration:
- Ajustement graphique
- Ajout d'options
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>

#include <getopt.h>



// Fonction qui traduit st_mode par le type du fichier analysé.
char* mode2Type(struct stat sb)
{
    char * string;
    switch (sb.st_mode & S_IFMT) {
            case S_IFREG:
                string = "Fichier Régulier";
                break;
            case S_IFDIR:
                string = "Répertoire";
                break;
            case S_IFCHR:
                string = "Périphérique sans stockage";
                break;
            case S_IFBLK:
                string = "Périphérique de stockage";
                break;
            case S_IFLNK:
                string = "Lien symbolique";
                break;
            case S_IFIFO:
                string = "Tube";
                break;
            case S_IFSOCK:
                string = "Socket";
                break;
            default:
                string = "Non Reconnu";
         }

    return string;
}

// Fonction qui traduit st_mode par les droits sur le fichier.
char* mode2Droits(struct stat sb)
{
    char string[12];

    // Propriétaire
    if ((sb.st_mode & S_IRUSR) == S_IRUSR)
        string[0]='r';
    else
        string[0]='-';

    if ((sb.st_mode & S_IWUSR) == S_IWUSR)
        string[1]='w';
    else
        string[1]='-';

    if ((sb.st_mode & S_IXUSR) == S_IXUSR)
        string[2]='x';
    else
        string[2]='-';

    string[3]='-';

    // Groupe du Proprio
    if ((sb.st_mode & S_IRGRP) == S_IRGRP)
        string[4]='r';
    else
        string[4]='-';

    if ((sb.st_mode & S_IWGRP) == S_IWGRP)
        string[5]='w';
    else
        string[5]='-';

    if ((sb.st_mode & S_IXGRP) == S_IXGRP)
        string[6]='x';
    else
        string[6]='-';

    string[7]='-';

    // Visiteur
    if ((sb.st_mode & S_IROTH) == S_IROTH)
        string[8]='r';
    else
        string[8]='-';

    if ((sb.st_mode & S_IWOTH) == S_IWOTH)
        string[9]='w';
    else
        string[9]='-';

    if ((sb.st_mode & S_IXOTH) == S_IXOTH)
        string[10]='x';
    else
        string[10]='-';

    string[11]='\0';

    return string;
}

char * mois2String(int mois) {
	switch(mois) {
		case 0:
			return "janvier";
		case 1:
			return "février";
		case 2:
			return "mars";
		case 3:
			return "avril";
		case 4:
			return "mai";
		case 5:
			return "juin";
		case 6:
			return "juillet";
		case 7:
			return "août";
		case 8:
			return "septembre";
		case 9:
			return "octobre";
		case 10:
			return "novembre";
		case 11:
			return "décembre";
        default:
            return "?mois?";
	}
}

int ls(int argc, char *argv[])
{
    // Récup des structures & déclarations
	struct stat sb; //fichier
    struct tm *tm;  //format time

    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "lha" ; // Les options disponibles
    char * path = NULL; // Stock le chemin demandé

    int help = 0; //option help -h
    int l = 0; // option mode complet -l
    int a = 0; // option fichiers cachés -a


    // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser -h pour obtenir de l'aide. \n");
                return 1;
                break;
            case 'h':
                help = 1;
                break;
            case 'l':
                l = 1;
                break;
            case 'a':
                a = 1;
                break;
            default:
                printf("ERREUR : Une erreur est survenu lors de la saisie des paramètres. \n");
                return 1;
                break;
        }
    }

    // Si -h
    if (help == 1)
    {
        printf("Pour utiliser la fonction ls taper : ls [-options] [répertoires] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t -l : Affiche les informations complètes sur les fichiers analysés. \n");
        printf("\t -a : Affiche les fichiers cachés. \n");
        return 0;
    }


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

    printf("Vous visitez le répertoire suivant: %s \n", path);

    //Définition Directory
	DIR *dirp;
	struct dirent *dptr;

    // Test l'ouverture du chemin et ouverture si OK, erreur sinon.
	if ((dirp=opendir(path)) == NULL)
	{
		printf("Erreur, impossible d'ouvrir le répertoire");
		return 1;
	}

    if (l == 1) // si option -l activé
        printf("%-14s %-3s %-5s %-5s %-22s %-21s %-19s %s \n","Droits","Liens", "User", "Grp", "Taille Blocks", "Dernière MaJ", "Nom","Type");

    //Pour chaque entrée du répertoire...
	while(dptr=readdir(dirp))
	{
        stat(dptr->d_name, &sb); // Obtenir des informations sur le fichier analysé

        if (!(a==0 && dptr->d_name[0] == '.')) // Inverse de ( Si Option -a désactivé && fichier commençant par . )
        {
            if (l == 1) // si option -l activé
            {
                // droits d'accés
                printf( "%-14.12s \t", mode2Droits(sb));

                // Nombre de liens
                printf("%-3ld ", (long) sb.st_nlink);

                // ID User
                printf("%-5.4ld ", (long) sb.st_uid);

                // ID Group
                printf("%-5.4ld ", (long) sb.st_gid);

                // Block Size
                printf("%-15ld \t", (long) sb.st_blksize);

                // Date dernière modif
                // NOTE : tm->tm_year renvoie l'heure à partir de l'an 1900. d'où +1900.
                tm = localtime(&sb.st_mtime);
                printf("%-2.2d %-4.5s %-3.4d %-2.2d:%-5.2d", tm->tm_mday, mois2String(tm->tm_mon), 1900+(tm->tm_year), tm->tm_hour, tm->tm_min);
            }
                // Nom de fichier
                printf("%-20.19s", dptr->d_name);

                // Type de Fichier
                printf( "%s \n", mode2Type(sb));
        }
	}
	return 0;
}

int main(int argc, char *argv[])
{
    ls(argc, argv);
    return 0;
}
