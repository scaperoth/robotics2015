Rotation matricies

Axis angle

Euler angles

Unit quaternarions

Rot(z) = {
	cosbeta -sinbeta 	0
	siny 	 cosbeta 	0
	0		 0			0
}

Rot(x) = {
	1 		0 			0
	siny 	cosalpha 	sinalpha
	0		-sinalpha	cosalpha
}

Rot(y) = {
	cosbeta		0 		-sinBeta
	0 			1 		0	
	singBeta	0		cosBeta
}

But how can we combine transformations? 
Matrix Multiplication

So what happens when we rot(z) * rot(y) * rot(x)? (using notation F'RF)

is it the same if we do Rot(x) * rot(y) * rot(z)? NO

They are not equivalent because matrix multiplication is not commutative.

####Euler angles: Example -> x*z*x

	- roll pitch yaw: independent degrees of freedom can be combined together to get full range

	- Euler: (look for ken shoemake to use eulers)
		- Disadvantages:
			- it's hard to communicate 
			- Multiple singularities (see below)
			- 24 representations! x*y*z, x*z*x, y*x*y ...
		- Advantages: 
			- minimal representation

	- Multiple singularities:
		- Is the nth rotation around the local or global frame?
		- Example:
			x = alph z = beta x = gamma
			x - z - x

			(0,0,0)
			(pi/2, 0, pi/2)

			^   
			|  
			| 
			|------->
			/
	       /
	      /

	 	multiple solutions for the problem:
	 	alpha1 beta1 gamma1 = (x,y)
	 	alpha2 beta2 gamma2 = (x,y)
	 	alpha3 beta3 gamma3 = (x,y)

	 	how do you get back to the correct equation from the solution? No fun. 

	 Rotation matrix -> euler angles is very tricky to do; it complicates things. 

	 Finally considering "Gimbal lock"
	 	- two of the axis align (lose a rotational degree of freedom)
	 	- look for YouTube video: Euler angle gimbal lock

####Rotation Matrix: (A must use) 
=  	{
		x^ y^ z^
	}
	9 values (3 vectors)

		- Example: {
						0 	1	0 
						1 	0 	0
						0 	0  -1
					}

				  x	^   /z
					|  /
					| / 
					|-------->y

		- Advantages:
			- human readable
			- easily invertible 
			- each matrix maps to a unique orientation and vice versa
		- Disadvantages
			- Lots of extra data (extrenuous)
				- we can always determine z (right handed coordinate system): z^ = x*y
				- how can we account for the other 3 redundancies?
					- hint: orthonormality 
					- they all have to be mutually ortahgonal 
					- x * y = 0
					- ||x|| = 1 
					- ||y|| = 1
			- Nasty numerical drift!
				- Example: matrix - matrix multiplication (R)
				- R(t) = integral(R dt)
				- particular problem when...
					- doing simulations
					- working with IMU's
				- But you can restore orthonormality
					- Gramm Schmidt orthogonalization (NO)
					- QR Factorization

Vision of semi-autonomous systems

####Axis-angle
	- specify arbitrary axis (v^)
	- rotate around that axis by some theta
	- non-minimal representation
	- v = [x y z] & theta
	- What you end up with? LOOK IT UP
	- follows orthonormality constraints
	- good source: Ravelin, Eigen
	- Advantages:
		- easy to communicate
		- easy for humans to use
	- Disadvantages
		- rotation matrix -> axis angle
			- sin(theta) = 0 can run into problems
			- theta = 0 , pi/2
			- if theta is 0, it can be a rotation around any axis; SINGULARITY! (bad...)
		- rotation about v by theta is equivalent to -v by -theta

###Unit Quaternion (another must use):
	- =  (V, E)  <-- vector, scalar
	- [qx qy qz] qw
		- qx^2 + qxy2 + qz^2 + qw^2 = 1
	- Disadvantages:
		- tough to conceptualize! (don't try just yet. You'll hurt yourself)
		- non-minimal representation
	- Advantages:
		- convert to rotation matrix. LOOK IT UP
		- also, rotation matrix -> quaternion LOOK IT UP
		- Quaternion1 * Quaternion2 is equivalent to two rotations!
		- inversion is very fast; no numerical issues
		- _extremely_ fast vector rotation
		- minimal drift representation
		- no singularity!!
		- if you want to smoothly interpolate between two orientations unit quaternions are great at this