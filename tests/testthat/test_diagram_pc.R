library('tdatools')
#Tests for diamgram_pc function:

#Test 1: H_0 of two points
test_that("H_0 of two points within threshold is correct", {
	p1 = c(0,0)
	p2 = c(1,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=0, threshold=2)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0),c(1,Inf))
	expect_equal(H_0, expected)
})

#Test 2: H_0 of two points outside threshold
test_that("H_0 of two points within threshold is correct", {
	p1 = c(0,0)
	p2 = c(3,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=0, threshold=2)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(Inf,Inf))
	expect_equal(H_0, expected)
})

#Test 3: H_0 of two points with threshold=Inf
test_that("H_0 of two points with threshold=Inf",{
	p1 = c(0,0)
	p2 = c(3,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=0, threshold=Inf)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(3,Inf))
	expect_equal(H_0, expected)

})

#Test 4: H_0 of two points with threshold=-1
test_that("H_0 of two points with threshold=Inf",{
	p1 = c(0,0)
	p2 = c(3,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=0, threshold=-1)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(3,Inf))
	expect_equal(H_0, expected)

})

#Helper function:
CircleUnif <- function(n,rad=1,centre=c(0,0)){ 
	x0 <- centre[1] ; y0 <- centre[2] 
	u <- 2*pi*runif(n) 
	rad*cbind(x=cos(u)+x0, y=sin(u)+y0) 
} 

#Test 5: H_1 of circle
test_that("H_1 of circle has large cycle",{
	#Sample from circle
	points = CircleUnif(900, rad=10)
	pd = diagram_pc(points, dim_max=1, threshold=5)
	H_1 = pd$pairs[[2]]
	expect_equal(H_1[1],0,tolerance=1)
	expect_equal(H_1[2],Inf)
})



#NEED TODO: Test Mod
