#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine){
    execvp(pCmdLine->arguments[0],pCmdLine->arguments);
    perror("execv failed !!! ");
    _exit(1);
} 


int main(int argc, char const *argv[])
{
    int debug=0;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-d")==0){
            debug=1;
            break;
        }
    }
    char cwd[PATH_MAX];
    char buffer[2048];
    while(1){
        getcwd(cwd, PATH_MAX);
        printf("%s> ", cwd);
        fflush(stdout);
        char* mystream=fgets(buffer,2048,stdin);
        if(strncmp(mystream,"quit",4)==0)
            exit(0);
        cmdLine* mycmdLine=parseCmdLines(mystream);
        pid_t pid=fork();
        if(pid==0){
            execute(mycmdLine);
        }
        else {
            if(debug){
                fprintf(stderr, "PID: %d\n", pid);
                fprintf(stderr, "Executing: %s\n", mycmdLine->arguments[0]);
                fprintf(stderr, "%s\n", mycmdLine->blocking ? "foreground" : "background");
            }
            if (mycmdLine->blocking == 1)
                waitpid(pid, NULL, 0);
            freeCmdLines(mycmdLine);
        }   
                 
    }
    return 0;
}
