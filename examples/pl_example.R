library('tdatools')
source('torus_sample.R')

print('Sampling points...')
points = TorusUnif(100,0.4,1)

print('Computing PD...')
#Compute persistence:
pd <- diagram(points, 'point-cloud', dim_max=2, threshold=3)

print('Computing PL for 1-dim cycles...')
pl <- landscape(pd$pairs[[2]], exact=TRUE)

print(dim(pd$pairs[[2]]))
print(pl$getInternal())
print(dim(pl$getInternal()))
