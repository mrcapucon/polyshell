#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <assert.h>

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
char CurrentDir[255];




char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char * printdir(char *dir, char * file)
{
    DIR *dp;
    char * ret = malloc(sizeof(char)*1024);
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return NULL;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            //printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            ret=printdir(entry->d_name,file);
            if(ret!=NULL){
                chdir("..");
                closedir(dp);
                return ret;
            }
            
        }
        else {
            if(strcmp(file,entry->d_name) == 0){
                getcwd(ret,1024);
                strcat(ret,"/");
                strcat(ret,file);
                return ret;
            }
        }
    }
    chdir("..");
    closedir(dp);
    return NULL;
}

char * getFunc(char * cmd,int * System){
    int i ;
    char * path =NULL;
    char ** token ;
    path=printdir(CurrentDir,cmd);
    if(path!=NULL){
        *(System)=1;
        chdir(CurrentDir);
        return path;
    }
    else{
        path=getenv("PATH");
        token=str_split(path,':');
        for (i = 0; *(token + i); i++){
            path=printdir(*(token+i),cmd);
            if(path!=NULL){
                *(System)=0;
                chdir(CurrentDir);
                return path ;
            }
            
        }
        
    }
   
   
    return NULL ;
    
}

int main()
{
    getcwd(CurrentDir,255);
    char * tmp = malloc(sizeof(char)*1024);
    int test =-1;
    tmp = getFunc("ls",&test);
    printf("done %s. %d \n",tmp,test);
    free(tmp);
    //execl(path,path,NULL);
    exit(0);
}