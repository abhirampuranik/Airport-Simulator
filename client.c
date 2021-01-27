#include<stdio.h> 						//all inbuilt funtions
#include<stdlib.h> 						//for rand() and RAND_INT
#include<math.h> 						//used for exp()
#include<time.h> 						//used for time()
#include<limits.h> 						//used for value of INT_MAX
#include "server.h"
#define MAX 5							//maximum number of nodes the queue can hold

int main()
{
	int simulation_time, plane_no=1, landed = 0, tookoff = 0, refusals=0, idletime = 0, landingwaiting = 0, takeoffwaiting = 0;
	double probability_takeoff, probability_landing;
	int rand_landings,rand_takeoffs;
	/*
	simulation_time - number of times the simulation would occur
	plane_no - the number of the plane used in generation
	landed - tracks the number of planes landed
	tookoff - tracks the number of planes took off
	refusals - tracks the number of planes refused to use the runway
	idletime - number of time the runway was idle
	landingwaiting - total number of waiting of each landing plane
	takeoffwaiting - total number of waiting of each takeoff plane
	rand_landings - random number, that many times a plane is generated in one single simualation for landing
	rand_takeoffs - random number, that many times a plane is generated in one single simualation for takeoff
	*/

	Init_simulation(&simulation_time, &probability_takeoff, &probability_landing); //inputs for respective variables
	
	/*
	creating landing queue and taking off queue
	*/
	queue *landing;				// landing queue
	queue *takingoff;			// takeoff queue
	landing = malloc(sizeof(queue));
	takingoff = malloc(sizeof(queue));
	initialize(landing);
	initialize(takingoff);

	srand(time(0)); // this is to seed the random generation for every run, or else, every execution starting would look same.
	
	/*
	simulation starts here as a loop
	*/ 
	for(int T=1; T<=simulation_time; T++)
	{
		printf("\n");
		
		/*
		random number is generated
		the generation of planes to land happen here
		if the queue exceeds limit of number to hold, then the plane will be refused to be added into the queue
		*/
		rand_landings = random1(probability_landing);
		for(int planes = 0; planes < rand_landings; planes++ )
		{
			printf("    Plane %d is ready to land\n", plane_no);

			if((count(&(landing->front))) < MAX)
				new_plane(&(landing->front), &(landing->rear), plane_no, T);
			else
			{
				printf("    Plane %d directed to another airport\n",plane_no);		// here the plane_no which occurs is skipped and isnt added to any queue
				refusals++;
			}
			plane_no++;
		}


		/*
		random number is generated
		the generation of planes to takeoff happen here
		if the queue exceeds limit of number to hold, then the plane will be refused to be added into the queue
		*/
		rand_takeoffs = random1(probability_takeoff);
		for(int planes = 0; planes < rand_takeoffs; planes++)
		{
	 		printf("    Plane %d is ready to takeoff\n", plane_no);

			if((count(&(takingoff->front))) < MAX)
				new_plane(&(takingoff->front), &(takingoff->rear), plane_no, T);
			else
			{
				printf("    Plane %d is told to wait\n",plane_no);			// here the plane_no which occurs is skipped and isnt added to any queue
				refusals++;
			}
			plane_no++;
		}


		/*
		the landing and takeoff happens here only when the count of their queue is greater than 0.
		as the landing is first priority, the condition for landing is written first then the takeoff happens.
		takeoff of a plane occurs only when the count of landing queue is 0.

		if both landing and take off queue is empty, then the runway is considered idle in the given simulation
		*/
		if(count(&(landing->front))>0 )
		{
			printf("%d:",T );
			landingwaiting += T - ((landing->front) -> time);			// the landingwaiting time is calculated by subtracting the current simulation time and the time when plane was created of each plane landing
			printf(" Plane %d landed;",del_plane(&(landing->front), &(landing->rear)) );
			printf(" In landing queue %d units\n", count(&(landing->front)));		// displays number of planes left in the landing plane
			landed++;
		}
		else if(count(&(takingoff->front)) > 0)
		{
			printf("%d:",T );
			takeoffwaiting += T - ((takingoff->front) -> time);		// the takewaiting time is calculated by subtracting the current simulation time and the time when plane was created of each plane taking off
			printf(" Plane %d took off;",del_plane(&(takingoff->front), &(takingoff->rear)) );
			printf(" In takeoff queue %d units\n", count(&(takingoff->front)));		// displays number of planes left in the takeoff queue
			tookoff++;
		}
		else
		{
			printf("%d:",T );
			printf(" Runway is idle\n");		//occurs when landing and takeoff queue is empty
			idletime++;
		}
	}


	// end summary of the simulation
	printf("\n");
	printf ( "\tSimulation has concluded after %d units.\n", simulation_time ) ;
    printf ( "\tTotal number of planes processed: %d\n", plane_no-1 ) ; // total number of planes is plane_no subtracted by 1 because at end of the generation, the plane_no is increamented by 1
    printf ( "\t   Number of planes landed: %d\n", landed ) ;
    printf ( "\t   Number of planes taken off: %d\n", tookoff ) ;
    printf ( "\t   Number of planes refused use: %d\n", refusals ) ;
    printf ( "\t   Number left ready to land: %d\n", count(&(landing->front)) ) ;			//number of planes left in the landing queue
    printf ( "\t   Number left ready to take off: %d\n", count(&(takingoff->front)) ) ;	//number of planes left in the take off queue

    if ( simulation_time > 0 )
        printf ( "\t   Percentage of time runway idle: %.2lf \n", ( ( double ) idletime / simulation_time ) * 100.0 ) ;	//(total number of time the runway was idle)/(total number of simulation)*100

	if ( landed > 0 )
        printf ( "\t   Average wait time to land: %.2lf \n", ( ( double ) landingwaiting / landed ) ) ;		//(sum of landing waiting of all planes)/(total number of planes landed)
  
  	if ( tookoff > 0 )
        printf ( "\t   Average wait time to takeoff: %.2lf \n", ( ( double ) takeoffwaiting / tookoff ) ) ;		//(sum of landing waiting of all planes)/(total number of planes landed)

	return 0;
}
