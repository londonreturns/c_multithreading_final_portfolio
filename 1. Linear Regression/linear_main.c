# include <stdio.h>
# include <stdlib.h>

int square(int num){
	return num * num;
}

void main(){
	
	printf("This program is for Linear Regression.\n");
	printf("It calculates A, B from files, and asks X from user to calculate the value of Y.\n");
	printf("Using this formula Y = B * X + A.\n\n");
	
	int x_value, y_value, i, numberOfFiles = 4;
	
	char filename[15];
	
	float A, B, X, Y, total_x = 0, total_y = 0, total_x_2 = 0, total_x_y = 0, numberOfData = 0; 
	
	FILE *fp;
	
	for (i = 0; i < numberOfFiles; i++){
	
		sprintf(filename, "datasetLR%d.txt", i + 1);
	
		fp = fopen(filename, "r");
		
		if (fp == NULL){
			printf("Error occurred, datasetLR%d.txt not found", i + 1);
			exit(-1);
		}
		while (fscanf(fp, "%d,%d", &x_value, &y_value)!= EOF){
			
			total_x += x_value;
			total_y += y_value;
			total_x_2 += square(x_value);
			total_x_y += x_value * y_value;
			numberOfData++;
			
		}	
	}
	
	A = (((total_y * total_x_2) - (total_x * total_x_y)) / ((numberOfData * total_x_2) - (square(total_x))));
	
	B = (((numberOfData * total_x_y) - (total_x * total_y)) / ((numberOfData * total_x_2) - (square(total_x))));
	
	printf("Calculated value of A = %f\n", A);		
	printf("Calculated value of B = %f\n\n", B);
	
	printf("Enter value for X: ");
	scanf("%f", &X);
	
	Y = B * X + A;
	printf("Y is %f", Y);
	
}
