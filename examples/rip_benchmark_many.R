library('tdatools')

x<-sample(1:200, 200)
y<-sample(1:200, 200)

point_mat <- matrix(c(x,y),nrow=length(x))


start_time <- Sys.time()
for(i in 1:1000){
	pd <- diagram_pc(point_mat, 2, 2, 2, 1)
}
end_time <- Sys.time()
print('tda-tools')
print(end_time- start_time)

source("ripser_wrapper.R")
start_time <- Sys.time()
for(i in 1:1000){
	pd2 <- ripser_wrapper(point_mat, "point-cloud", 1, 2)
}
end_time <- Sys.time()
print('ripser_wrapper:')
print(end_time-start_time)
