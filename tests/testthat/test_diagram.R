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

#Test 4: H_0 of two points with threshold=-1
test_that("H_0 of two points with threshold=0.5",{
	p1 = c(0,0)
	p2 = c(0.2,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=0, threshold=0.5)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(0.2,Inf))
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

#Test 6: H_1 of cicle using diagram
test_that("H_1 of circle has large cycle with diagram",{
	#Sample from circle
	points = CircleUnif(900, rad=10)
	pd = diagram_pc(points, dim_max=1, threshold=5)
	pd2 = diagram(points, type='point-cloud', dim_max=1, threshold=5)
	H_11 = pd$pairs[[2]]
	H_12 = pd2$pairs[[2]]
	expect_equal(H_11, H_12)
})

#Test 7: H_0 of two points outside threshold using diagram
test_that("H_0 of two points within threshold is correct with diagram", {
	p1 = c(0,0)
	p2 = c(3,0)
	pd = diagram(cbind(p1,p2), type='point-cloud',dim_max=0, threshold=0.5)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(Inf,Inf))
	expect_equal(H_0, expected)
})

#Test 8: distance_matrix input works with dist object
test_that("Distance matrix input works with dist object",{
    dm = as.dist(matrix(c(1,1,1,1), nrow=2, ncol=2))
	pd = diagram_dm(dm, dim_max=0, threshold=3)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(1,Inf))
	expect_equal(H_0, expected)
})

#Test 9: distance_matrix input works with matrix object
test_that("Distance matrix input works with dist object",{
    dm = matrix(c(1,1,1,1), nrow=2, ncol=2)
	pd = diagram_dm(dm, dim_max=0, threshold=3)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(1,Inf))
	expect_equal(H_0, expected)
})

#Test 10: distance_matrix input works with matrix object
test_that("Distance matrix input works with dist object",{
    dm = as.vector(as.dist(matrix(c(1,1,1,1), nrow=2, ncol=2)))
	pd = diagram_dm(dm, dim_max=0, threshold=3)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(1,Inf))
	expect_equal(H_0, expected)
})

#NEED TODO: Test Mod
