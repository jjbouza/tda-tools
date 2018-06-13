library('Rcpp')
loadModule("landscape", TRUE)
loadModule("diagram", TRUE)



average <- function(PersistenceLanscapeList){
	mean = PLsum(PersistenceLanscapeList[[1]], PersistenceLanscapeList[[2]])
	i = 3
	while(i < length(PersistenceLanscapeList)){
		mean = mean$sum(PersistenceLanscapeList[[i]])
		i  = i + 1
	}

	mean$scale(1.0/length(PersistenceLanscapeList))
}


#Diagram R helper functions:

#The R dist function is actually implemented efficiently in C. 
#For this reason we can just compute dist in R and pass that to the ripser function
#that takes a distance matrix. See:
#https://stackoverflow.com/questions/36829700/rcpp-my-distance-matrix-program-is-slower-than-the-function-in-package?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa


rip_dm <- function(distances, modulus=2, dim_max=3, threshold=1000, do_cocycles=0) {
	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}

rip_pc <- function(points, modulus=2, dim_max=3, threshold=1000, do_cocycles=0){
	distances <- as.vector(dist(points))
    	rip_raw(as.vector(distances), modulus, dim_max, threshold, do_cocycles)
}
