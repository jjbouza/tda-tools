#Test Description: Computes landscape for 10,000 points sampled from a torus. Then samples 100 points from a torus 10
#times and computes average landscape. Computes distance between these two PL's.
library('tdatools')
source('torus_sample.R')

print('Sampling points...')
points = TorusUnif(100,0.4,1)

print('Computing PD...')
#Compute persistence:
pd <- diagram(points, 'point-cloud',dim_max=2,threshold=3)

print('Computing PL for 1-dim cycles...')
pl <- landscape(pd$pairs[[2]], exact=TRUE)
PLplot(pl)

pl_list <- c()
for(i in 1:100){
	points = TorusUnif(100,0.4,1)
	pd <- diagram(points, 'point-cloud', dim_max=2,threshold=3)
	pl <- landscape(pd$pairs[[2]], exact=TRUE)
	pl_list <- c(pl_list, pl)
}


print('Computing average persistence...')
pl_average = PLaverage(pl_list)
PLplot(pl_average)
d = pl$add(pl_average$scale(-1))

print('Inner Product')
print(PLinner(d,d))

