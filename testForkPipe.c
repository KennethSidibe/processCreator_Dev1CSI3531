#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char* concatenate(char* string1, char* string2) {
    
    int len1 = strlen(string1);
    int len2 = strlen(string2);

    char* result = malloc(len1 + len2 + 1);

    int i;
    for(i=0; i < len1;i++) {
        result[i] = string1[i];
    }

    i=0;

    for(i=0; i < len2;i++) {
        result[i+len1] = string2[i];
    }

    result[len1+len2+1] = '\0';


    return result;

}

int main() {

    // Program create two processes P1 and P2
    // P1 takes a string and passes to P2
    // P2 concatenate string provided by P1 without string func
    // P2 sends concatenated string to P1
    // P1 prints results given by P2

    char* url = malloc(100);
    int fd1[2];
    int fd2[2];
    int status;
    char urlRemain[] = ".kennethsidibe.com";
    

    if(pipe(fd1) == -1){
        printf("pipe 1 error");
        return 1;
    }

    if(pipe(fd2) == -1){
        printf("pipe 2 error");
        return 1;
    }
    
    char* userInput = malloc(100);

    printf("Enter a URL: ");
    scanf("%s", userInput);

    printf("here is the user input : %s\n", userInput);

    int P1 = fork();

    if (P1 < 0) {
        printf("Error while creating P1");
        return 1;
    }

    // Create process P1
    else if(P1 > 0){

        char concat_string[100];

        close(fd1[0]); //close pipe 1 read end 

        write(fd1[1], userInput, strlen(userInput)+1); //write userInput to pipe

        printf("Finished writing to pipe \n");

        close(fd1[1]); //close write input

        wait(NULL);

        close(fd2[1]); //close child write pipe 

        read(fd2[0], concat_string, sizeof(concat_string));

        printf("concatenated string : %s\n", concat_string);

        close(fd2[0]); //Close child read pipe
    }

    // P1 is executing 
    else{

        char resultProcess[100];

        close(fd1[1]); //Close write pipe 1

        read(fd1[0], resultProcess, sizeof(resultProcess));

        url = concatenate(resultProcess, urlRemain);

        close(fd1[0]); //close pipe 1 reader
        close(fd2[0]);

        write(fd2[1], url, strlen(url) + 1);
        close(fd2[1]);

        // Finished concat

        exit(0);
    }

    return 0;

}