// gcc gaussian_main.c lodepng.c -o executable.exe -lpthread
// import header files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lodepng.h"

struct Thread {
    int start;
    int end;
};

typedef struct Thread thread;

// declare variables
unsigned char *ImageOne, *ImageTwo;
unsigned int width, height;

// applyGaussianBlur function
void applyGaussianBlur(unsigned char *output, unsigned char *input, int x, int y, int width, int height) {
    
	// declare and initialize variables
	int channels = 4;
	int c, i, j, nx, ny;
	
	// loop for pixel details
    for (c = 0; c < channels; c++) {
    	
		// declare and initialize variables    	
        float sum = 0.0;
        int count = 0;
		
		// loop for 3*3 matrix
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
            	
            	// newx newy
                nx = x + i;
                ny = y + j;
				
				// if not out of bounds
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                	
                	// sum pixel values
                    sum += input[(ny * width + nx) * channels + c];
                    
                    // count number of pixels
                    count++;
                }
            }
        }

        // average
        output[(y * width + x) * channels + c] = sum / count;
    }
}

// gaussianBlur function
void* gaussianBlur(void* p) {
	
	// typecast thread
    thread* pt = (thread*)p;
    
    // declare variables
	int x, y;
	
	// loop for start, end of slices
    for (y = pt->start; y < pt->end; y++) {
    	
    	// loop for pixels
        for (x = 0; x < width; x++) {
            applyGaussianBlur(ImageTwo, ImageOne, x, y, width, height);
        }
    }
}

// main function
int main() {
	
	// show details
	printf("This program applies Gaussian blur to png image.\n\n");
	
	// declare and initialize variables
    char input[] = "image.png", output[] = "imageNew.png";
    unsigned int err;
    int numThread;
	
	// decode png image
    err = lodepng_decode32_file(&ImageOne, &width, &height, input);
	
	// if error or not display relevant information
    if (err) {
        printf("%d %s", err, lodepng_error_text(err));
        exit(-1);
    } else {
        printf("Height of image is: %d pixels\nWidth of image is: %d pixels.\n", height, width);
    }
	
	// allocate dynamic memory
    ImageTwo = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned char));
    
    // if dynamic memory allocation was successful
    if (ImageTwo == NULL) {
	    fprintf(stderr, "Memory allocation error\n");
	    exit(EXIT_FAILURE);
	}
	
	// prompt user for number of threads
    printf("Enter number of threads: ");
    scanf("%d", &numThread);
	
	// allocate dynamic memory
    pthread_t *thrd = (pthread_t *)malloc(numThread * sizeof(pthread_t));
	thread *st = (thread *)malloc(numThread * sizeof(thread));
	
	// if dynamic memory allocation was successful
	if (thrd == NULL || st == NULL) {
	    fprintf(stderr, "Memory allocation error\n");
	    exit(EXIT_FAILURE);
	}
	
	// declare and initialize variables
    int slice[numThread], i;
    int start = 0;
    int remainder = height % numThread;
	
	// slicing calculation
    for (i = 0; i < numThread; i++) {
        slice[i] = height / numThread + (i < remainder ? 1 : 0);
    }
    
	// loop for slicing
    for (i = 0; i < numThread; i++) {
        st[i].start = start;
        st[i].end = start + slice[i];
        start = st[i].end;
    }
	
	// loop to start threading
    for (i = 0; i < numThread; i++) {
        pthread_create(&thrd[i], NULL, gaussianBlur, &st[i]);
    }

	// loop to end threading
    for (i = 0; i < numThread; i++) {
        pthread_join(thrd[i], NULL);
    }
	
	// encode png image
    lodepng_encode32_file(output, ImageTwo, width, height);

	// free dynamic memory
    free(ImageTwo);
    free(thrd);
	free(st);
}
