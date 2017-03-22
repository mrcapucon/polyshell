#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <getopt.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define debug   // debug

/*

// Busybox : Projet utile pour la fonction..
   /* sp = alloc_procps_scan();
    sp->dir = xopendir("/proc");

    for (;;) {
    entry = readdir(sp->dir);

     pid = bb_strtou(entry->d_name, NULL, 10);

      strcpy(filename_tail, "stat");
       n = read_to_buf(filename, buf);

    p = NULL;
	while ((p = procps_scan(p, need_flags)) != NULL) {
		format_process(p);
	}

	link : http://code.metager.de/source/xref/busybox/procps/
*/

char * PROC_DIR = "/proc";

int MyPs(int argc, char *argv[])
{
    DIR *repProc;   // Rep Proc
    DIR *repPs;     // Rep Ps
    struct dirent* file = NULL;

    if ((rep=opendir(PROC_DIR))==NULL) {
        printf("ERREUR : Impossible d'ouvrir le repertoire des processus...\n");
        exit(EXIT_FAILURE);
    }

    #ifdef debug
    printf("DEBUG : Repertoire /proc ouvert ! \n");
    #endif // debug

    while( (file = readdir(repProc)) != Null )
    {
        if(file->d_type)
    }


}
