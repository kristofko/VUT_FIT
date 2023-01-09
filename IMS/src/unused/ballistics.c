/**
 * @file ballistics.c
 * @author Kristof Siska (xsiska16)
 * 
 * Last edited : Dec 2022
 * Compiled with : 9.4.0 (Ubuntu 20.04.1)
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv){

	// Calculating the initial angle of missile fire
	FILE *f_x = fopen("X_out.txt", "w");
	FILE *f_y = fopen("Y_out.txt", "w");
	float mass = 0.01546;
	int tr = 150; // Target is 150 km away
	float c = 0.4; // Drag coeficient
	float airD = 1.2; // Air density
	float grav = 9.81; // Gravitational force
	float hv[2] = {0 , 0};	// horizontal speed
	float vv[2] = {0, 0}; // vertical speed
	float *posX = malloc(5000000 * sizeof(float)); // Initial horizontal position
	float *posY =  malloc(5000000 * sizeof(float)); // Initial vertical position
	float angle[900];		// Array of possible angles. 
	for (int i = 0; i < 900; i++){
		angle[i] = i / 10;
	} 
	float delta = 0.001; // Time step
	float thrust;
	float drag;

	float distance = 0;

	float distance_x = 0;
	float distance_y = 0;

	float Launh_Rod_length = 1;
	float A = 4.9 * 0.0001;
	float launch_angle = 60 * M_PI / 180;
	float cur_angle = 60 * M_PI / 180;

	// Short range ballistic missile attack

	// Configuring the angle

	posX[0] = 0;
	posY[1] = 0.1;

	int i = 1;


	// Run the simulation till rocket hits the ground
	while (posY[i] > 0){
		posY[0] = 0.1;
		float time_elapsed = (i) * delta;
		i++;

		if (time_elapsed <= 0.1){
			thrust = 56;
		}
		else if (time_elapsed < 0.5){
			thrust = 5.6;
		} 
		else {
			thrust = 0.0;
		}

		// Normal force
		float Fn = 0;
		if (distance <= Launh_Rod_length){
			Fn = mass * grav * cos(launch_angle);
		}

		drag = 0.5 * c * A * airD * ( pow(hv[0], 2) + pow(vv[0], 2) );

		printf("Drag : %f\n", drag);

		// Sum x forces 

		float Fx = thrust * cos(cur_angle) - drag * cos(cur_angle) - Fn * sin(cur_angle);

		// printf("Fx : %f\n", Fx);

		float Fy = thrust * sin(cur_angle) - mass * grav - drag * sin(cur_angle) + (Fn * cos(cur_angle));

		printf(" FY : %f %f %f %f \n", thrust * sin(cur_angle) , mass * grav , drag * sin(cur_angle) , (Fn * cos(cur_angle)));


		printf("Fy : %f\n", Fy);
		// Acceleration

		float accelX = Fx / mass;
		float accleY = Fy / mass;

		printf("Accel Y : %f\n", accleY);

		// velocity

		hv[1] = hv[0] + accelX * delta;
		vv[1] = vv[0] + accleY * delta;

		// position

		posX[i] = posX[i - 1] + hv[1] * delta;
		posY[i] = posY[i - 1] + vv[1] * delta;

		// angle
			// printf("HV : %f\n", hv[1]);
		// printf("VV : %f\n", vv[1]);

		distance_x = distance_x + fabs(hv[1] * delta);
		distance_y = distance_y + fabs(vv[1] * delta);
		distance = (sqrt(pow(distance_x, 2) + (pow(distance_y, 2))));

		printf("Curr angle : %f\n", cur_angle);
		printf("vv and hv : %f %f\n", vv[1], hv[1]);
		cur_angle = (atan(vv[1]/hv[1]));


		fprintf(f_x, "%f ", posX[i]);	
		fprintf(f_y, "%f ", posY[i]);	

		hv[0] = hv[1];
		hv[0] = hv[1]; 
	}
	printf("Ground Hit : %f", i * delta);
	return 0;
}