/**
 * @file howitzer_tank.c
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
	float target = 29500; // Assume that target is 29.5 km away if not given 
	if (argc == 2){
		
		target = atoi(argv[1]);
		if (target > 41000){
			printf("Target is too far away\n");
			return 0;
		}
	}

	float angle = TO_RAD(45);
	float v = 983; // Got from experiments
	float x0 = 0;
	float y0 = 0.1;
	float grav = -9.81; // Gravitational acceleration [m/s^2]
	int accuracy = 10000;  
	float bullet_mass = 4.4; // Mass of fired bullet [kg]

	double *vx = calloc(accuracy, sizeof(double)); // Array of horizontal speed [m/s]
	double *vy = calloc(accuracy, sizeof(double)); // Array of vertical speed [m/s]
	double *ax = calloc(accuracy, sizeof(double)); // Array of horizontal acceleration [m/s^2]
	double *ay = calloc(accuracy, sizeof(double)); // Array of vertical acceleration [m/s^2]
	double *x = calloc(accuracy, sizeof(double));  // Array of horizontal position of bullet [m]
	double *y = calloc(accuracy, sizeof(double));  // Array of vertical position of bullet [m]
	double *t = calloc(accuracy, sizeof(double));  // Array of elapsed time [s]

	y[0] = y0;
	x[0] = x0;
	float delta = 0.1;
	float Fd = 0.00009; // Drag coeficient = 1/2 * A * c
		
	for(float test_angle = 100; test_angle <= 6500; test_angle++){
		float tmp = test_angle / 100;
		float angle = TO_RAD(tmp);

		float drag = Fd * pow(v, 2);
		ax[0] = -(drag * cos(angle)) / bullet_mass;
		ay[0] = grav - ((drag * sin(angle)) / bullet_mass);
		vx[0] = v * cos(angle);
		vy[0] = v * sin(angle);
		t[0] = 0.0;

		int i = 1;
		
		while(y[i - 1] >= 0.0){
			t[i] = 	i * delta;

			// Velocity update
			vx[i] = vx[i - 1] + delta * ax[i - 1];
			vy[i] = vy[i - 1] + delta * ay[i - 1];

			x[i] = x[i - 1] + delta * vx[i - 1];
			y[i] = y[i - 1] + delta * vy[i - 1];

			//fprintf(x_out, "%f ", x[i]);
			//fprintf(y_out, "%f ", y[i]);

			float vel =  sqrt(pow(vx[i - 1], 2) + pow(vy[i - 1], 2));
			drag = Fd * pow(vel, 2);
			ax[i] = -(drag * cos(angle)) / bullet_mass;
			ay[i] = grav - (drag * sin(angle)) / bullet_mass;

			i++;

		}
		if (x[i - 1] >= target - 50 && x[i - 1] <= target + 50){
			printf("Target hit, angle : %f\n", TO_DEG(angle));
				FILE *x_out = fopen("data/X_howitzer.txt", "w");
				FILE *y_out = fopen("data/Y_howitzer.txt", "w");
			for (int j = 0; j < i; j += 5){
				fprintf(x_out, "%f ", x[j]);
				fprintf(y_out, "%f ", y[j]);
			}
				fclose(x_out);
				fclose(y_out);
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
