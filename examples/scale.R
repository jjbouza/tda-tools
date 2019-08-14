
library('tdatools')
library('methods')
source('torus_sample.R')

x <- TorusUnif(4, 0.4, 1)
pd <- diagram_pc(x, dim_max=1, threshold=2)
pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)

pl_d = pl$getInternal()
    
print(pl_d)
print('\n')
print(pl$scale(0.5)$getInternal())

