library('tdatools')

#DISCRETE Lanscape tests:

test_that("PL is correct for one persistence pair.", {
	p1 <- matrix(c(0,2), nrow=1, ncol=2)
	pd <- p1
	pl <- landscape(pd, exact=FALSE,min_x=0, max_x=5,dx=0.1)

	x_val <- seq(0,5,0.1)
	y_val_0 <- seq(0,1,0.1)
	y_val_1 <- seq(1-0.1,0,-0.1)
	y_zeros <- rep(0,length(x_val)-(length(y_val_0)+length(y_val_1)))
	y_val <- c(y_val_0, y_val_1, y_zeros)
	expected <- array(cbind(x_val, y_val), c(1,51,2))

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})

#Helper function:
CircleUnif <- function(n,rad=1,centre=c(0,0)){ 
	x0 <- centre[1] ; y0 <- centre[2] 
	u <- 2*pi*runif(n) 
	rad*cbind(x=cos(u)+x0, y=sin(u)+y0) 
} 

matchDimension <- function(pl_1, pl_2){
	if(dim(pl_1)[1] < dim(pl_2)[1]){
		pl_1_t <- array(0, dim(pl_2))
		pl_1_t[1:dim(pl_1)[1],,] = pl_1
		pl_1_t[,,1] = pl_2[,,1]
		pl_1 = pl_1_t
	}

	else{
		pl_2_t <- array(0, dim(pl_1))
		pl_2_t[1:dim(pl_2)[1],,] = pl_2
		pl_2_t[,,1] = pl_1[,,1]
		pl_2 = pl_2_t
	}

	list(pl_1,pl_2)

}

sumPL <- function(pl1,pl2){
	pl_l = matchDimension(pl1, pl2)
	pl_1 = pl_l[[1]]
	pl_2 = pl_l[[2]]

	x <- pl_1[,,1]
	y <- (pl_1[,,2]+pl_2[,,2])

	array(c(x,y), dim=dim(pl_1))
}

scalePL <- function(lambda, pl){
	x <- pl[,,1]
	y <- lambda*pl[,,2]
	
	array(c(x,y), dim=dim(pl))
}

innerPL <- function(pl1,pl2){
	pl_l <- matchDimension(pl1,pl2)
	pl_1 <- as.vector(pl_l[[1]][,,2])
	pl_2 <- as.vector(pl_l[[2]][,,2])

	
	inner <- sum(pl_1%*%pl_2)
	inner*(pl1[1,2,1]-pl1[1,1,1])
}

test_that("PL sum is correct.", {
	X <- CircleUnif(100)
	Y <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pd2 <- diagram_pc(Y, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl2 <- landscape(pd2$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	print(pd$pairs[[1]])
	print(pl$getInternal()[1,,])

	pl_d = pl$getInternal()
	pl2_d = pl2$getInternal()

	expect_equal(PLsum(pl,pl2)$getInternal(), sumPL(pl_d, pl2_d))
})


test_that("add PL is correct.", {
	x <- CircleUnif(100)
	y <- CircleUnif(100)
	pd <- diagram_pc(x, dim_max=1, threshold=2)
	pd2 <- diagram_pc(y, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl2 <- landscape(pd2$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	
	pl_d = pl$getInternal()
	pl2_d = pl2$getInternal()

	expect_equal(pl$add(pl2)$getInternal(), sumPL(pl_d, pl2_d))
})


test_that("PL scale is correct.", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl_d = pl$getInternal()

	expect_equal(PLscale(0.5, pl)$getInternal(), scalePL(0.5, pl_d))
})

test_that("scale PL is correct.", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl_d = pl$getInternal()

	expect_equal(pl$scale(0.5)$getInternal(), scalePL(0.5, pl_d))
})

test_that("PL inner is correct.", {
	x <- CircleUnif(100)
	y <- CircleUnif(100)
	pd <- diagram_pc(x, dim_max=1, threshold=2)
	pd2 <- diagram_pc(y, dim_max=1, threshold=2)

	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl2 <- landscape(pd2$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	
	pl_d = pl$getInternal()
	pl2_d = pl2$getInternal()

	expect_equal(PLinner(pl,pl2), innerPL(pl_d,pl2_d))
})

test_that("inner PL is correct.", {
	x <- CircleUnif(100)
	y <- CircleUnif(100)
	pd <- diagram_pc(x, dim_max=1, threshold=2)
	pd2 <- diagram_pc(y, dim_max=1, threshold=2)

	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	pl2 <- landscape(pd2$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)
	
	pl_d = pl$getInternal()
	pl2_d = pl2$getInternal()

	expect_equal(pl$inner(pl2), innerPL(pl_d,pl2_d))
})

test_that("getExact from discrete is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)

	expect_error(pl$getExact())
})

test_that("getDiscrete from discrete is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)

	expect_error(pl$getDiscrete(), NA)
})

test_that("getInternal from discrete is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=FALSE, max_x=2.5, dx=0.1)

	expect_equal(pl$getInternal(), pl$getDiscrete())
})









