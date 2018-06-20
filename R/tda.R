library('Rcpp')
loadModule("Landscape", TRUE)
loadModule("Diagram", TRUE)



#Landscape R helper functions:

#' Compute landscape of a persistence diagram.
#'
#' @param PersistenceDiagram A list of persistence pairs.
#' @param exact Set to TRUE for exact computation, FALSE for discrete.
#' @param max_x Domain threshold for discrete PL.
#' @param dx Domain grid diameter for discrete PL.
#' @return A PersistenceLandscape object.
landscape <- function(PersistenceDiagram, exact=FALSE, max_x=10, dx=0.1){
	#Construct a persistence landscape
	new(PersistenceLandscape, PersistenceDiagram, exact, max_x, dx)
}

#This can be done in C++ as well but the performance gains are essentially zero.

#' Compute average of list of landscapes.
#'
#' @param PersistenceLanscapeList A list of PersistenceLandscape objects.
#' @return The average (in the vector space of PL's).
average <- function(PersistenceLanscapeList){
	mean = PLsum(PersistenceLanscapeList[[1]], PersistenceLanscapeList[[2]])
	i = 3
	while(i < length(PersistenceLanscapeList)){
		mean = mean$add(PersistenceLanscapeList[[i]])
		i  = i + 1
	}

	return(mean$scale(1.0/length(PersistenceLanscapeList)))
}


#Diagram R helper functions:

#The R dist function is actually implemented efficiently in C. 
#For this reason we can just compute dist in R and pass that to the ripser function
#that takes a distance matrix. See:
#https://stackoverflow.com/questions/36829700/rcpp-my-distance-matrix-program-is-slower-than-the-function-in-package?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa


#' Compute persistence diagram of data.
#'
#' @param input Either a distance matrix or a point cloud matrix.
#' @param type 'point-cloud' or 'distance-matrix'
#' @param modulus Homology coefficients.
#' @param threshold Maximum distance to generate edge in simplicial complex.
#' @param do_cocycles Set to 1 to generate cocycles, 0 otherwise.
#' @return The persistence data, including persistence pairs.
diagram <- function(input, type, modulus=2, dim_max=3, threshold=1000, do_cocycles=0){
	out <- NULL
	if(type == 'point-cloud'){
		out <- diagram_pc(input, modulus, dim_max, threshold, do_cocycles)
	}
	
	if(type == 'distance-matrix'){
		out <- diagram_dm(input, modulus, dim_max, threshold, do_cocycles)
	}

	return (out)
}

diagram_dm <- function(distances, modulus=2, dim_max=3, threshold=1000, do_cocycles=0) {
	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}

diagram_pc <- function(points, modulus=2, dim_max=3, threshold=1000, do_cocycles=0){
	distances <- as.vector(dist(points))
    	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}
