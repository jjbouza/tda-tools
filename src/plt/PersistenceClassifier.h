//    Copyright 2013-2014 University of Pennsylvania
//    Created by Pawel Dlotko
//
//    This file is part of Persistence Landscape Toolbox (PLT).
//
//    PLT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PLT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with PLT.  If not, see <http://www.gnu.org/licenses/>.



#pragma once

#include <iostream>
#include <set>
#include "PersistenceLandscape.h"
#include "VectorSpaceOfPersistenceLandscapes.h"

bool comaprePairsAccodringToTheSecondCoord( const std::pair< size_t , double >& f , const std::pair< size_t , double >& s )
{
    return (f.second < s.second);
}//comaprePairsAccodringToTheSecondCoord

class persistenceClassifierSingleDimension
{
public:
	persistenceClassifierSingleDimension(){};
	persistenceClassifierSingleDimension( const std::vector< std::vector<PersistenceLandscape> >& trainingDataInDifferentClasses , unsigned whichDistance = 2 );
	persistenceClassifierSingleDimension( const std::vector<PersistenceLandscape>& averages , unsigned whichDistance = 2 )
	{
	    this->whichDistance = whichDistance;
	    this->averagesOfBins.insert( this->averagesOfBins.end() , averages.begin() , averages.end() );
	}

	std::vector<size_t> classifySimple( const PersistenceLandscape& data );
	std::vector< std::pair< size_t , double > > classify( const PersistenceLandscape& data );

	std::vector< std::vector<size_t> > classifySimpleVector( std::vector<PersistenceLandscape> data );
	std::vector< std::vector< std::pair< size_t , double > > > classifyVector( std::vector<PersistenceLandscape> data );

	void writeToFile( char* prefix );
	void readFromFile( char* prefix , size_t numberOfClasses , unsigned whichDistance );
private:
	std::vector< PersistenceLandscape > averagesOfBins;
	unsigned whichDistance;
};

//for a bottleneck distance, please set whichDistance = 0.
persistenceClassifierSingleDimension::persistenceClassifierSingleDimension( const std::vector< std::vector<PersistenceLandscape> >& trainingDataInDifferentClasses , unsigned whichDistance )
{
    for ( size_t classs = 0 ; classs != trainingDataInDifferentClasses.size() ; ++classs )
    {
        //average data in this class:
        vectorSpaceOfPersistenceLandscapes v;
        for ( size_t i = 0 ; i != trainingDataInDifferentClasses[classs].size() ; ++i )
        {
            v.addLandscape( trainingDataInDifferentClasses[classs][i] );
        }
        PersistenceLandscape av = v.average();
        this->averagesOfBins.push_back( av );
    }
    if ( whichDistance > 0 )
    {
        this->whichDistance = whichDistance;
    }
    else
    {
        whichDistance = 0;//in this case, we compute bottleneck distance.
    }
}

//that procedure returns only one index with minimal distance. If there are more indices for which minimum is obtained, it return the last one.
std::vector<size_t> persistenceClassifierSingleDimension::classifySimple( const PersistenceLandscape& data )
{
    if ( this->averagesOfBins.size() == 0 )
    {
        std::cerr << "The classifier is uninitialized, the program will terminate now \n";
        throw("The classifier is uninitialized, the program will terminate now \n");
    }
    std::vector< double > distances(this->averagesOfBins.size());
    double minimalDistance;
    if ( this->whichDistance != 0 )
    {
        //L^whichDistance
        minimalDistance = computeDiscanceOfLandscapes( this->averagesOfBins[0] , data , this->whichDistance );
    }
    else
    {
        //bottleneck distance
        minimalDistance = computeMaxNormDiscanceOfLandscapes( this->averagesOfBins[0] , data  );
    }
    distances[0] = minimalDistance;

    for ( size_t i = 1 ; i != this->averagesOfBins.size() ; ++i )
    {
        double distance;
        if ( this->whichDistance != 0 )
        {
            //L^whichDistance
            distance = computeDiscanceOfLandscapes( this->averagesOfBins[i] , data , this->whichDistance );
        }
        else
        {
            //bottleneck distance
            distance = computeMaxNormDiscanceOfLandscapes( this->averagesOfBins[i] , data  );
        }

        if ( minimalDistance > distance )
        {
            minimalDistance = distance;
        }
        distances[i] = distance;
    }
    //return guy with the minimal distance
    std::vector<size_t> result;
    for ( size_t i = 0 ; i != distances.size() ; ++i )
    {
        if ( distances[i] == minimalDistance )
        {
            result.push_back(i+1);
        }
    }
    return result;
}//classifySimple

std::vector< std::pair< size_t , double > > persistenceClassifierSingleDimension::classify( const PersistenceLandscape& data )
{
    std::vector< std::pair< size_t , double > > result(this->averagesOfBins.size());
    for ( size_t i = 0 ; i != this->averagesOfBins.size() ; ++i )
    {
        double distance;
        if ( this->whichDistance != 0 )
        {
            //L^whichDistance
            distance = computeDiscanceOfLandscapes( this->averagesOfBins[i] , data , this->whichDistance );
        }
        else
        {
            //bottleneck distance
            distance = computeMaxNormDiscanceOfLandscapes( this->averagesOfBins[i] , data  );
        }
        result[i] = std::make_pair( i , distance );
    }
    std::sort( result.begin() , result.end() , comaprePairsAccodringToTheSecondCoord );
    return result;
}//classify


std::vector< std::vector<size_t> > persistenceClassifierSingleDimension::classifySimpleVector( std::vector<PersistenceLandscape> data )
{
    std::vector< std::vector<size_t> > result;
    for ( size_t i = 0 ; i != data.size() ; ++i )
    {
        std::vector<size_t> classification = this->classifySimple( data[i] );
        result.push_back( classification );
    }
    return result;
}//classifySimpleVector


std::vector< std::vector< std::pair< size_t , double > > > persistenceClassifierSingleDimension::classifyVector( std::vector<PersistenceLandscape> data )
{
    std::vector< std::vector< std::pair< size_t , double > > > result;
    for ( size_t i = 0 ; i != data.size() ; ++i )
    {
        std::vector< std::pair< size_t , double > > localClassification = this->classify( data[i] );
        result.push_back( localClassification );
    }
    return result;
}//classifyVector


void persistenceClassifierSingleDimension::writeToFile( char* prefix )
{
    for ( size_t i = 0 ; i != this->averagesOfBins.size() ; ++i )
    {
        std::ostringstream name;
        name << prefix << "_" << i << ".lan";
        std::string fName = name.str();
        const char* FName = fName.c_str();
        this->averagesOfBins[i].printToFile( (char*)FName );
    }
}//writeToFile


void persistenceClassifierSingleDimension::readFromFile( char* prefix , size_t numberOfClasses , unsigned whichDistance )
{
    this->whichDistance = whichDistance;
    for ( size_t i = 0 ; i != numberOfClasses ; ++i )
    {
        std::ostringstream name;
        name << prefix << "_" << i << ".lan";
        std::string fName = name.str();
        const char* FName = fName.c_str();
        this->averagesOfBins.push_back( PersistenceLandscape( (char*)FName ) );
    }
}//readFromFile




//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************




class persistenceClassifierAllDimension
{
public:
	persistenceClassifierAllDimension(){};

	//in this constructor we assume that in the internal vector trainingDataInDifferentClasses the different dimensions came in sequence: dim_0, dim_1,...,dim_numberOfDimensionsToConsider
	persistenceClassifierAllDimension( const std::vector< std::vector<PersistenceLandscape> >& trainingDataInDifferentClasses , int numberOfDimensionsToConsider , unsigned whichDistance = 2 );
	persistenceClassifierAllDimension( const std::vector< std::vector< std::vector<PersistenceLandscape> > > trainingDataInDifferentClasses , unsigned whichDistance = 2 );

    //Here the user must be careful, since the input to those two functions are the same. In the case of classifySimple it should be _jsut_ series of landscapes in dimension 0,...
    //of the same space.
    //In the case of classifySimpleVector there should be vector of a type ( landscape in dim 0, landsape in dim 1, ... )^n
	size_t classifySimple( std::vector<PersistenceLandscape> data );//in this case, we return first index on which appears in all dimensions
	std::vector<size_t> classifySimpleVector(  std::vector<PersistenceLandscape> data );

	persistenceClassifierAllDimension( const std::vector<persistenceClassifierSingleDimension*> classifiers )
	{
	    this->classifierInEachDimension.insert( this->classifierInEachDimension.end() , classifiers.begin() , classifiers   .end() );
	}

	void writeToFile( char* prefix );
	void readFromFile( char* prefix , size_t numberOfClasses , unsigned dimension , unsigned whichDistance );
private:
	std::vector< persistenceClassifierSingleDimension* > classifierInEachDimension;
};


persistenceClassifierAllDimension::persistenceClassifierAllDimension( const std::vector< std::vector<PersistenceLandscape> >& trainingDataInDifferentClasses , int numberOfDimensionsToConsider , unsigned whichDistance )
{
    bool dbg = false;
    if ( dbg )
    {
        std::cerr << "Entering persistenceClassifierAllDimension( const std::vector< std::vector<PersistenceLandscape> >& trainingDataInDifferentClasses , int numberOfDimensionsToConsider , unsigned whichDistance ) constructor \n";
    }
    //trainingDataInDifferentClasses[i] = dim 0 , dim 1, .. , dim k , dim 0 , dim 1 ,..., dim k <-- each from the same class. We need to de-couple them.
    for ( size_t dim = 0 ; dim != numberOfDimensionsToConsider ; ++dim )
    {
        if ( dbg ){std::cerr << "Alocating barcodes in dimension :" << dim << std::endl;}
        std::vector< std::vector<PersistenceLandscape> > classesInThisDimension;
        for ( size_t classNo = 0 ; classNo != trainingDataInDifferentClasses.size() ; ++classNo )
        {
            if ( dbg ){std::cout << "Considering the class number : " << classNo << std::endl;}
            std::vector< PersistenceLandscape > landscapesOfThisDimensionInThisClass;
            for ( size_t i = dim ; i < trainingDataInDifferentClasses[classNo].size() ; i = i+numberOfDimensionsToConsider )
            {
                if (dbg){std::cerr << "Considering file number : " << i << std::endl;}
                landscapesOfThisDimensionInThisClass.push_back( trainingDataInDifferentClasses[classNo][i] );
            }
            if (dbg){std::cerr << "landscapesOfThisDimensionInThisClass.size() : " << landscapesOfThisDimensionInThisClass.size() << std::endl;}
            classesInThisDimension.push_back( landscapesOfThisDimensionInThisClass );
        }
        if ( dbg ){std::cerr << "classesInThisDimension.size(): " << classesInThisDimension.size() << std::endl;}
        this->classifierInEachDimension.push_back(new persistenceClassifierSingleDimension( classesInThisDimension , whichDistance ));
    }
}

persistenceClassifierAllDimension::persistenceClassifierAllDimension( const std::vector< std::vector< std::vector<PersistenceLandscape> > > trainingDataInDifferentClasses , unsigned whichDistance )
{
    for ( size_t dim = 0 ; dim != trainingDataInDifferentClasses.size() ; ++dim )
    {
        this->classifierInEachDimension.push_back( new persistenceClassifierSingleDimension(trainingDataInDifferentClasses[dim] , whichDistance) );
    }
}

size_t persistenceClassifierAllDimension::classifySimple( std::vector<PersistenceLandscape> data )
{

    bool dbg = false;
    if ( dbg ){std::cerr << "Entering persistenceClassifierAllDimension::classifySimple" << std::endl;}

    std::vector< std::vector< std::pair< size_t , double > > > matchesInEachDimension( this->classifierInEachDimension.size() );
    for ( size_t dim = 0 ; dim != this->classifierInEachDimension.size() ; ++dim )
    {
        matchesInEachDimension[dim] = this->classifierInEachDimension[dim]->classify(data[dim]);
    }

    if ( dbg ){std::cerr << "Local distances created. Now we will compute intersection" << std::endl;}

    //now we need to find first nonempty intersection. To do so, we create a vector of sets in which we will store the indices
    std::vector< std::set<size_t> > sets( this->classifierInEachDimension.size() );

    for ( size_t i = 0 ; i != matchesInEachDimension[0].size() ; ++i )
    {
        //adding new element:
        for ( size_t j = 0 ; j != matchesInEachDimension.size() ; ++j )
        {
            sets[j].insert( matchesInEachDimension[j][i].first );
        }
        //and checking if the intersection of vectors is not empty:
        std::set<size_t> interection = sets[0];
        for ( size_t j = 0 ; j < sets.size() ; ++j )
        {
            std::set<size_t> newIntersection;
            set_intersection(sets[j].begin(),sets[j].end(),interection.begin(),interection.end(),std::inserter(newIntersection,newIntersection.begin()));
            interection.swap( newIntersection );
            if ( dbg )
            {
                std::cerr << "Set number : " << j << " has the following elements : ";
                for ( std::set<size_t>::iterator it = sets[j].begin() ; it != sets[j].end() ; ++it )
                {
                    std::cerr << *it << " " ;
                }
                std::cerr << std::endl;
                std::cerr << "Intersection at the current stage : ";
                for ( std::set<size_t>::iterator it = interection.begin() ; it != interection.end() ; ++it )
                {
                    std::cerr << *it << " ";
                }
                std::cerr << std::endl;
            }
        }
        if ( dbg )
        {
            std::cerr << "Intersction computed for all sets \n\n\n";
            getchar();
        }
        if ( interection.size() )
        {
            if ( dbg ){std::cerr << "Intersection is nonempty \n";}
            return *interection.begin();
        }
    }
}//classifySimple


std::vector<size_t> persistenceClassifierAllDimension::classifySimpleVector( std::vector<PersistenceLandscape> landscapes )
{
    std::vector<size_t> result;
    size_t i = 0;
    while ( i != landscapes.size() )
    {
        //prepare a package
        std::vector< PersistenceLandscape > landscapesOfTheSameSpaceInDifferentDimensions;
        for ( size_t j = 0 ; j != this->classifierInEachDimension.size() ; ++j )
        {
            landscapesOfTheSameSpaceInDifferentDimensions.push_back( landscapes[i] );
            ++i;
        }
        size_t v = this->classifySimple( landscapesOfTheSameSpaceInDifferentDimensions );
        result.push_back(v);
    }
    return result;
}//classifySimple


void persistenceClassifierAllDimension::writeToFile( char* prefix )
{
    for ( size_t dim =  0 ; dim != this->classifierInEachDimension.size() ; ++dim )
    {
        std::ostringstream name;
        name << "classifier_" << dim;
        std::string fName = name.str();
        const char* FName = fName.c_str();
        this->classifierInEachDimension[dim]->writeToFile( (char*)FName );
    }
}//writeToFile

void persistenceClassifierAllDimension::readFromFile( char* prefix , size_t numberOfClasses , unsigned dimension , unsigned whichDistance )
{
    for ( size_t dim = 0 ; dim != dimension ; ++dim )
    {
        std::ostringstream name;
        name << "classifier_" << dim;
        std::string fName = name.str();
        const char* FName = fName.c_str();
        persistenceClassifierSingleDimension* c = new persistenceClassifierSingleDimension();

        std::cerr << "Reading from file : " << FName << std::endl;
        c->readFromFile( (char*)FName , numberOfClasses , whichDistance );
        std::cerr << "Done reading \n";

        this->classifierInEachDimension.push_back( c );
    }
}//readFromFile
