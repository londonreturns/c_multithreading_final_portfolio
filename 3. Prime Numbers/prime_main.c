#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

struct Range {
    int* numbers;
    int thread_number;
    int start;
    int end;
};

typedef struct Range range;

pthread_mutex_t L;
pthread_mutex_t L2;

int countOfNumbers = 0;
int totalPrimeNumbers = 0;
int max_number = 0;

void* prime(void* ran) {
    pthread_mutex_lock(&L);
    range* r = (range*)ran;

    int i, j;
    int num;

    for (i = r->start; i <= r->end; i++) {
        int isPrime = 1;

        num = r->numbers[i];

        for (j = 2; j <= num / 2; j++) {
            if (num == 1) {
                isPrime = 0;
                break;
            }
            if (num % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
        	pthread_mutex_lock(&L2);
        	
        	FILE *ptrA = fopen("prime.txt", "a");
        	fprintf(ptrA, "%d\n", num);
        	fclose(ptrA);
        	pthread_mutex_unlock(&L2);
            totalPrimeNumbers++;
        }
    }
    pthread_mutex_unlock(&L);

    return NULL;
}

void main() {
    pthread_mutex_init(&L, NULL);
	pthread_mutex_init(&L2, NULL);
    int number_of_threads;
	
    printf("Enter number of threads: ");
    scanf("%d", &number_of_threads);

    int count = 0;
    int i, x;
    int* p = (int*)malloc(sizeof(int));
    int numberOfFiles = 3;
    FILE* fp;

    char filename[15];

    for (i = 0; i < numberOfFiles; i++) {
        sprintf(filename, "PrimeData%d.txt", i + 1);

        fp = fopen(filename, "r");

        if (fp == NULL) {
            printf("Error occurred, datasetLR%d.txt not found", i + 1);
            exit(-1);
        }
        while (fscanf(fp, "%d", &x) != EOF) {
            count++;
            p = (int*)realloc(p, count * sizeof(int));
            p[count - 1] = x;
            if (max_number < x) {
                max_number = x;
            }
        }
    }

    printf("first element: %d\n", p[0]);
    printf("last element: %d\n", p[count - 1]);
    printf("max number is %d\n", max_number);
    printf("count %d\n", count);

    int slice_per_thread = (count / number_of_threads);

    range* d = (range*)malloc(number_of_threads * sizeof(range));

    if (d == NULL) {
        exit(-1);
    }

    pthread_t* id = (pthread_t*)malloc(number_of_threads * sizeof(pthread_t));

    if (id == NULL) {
        exit(-1);
    }

    printf("slice per thread: %d\n", slice_per_thread);

    for (i = 0; i < number_of_threads; i++) {
        d[i].numbers = p;
        d[i].thread_number = i + 1;
        d[i].start = i * slice_per_thread;

        if (i == number_of_threads - 1) {
            d[i].end = count - 1;
        } else {
            d[i].end = (i + 1) * slice_per_thread - 1;
        }
    }
	
	FILE *ptrW = fopen("prime.txt", "w");
	fclose(ptrW);
	
    for (i = 0; i < number_of_threads; i++) {
        pthread_create(&id[i], NULL, prime, &d[i]);
    }

    for (i = 0; i < number_of_threads; i++) {
        pthread_join(id[i], NULL);
    }

    printf("Total number of primes: %d", totalPrimeNumbers);

    free(p);
    free(d);
    free(id);

    pthread_mutex_destroy(&L);
	pthread_mutex_destroy(&L2);
}

