#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include "server.h"
#define MAX 5

/*
funtion to initialize the pointers of the queue in the beginning to NULL
*/
void initialize(queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

/*
function used for taking the inputs from the user
1. number of simulations
2. the probability of landing
3. the probability of takeoff
all these values have to be positive.
if the sum of porbablities are greater than 1, then the airport will saturate, this situation is not accepted
*/
void Init_simulation(int *simulation_time, double *probability_takeoff, double *probability_landing)
{

	printf("One plane can land or depart in each unit of time.\n");
	printf("Up to %d planes can be waiting to land or take off at any time\n",MAX );	

 	printf("How many units of time will the simulation run? ");
 	while(1)
 	{
		scanf("%d", simulation_time);
		if(*simulation_time<0)
			printf("Enter postive value: ");
		else
			break; 		
 	}
 	
	int  flag = 0;
	char choice;
	do{
		printf("Expected number of arrivals per unit time: ");
		scanf("%lf", probability_landing);

		printf("Expected number of departures per unit time: ");
		scanf("%lf", probability_takeoff);

		if (*probability_takeoff<0.0 || *probability_landing<0.0)
		{
			printf("These numbers must be positive\n");
			flag = 0;
		}
		else
		{
			if(*probability_landing + *probability_takeoff > 1.0)    // sum of probablities shouldn't be greater than 1, therefore, inputs are accepted again
			{
				printf("\nThe sum of probabilites of arrival and departure is greater than one.\nThe airport will become saturated, the sum of probablities should be less than 1\n\n");
				flag == 0;
			}
			else
				flag = 1;

		}
	}while (flag == 0);
}


/*
This uses poissons distribution for generating random numbers,
usually this gives numbers between 0 - 3(but varies depending the random number generated)
*/
int random1(double expect)
{
 	int n=0;
 	float x = rand()/(double)RAND_MAX;
	while(x >exp(-(expect*1.5)))
 	{
 		n++;
 		x *= rand()/(double)INT_MAX;// INT_MAX is used to reduce the number of loops
 	}
 	return n;	
}


/*
function is used to create a new plane and put it in the queue
double pointer is used because the address of the pointers of front and rear(of the landing and takeoff queue) is passed everytime 
we pass the address of the pointer to this function
*/
void new_plane(plane **front, plane **rear, int x, int T)
{
	/*
	*front - the pointer which points the address of the front pointer passed(land_front or takeoff_front)
	*rear - the pointer which points the address of the rear pointer passed(land_rear or takeoff_rear)
	x - the number of the plane passed for generation
	T - the simulation time the plane is asked to be generated, this is considered for landingwaiting later
	*/

 	plane *temp = (plane *)malloc(sizeof(plane));  // temp is a temporary plane node created for creating a plane 
 	temp->planenum = x;
 	temp->time = T;
 	temp->next = NULL;
 	if(*front == NULL && *rear == NULL) // if the queue is empty
 	{
 		
 		*front = temp;
 		*rear = temp;
 	}
 	else
 	{
 		
 		(*rear)->next = temp;		// this is enqueue process for a queue with existing nodes
 		*rear = temp;
 	}
 }
 

/*
Funtion used to delete the plane from the front node(meaning a plane tookoff or landed)
Double pointer is used as an address of the front or rear node is passed to the function
This returns the plane number deleted
*/
int del_plane(plane **front, plane **rear)
{
	/*
	*front - the pointer which points the address of the front pointer passed(land_front or takeoff_front)
	*rear - the pointer which points the address of the rear pointer passed(land_rear or takeoff_rear)
	*/

	plane *temp = (plane *)malloc(sizeof(plane));  // creation of temporary node for returning planenum
 	if(*front == *rear)				// if there exist only one node, then both rear and front points to the same
 	{
 		temp = *front;
 		*front = NULL;
 		*rear = NULL;
 		return temp->planenum;
 	}
 	else if( *front != *rear)		// this is dequeue process for a queue with nodes present
 	{
 		temp = *front;
 		*front = (*front)->next;
 		return temp->planenum;
 	}
 }


/*
Function to return the number of nodes present in the given queue
It accepts the address the front pointer and copies it to a temporary plane node and traverse until the next of some node is NULL
returns the number of nodes present in the given queue
*/
int count(plane **front)
{
	//*front - the pointer which points the address of the front pointer passed(land_front or takeoff_front)
 	int count = 0;
 	plane *temp = (plane *)malloc(sizeof(plane));
 	temp = *front;
 	while(temp != NULL)
 	{
	 	count++;
	 	temp = temp->next;
 	}
 	return count;
 }