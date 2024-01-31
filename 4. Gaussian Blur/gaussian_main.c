#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lodepng.h"

pthread_mutex_t mutex1;
unsigned char *ImageOne, *ImageTwo;
unsigned int weight, height;

struct thread {
    int start;
    int end;
};

void applyGaussianBlur(unsigned char *output, unsigned char *input, int x, int y, int width, int height) {
    int channels = 4;
	
	int c, i, j, nx, ny;
	
    for (c = 0; c < channels; c++) {
        float sum = 0.0;
        int count = 0;


        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                nx = x + i;
                ny = y + j;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    sum += input[(ny * width + nx) * channels + c];
                    count++;
                }
            }
        }

        
        output[(y * width + x) * channels + c] = sum / count;
    }
}

void* gaussianBlur(void* p) {
    struct thread* pt = (struct thread*)p;
	int x, y;
    for (y = pt->start; y < pt->end; y++) {
        for (x = 0; x < weight; x++) {
            applyGaussianBlur(ImageTwo, ImageOne, x, y, weight, height);
        }
    }

    pthread_exit(NULL);
}

int main() {
    char input[] = "image.png", output[] = "imageNew.png";
    unsigned int err;
    int numThread;

    err = lodepng_decode32_file(&ImageOne, &weight, &height, input);

    if (err) {
        printf("%d %s", err, lodepng_error_text(err));
        exit(-1);
    } else {
        printf("Height of image is: %d pixels\nWidth of image is: %d pixels.\n", height, weight);
    }

    ImageTwo = (unsigned char*)malloc(weight * height * 4 * sizeof(unsigned char));

    printf("Enter number of threads: ");
    scanf("%d", &numThread);

    pthread_mutex_init(&mutex1, NULL);
    pthread_t thrd[numThread];
    struct thread st[numThread];

    int slice[numThread], i;
    int remainder = height % numThread;

    for (i = 0; i < numThread; i++) {
        slice[i] = height / numThread + (i < remainder ? 1 : 0);
    }

    int start = 0;
    for (i = 0; i < numThread; i++) {
        st[i].start = start;
        st[i].end = start + slice[i];
        start = st[i].end;
    }

    for (i = 0; i < numThread; i++) {
        pthread_create(&thrd[i], NULL, gaussianBlur, &st[i]);
        
    }

    for (i = 0; i < numThread; i++) {
        pthread_join(thrd[i], NULL);
    }

    lodepng_encode32_file(output, ImageTwo, weight, height);

    pthread_mutex_destroy(&mutex1);
    free(ImageTwo);

    return 0;
}

