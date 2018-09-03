library(tdatools)


CircleUnif <- function(n,rad=1,centre=c(0,0)){
	x0 <- centre[1] ; y0 <- centre[2]
	u <- 2*pi*runif(n)
	rad*cbind(x=cos(u)+x0, y=sin(u)+y0)
}
 
X = CircleUnif(100)
pd = diagram(X, 'point-cloud', dim_max=1, threshold=0.5)
pl <- landscape(pd$pairs[[2]], exact=FALSE, dx=0.05, min_x=0, max_x=1)
PLplot(pl)
print(pl$getInternal()[1,,])
