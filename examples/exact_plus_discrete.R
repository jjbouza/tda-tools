# Here we add an exact landscape to a discrete one. The exact landscape is automatically converted to
# a discrete landscape and then added.



library('tdatools')
source('torus_sample.R')

print('Sampling points...')
points1 = TorusUnif(100,0.4,1)
pd1 <- diagram(points1, 'point-cloud',dim_max=2,threshold=3)

points2 = TorusUnif(100,0.4,1)
pd2 <- diagram(points2, 'point-cloud',dim_max=2,threshold=3)

print('Computing landscapes')
pl_exact <- landscape(pd1$pairs[[2]], exact=TRUE)
PLplot(pl_exact)
pl_discrete <- landscape(pd2$pairs[[2]], exact=FALSE, min_x=0, max_x=1.2, dx=0.00001)
PLplot(pl_discrete)


print('Summing')
result <- PLsum(pl_exact, pl_discrete)
PLplot(result)

