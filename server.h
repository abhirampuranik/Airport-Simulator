typedef struct node
{
 	int planenum;		// the number of the current plane
 	int time;		// the time when the plane was created(will the value of the simulation time it was created)
 	struct node *next;	// next is the one which is used to point the next node's adress
}plane;

typedef struct queue
{
 	plane *front;		// front pointer
 	plane *rear;		// rear pointer
}queue;

//every function is explained in the defining part.
//double pointer is used for storing the address of the pointer passed
void Init_simulation(int *, double *, double *);
void initialize(queue *);
void new_plane(plane **, plane **, int, int);
int del_plane(plane **, plane **);
int count(plane **);
int random1(double);