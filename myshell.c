#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
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
        if(mycmdLine == NULL) continue;
        if(strcmp(mycmdLine->arguments[0],"cd")==0){
            if(chdir(mycmdLine->arguments[1])==-1){
                fprintf(stderr,"Error changing directory!\n");
            }
            freeCmdLines(mycmdLine);
            continue;
        }
        if(strcmp(mycmdLine->arguments[0],"stop")==0){
            char* id=mycmdLine->arguments[1];
            int int_id=atoi(id);
            kill(int_id,SIGTSTP);
            freeCmdLines(mycmdLine);
            continue;
        }
        if(strcmp(mycmdLine->arguments[0],"wakeup")==0){
            char* id=mycmdLine->arguments[1];
            int int_id=atoi(id);
            kill(int_id,SIGCONT);
            freeCmdLines(mycmdLine);
            continue;
        }
        if(strcmp(mycmdLine->arguments[0],"ice")==0){
            char* id=mycmdLine->arguments[1];
            int int_id=atoi(id);
            kill(int_id,SIGINT);
            freeCmdLines(mycmdLine);
            continue;
        }
        if(strcmp(mycmdLine->arguments[0],"nuke")==0){
            char* id=mycmdLine->arguments[1];
            int int_id=atoi(id);
            kill(-int_id,SIGKILL);
            freeCmdLines(mycmdLine);
            continue;
        }
        pid_t pid=fork();
        if(pid==0){ 
            if(mycmdLine->inputRedirect != NULL){
                int fd = open(mycmdLine->inputRedirect, O_RDONLY);
                if(fd == -1){
                    perror("open input failed");
                     _exit(1); 
                }
                dup2(fd, STDIN_FILENO);  
                close(fd);
            }
            if(mycmdLine->outputRedirect != NULL){
                int fd = open(mycmdLine->outputRedirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd == -1){
                    perror("open output failed");
                     _exit(1); 
                }
                dup2(fd, STDOUT_FILENO); 
                close(fd);
            }
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
