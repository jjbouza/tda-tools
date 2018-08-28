#Test Description: Computes landscape for 10,000 points sampled from a torus. Then samples 100 points from a torus 10
#times and computes average landscape. Computes distance between these two PL's.
library('tdatools')
source('torus_sample.R')

print('Sampling points...')
points = TorusUnif(5000,1,5)

print('Computing PD...')
#Compute persistence:
pd <- diagram(points, 'point-cloud',dim_max=2,threshold=1)


print('Computing PL for 1-dim cycles...')
pl <- landscape(pd$pairs[[2]], exact=TRUE)
PLplot(pl)

print('Computing average persistence...')
pl_list <- c()
for(i in 1:2){
	points = TorusUnif(1000,1,5)
	pd <- diagram(points, 'point-cloud', dim_max=2,threshold=1)
	pl <- landscape(pd$pairs[[2]], exact=TRUE)
	pl_list <- c(pl_list, pl)
}


pl_average = average(pl_list)
PLplot(pl_average)
d = pl$add(pl_average$scale(-1))
PLplot(d)

print(PLinner(d,d))

