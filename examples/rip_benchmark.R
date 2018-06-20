library('methods')
library('tdatools')
source('torus_sample.R')

point_mat <- TorusUnif(5000,1,5)

start_time1 <- Sys.time()
pd <- diagram_pc(point_mat, 2, 2, 1, 0)
end_time1 <- Sys.time()

source("ripser_wrapper.R")
start_time <- Sys.time()
pd2 <- ripser_wrapper(point_mat, "point-cloud", 1, 1)
end_time <- Sys.time()

print('tda-tools Result:')
print(pd)

print('ripser_wrapper.R Result:')
print(pd2)

print('\n \n Time:')

print('tda-tools:')
print(end_time1-start_time1)
print('ripser_wrapper:')
print(end_time-start_time)


