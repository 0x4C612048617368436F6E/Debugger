#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ptrace.h>
#include<sys/wait.h>
#include<string.h>
#include<libelf.h>
#include<stdint.h>
//#include<libdwarf.h>
#include<fcntl.h>

//LOL, NO POINT :)
#if !defined(BUFFERSIZE)
#define BUFFERSIZE 1025
#endif

void debugger(char** argv);
char* getInput();
char* readDAWRFInfoFromElfFileFormat(char* ELFfileFormat);

int main(int argc, char** argv){
    //Will allow used to include the compiled program (In debug mode. For now, only 2 arguments allowed)
    if(argc < 2 || argc >2){
        printf("Number of Arguments is to little or too much\n");
        return -1;
    }
    //printf("Welcome to this simple debuger");
    //debugger(argv);
    readDAWRFInfoFromElfFileFormat("./hold");
    return 0;
}


void debugger(char** argv){
    /**/
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
        
        //debug loop
        while(1){
            //implement input
            char* userInput = getInput();
            printf("Hel %s\n",userInput);
            fflush(stdout);
            if(strcmp(userInput,"q") == 0){
                //i think we will have to detach the child process so that it will not run
                free(userInput);
                break;
            }else if(strcmp(userInput,"l")){
                //This is where the fun begins
            }
            else{
                continue;
                
                //ptrace(PTRACE_CONT,processCreate,NULL,NULL);
            //perror("Bad: ");
            //printf("%s\n",userInput);
            //waitpid(processCreate,&status,0);
            //waitpid(processCreate, &status, 0);
                
            }
            

        }
    }
}

char* getInput(){
    printf("(debugger) ");
    char c;
    int currentBufferSize = BUFFERSIZE;
    char* userInput = (char*) malloc(sizeof(char)*currentBufferSize);
    if(!userInput){
        perror("Bad Program: ");
        exit(-1);
    }
    int pointer = 0;

    while((c = getc(stdin))!= '\n' && c != EOF){
        if(pointer >= currentBufferSize){
            currentBufferSize+=BUFFERSIZE;
            userInput = (char*) realloc(userInput,sizeof(char)*currentBufferSize);
            if(!userInput){
                perror("Bad Realloc: ");
                exit(-1);
            }
        }
        *(userInput+pointer) = c;
        pointer++;
    }
    
    *(userInput+pointer) = '\0';
    printf("userInput: %s\n", userInput);
    return userInput;

    //readDAWRFInfoFromElfFileFormat(argv[1]);
}

char* readDAWRFInfoFromElfFileFormat(char* ELFfileFormat){
    if (elf_version(EV_CURRENT) == EV_NONE)
    {
	    // library out of date
	    recover from error
        printf("EV_NONE\n");
    }else{
        printf("NOT_NONE\n");
    }
    //now in linux, ELF is a file format for binary executables, shared, etc. We need to read this file for the specific Object file we want to debug and get the DWARF (in each section) information.
    int fd = open(ELFfileFormat, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        fprintf(stderr, "elf_begin failed: %s\n", elf_errmsg(-1));
        close(fd);
        exit(EXIT_FAILURE);
    }
    return "";
}