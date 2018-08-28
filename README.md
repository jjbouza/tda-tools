# tda-tools
## Author: Jose Bouza.

Internal TDA pipeline used in Peter Bubenik's group at UF. Currently use's code from the following packages:
1. The Ripser package by Ulrich Bauer: https://github.com/Ripser/ripser
2. The fork of the Ripser package which adds some easier interfacing by Chris Tralie: https://github.com/ctralie/ripser

## Quickstart Guide

The tdatools package supports two major persistent homology operations: Persistence Diagram computation and Persistent
Homology computation.

### Persistence Diagram Computation

The primary function to be aware of is
```R
diagram <- function(input, type, dim_max=3, threshold=Inf, modulus=2, do_cocycles=0)
```

The parameters are mostly self explanatory and are explained in the documentation. Say we want to compute first degree homology of points sampled from the unit circle. We can start by defining a function to uniformly sample from the circle.
```R
CircleUnif <- function(n,rad=1,centre=c(0,0)){
          x0 <- centre[1] ; y0 <- centre[2]
          u <- 2*pi*runif(n)
          rad*cbind(x=cos(u)+x0, y=sin(u)+y0)
 }
 
 X = CircleUnif(100)
```

This function generated an N by 2 matrix corresponding to the point cloud sampled from the circle. Now the persistence diagram calculation is simple:

```R
pd = diagram(X, 'point-cloud', dim_max=1)
```
Since we are satisfied with the other default parameters, we only change the ```dim_max``` parameter, which tells Ripser the top dimension to compute homology in. Lets look closer at the returned ```pd``` object.
