#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ptrace.h>
#include<sys/wait.h>

void debugger(char** argv);
int getInput(char* input);

int main(int argc, char** argv){
    //Will allow used to include the compiled program (In debug mode. For now, only 2 arguments allowed)
    if(argc < 2 || argc >2){
        printf("Number of Arguments is to little or too much\n");
        return -1;
    }
    //printf("Welcome to this simple debuger");
    debugger(argv);
    return 0;
}


void debugger(char** argv){
    pid_t processCreate = fork();
    if(processCreate < 0){
        printf("Unable to create child process\n");
        exit(-1);
    }else if(processCreate == 0){
        //child process created
        pid_t childProcessID = getpid();
        if(ptrace(PTRACE_TRACEME,0,NULL,NULL) == -1){
            printf("ptrace system call failed\n");
            exit(-1);
        }
        if(execl(argv[1],argv[1],NULL) == -1){
            printf("Error occured");
            exit(-1);
        }
        
    }else{
        //parent process
        //pid_t parentProcessID = getpid();
        printf("For help, type \"help\".\n");
        printf("(No debugging symbols found in ./test)\n");
        int status;
        waitpid(processCreate, &status, 0);
        while(1){
            //ptrace(PTRACE_CONT, processCreate, NULL, NULL);
            waitpid(processCreate, &status, 0);
        }
    }
}

int getInput(char* input){

}
