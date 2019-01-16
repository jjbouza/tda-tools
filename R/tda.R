library('Rcpp')
loadModule("Landscape", TRUE)
loadModule("Diagram", TRUE)


myColorRamp <- function(colors, values) {
	library("wesanderson")
        wes_palette('GrandBudapest1',values,type='continuous')
}

accessLevel <- function(internal, level){
    #Trick to check if internal is vector or list.
    #From: https://stackoverflow.com/questions/19501186/how-to-test-if-object-is-a-vector
    if (is.atomic(internal)){
        return(internal[level,,])
    }
    else{
        return(internal[[level]])
    }

}

numLevels <- function(internal){
    if (is.atomic(internal)){
        return(dim(internal)[1])
    }
    else{
        return(length(internal))
    }
}

#Landscape R helper functions:

#' Plot a persistenceLandscape object.
#' @param PersistenceLandscape A PL object.
#' @param  infinity_sub When using an exact representation, a value of Inf can appear in the PL. infinity_sub replaces
#' this with a different value in the plot.
PLplot <- function(PersistenceLandscape, infinity_sub=-1){
	
	internal <- PersistenceLandscape$getInternal()
    for(level in 1:numLevels(internal)){
        level_d <- accessLevel(internal,level)
        if(infinity_sub != -1){
            level_d[level_d == Inf] <- infinity_sub
            level_d[level_d == -Inf] <- -infinity_sub
        }
    }
	level1 <- accessLevel(internal,1)
    
	cols <- myColorRamp(c("red", "blue"), numLevels(internal))
	plot(level1[,1],level1[,2], type='l', xlab='x', col=cols[1], lwd=2)

	if(numLevels(internal) > 1){
		for(level in 2:numLevels(internal)){
			level_d <- accessLevel(internal,level)
			lines(level_d[,1], level_d[,2], col=cols[level+1], lwd=2)
		}
	}
}

PDplot <- function(PersistenceDiagram){
	finite <- matrix(PersistenceDiagram[PersistenceDiagram[,2] != Inf], ncol=2)
	infinite <- matrix(PersistenceDiagram[PersistenceDiagram[,2] == Inf], ncol=2)
	max = 0

	if(dim(finite)[1] > 0){
		max = rep(max(finite), dim(infinite)[1])
		min_z = min(finite)
		max_z = max(finite)
		plot(finite[,1], finite[,2], xlim=c(min_z, max_z), ylim=c(min_z,max_z), col='blue')
	}
	else{
		max = 1
		plot(-1,-1, xlim=c(0,1), ylim = c(0,1))
	}

  	abline(0,1)
	par(xpd=TRUE)
	if (dim(infinite)[1] > 0){
		points(infinite[,1], 1.1*max, pch=8, col='red')
	}
	par(xpd=FALSE)
}


#' Compute landscape of a persistence diagram.
#'
#' @param PersistenceDiagram A pd object from the diagram function, or a list of pairs.
#' @param exact Set to TRUE for exact computation, FALSE for discrete.
#' @param max_x Domain threshold for discrete PL.
#' @param dx Domain grid diameter for discrete PL.
#' @param degree If input is pd object then degree specifies which degree to select from.
#' @return A PersistenceLandscape object.
landscape <- function(PersistenceDiagram, degree=NULL, exact=FALSE, dx=0.1,  min_x=0, max_x=10, threshold=1000){

    diagram = NULL
    
    #Automatic parameter deduction logic:
    
    # Input is bd pairs (no threshold info).
    if( is.atomic(PersistenceDiagram) || is.null(PersistenceDiagram$param.threshold)){
        diagram = PersistenceDiagram
        
        #We check if diagram has max death value < infinity:
        if(max(diagram[,2]) < Inf){
            max_y = max(diagram[,2])
        }
        
        else{
            max_y = max_y
        }
    }
    
    # Input is from diagram output.
    else{
        if(is.null(degree)){
            stop("Error: If input persistence diagram is directly from 
                 the diagram fucntion then a homological degree degree must be specified (add degree=__ to landscape call). ")
            
        }

        else{
            diagram = PersistenceDiagram$pairs[[degree]]
            max_y = PersistenceDiagram$param.threshold
        }
    }


	#Construct a persistence landscape.
	landscape_raw <- new(PersistenceLandscape, diagram, exact, min_x, max_x, dx, max_y)
}


#' Consider using divide and conquer algo. here.

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
diagram <- function(input, type, dim_max=3, threshold=Inf, modulus=2, do_cocycles=0){
	out <- NULL
	if(type == 'point-cloud'){
		out <- diagram_pc(input, dim_max, threshold, modulus, do_cocycles)
	}
	
	if(type == 'distance-matrix'){
		out <- diagram_dm(input, dim_max, threshold, modulus, do_cocycles)
	}

	return (out)
}


diagram_dm <- function(distances, dim_max=3, threshold=Inf, modulus=2, do_cocycles=0) {
    #Process type of distance matrix:
    dist_obj <- 0

    if (class(distances) == "dist"){
        dist_obj = as.vector(distances)
    }

    else if (class(distances) == "matrix"){
        dist_obj = as.vector(as.dist(distances))
    }

    else if (is.vector(distances)){
        dist_obj <- distances
    }

    else{
        stop("Error: Distance matrix input type unsupported.")
    }

	rip_raw(dist_obj, modulus, dim_max, threshold, do_cocycles)
}

diagram_pc <- function(points, dim_max=3, threshold=Inf, modulus=2, do_cocycles=0){
	distances <- as.vector(dist(points))
    rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}
