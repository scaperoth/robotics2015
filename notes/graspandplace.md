##Pick and Place

###Piano movers problem! look it up
	- given initial config. qinit and goal configuration
	- qgoal, find a path q(t), for t E 0,1
	- s.t. every q(t) corresponds to a collision free configuration

####How to solve?
	- probabilistic roadmap algorithm! Look it up
		- Build a roadmap, G
		- insert qinit into the roadmap
		- insert qgoal into the roadmap
		- attempt to connect qinit and qgoal to the remainder of the roadmap
		- find a path from qinit to qgoal 

	- so how do we approach this algorithmically
		- G = (V,E) <-- what we want to return; a graph. 
		- input: n <-- some number of nodes in the graph
				 k <-- number of closest neighbors to examine
		1) V = {}, E = {}
		2) while |V| < n {
				q = random configuration <-- element of Cfree, or not in collision
				V = V U {q} 
		   }
		3) For all v in V
			Nq = k closest neighbors of v
			for all v' in Nq
				if(v,v') not in E and localPlanner(v,v') != NULL
					E = E U {(v, v')}