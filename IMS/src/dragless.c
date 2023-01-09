/**
 * @file dragless.c
 * @author Kristof Siska (xsiska16)
 * 
 * Last edited : Dec 2022
 * Compiled with : 9.4.0 (Ubuntu 20.04.1)
*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TO_RAD(x) (x * M_PI / 180)

#define TO_DEG(x) (x * 180 / M_PI)

int main(int argc, char **argv){
	FILE *x_out = fopen("data/X_dragless.txt", "w");
	FILE *y_out = fopen("data/Y_dragless.txt", "w");
	float angle = TO_RAD(45);
	float v = 983; // Got from experiments
	float x0 = 0;
	float y0 = 10;
	float grav = -9.81;
	int accuracy = 10000;
	float bullet_mass = 4.4;
	float best_angle = 10.0;

	// Velocity components

	// Calculating the targets position after 3 seconds

	double *vx = calloc(accuracy, sizeof(double));
	double *vy = calloc(accuracy, sizeof(double));
	double *ax = calloc(accuracy, sizeof(double));
	double *ay = calloc(accuracy, sizeof(double));
	double *x = calloc(accuracy, sizeof(double));
	double *y = calloc(accuracy, sizeof(double));
	double *best_x = calloc(accuracy, sizeof(double));
	double *best_y = calloc(accuracy, sizeof(double));
	y[0] = 0.1;
	x[0] = 0;
	double *t = calloc(accuracy, sizeof(double));
	float delta = 0.3;
	float c = 0; // Drag coeficient
	float best_range = 0.0;
		
	for(float test_angle = 4500; test_angle <= 4500; test_angle++){
		float tmp = test_angle / 100;
		float angle = TO_RAD(tmp);

		float drag = c * pow(v, 2);
		ax[0] = -(drag * cos(angle)) / bullet_mass;
		ay[0] = grav - ((drag * sin(angle)) / bullet_mass);
		vx[0] = v * cos(angle);
		vy[0] = v * sin(angle);
		t[0] = 0.0;

		fprintf(x_out, "%f ", x[0]);
		fprintf(y_out, "%f ", y[0]);
		int i = 1;
		
		while(y[i - 1] >= 0.0){
			t[i] = 	i * delta;

			// Velocity update
			vx[i] = vx[i - 1] + delta * ax[i - 1];
			vy[i] = vy[i - 1] + delta * ay[i - 1];

			x[i] = x[i - 1] + delta * vx[i - 1];
			y[i] = y[i - 1] + delta * vy[i - 1];

			float vel =  sqrt(pow(vx[i - 1], 2) + pow(vy[i - 1], 2));
			drag = c * pow(vel, 2);
			ax[i] = -(drag * cos(angle)) / bullet_mass;
			ay[i] = grav - (drag * sin(angle)) / bullet_mass;


			i++;

		}
		if (x[i - 1] > best_range){
			for(int j = 0; j < i; j++){
				best_x[j] = x[j];
				best_y[j] = y[j];
				fprintf(x_out, "%f ", x[j]);
				fprintf(y_out, "%f ", y[j]);
			}

			best_angle = angle;
			best_range = x[i - 1];
		}
	
	}

	free(x); 
	free(y); 
	free(ax);	
	free(ay); 
	free(vx); 
	free(vy); 
	free(t);

	return 0;	
}
