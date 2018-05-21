#Test Description: Computers Z_2 persistence pairs for 4 points in R^2. 

source('../load.R')
library('tdatools')

#Synthetic data containing the following points in R^2:
# [0,0], [1,0], [0,1]

point_vector <- c(0,0, 1,0, 0,1)
point_mat <- matrix(point_vector, nrows=3, ncols=2)
dist_mat = dist(point_mat)

print(rip(dist_mat, 9, 1, 2, 1, 0))
