#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ptrace.h>

void debugger();

int main(int argc, char** argv){
    //Will allow used to include the compiled program (In debug mode. For now, only 2 arguments allowed)
    if(argc < 2 || argc >2){
        printf("Number of Arguments is to little or too much\n");
        return -1;
    }
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
       //rintf("Child process ID is: %i\n",childProcessID);
        /**
    * Will make use of the ptrace system call which provides the mechansim for one process to trace a child process
    * 
    */
   if(ptrace(PTRACE_TRACEME,0,NULL,NULL) == -1){
    printf("ptrace system call failed\n");
    exit(-1);
   }

   execl(argv[1],argv[1],NULL);
        
    }else{
        //parent process
        pid_t parentProcessID = getpid();
       //rintf("Parent process ID is: %i\n",parentProcessID);
    }
}

