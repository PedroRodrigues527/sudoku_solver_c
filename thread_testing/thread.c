
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>


void* func1(){
    printf("***Thread1*** \n");
    int i = 0;
    for(i; i< 100; i++ ){
        printf("%d",i);
        printf("\n");
    }
}

void* func2(){
    printf("***Thread2*** \n");
    int k = 100;
    for(k; k< 200; k++ ){
        printf("%d",k);
        printf("\n");
    }
}

void* func3(){
    printf("***Thread3*** \n");
    int l = 200;
    for(l; l< 300; l++ ){
        printf("%d",l);
        printf("\n");
    }
}



int main(){
    //int i;
    pthread_t id1;
    pthread_t id2;
    pthread_t id3;

    pthread_create(&id1, NULL, func1, NULL);
    pthread_join(id1, NULL);

    pthread_create(&id2, NULL, func2, NULL);
    pthread_join(id2, NULL);

    pthread_create(&id3, NULL, func3, NULL);
    pthread_join(id3, NULL);


    //pthread_exit(NULL);

    return EXIT_SUCCESS;
}
