#Test Description: Computers Z_2 persistence pairs for some points in R^2 
library('methods')
library('tdatools')

#Synthetic data of a figure homology equivalent to a circle:
point_vector_1 <- c(0,1, 1,1, 1,0)
point_vector_2 <- c(-1,0, 1,4, -1,0)
#Structure as a list of points.
point_mat_1 <- t(matrix(point_vector_1, nrow=2, ncol=2))
point_mat_2 <- t(matrix(point_vector_2, nrow=2, ncol=2))

#Persistence Diagrams of points
pd1 <- diagram_pc(point_mat_1, 2, 2, 10, 0)
pd2 <- diagram_pc(point_mat_2, 2, 2, 10, 0)

#Landscapes of first degree persistence (connected components), computed exactly.
pl1 <-  landscape(pd1$pairs[[1]], FALSE)

print(pl1$getDiscrete())

####################Examples of operations:#################################

#Average
pl_list = c(pl1, pl2)
pl_average = average(c(pl1, pl2))

#Sum
pl_sum_inline = pl1$add(pl2)
pl_sum_func = PLsum(pl1, pl2)

#Scale
pl_scale_inline = pl1$scale(10)
pl_scale_func = PLscale(10,pl1)

#Inner Product
pl_inner_inline = pl1$inner(pl2)
pl_inner_func = PLinner(pl1,pl2)



print('Done')




