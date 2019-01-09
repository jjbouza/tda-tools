library('tdatools')

#EXACT Lanscape tests:

test_that("PL is correct for one persistence pair.", {
	p1 <- matrix(c(0,2), nrow=1, ncol=2)
	pd <- p1
	pl <- landscape(pd, exact=TRUE)
    expected <- list(matrix(c(-Inf,0,1,2,Inf,0,0,1,0,0), nrow=5, ncol=2))

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})


#Helper function:
CircleUnif <- function(n,rad=1,centre=c(0,0)){ 
	x0 <- centre[1] ; y0 <- centre[2] 
	u <- 2*pi*runif(n) 
	rad*cbind(x=cos(u)+x0, y=sin(u)+y0) 
} 

scale <- function(pl_exact, h){
    out <- list()

    for(i in 1:length(pl_exact)){
        out[[i]] <- h*pl_exact[[i]]
        out <- list(out,h*pl_exact[[i]])
    }
    print(length(out))
    
    return(out)
}

test_that("PL sum is correct for simple case.", {
	pd1 <- matrix(c(0,2), nrow=1, ncol=2)
	pd2 <- matrix(c(1,2), nrow=1, ncol=2)

	pl1 <- landscape(pd1, exact=TRUE)
	pl2 <- landscape(pd2, exact=TRUE)

    pl <- PLsum(pl1,pl2)
    expected <- list(matrix(c(-Inf,0,1,1.5,2,Inf,0,0,1,1,0,0), nrow=6, ncol=2))

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})


test_that("add PL is correct for simple case.", {
	pd1 <- matrix(c(0,2), nrow=1, ncol=2)
	pd2 <- matrix(c(1,2), nrow=1, ncol=2)

	pl1 <- landscape(pd1, exact=TRUE)
	pl2 <- landscape(pd2, exact=TRUE)

    pl <- pl1$add(pl2)
    expected <- list(matrix(c(-Inf,0,1,1.5,2,Inf,0,0,1,1,0,0), nrow=6, ncol=2))

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})


test_that("PL scale is correct.", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=TRUE)
	pl_d = pl$getInternal()

    print(length(pl_d))
	expect_equal(PLscale(0.5, pl)$getInternal(), scale(pl$getInternal(),0.5))
})



test_that("getExact from exact is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=TRUE)

	expect_error(pl$getExact(), NA)
})

test_that("getDiscrete from exact is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=TRUE)

	expect_error(pl$getDiscrete(), NA)
})

test_that("getInternal from discrete is correct", {
	X <- CircleUnif(100)
	pd <- diagram_pc(X, dim_max=1, threshold=2)
	pl <- landscape(pd$pairs[[1]], exact=TRUE)

	expect_equal(pl$getInternal(), pl$getExact())
})
