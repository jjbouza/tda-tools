library('tdatools')
library('methods')
source('torus_sample.R')

x <- TorusUnif(4, 0.4, 1)
y <- TorusUnif(4, 0.4, 1)
pd <- diagram_pc(x, dim_max=1, threshold=2)
pd2 <- diagram_pc(y, dim_max=1, threshold=2)
pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
pl2 <- landscape(pd2$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)

pl_d = pl$getInternal()
pl2_d = pl2$getInternal()
    
print(pl_d)
print('\n')
print(pl2_d)
print('\n')
print(PLaverage(list(pl,pl2))$getInternal())

