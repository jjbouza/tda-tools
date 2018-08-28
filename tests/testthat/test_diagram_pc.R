library('tdatools')
#Tests for diamgram_pc function:

#Test 1: H_0 of two points
test_that("H_0 of two points within threshold is correct", {
	p1 = c(0,0)
	p2 = c(1,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=1, threshold=2)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0),c(1,Inf))
	expect_equal(H_0, expected)
})

#Test 2: H_0 of two points outside threshold
test_that("H_0 of two points within threshold is correct", {
	p1 = c(0,0)
	p2 = c(3,0)
	pd = diagram_pc(cbind(p1,p2), dim_max=1, threshold=2)
	H_0 = pd$pairs[[1]]
	expected = cbind(c(0,0), c(Inf,Inf))
	expect_equal(H_0, expected)
})
