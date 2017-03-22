#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> // Pour les arguments

// Fonction echoarg avec écriture de l'entrée arg return 0 si ca marche et print le message sinon return 1 et écrit qu'il y a un probleme
int MyEcho(int argc, char *argv[])
{
    char opt; // Pour stocker les options passé en paramètre 1 à 1
    char * options = "hna" ; // Les options disponibles

    int help = 0; //option h
    int n = 0; // option n : Enleve le retour chariot (ENTRÉE)
    int a = 0; //option e : retour chariot a chaque mot. Option non présente dans le echo de base.
    int i ;
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
            case 'n':
                n = 1;
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
        printf("Pour utiliser la fonction echo taper : echo [-options] \n");
        printf("Echo : affiche le message écrit au clavier. Ps: Vous pouvez utiliser les variables d'environnements. \n");
        printf("\t -h : Affiche l'aide. \n");
        printf("\t -n : Permet de ne pas faire de retour chariot (ENTRÉE). \n");
        printf("\t -a : Permet de sauter une ligne a chaque mot. \n");
        return 0;
    }

    if (n == 1)     //sans retour chariot
    {
        for(i=1;i<argc;i++)
		{
            if(*argv[i] == '$'){     //Je regarde si ia un $ pour pouvoir savoir si var d'env si oui j'affiche, pointeur car je compare avec un integer
                char *res;
                res = argv[i]+1;
                if((res=getenv(res))!=NULL){
	               printf("%s\n",res);
                }
	            else{
	               printf("%s\n",argv[i]);
               }
            }
            else{
		        printf("%s", argv[i]);
            }
		}
		return 0;
    }

	if (a == 1) 	//Avec un saut de ligne a chaque mot
	{
		for(i=1;i<argc;i++)
		{
            if(*argv[i] == '$'){     //Je regarde si ia un $ pour pouvoir savoir si var d'env si oui j'affiche, pointeur car je compare avec un integer
                char *res;
                res = argv[i]+1;
                if((res=getenv(res))!=NULL){
	               printf("%s\n",res);
                }
	            else{
	               printf("%s\n",argv[i]);
               }
            }
            else{
		        printf("%s \n", argv[i]);
            }
		}
		return 0;
	}
	else	//Sinon c'est le echo de base avec retour chariot.
	{
        for(i=1;i<argc;i++)
		{
            if(*argv[i] == '$'){     //Je regarde si ia un $ pour pouvoir savoir si var d'env si oui j'affiche, pointeur car je compare avec un integer
                char *res;
                res = argv[i]+1;
                if((res=getenv(res))!=NULL){
	               printf("%s\n",res);
                }
	            else{
	               printf("%s\n",argv[i]);
               }
            }
            else{
		        printf("%s ", argv[i]);
            }
		}
		printf("\n");
		return 0;
    }
	return 0;
}

