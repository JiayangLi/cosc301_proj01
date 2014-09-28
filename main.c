/*
 * Project 1 Data scanner and listifier
 * Jiayang Li
 * 9-28-2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"
#define MAX_SIZE 128

/*Borrowed from previous lab*/
char** tokenify(const char *s) {
    char delimiters[] = " \t\n";
    char *copy = strdup(s); //first copy to get the size 
    char *token = strtok(copy, delimiters);

    int counts = 1; //account for NULL

    for (; token != NULL; token = strtok(NULL, delimiters)){
        counts++;
    }

    free(copy);

    char **rv = malloc(counts * sizeof(char *));

    char *copy1 = strdup(s);    //second copy to tokenify
    
    int i = 0;
    token = strtok(copy1, delimiters);

    while (token != NULL){
        *(rv + i) = strdup(token);  //rv[i]
        token = strtok(NULL, delimiters);
        i++;
    }
    *(rv + i) = NULL;

    free(copy1);

    return rv;
}

/*Borrowed from previous lab*/
void free_tokens(char **tokens) {
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

/*Check an input string, return 1, 0 or -1 for different cases*/
int check_input(char *s){
    int i;  

    if (*s == '-'){
        i = 1;  //potential negative number, start check from index 1
    }
    else{
        i = 0;  //potential positive number, start check from index 0
    }

    for (; i < strlen(s); i++){
        if (s[i] == '#'){
            return -1;  // -1 is comment
        }
        if (!isdigit(s[i])){
            return 0;   // 0 is invalid input
        }
    }
    //every character is digit
    return 1;   // 1 is valid input
}

/*Helper function to get user and system time in microseconds*/
void get_time(long int *utime, long int *stime, struct rusage *usage){
    if (getrusage(RUSAGE_SELF, usage) == -1){
        perror("Error in getrusage()"); //handle error with getrusage
    }

    *utime = (usage->ru_utime).tv_sec * 1000000 + (usage->ru_utime).tv_usec;
    *stime = (usage->ru_stime).tv_sec * 1000000 + (usage->ru_stime).tv_usec;
}

void process_data(FILE *input_file){
    struct rusage usage;
    long int utime_start, utime_end, stime_start, stime_end;
    
    //get start time
    get_time(&utime_start, &stime_start, &usage);

    struct node *head = NULL;
    char buffer[MAX_SIZE];
    char **tokens = NULL;

    while (fgets(buffer, MAX_SIZE, input_file) != NULL){
        tokens = tokenify(buffer);
        for (int i = 0; tokens[i] != NULL; i++){
            int input_validity = check_input(tokens[i]);
            if (input_validity == 1){
                list_insert(atoi(tokens[i]), &head);
            }
            else if (input_validity == -1){
                break;
            }
        } //end for
        free_tokens(tokens);
    } //end while

    printf("*** List Contents Begin ***\n");
    list_print(head);
    printf("*** List Contents End ***\n");

    list_clear(head);

    //get end time
    get_time(&utime_end, &stime_end, &usage);

    long int utime = utime_end - utime_start;
    long int stime = stime_end - stime_start;

    printf("user time: %ld.%lds\n", utime/1000000, utime%1000000);
    printf("stime_end: %ld.%lds\n", stime/1000000, stime%1000000);
   
}


void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
