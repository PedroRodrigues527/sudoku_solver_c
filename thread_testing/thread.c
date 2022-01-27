/*
* Testing threads and his functionalities
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


//Child thread executes this
void *entry_point(void *value){
    printf("Hello from the other side!");

    int *num = (int*) value;

    printf("value: %d", *num);

    return NULL;
}

int main(){
    pthread_t thread;

    printf("First thread \n");

    int num = 123;

    //Args: ref to thread struct; atributes; Func. entry point; value to send to function
    pthread_create(&thread, NULL, entry_point, &num);

    //Args: value of the struct;
    pthread_join(thread, NULL); //Ensures the main thread wait for the second thread finishes

    return EXIT_SUCCESS;
}


