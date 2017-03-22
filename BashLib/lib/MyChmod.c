#include <stdlib.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h> // isdigit ....

#include <getopt.h> // Pour les arguments


// Pour le parsing de la commande
enum Etat
{
    Etat0Commande,
    Etat1Options,
    Etat2Droits,
    Etat3Paths
};


int estDigit(char * string)
{
    int i;
    for (i=0; i< sizeof(string) ; i++)
    {
        if (string[i] == '\0')
            break;

        if (!(isdigit(string[i])))
        {
            return -1;
        }
    }
    return 0;
}

mode_t octal2Mode(int octets)
{
    // Déclarations variables d'entrée et de sortie;
    int user;
    int owner;
    int group;

    mode_t userM;
    mode_t ownerM;
    mode_t groupM;

    // Vérification de la cohérence des droits & divisions par catégorie:
    if (octets >= 0 && octets<=777)
    {
        //Récupération des droits concernants les Guests
        user = octets%10;
        octets /= 10;

        //Récupération des droits concernants les users du groupe.
        group = octets%10;
        octets /= 10;

        //Récupération des droits concernants le propriétaire.
        owner = octets;
    }
    else
    {
        printf("ERREUR : Les droits saisis au format octal sont invalide et doit être compris entre 0 et 777. Saisir chmod -h pour plus d'informations.\n");
        return -1;
    }

    // Vérification cohérence dans chaque catégorie.
    if (user > 7 || group > 7 || owner > 7)
    {
        printf("ERREUR : Les droits saisis au format octal sont invalide. Saisir chmod -h pour plus d'informations.\n");
        return -1;
    }

    //Conversion int en mode_t
    //Le proprio
    switch (owner)
    {
        case 0: break;
        case 1: ownerM = S_IXUSR;                break;
        case 2: ownerM = S_IWUSR;                break;
        case 3: ownerM = S_IWUSR | S_IXUSR;      break;
        case 4: ownerM = S_IRUSR;                break;
        case 5: ownerM = S_IRUSR | S_IXUSR;      break;
        case 6: ownerM = S_IRUSR | S_IWUSR;      break;
        case 7: ownerM = S_IRWXU;                break;
        default: printf("ERREUR: Les droits du propriétaire sont incorrect. Saisir chmod -h pour obtenir de l'aide \n"); break;
    }
    //Utilisateurs de groupe
    switch (group)
    {
        case 0: break;
        case 1: groupM = S_IXGRP;                break;
        case 2: groupM = S_IWGRP;                break;
        case 3: groupM = S_IWGRP | S_IXGRP;      break;
        case 4: groupM = S_IRGRP;                break;
        case 5: groupM = S_IRGRP | S_IXGRP;      break;
        case 6: groupM = S_IRGRP | S_IWGRP;      break;
        case 7: groupM = S_IRWXG;                break;
        default: printf("ERREUR: Les droits GROUPE sont incorrect. Saisir chmod -h pour obtenir de l'aide \n"); break;
    }

    //Tout Utilisateurs
    switch (user)
    {
        case 0: break;
        case 1: userM = S_IXOTH;                break;
        case 2: userM = S_IWOTH;                break;
        case 3: userM = S_IWOTH | S_IXOTH;      break;
        case 4: userM = S_IROTH;                break;
        case 5: userM = S_IROTH | S_IXOTH;      break;
        case 6: userM = S_IROTH | S_IWOTH;      break;
        case 7: userM = S_IRWXO;                break;
        default: printf("ERREUR: Les droits GROUPE sont incorrect. Saisir chmod -h pour obtenir de l'aide \n"); break;
    }

    return userM | groupM | ownerM;
}

int MyChmod(int argc, char * argv[])
{
   // Déclaration variable pour arguments

    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "h" ; // Les options disponibles

    int help = 0; // -h

    char * path = NULL; // Stock le chemin demandé
    mode_t mode = octal2Mode(755); // Stock la variable droits (par défaut 755)

 // Récupérer les options
    while ((opt = getopt (argc, argv, options)) != -1)
    {
        switch(opt)
        {
            case '?':
                printf("ERREUR : Un des paramètres n'est pas reconnu. Utiliser chmod -h pour obtenir de l'aide. \n");
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

    if (help == 1)
        // Si -h
    {
        printf("Pour utiliser la fonction chmod taper : chmod [-options] [(Nom Du Nouveau Répertoire)] \n");
        printf("Astuce : vous pouvez concaténer les options : -l -a = -la \n \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t \n");
        return 0;
    }

    int i;
    enum Etat etatParse = Etat2Droits;

    // Récupérer le répertoire et le mode (Parsing de la commande)
    if (argc > 1)
    {
        for (i = 1; i<argc; i++)
        {

        //if (argv[i][0] == '-') etatParse = Etat1Options;

            switch(etatParse)       // ICI, on implémente un parsing à état (comme au TP1 de C)
            {
                case Etat1Options:
                    if(argv[i][0] == '-')
                    {
                        // On ne fait rien, le getOpt ci-dessus, s'en charge très bien ! :) !
                    }
                    else
                    {
                        etatParse = Etat2Droits;
                    }

                    break;
                case Etat2Droits:
                    if(estDigit(argv[i]) == 0)      //On regarde si la string est au format numérique
                    {
                        mode = octal2Mode(atoi(argv[i])); // atoi converti un String vers un int
                        etatParse = Etat3Paths;
                    }
                    else
                    {
                        ///TODO : les droits sont au format textuel
                        etatParse = Etat3Paths;
                    }
                break;
                case Etat3Paths:
                    etatParse = Etat3Paths;
                    path = argv[i];
                    break;

                default:
                    printf("ERREUR : Une erreur est survenu lors du parsing. Saisir chmod -h pour obtenir de l'aide. \n");
                    break;
            }
        }
    }

    if (path == NULL)
    {
        printf("ERREUR : Veuillez spécifier le nom d'un répertoire ou d'un fichier. Saisir chmod -h pour obtenir de l'aide. \n");
        return 1;
    }

    if( (chmod(path,mode)) != -1)
    {
        printf("Les changements des droits ont bien été effectués sur le fichier/dossier %s \n",path);
    }
    else
    {
        printf("ERREUR : Impossible d'affecter les droits au répertoire/fichier %s . Saisir chmod -h pour obtenir de l'aide. \n", path);
        return 1;
    }


    return 0;
}
