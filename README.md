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
pd = diagram(X, 'point-cloud', dim_max=1, threshold=0.5)
```
Since we are satisfied with the other default parameters, we only change the ```dim_max``` parameter, which tells Ripser
the top dimension to compute homology in. 

The ```pd``` object is a class with three members, which can be accessed by
```R pd$member```. The first class member is ```R pd$pairs```, which contains the persistence pairs calculated from X,
indexed in a list by homology degree. So, for example, to view the degree 0 homology of X we can call

```R
print(pd$pairs[[1]])
```

You should see that all persitence pairs in degree 0 are born at 0 and die shortly after, which is what we expect.
Note that R indexing causes 0 degree homology to be stored at index 1. In general n degree homology is stored in index n+1.
Now lets look at homology in degree 1, which is what we were originally interested in.

```R
print(pd$pairs[[2]])
```

Now you should see a persistence pair born early and never dying. For example, I get

```R
          [,1] [,2]
[1,] 0.3216245  Inf
```

Meaning that a degree 1 persistence cycle was born at 0.3216 and did not die before the threshold value.

