#Test Description: Computers Z_2 persistence pairs for some points in R^2 
library('tdatools')

#Synthetic data of a figure homology equivalent to a circle:
point_vector <- c(0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1, -1,0, -1,1)
#Structure as a list of points.
point_mat <- t(matrix(point_vector, nrow=2, ncol=8))

pd <- rip_pc(point_mat, 2, 2, 10, 1)
lsf.str("package:tdatools")
per_land <- PersistenceLandscapeInterface
pl <- new(per_land, pd, 1)

print(pl)

