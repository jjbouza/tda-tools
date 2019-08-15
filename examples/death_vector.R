library('tdatools')
library('methods')
source('torus_sample.R')

x <- TorusUnif(4, 0.4, 1)
y <- TorusUnif(4, 0.4, 1)
pd <- diagram_pc(x, dim_max=1, threshold=2)

death_vector(pd)
