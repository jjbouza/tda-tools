#Test Description: Computers Z_2 persistence pairs for 4 points in R^2. 

source('../load.R')
library('tdatools')

#Synthetic data containing the following points in R^2:
# [0,0], [1,0], [0,1]
point_vector <- c(0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1, -1,0, -1,1)
point_mat <- t(matrix(point_vector, nrow=2, ncol=8))
dist_mat = as.vector(dist(point_mat))


rip(dist_mat, 2, 2, 10, 1)
