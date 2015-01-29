##FORWARD KINEMATICS

given joint angles and want to know where some point on the robot is as a funnction of those angles

	- commonly looking for what's called "the end effector"

			 _________________| < -- end effector example
			/theta
		   /
		  /
		 /
		- theta

###The Problem (of forward kinematics)
		- Given the current configuration of the robot, where is some point (w.r.t. some frame)?
		- How fast (and what direction) is the point moving?
		- Rigid Links
			- definition of rigid (body/links/etc): pick any two points on the body, and the distance between any two points on the rigid body remains constant over time.
		- Example:
			- How do we translate something like this:
			_______________________________
		   |							   |
		   |			x(t) ------------->| P
		   |________________________u______|							   |

		   - x is center of mass and in the global frame, P is in the global frame and U is constant in the local frame
		   - P. = X^ + Ru 
		   - where R is a rotation matrix from global to local

		   - Using a 4x4 matrix:

		   gTb * | u1 |    <-- global to local transformation then multiply by matrix
		   		 | u2 |	
		   		 | 1  |

###Coordinate Types
	- minimal/independent (as shown above)
	- robot affixed to the environment equivalent to joint space configuration 
	- "Floating base"

###Considering the first example:
	
			 _______q2________| < -- end effector example
			/theta
		q1 /
		  /
		 /
		- theta

	where q1 = 	 |  x1   |   
		   		 |  x2   |	
		   		 | theta |

	- so how do we make sure the joints are connected? 
	- if q are vectors with another local vector u and u' that point towards the joint, we have to ensure that:
		x1 + Ru = x2 + R'u'   OR
		x1 + Ru - x2 + R'u' = 0

	- So now we can talk about degrees of freedom!

###Degrees of freedom
	- same number of the minimal coordinates
	- other way is to find the maximum number of representations and subtract the number of constraints
		- (#of links * degrees-of-freedom/links) - constraint equations
		- in the above case we have 6 - 2 * 2 = 2 degrees-of-freedom

	- how many ways can you think of to constrain two bodies in three dimensions?
	- x,y,z and roll,pitch, yaw

###Calculating the derivative of a rotation matrix with respect to time:
	- R * RT = I
	- why is this true? It is orthonormality!
	- R * R^-1 = I 
	- then we take the derivative with respect to time: d/dt (R * RT = I)
	- SOOOOOO this is equivalent to:
	 (R' * RT) + (R * RT') = 0
	- Also know that (R*RT)T = (RT)T * R'T = R * R'T
	- ^^ = S * ST = 0 
	- where S = -ST <--- S is a skew symmetric matrix
	- R' = SR === R' = w~ R <--- omega hat. a.k.a angular velocity matrix
	

