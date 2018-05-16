UF TDA Group Software
Author: Jose Bouza 


Development Outline:
1. Interface Ripser with R with some extended capabilities: cocycle representitives and accepting pre-computed boundary
   matricies.
2. Interface PL Toolbox with R and make the data-structure transfer seamless and efficient.
3. Package all in a TDA class to encasulate the behavior (this would have to be done in Python).



PL Class Structure:
Description: Encapsulates the computations of a TDA analysis on one particular dataset. 
Methods Descriptions: 
	init: Takes a series of settings which will be used for computations. NOTE: Most of these
	settings should have natural default settings for ease of use.
	loadData: Accepts data in any of the possible input formats.
	calculatePersistenceDiagram:  Computes the persistence diagram for the loaded data.
	calculateLandscape: Computes the PD and the PL for the data.
	sumTo: Add this PL to another.
	scaleWith:  Multiply this PL by a number.

A series of functions can be written that act on objects of this PL class:
	add: Sum of two PL's.
	average: Average of a list of PL's.
	inner: Inner product of PL's.

Consider the following advantage to this OOP structure: We wanted to be able to compute PL's in both the discrete and
exact ways. This structure allows us to do this naturally. We can consider the PL class above as a virtual class and
have two implementations:
1. DiscretePL
2. ExactPL
We would implement the sumTo function correctly in either case to get the desired behavior. 

A possible issue is how to naturally interpolate this with Machine Learning algorithms. I.e. how do we deal with the
distinction between exact and discrete when using something like an SVM package.




Goal Example (Python interface):

import PL

#Load data and init PL objects:
pointcloud1, pointcloud2 = getData()
persistence1 = PL.ExactPL(pointcloud)
persistence2 = PL.ExactPL(pointcloud2)

average = (persistence1+persistence2)/2



