// import header files
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// range structure
struct Range {
    int* numbers;
    int thread_number;
    int start;
    int end;
};

typedef struct Range range;

// declare mutex
pthread_mutex_t L;
pthread_mutex_t L2;

// intialize variable
int countOfNumbers = 0;
int totalPrimeNumbers = 0;
int max_number = 0;

// prime_check function
void* prime(void* ran) {
	
	// lock mutex
    pthread_mutex_lock(&L);
    
    // typecast range
    range* r = (range*)ran;

	// declare variables
    int i, j;
    int num;

	// loop for start, end of slices
    for (i = r->start; i <= r->end; i++) {
        int isPrime = 1;
		
		// get number
        num = r->numbers[i];

		// check for prime
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
        
        // if prime
        if (isPrime) {
        	// lock mutex
        	pthread_mutex_lock(&L2);
        	
        	// write to file
        	FILE *ptrA = fopen("prime.txt", "a");
        	fprintf(ptrA, "%d\n", num);
        	fclose(ptrA);
        	
        	// unlock mutex
        	pthread_mutex_unlock(&L2);
        	
        	// increment prime count
            totalPrimeNumbers++;
        }
    }
    // unlock mutex
    pthread_mutex_unlock(&L);
}

// get_valid_input function
int get_valid_input(const char *prompt) {
	
	// declare variables
    int input, inputStatus;

	// loop to prompt until correct input
    do {
    	
    	// prompt user
        printf("%s", prompt);
        inputStatus = scanf("%d", &input);
		
		// if not integer
        if (inputStatus != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            while (getchar() != '\n');
        }
    } while (inputStatus != 1);
	
	// return data
    return input;
}

// main function
void main() {
	
	// show details
	printf("This program calculates if a number is Prime and stores in a new file.\n\n");
	
	// initialize mutex
    pthread_mutex_init(&L, NULL);
	pthread_mutex_init(&L2, NULL);
	
	// initialize variables
    int number_of_threads;

	// declare and initialize variables
    int count = 0;
    int i, x;
    int numberOfFiles = 3;
    FILE* fp;
    char filename[15];
    
    // prompt user for number of threads
	number_of_threads = get_valid_input("Enter number of threads: ");

	// allocate dynamic memory
	int* p = (int*)malloc(sizeof(int));
	
	// loop for files
    for (i = 0; i < numberOfFiles; i++) {
        sprintf(filename, "PrimeData%d.txt", i + 1);

        fp = fopen(filename, "r");
		
		// check if fp is null
        if (fp == NULL) {
            printf("Error occurred, datasetLR%d.txt not found", i + 1);
            exit(-1);
        }
        
        // loop for lines
        while (fscanf(fp, "%d", &x) != EOF) {
        	
        	// count number of numbers
            count++;
            
            // reallocate dynamic memory
            p = (int*)realloc(p, count * sizeof(int));
            
            // store value in p
            p[count - 1] = x;
            
            // store highest number
            if (max_number < x) {
                max_number = x;
            }
        }
    }
	
	// show details
    printf("First element: %d\n", p[0]);
    printf("Last element: %d\n", p[count - 1]);
    printf("Max number is %d\n", max_number);
    printf("Count %d\n", count);

    int slice_per_thread = (count / number_of_threads);
	
	// allocate dynamic memory
    range* d = (range*)malloc(number_of_threads * sizeof(range));
    pthread_t* id = (pthread_t*)malloc(number_of_threads * sizeof(pthread_t));
    
	// check if d is null
    if (d == NULL) {
        exit(-1);
    }
    
	// check if id is null
    if (id == NULL) {
        exit(-1);
    }
	
	// loop for slicing
    for (i = 0; i < number_of_threads; i++) {
        d[i].numbers = p;
        d[i].thread_number = i + 1;
        d[i].start = i * slice_per_thread;
		
		// handle uneven slices
        if (i == number_of_threads - 1) {
            d[i].end = count - 1;
        } else {
            d[i].end = (i + 1) * slice_per_thread - 1;
        }
    }
	
	// open, close to clear file
	FILE *ptrW = fopen("prime.txt", "w");
	fclose(ptrW);
	
	// loop to start threading
    for (i = 0; i < number_of_threads; i++) {
        pthread_create(&id[i], NULL, prime, &d[i]);
    }
	
	// loop to end threading
    for (i = 0; i < number_of_threads; i++) {
        pthread_join(id[i], NULL);
    }

	// display total number of prime to user
    printf("Total number of primes: %d", totalPrimeNumbers);
	
	// free dynamic memory
    free(p);
    free(d);
    free(id);
	
	// destroy mutex
    pthread_mutex_destroy(&L);
	pthread_mutex_destroy(&L2);
}
