        POLYSHELL VERSION 1.0 
        
        
            make pour créer les trois versions du shell 
                
                bashfile est la version cherchant à executer  les commandes à partir des fichiers contenus dans le repertoire principal ou dans les rep de PATH
                
                bashfull est la version ou toutes les commandes sont compilées avec le shell
                
                bashlib est la version qui genere libFunc.so une librairie dynamique contenant toute les fonctions
            
            Pour lancer le serveur il sufft de taper launchServ dans polyshell. Pour le fermer tapez quitServ
            
            Pour se connecter au serveur lancez client de la maniere suivante ./client localhost numPortServeur 
            
            Make clean pour vider tout le contenu autre que les sources .c et les headers .h
            
            