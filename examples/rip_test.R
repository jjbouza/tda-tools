#Test Description: Computers Z_2 persistence pairs for some points in R^2 
library('tdatools')

#Synthetic data of a figure homology equivalent to a circle:
point_vector <- c(0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1, -1,0, -1,1)
#Structure as a list of points.
point_mat <- t(matrix(point_vector, nrow=2, ncol=8))

#Show an example of the rip_pc computation using the point cloud point_mat.
cat("\n \n \n Point Cloud Calculation: \n")
print(rip_pc(point_mat, 2, 2, 10, 1))

cat("------------------------------------------------------------------------------------------------------")

#Show an example of the rip_dm computation using the distance matrix dm.
dm = dist(point_mat)
cat("\n \n \n Distance matrix Calculation: \n")
print(rip_dm(dm, 2, 2, 10, 1))

cat("------------------------------------------------------------------------------------------------------")

#Illustration of output format (same for rip_dm):
persistence_data <- rip_pc(point_mat, 2, 2, 10, 1)
persistence_diagram <- persistence_data[["pairs"]]
persistence_cocycles <- persistence_data[["reps"]]

#Say we want care about 1-dim homology:
persistence_diagram_dim_1 <- persistence_diagram[[2]]
cat("\n \n \n Persistence pair in dimension 1: \n")
print(persistence_diagram_dim_1)
#And we want a representitive co-cycle for the only persistence pair in dim 1:
representitive_cocycle <- persistence_cocycles[[2]][[1]]


#Explanation of output format: The output in this case will be a nx3 matrix which will encode a co-cycle.
#Recall that a cocycle will be an element of Hom(C_n(X), Z_2), call this element \phi.
#Each row in the output will represent the action of \phi on a generator of C_n(X). I.e. the first 2 elements of row 1
#will represent a simplex, and the last element of row 1 will be the value that simplex takes in Z_2. 
#So if row 1 reads [0,1,1] then that means that the simplex from point 0 to point 1 (that is, the first and second
#points in point_mat) will be 1. 
#IMPORTANT NOTE: If a simplex takes value 0, then it will not be included in the output.
cat("\n \n \n Representitive co-cycle for 1-dimensional persistence pair: \n")
print(representitive_cocycle)

#NOTE:
#The function signature for rip_* is:
#   rip_*(points, coefficient_field, max_dimension, computation_threshold, do_cocycle)

# Where do_cocycles is 0 for NO and 1 for YES.

