#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int fd[2];  // fd[0]=read end, fd[1]=write end
    pipe(fd);

    pid_t pid = fork();
    if(pid == 0){
        close(fd[1]);                         
        char buf[2048];
        int n = read(fd[0], buf, sizeof(buf));
        buf[n] = '\0';
        printf("Child received: %s\n", buf);
        close(fd[0]);
    } else {
        close(fd[0]);                           
        write(fd[1], argv[1], strlen(argv[1]));
        close(fd[1]);                           
    }
    return 0;
}
