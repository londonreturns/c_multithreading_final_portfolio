// import header files
# include <stdio.h>
# include <stdlib.h>

// square function
int square(int num){
	
	return num * num;
}

// main function
void main(){
	
	// show details
	printf("This program is for Linear Regression.\n");
	printf("It calculates A, B from files, and asks X from user to calculate the value of Y.\n");
	printf("Using this formula Y = B * X + A.\n\n");
	
	// declare and initialize variables
	int x_value, y_value, i, numberOfFiles = 4;
	char filename[15];
	float A, B, X, Y, total_x = 0, total_y = 0, total_x_2 = 0, total_x_y = 0, numberOfData = 0; 
	FILE *fp;
	
	// loop for files
	for (i = 0; i < numberOfFiles; i++){
	
		sprintf(filename, "datasetLR%d.txt", i + 1);
	
		fp = fopen(filename, "r");
		
		// check if fp is null
		if (fp == NULL){
			printf("Error occurred, datasetLR%d.txt not found", i + 1);
			exit(-1);
		}
		
		// loop for lines
		while (fscanf(fp, "%d,%d", &x_value, &y_value)!= EOF){
			
			total_x += x_value;
			total_y += y_value;
			total_x_2 += square(x_value);
			total_x_y += x_value * y_value;
			numberOfData++;
			
		}	
	}
	
	// calculate A and B
	A = (((total_y * total_x_2) - (total_x * total_x_y)) / ((numberOfData * total_x_2) - (square(total_x))));
	B = (((numberOfData * total_x_y) - (total_x * total_y)) / ((numberOfData * total_x_2) - (square(total_x))));

	// display value of A and B	
	printf("Calculated value of A = %f\n", A);		
	printf("Calculated value of B = %f\n\n", B);
	
	// user input validation
	int inputStatus;

	// prompt user for x
    do {
        printf("Enter value for X: ");
        inputStatus = scanf("%f", &X);

        // if input value is float
        if (inputStatus != 1) {
            // clear input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter a valid float or integer.\n");
        }
    } while (inputStatus != 1);

	// calculate x
	Y = B * X + A;
	
	// display y to user
	printf("Y is %f", Y);
	
	// file close
	fclose(fp);
	
}
