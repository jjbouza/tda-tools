library('tdatools')

#DISCRETE Lanscape tests:

test_that("PL is correct for one persistence pair.", {
	p1 = matrix(c(0,2), nrow=2, ncol=1)
	pd = p1
	pl = landscape(pd, exact=FALSE,min_x=0, max_x=5,dx=0.1)
	x_val = seq(0,5,0.1)
	y_val_0 = seq(0,1,0.1)
	y_val_1 = seq(1-0.1,0,-0.1)
	y_zeros = rep(0,length(x_val)-(length(y_val_0)+length(y_val_1)))
	y_val = c(y_val_0, y_val_1, y_zeros)
	expected = list(cbind(x_val, y_val))

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})
