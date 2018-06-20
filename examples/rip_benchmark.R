
library('methods')
library('tdatools')


x<-sample(1:2000, 2000)
y<-sample(1:2000, 2000)

point_mat <- matrix(c(x,y),nrow=length(x))

start_time <- Sys.time()
pd <- diagram_pc(point_mat, 2, 2, 2, 0)
end_time <- Sys.time()

print(end_time-start_time)

source("ripser_wrapper.R")
start_time <- Sys.time()
pd2 <- ripser_wrapper(point_mat, "point-cloud", 2, 2)
end_time <- Sys.time()

print(end_time-start_time)

