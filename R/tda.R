library('Rcpp')
loadModule("Landscape", TRUE)
loadModule("Diagram", TRUE)



#Landscape R helper functions:

#' Plot a persistenceLandscape object.
#' @param PersistenceLandscape A PL object.
#' @param  infinity_sub When using an exact representation, a value of Inf can appear in the PL. infinity_sub replaces
#' this with a different value in the plot. If infinity_sub=-1 then the Inf point will be excluded. If infinity_sub is
#' any other number then the Inf point will be replaced by that number.
PLplot <- function(PersistenceLandscape, infinity_sub=-1){
	
	#Needed to process Inf values:
	level_process <- function(level, infinity_sub){
		level <- t(level)
		if(infinity_sub == -1){
			level <- level[,(1:dim(level)[2]-1)]
		}

		else{
			level[level == Inf] <- infinity_sub
		}


		return(level)
	}
	
	internal <- PersistenceLandscape$getInternal()
	level1 <- level_process(internal[[1]], infinity_sub)
	plot(level1[1,],level1[2,], type='l', xlab='x', ylab='')

	for(level in internal){
		level <- level_process(level, infinity_sub)
		lines(level[1,], level[2,])
	}
}


#' Compute landscape of a persistence diagram.
#'
#' @param PersistenceDiagram A list of persistence pairs.
#' @param exact Set to TRUE for exact computation, FALSE for discrete.
#' @param max_x Domain threshold for discrete PL.
#' @param dx Domain grid diameter for discrete PL.
#' @return A PersistenceLandscape object.
landscape <- function(PersistenceDiagram, exact=FALSE, max_x=10, dx=0.1){
	#Construct a persistence landscape
	landscape_raw <- new(PersistenceLandscape, PersistenceDiagram, exact, max_x, dx)
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

	#return(mean$scale(1.0/length(PersistenceLanscapeList)))
	return(mean)
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
diagram <- function(input, type, dim_max=3, threshold=1000, modulus=2, do_cocycles=0){
	out <- NULL
	if(type == 'point-cloud'){
		out <- diagram_pc(input, modulus, dim_max, threshold, do_cocycles)
	}
	
	if(type == 'distance-matrix'){
		out <- diagram_dm(input, modulus, dim_max, threshold, do_cocycles)
	}

	return (out)
}

diagram_dm <- function(distances, dim_max=3, threshold=1000, modulus=2, do_cocycles=0) {
	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}

diagram_pc <- function(points, dim_max=3, threshold=1000, modulus=2, do_cocycles=0){
	distances <- as.vector(dist(points))
    	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}
