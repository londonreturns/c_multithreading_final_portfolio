# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <math.h>

struct Range{
	int start;
	int end;
	int thread_number;
};

typedef struct Range range;

pthread_mutex_t L;

double pi = 0;

void * calculate(void *ran){
	
	range* r = ran;
	
	double tempSum = 0;
	
	int i;
	for (i = r->start; i <= r->end; i++){
		tempSum += (pow(-1, i) / (2 * i + 1));
	}
	pthread_mutex_lock(&L);
	pi += tempSum;	
	pthread_mutex_unlock(&L);
}

void main(){
	
	printf("This program calculates the value of PI using Leibniz formula and multithreading.\n\n");
	
	pthread_mutex_init(&L, NULL);
	
	int total,  number_of_threads;
	
	printf("Enter number of iterations: ");
	scanf("%d", &total);
	
	printf("Enter number of threads: ");
	scanf("%d", &number_of_threads);
	
	int slice_per_thread = total / number_of_threads;
	
	int i;
	
	range *d = (range *)malloc(number_of_threads * sizeof(range));
	
	if (d == NULL){
		exit(-1);
	}
	
	pthread_t *id = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
	
	if (id == NULL){
		exit(-1);
	}
	
	for (i = 0; i < number_of_threads; i++){
		d[i].start = slice_per_thread * i;
		d[i].thread_number = i + 1;
		if (i == number_of_threads - 1){
			d[i].end = total;
		}else{
			d[i].end = (1 + i) * slice_per_thread;
		}
	}
	
	for (i = 0; i < number_of_threads; i ++){
		pthread_create(&id[i], NULL, calculate, &d[i]);
	}
	
	for (i = 0; i < number_of_threads; i ++){
		pthread_join(id[i], NULL);
	}
	
	pi *= 4;
	
	printf("Value of pi is %lf", pi);
	
	free(d);
	free(id);
}
