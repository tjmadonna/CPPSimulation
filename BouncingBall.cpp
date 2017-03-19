/* Tyler Madonna
* Due 9/13/2012
* Ball Bouncing Simulation */

/* Simulation of a ball bouncing with a perfectly elastic collision,
* using conservation of momentum: p = mv */

/*
*
* use equations F=MA and F = G * (M1 * M2)/R^2 to model ball falling from
* height. see http://hyperphysics.phy-astr.gsu.edu/Hbase/traj.html
* use form for objects on earth: A = a = 9.8m/s^2 
* velocity is integral of acceleration, distance is integral of velocity
* use sum to compute integrals v = v0 + a * dt, x = x0 + v * dt
*
* Version 0.0 - ball falls
* Version 0.5 - use cpu time to make movement "real time"
* Version 1 - formatting and comments added
*
*/
 


#include "C:\Python31\include\Python.h"
#include <string.h>
#include <time.h>

#define H0 100.0 	// initial height of ball in meters
#define A 9.8 		// acceleration due to gravity at earth's surface in meters/second^2
#define ENDTIME	20	// end time of the simulation in seconds
#define VMAX 44.2		// maximum ball velocity in meters/sec for a fall from 100m (from fallingBallRT simulation)

int main(int argc, char *argv[])
{ 
	double t, dt;		// simulation time and time step in seconds
	double x,y,z; 		// x, y, z position
	double v, a; 		// velocity and acceleration in the y direction
	char cmd[100];		// temp string for VPython commands
	double min_window;	// position window used to make the ball bounce
	clock_t starttime, 
			oldtime,
			newtime;	// cpu time values for real time calculations

	// initialize graphics
	Py_Initialize();
	PyRun_SimpleString("from visual import *\n");
	
	// set view position and range
	// center y position at half the ball's maximum height, set range as 20 from the center point 
	sprintf(cmd,"scene1 = display(autocenter = 0, autoscale = 0, center = (0,%f,0), range = %f)\n", H0/2.0, H0/2.0+20);
	PyRun_SimpleString(cmd);

	// put a square landing pad at 0,0,0
	PyRun_SimpleString("box(length = 50.0, height = 0.5, width = 50.0, color = color.blue)\n");


	// set initial conditions no x/z offset, no initial velocity
	// time will be defined using cpu time values
	a = -A;
	x = 0.0;
	y = H0;
	z = 0.0;
	v = 0.0;


	// place ball at initial position
	sprintf(cmd,"ball = sphere(color = color.red, radius = 2.0, pos = (%f,%f,%f))\n", x,y,z);
	PyRun_SimpleString(cmd);

	// add a box at the ball's initial position to check that the ball bounces back to the initial position each time
	// (the collision is perfectly elastic)
	sprintf(cmd,"box(length = 5.0, height = 5.0, width = 5.0, color = color.cyan,pos = (%f,%f,%f))\n", x,y,z);
	PyRun_SimpleString(cmd);

	// initialize real time values
	starttime = oldtime = newtime = clock(); 

	/* To simulate ball falling, step time forward by increments of dt
	* update velocity at each time point by incrementally computing the integral of acceleration: v = a*DT,
	* update position by incrementally computing the integral of velocity: y = v*DT (movement only occurs in the y direction)
	*/
	/* 
	* Simulate the ball bouncing according to conservation of momentum (p = mv).
	* To model a perfectly elastic collision: if the ball's momentum immediately before 'contact'
	* with the platform is p, then momentum immediately after 'contact' is -p.
	* The ball's mass does not change, so we can set v = -v once the position is within 
	* a certain window of y = 0 to simulate the bounce.
	*/

	while(((oldtime-starttime)/(double) CLOCKS_PER_SEC) < ENDTIME)
	{

		dt = (newtime - oldtime)/(double)CLOCKS_PER_SEC; // calculate dt in seconds
	
		/* 
		* At least one position value must fall within the window, so the minimum window 
		* size can be calculated using VMAX*dt, where VMAX is the velocity at the platform
		* printed in the fallingBallRT simulation
		*/
		min_window = VMAX*dt;

		// perform incremental integration
		v = v + a*dt; 
		y = y + v*dt;

		if(y <= min_window) 
		{
			v = -v;			// reverse velocity, 
			y = y + v*dt;	//return ball to its position at the start of the bounce
		}

		// display the ball's new position
		sprintf(cmd,"ball.pos = (%f,%f,%f)\n", x, y, z);
		PyRun_SimpleString(cmd);
			
		// set up to compute the new time interval
		oldtime = newtime; // use oldtime as the start time of each loop iteration
		newtime = clock(); // use newtime as the end time of each loop iteration

	}

	// close graphics
	Py_Finalize();

	return 0;

}







