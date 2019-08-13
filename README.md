# tda-tools
## Author: Jose Bouza.

Internal TDA pipeline used in Peter Bubenik's group at UF. Currently use's modified code from the following packages:
1. The Ripser package by Ulrich Bauer: https://github.com/Ripser/ripser
2. The fork of the Ripser package which adds some easier interfacing by Chris Tralie: https://github.com/ctralie/ripser
3. The Persistence Landscape toolbox by Paweł Dłotko: https://www.math.upenn.edu/~dlotko/persistenceLandscape.html

### Installation Guide
1. Since the package is not on CRAN, you will need to install devtools on your system. To do this just open an R session and run:
install.packages(‘devtools’)

2. Next clone the github repo either using
git clone https://github.com/jjbouza/tda-tools.git
or just download from Github using the green button.

3. Run the install.sh bash script in the cloned repo using ./install.sh or bash install.sh, this script will compile the C++ code and install a package “tdatools” to your R package directory along with all of the dependencies. 


Now you should be able to load the package normally from an R session using
library(‘tdatools’)
## Quickstart Guide

The ```tdatools``` package supports two major persistent homology operations: Persistence Diagram computation and Persistence
Landscape computation.

### Persistence Diagram Computation

The primary function to be aware of is
```R
diagram <- function(input, type, dim_max=3, threshold=Inf, modulus=2, do_cocycles=0)
```

The parameters are mostly self explanatory. Say we want to compute first degree homology of points sampled from the unit circle. We can start by defining a function to uniformly sample from the circle.
```R
CircleUnif <- function(n,rad=1,centre=c(0,0)){
          x0 <- centre[1] ; y0 <- centre[2]
          u <- 2*pi*runif(n)
          rad*cbind(x=cos(u)+x0, y=sin(u)+y0)
 }
 
 X <- CircleUnif(100)
```

This function generated an N by 2 matrix corresponding to the point cloud sampled from the circle. Now the persistence diagram calculation is simple:

```R
pd <- diagram(X, 'point-cloud', dim_max=1, threshold=0.5)
```
Since we are satisfied with the other default parameters, we only change the ```dim_max``` parameter, which tells Ripser
the top dimension to compute homology in, and the ```threshold``` parameter, which tells Ripser when to stop the filtration.

The ```pd``` object is a class with three members, which can be accessed by
```pd$member```. The first class member is ```pd$pairs```, which contains the persistence pairs calculated from X,
indexed in a list by homology degree. So, for example, to view the degree 0 homology of X we can call

```R
print(pd$pairs[[1]])
```

You should see that all persitence pairs in degree 0 are born at 0 and die shortly after, which is what we expect. The
last pair in degree 0 homology should persist to ```Inf```.
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

The ```pd``` object also contains an ```pd$edges``` member which returns the number of edges used in the calculation by ripser. This can be useful for debuggining long calculation times. Finally ```pd$reps``` usually stores cocycle representitives, although in our case it will be empty since ```do_cocycles``` was not set to 1. A section on this feature will be added in the future.


### Persistence Landscape Calculation

Here we will mainly use the function
```R
landscape <- function(PersistenceDiagram, exact=FALSE, dx=0.1,  min_x= 0, max_x=10, max_y=1000)
```
which is the primary constructor for landscape objects. This function will not return an R array, but rather an object
that encapsulates the landscape operations and data. This allows us to perform operations on different types of
landscapes without worrying about preprocessing the landscape representations. At any point the underlying landscape can
be extracted using ```landscape$getInternal()```, which returns a three dimensional array of shape ```[levels, values,
2]```. Lets see an example of this by continuing from the last example. First we will construct a discrete persistence
landscape object with ```dx=0.05``` on the interval ```[0,1]```, with homology in degree 1 from the previous example as
the input diagram.
```R
pl <- landscape(pd$pairs[[2]], exact=FALSE, dx=0.1, min_x=0, max_x=1)
```

Now lets see what the underlying landscape looks like:
```R
print(dim(pl$getInternal()))
```
I get ```1 11 2``` which means that our landscape has 1 level with 11 ```x, y``` pairs. Concretely,
```R
print(pl$getInternal()[1,,])
```
gives us the values of the first (and only) level. I get
```R
      [,1]       [,2]
 [1,]  0.0 0.00000000
 [2,]  0.1 0.00000000
 [3,]  0.2 0.00000000
 [4,]  0.3 0.00000000
 [5,]  0.4 0.07286244
 [6,]  0.5 0.17286244
 [7,]  0.6 0.27286244
 [8,]  0.7 0.37286244
 [9,]  0.8 0.47286244
[10,]  0.9 0.57286244
[11,]  1.0 0.67286244
```
So we can see that the grid is spaced by ```dx=0.1``` and the lanscape is increasing along the input interval, as
expected. We can also use the built in plotting function to visualize the landscape.
```R
PLplot(pl)
```
Now let us operate on landscapes. We can start by sampling from a circle again.
```R
X2 <- CircleUnif(100)
pd2 <- diagram(X2, 'point-cloud', dim_max=1, threshold=0.5)
pl2 <- landscape(pd2$pairs[[2]], exact=FALSE, dx=0.1, min_x=0, max_x=1)
```
Lets calculate the sum of this landscape with the previous one.
```R
pl_sum <- PLsum(pl,pl2)
```
This function checks that ```pl``` and ```pl2``` are compatible, i.e. are defined on the same grid and then adds the
underlying landscapes. Now lets scale by ```0.5``` to compute the average of the two landscapes and plot.
```R
pl_average <- PLscale(0.5, pl_sum)
PLplot(pl_average)
```
We should expect the average to be close to the originals. To test this we can compute the norm of the difference using
the built in ```PLinner``` function.
```R
diff <- PLsum(pl,PLscale(-1,pl_average))
inner <- PLinner(diff,diff)
print(inner)
```
As expected, ```inner``` is fairly small. 

Note that all operations can also be done in-place as methods on landscapes. For example, ```PLsum(pl,pl2)``` is equivalent to ```pl$sum(pl2)```.



