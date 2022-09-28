#include "headers.h"

int main(int argc, char** argv) {
    char* X = argv[1];
    char* Y = argv[2];
    char* Z = argv[3];
    pid_t pid;
    int status;
    int port[2];
    char str [20];

    if (pipe(port) < 0) {
        perror("Pipe error\n");
        exit(1);
    }

    pid = fork();

    if (pid < 0) { //Failed fork
        printf("Fork failed\n");
        exit(1);
    }

    if (pid > 0) { //Parent process
        printf("parent (PID %d): created child (PID %d)\n", getpid(), pid);
        read(port[0], &str, strlen(str)); //Reading X from the pipe, will wait until there is something to read
        printf("%s\n", str);
        printf("parent (PID %d): read from pipe %s\n", getpid(), str);
        printf("parent (PID %d): received Y = \"%s\"\n", getpid(), Y);
        strcat(str, Y); //Concatenating read string and Y
        printf("parent (PID %d): writing into pipe \"%s\"\n", getpid(), str);
        write(port[1], str, strlen(str)); //Writing concatenated strings to pipe
        read(port[0], &str, strlen(str)); //Will wait until there is something to read
        printf("parent (PID %d): read from pipe \"%s\"\n", getpid(), str);
        printf("parent (PID %d): all tasks completed\n", getpid());
    }

    if (pid == 0) { //Child process
        printf("child (PID %d): received X = \"%s\"\n", getpid(), X);
        printf("child (PID %d): writing \"%s\" into pipe\n", getpid(), X);
        write(port[1], X, strlen(X)); //Writing X to the pipe
        read(port[0], &str, strlen(str)); //Will wait until there is something to read
        printf("child (PID %d): read from pipe \"%s\"\n", getpid(), str);
        printf("child (PID %d): received Z = \"%s\"\n", getpid(), Z);
        printf("child (PID %d): \"%s\" + Z = \"%s\"\n", getpid(), str, strcat(str, Z));
        printf("child (PID %d): writing into pipe \"%s\"\n", getpid(), str);
        write(port[1], str, strlen(str)); //Write concatenated string into pipe
        printf("child (PID %d): all tasks completed\n", getpid());
    }
    return 0;
}
