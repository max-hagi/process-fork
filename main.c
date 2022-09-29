#include "headers.h"
#define STRSIZE 50

int main(int argc, char** argv) {

    if (argc != 4) { //Make sure there are only 3 command line arguments passed
        printf("Please include only 3 arguments\n");
        exit(1);
    }

    char* X = argv[1];
    char* Y = argv[2];
    char* Z = argv[3];
    pid_t pid;
    int port[2];
    char str [STRSIZE];
    char temp [STRSIZE]; //Temporary holder for strings read from pipe before concatenation

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
        read(port[0], &str, sizeof (str));
        printf("parent (PID %d): read from pipe \"%s\"\n", getpid(), str);
        printf("parent (PID %d): received Y = \"%s\"\n", getpid(), Y);
        strcpy(temp, str); //Copy read string to temporary holder
        strcat(str, " ");
        printf("parent (PID %d): \"%s\" + Y = \"%s\"\n", getpid(), temp, strcat(str,Y));
        printf("parent (PID %d): writing into pipe \"%s\"\n", getpid(), str);
        write(port[1], str, sizeof (str)); //Writing concatenated strings to pipe
        sleep(1);
        read(port[0], &str, sizeof (str)); //Will wait until there is something to read
        printf("parent (PID %d): read from pipe \"%s\"\n", getpid(), str);
        printf("parent (PID %d): all tasks completed\n", getpid());
    }

    if (pid == 0) { //Child process
        printf("child (PID %d): received X = \"%s\"\n", getpid(), X);
        printf("child (PID %d): writing \"%s\" into pipe\n", getpid(), X);
        write(port[1], X, sizeof (X)); //Writing X to the pipe
        sleep(1);
        read(port[0], &str, sizeof (str)); //Will wait until there is something to read
        printf("child (PID %d): read from pipe \"%s\"\n", getpid(), str);
        printf("child (PID %d): received Z = \"%s\"\n", getpid(), Z);
        strcpy(temp, str);
        strcat(str, " ");
        printf("child (PID %d): \"%s\" + Z = \"%s\"\n", getpid(), temp, strcat(str, Z));
        printf("child (PID %d): writing into pipe \"%s\"\n", getpid(), str);
        write(port[1], str, sizeof (str)); //Write concatenated string into pipe
        printf("child (PID %d): all tasks completed\n", getpid());
    }
    return 0;
}
