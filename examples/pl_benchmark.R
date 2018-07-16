#Test Description: Computes landscape for 10,000 points sampled from a torus. Then samples 100 points from a torus 10
#times and computes average landscape. Computes distance between these two PL's.
library('tdatools')
source('torus_sample.R')
source('tda_functions.R')

print('Sampling points...')
points = TorusUnif(5000,1,5)

print('Computing PD...')
#Compute persistence:
pd <- diagram(points, 'point-cloud', modulus=2, dim_max=2,threshold=1,do_cocycles=0)

#Calculuate PL by going directly to Discrete (my method)
begin = Sys.time()
pl_direct = landscape(pd$pairs[[2]], exact=FALSE, dx=0.0005)
end = Sys.time()

#Calculate PL by going pd -> exact pl -> discrete PL
begin_ind = Sys.time()
pl_indirect_exact = landscape(pd$pairs[[2]], exact=TRUE)
pl_indirect_exact_in = pl_indirect_exact$getInternal()
pl_param_vals <- seq(0,10,0.1)
pl_indirect = persistence_landscape_discretize(pl_indirect_exact_in, pl_param_vals) 
end_ind = Sys.time()

print('Direct method:')
print(end -  begin)
print('Indirect method:')
print(end_ind - begin_ind)

