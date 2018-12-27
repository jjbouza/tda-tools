library('tdatools')

#EXACT Lanscape tests:

test_that("PL is correct for one persistence pair.", {
	p1 <- matrix(c(0,2), nrow=1, ncol=2)
	pd <- p1
	pl <- landscape(pd, exact=TRUE,min_x=0, max_x=5,dx=0.1)
    print(pl)

	expect_equal(pl$getInternal(), expected, check.attributes=FALSE)
})
