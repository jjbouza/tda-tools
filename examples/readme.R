library(tdatools)


CircleUnif <- function(n,rad=1,centre=c(0,0)){
	x0 <- centre[1] ; y0 <- centre[2]
	u <- 2*pi*runif(n)
	rad*cbind(x=cos(u)+x0, y=sin(u)+y0)
}

#Generate
X = CircleUnif(100)
pd = diagram(X, 'point-cloud', dim_max=1, threshold=0.5)
pl <- landscape(pd$pairs[[2]], exact=FALSE, dx=0.1, min_x=0, max_x=1)


#Plot/Print
print(dim(pl$getInternal()))
print(pl$getInternal()[1,,])
PDplot(pd$pairs[[1]])
PDplot(pd$pairs[[2]])
PLplot(pl)

#Generate second
X2 <- CircleUnif(100)
pd2 <- diagram(X2, 'point-cloud', dim_max=1, threshold=0.5)
pl2 <- landscape(pd2$pairs[[2]], exact=FALSE, dx=0.1, min_x=0, max_x=1)

#Average
pl_sum <- PLsum(pl,pl2)
pl_average <- PLscale(0.5, pl_sum)
PLplot(pl_average)

#Diff
diff <- PLsum(pl,PLscale(-1,pl_average))
inner <- PLinner(diff,diff)
print(inner)


