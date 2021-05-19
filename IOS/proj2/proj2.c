/** 
 * Solution to IOS proj2 homework
 * 
 * Using semaphores, shared memory and forks 
 * to solve the Santa problem
 * 
 * @file proj2.c 
 * @author Krištof Šiška (xsiska16)
 * @date 16.4.2021
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include <signal.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "proj2.h"

// s_mutex is used for manipulation with variables

int *shared_shop_closed,*shared_counter, *shared_elf_counter, *shared_reind_counter = NULL;
sem_t *s_mutex, *s_elf, *s_santa, *s_reind, *s_workshop, *s_sleep = NULL;
FILE *out_file;



#define MYSLEEP(max_time) usleep(rand() % max_time)

int main(int argc, char **argv){
	int arg_ret = check_args(argc, argv);
	if (arg_ret != 0){
		fprintf(stderr, "Program arguments are not right, aborting.\n");
		return EXIT_FAILURE;
	}

	int check = init_components();
	if (check != 0){
		fprintf(stderr,"Problem with initializing components, aborting..\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	int num_of_elves = atoi(argv[1]); 
	int num_of_reindeer =  atoi(argv[2]);
	int elf_work = ( atoi(argv[3]) + 1 );
	int reindeer_work = ( atoi(argv[4]) + 1 );

	pid_t santa_process = fork();
	if (santa_process == 0){
		process_santa(num_of_reindeer);
	}
	else {
		for (int i = 0; i < num_of_elves; i++){
			if ( fork() == 0 ){
				process_elf(i + 1, elf_work);
				exit(0);
			}
		}

		for (int i = 0; i < num_of_reindeer; i++){
			if ( fork() == 0){
				process_reindeer(i + 1, reindeer_work, num_of_reindeer);
				exit(0);
			}
		}
	}

	// Wait for all child proccesses to die
	while( wait(NULL) != -1 );

	if (santa_process){
		destruct_components(out_file);	
	}
	exit(0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////
////////////////// END OF MAIN FUNCTION ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int init_components(){
	out_file = fopen("proj2.out", "w");
	if (out_file == NULL){
		fprintf(stderr,"Cannot open proj2.out file, aborting...\n");
		exit(0);
	}
	// Sem unlink in case of of failing in the past.
	sem_unlink("/proj2.ios.out.semaphor.mutex.log");
	sem_unlink("/proj2.ios.out.semaphor.writing.log");
	sem_unlink("/proj2.ios.out.semaphor.elf.log");
	sem_unlink("/proj2.ios.out.semaphor.reind.log");
	sem_unlink("/proj2.ios.out.semaphor.santa.log");
	sem_unlink("/proj2.ios.out.semaphor.workshop.log");
	sem_unlink("/proj2.ios.out.semaphor.sleep.log");
	
	shared_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
	shared_elf_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
	shared_reind_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
	shared_shop_closed = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0 );
	
	shared_counter[0] = 0;
	shared_elf_counter[0] = 0;
	shared_reind_counter[0] = 0;
	shared_shop_closed[0] = 0;


	// Sem for mutual exclusion - shared memories, and out_file
	s_mutex = sem_open("/proj2.ios.out.semaphor.mutex.log", O_CREAT | O_EXCL, 0666, 1);
	if (s_mutex == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}

	// Sem for 
	s_elf = sem_open("/proj2.ios.out.semaphor.elf.log", O_CREAT | O_EXCL, 0666, 0);
	if (s_elf == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}
	
	s_reind = sem_open("/proj2.ios.out.semaphor.reind.log", O_CREAT | O_EXCL, 0666, 0);
	
	if (s_reind == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}
	
	s_santa = sem_open("/proj2.ios.out.semaphor.santa.log", O_CREAT | O_EXCL, 0666, 0);
	if (s_santa == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}

	s_workshop = sem_open("/proj2.ios.out.semaphor.workshop.log", O_CREAT | O_EXCL, 0666, 1);
	if (s_workshop == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}

	s_sleep = sem_open("/proj2.ios.out.semaphor.sleep.log", O_CREAT | O_EXCL, 0666, 0);
	if (s_sleep == SEM_FAILED){
		fprintf(stderr, "Sem failed, aborting...\n");
		return -1;
	}
	
	return 0;
}

void destruct_components(FILE *f){
	sem_unlink("/proj2.ios.out.semaphor.mutex.log");
	sem_unlink("/proj2.ios.out.semaphor.elf.log");
	sem_unlink("/proj2.ios.out.semaphor.reind.log");
	sem_unlink("/proj2.ios.out.semaphor.santa.log");
	sem_unlink("/proj2.ios.out.semaphor.workshop.log");
	sem_unlink("/proj2.ios.out.semaphor.sleep.log");
	sem_close(s_mutex);
	sem_close(s_elf);
	sem_close(s_reind);
	sem_close(s_santa);
	sem_close(s_workshop);
	sem_close(s_sleep);

	munmap(shared_counter, 1);
	munmap(shared_elf_counter, 1);
	munmap(shared_reind_counter, 1);
	munmap(shared_reind_counter, 1);
	fclose(f);
	

	return ;
}

/** 
 * A three step process, first step print rstarted
 * 2nd step  return home 
 * 3rd step  get hitched 
 * 
 * @param reindeerID Unique identifier of reindeer process
 * @param reindeer_work Number to calculate reindeers vacation time
 * @param max_reindeer Number of all reindeer processess 
*/
void process_reindeer(int reindeerID, int reindeer_vacation, int max_reindeer){
	sem_wait(s_mutex);
	// Critical section
	shared_counter[0]++;
	fprintf(out_file, "%d: RD %d: rstarted\n",shared_counter[0], reindeerID);
	fflush(out_file);
	sem_post(s_mutex);

	// Reindeer going to vacation
	usleep(rand() % reindeer_vacation + (reindeer_vacation / 2) );
	
	// Critical section
	sem_wait(s_mutex);
	shared_reind_counter[0]++;
	shared_counter[0]++;
	fprintf(out_file, "%d: RD %d: return home\n", shared_counter[0], reindeerID);
	fflush(out_file);
	// Last reindeer signals santa
	if ( shared_reind_counter[0] == max_reindeer ){
		sem_post(s_santa);
	}
	sem_post(s_mutex);

	// Waiting for all the reindeers to come home
	sem_wait(s_reind);
	sem_wait(s_mutex);
	shared_counter[0]++;
	fprintf(out_file, "%d: RD %d: get hitched\n", shared_counter[0], reindeerID);
	fflush(out_file);
	shared_reind_counter[0]--;

	// Last hitched reindeer signals santa and christmas starts
	if (shared_reind_counter[0] == 0){
		sem_post(s_santa);
	}
	sem_post(s_mutex);
	exit(0);

}

/** 
 * Child process of main function (only one)
 * 
 * @param num_of_reindeer Number of all reindeers 
*/
void process_santa(int num_of_reindeer){
	// Critical section
	sem_wait(s_mutex);
	shared_counter[0]++;
	fprintf(out_file, "%d: Santa: going to sleep\n", shared_counter[0]);
	fflush(out_file);
	sem_post(s_mutex);

	while(1){
		sem_wait(s_santa);
		sem_wait(s_mutex);
		shared_counter[0]++;
		if (shared_reind_counter[0] >= num_of_reindeer){
			fprintf(out_file, "%d: Santa: closing workshop\n", shared_counter[0]);
			fflush(out_file);
			for (int i = 0; i < num_of_reindeer; i++){
				sem_post(s_reind);
			}
			shared_shop_closed[0] = 1; // Signal to elves
			for (int i = 0; i < 3; i++){
			sem_post(s_elf);
			}
			sem_post(s_mutex);

			// Waiting for last reind to get hitched
			sem_wait(s_santa);
			sem_wait(s_mutex);
			
			christmas_started();

			sem_post(s_mutex);
			exit(0);
		}
		else if (shared_elf_counter[0] >= 3){
			helping_elves();
			
		}		
	}
}



/** 
 * Elf process for "making toys"
 * 
 * @param elfID Unique identification number of elf
 * @param work_time Maximum time elves can work without help  
*/
void process_elf(int elfID, int work_time){
	// A elf started
	sem_wait(s_mutex);
	shared_counter[0]++;
	fprintf(out_file, "%d: Elf %d: started\n", shared_counter[0], elfID);
	fflush(out_file);
	sem_post(s_mutex);

	
	while(1){
		MYSLEEP(work_time);
		/* Need help */
		sem_wait(s_workshop);
		sem_wait(s_mutex);
		need_help(elfID);
		sem_post(s_mutex);

		/* Get help */
		sem_wait(s_elf);
		sem_wait(s_mutex);
		// Critical section
		check_sign(elfID);
		get_help(elfID);
		sem_post(s_mutex);
	}
}

/** 
 * Elf function to determine whether workshop 
 * is closed or open
 * 
 * @param elfID Uniqie identification number of elf
*/
void check_sign(int elfID){
	if (shared_shop_closed[0] != 0){
		shared_counter[0]++;
		fprintf(out_file , "%d: Elf %d: taking holidays\n", shared_counter[0], elfID);
		fflush(out_file);
		sem_post(s_workshop);
		sem_post(s_elf);
		sem_post(s_mutex);
		exit(0);
	}
}

/** 
 * Elf function to print need help (used after elves work). 
 * If there are less than 3 elves in workshop, step in, otherwise wait.
 * 
 * @param elfID Unique identification number of elf
*/
void need_help(int elfID){
	shared_counter[0]++;
	fprintf(out_file, "%d: Elf %d: need help\n", shared_counter[0], elfID);
	fflush(out_file);
	shared_elf_counter[0]++;
	if (shared_elf_counter[0] == 3){
		sem_post(s_santa);
	}
	else {
		sem_post(s_workshop);
	}
}

/** 
 * Elf function to print get help (elf got help from santa in workshop).
 * 
 * @param elfID Unique identification number of elf
*/
void get_help(int elfID){
	shared_counter[0]++;
	fprintf(out_file, "%d: Elf %d: get help\n", shared_counter[0], elfID);
	fflush(out_file);
	shared_elf_counter[0]--;
	if (shared_elf_counter[0] == 0){
		sem_post(s_sleep);
	}
}

/** 
 * Santa function to print helping elves and signal 3 of them 
 * to step in workshop.
 * 
*/
void helping_elves(){
	fprintf(out_file, "%d: Santa: helping elves\n", shared_counter[0]);
	fflush(out_file);
	for (int i = 0; i < 3; i++){
		sem_post(s_elf);
	}
	sem_post(s_mutex);

	sem_wait(s_sleep);
	sem_wait(s_mutex);
	shared_counter[0]++;
	fprintf(out_file, "%d: Santa: going to sleep\n", shared_counter[0]);
	fflush(out_file);
	sem_post(s_workshop);
	sem_post(s_mutex);
}

/** 
 * Last santa function. After all reindeer got hitched
 * print Christmas started.
*/
void christmas_started(){
	if ( shared_reind_counter[0] != 0 ){
		sem_post(s_mutex);
		sem_wait(s_santa);
		sem_wait(s_mutex);
	}
	
		shared_counter[0]++;
		fprintf(out_file, "%d: Santa: Christmas started\n", shared_counter[0]);
		fflush(out_file);
}