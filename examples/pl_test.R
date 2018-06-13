#Test Description: Computers Z_2 persistence pairs for some points in R^2 
library('methods')
library('tdatools')

#Synthetic data of a figure homology equivalent to a circle:
point_vector <- c(0,1, 1,1, 1,0)
#Structure as a list of points.
point_mat <- t(matrix(point_vector, nrow=2, ncol=2))

pd <- rip_pc(point_mat, 2, 2, 10, 0)

pl1 <- new(landscape, pd$pairs[[1]], TRUE)
pl2 <- new(landscape, pd$pairs[[1]], TRUE)
pl_list = c(pl1, pl2)
pl = average(c(pl1, pl2))

print(pl$getExact())
print(pl1$getExact())
