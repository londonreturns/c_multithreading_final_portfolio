// import header files
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <math.h>

// range structure
struct Range{
	int start;
	int end;
	int thread_number;
};

typedef struct Range range;

// declare mutex
pthread_mutex_t L;

// intialize variable
double pi = 0;

// calculate_pi function
void * calculate_pi(void *ran){
	
	// mutex lock
	pthread_mutex_lock(&L);
	
	// typecast range
	range* r = ran;
	
	// declare and initialize variables
	double tempSum = 0;
	int i;
	
	// loop for start, end of slices
	for (i = r->start; i <= r->end; i++){
		tempSum += (pow(-1, i) / (2 * i + 1));
	}
	
	// change value of pi
	pi += tempSum;	
	
	// mutex lock
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
void main(){
	
	// show details
	printf("This program calculates the value of PI using Leibniz formula and multithreading.\n\n");
	
	// initialize mutex
	pthread_mutex_init(&L, NULL);
	
	// declare and initialize variables
	int total,  number_of_threads;	

	// prompt user for number of iteration, threads
    total = get_valid_input("Enter number of iterations: ");
	number_of_threads = get_valid_input("Enter number of threads: ");
	
	// declare and initialize variables
	int slice_per_thread = total / number_of_threads;
	int i;
	
	// allocate dynamic memory
	range *d = (range *)malloc(number_of_threads * sizeof(range));
	pthread_t *id = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
	
	// check if d is null
	if (d == NULL){
		exit(-1);
	}
	
	// check if id is null
	if (id == NULL){
		exit(-1);
	}
	
	// loop for slicing
	for (i = 0; i < number_of_threads; i++){
		d[i].start = slice_per_thread * i;
		d[i].thread_number = i + 1;
		
		// handle uneven slices
		if (i == number_of_threads - 1){
			d[i].end = total;
		}else{
			d[i].end = (1 + i) * slice_per_thread;
		}
	}
	
	// loop to start threading
	for (i = 0; i < number_of_threads; i ++){
		pthread_create(&id[i], NULL, calculate_pi, &d[i]);
	}
	
	// loop to end threading
	for (i = 0; i < number_of_threads; i ++){
		pthread_join(id[i], NULL);
	}
	
	pi *= 4;
	
	// display pi to user
	printf("Value of pi is %lf", pi);
	
	// free dynamic memory
	free(d);
	free(id);
	
	// destroy mutex
	pthread_mutex_destroy(&L);
}
