###Inverse Kinematics
	- Thought Exercise:
		- think of the joints in the robot as a series of nodes in a graph
          
	        o------o    Serial topology
     	   /
     joint/
		 /
		o link

		- looks like a branched _tree_...
		- most robots we will see are serial and either branched or unbranched
		- (note that other types do exist: parallel topology, closed chain, kinematic loop) 
		- Forward kinematics is easy... and inverse kinematics is NOT
			- P vs NP?
	- review: d/dt P = Ru + x_
			 	   P' = R'u + x_'

	- what is p? if...
		^
		|
		|
		|_______>

		theta = pi/2
		x = [0 0 0],  x' = [0 0 0], u=[1 0 0], w = | 0 | ~w = | 0 -1  0 |, ~w = |  0  -wz  wy |
												   | 0 | 	  | 1  0  0 |       |  wz  0  -wx |
												   | 1 |	  | 0  0  0 |       | -wy  wx  0  |
			  e1   e2  e3
		I = |  1   0   0 |
		    |  0   1   0 |		
		    |  0   0   1 |

		| 					|
		| T(e1) T(e2) T(e3) | 
		|					|