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

The parameters are mostly self explanatory and are explained in the documentation. Say we want to compute.


