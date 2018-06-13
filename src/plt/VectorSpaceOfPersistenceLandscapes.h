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

#ifndef VECTORSPACEOFPERSISTENCELANDSCAPES_H
#define VECTORSPACEOFPERSISTENCELANDSCAPES_H

#include <climits>
#include <cmath>
#include "PersistenceLandscape.h"

double computeDiscanceOfLandscapes( const PersistenceLandscape& first, const PersistenceLandscape& second , unsigned p );
double computeMaxNormDiscanceOfLandscapes( const PersistenceLandscape& first, const PersistenceLandscape& second );



class vectorSpaceOfPersistenceLandscapes
{
public:
    vectorSpaceOfPersistenceLandscapes(){};
    vectorSpaceOfPersistenceLandscapes(std::vector<PersistenceLandscape> vectOfLand)
    {
        this->vectOfLand.insert( this->vectOfLand.end() , vectOfLand.begin() , vectOfLand.end() );
    }

    void addLandscape( const PersistenceLandscape& aa ){this->vectOfLand.push_back( aa );}
    void removeLastLandscape(){this->vectOfLand.pop_back();};

    double computeDiscanceOfLandscapes( unsigned p );//computes L^p distance of landscapes. Assume that this->vectOfLand.size() == 2, othervise works on first two
    double computeMaxNormDiscanceOfLandscapes();//assume that this->vectOfLand.size() == 2, othervise works on first two

    std::vector<std::pair<double,double> > averageLandscpceInDiscreteSetOfPoints( unsigned dim , unsigned numberOfPoints , double from , double to);//compute a vector of discrete values, which are values of landscapes on
                                                                                                                                            //discrete set of numberOfPoints points in interval [ from , to ]
    PersistenceLandscape average();//compute persistence landscape which is average of all the landscapes in this->vectOfLand
    double standardDeviation( int whichDistance );
    size_t size()const{return this->vectOfLand.size();}

private:
    std::vector< PersistenceLandscape > vectOfLand;
};


bool averageLandscpceInDiscreteSetOfPointsBDG = false;
std::vector<std::pair<double,double> > vectorSpaceOfPersistenceLandscapes::averageLandscpceInDiscreteSetOfPoints( unsigned dim , unsigned numberOfPoints , double from  , double to )
{
    if ( numberOfPoints <= 1 )
    {
        std::ostringstream errMessage;
        errMessage << "Error in function vectorSpaceOfPersistenceLandscapes::averageLandscpceInDiscreteSetOfPoints. You want to compute average using : " << numberOfPoints << " points. This variable cannot be less than 2 \n";
        std::string errMessageStr = errMessage.str();
        const char* err = errMessageStr.c_str();
        throw( err );
    }
    if ( to <= from )
    {
        std::ostringstream errMessage;
        errMessage << "Error in function vectorSpaceOfPersistenceLandscapes::averageLandscpceInDiscreteSetOfPoints. You want to compute average in intrval ["<<from << "," << to << "]. Please correct the vinterval in a way that it is in a form [from,to], where from <to\n";
        std::string errMessageStr = errMessage.str();
        const char* err = errMessageStr.c_str();
        throw( err );
    }

    std::vector< std::pair<double,double> > result(numberOfPoints);
    for ( size_t i = 0 ; i != numberOfPoints-1 ; ++i )
    {
        result[i] = std::make_pair( from+(to-from)*i/(numberOfPoints-1) , 0 );
    }
    result[numberOfPoints-1] = std::make_pair( to , 0 );


    if (averageLandscpceInDiscreteSetOfPointsBDG)
    {
        std::cout << "Initial values \n";
        for ( size_t i = 0 ; i != numberOfPoints ; ++i )
        {
            std::cout << "( " << result[i] << " ) , ";
        }
        std::cout << "\n";
    }


    //It is O(nlog(n)). In below I am changing it into O(n) algorithm. This version is tested and works well.
    /*
    for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
    {
        if ( averageLandscpceInDiscreteSetOfPointsBDG )
        {
            std::cout << "i : " << i << "\n";
            std::cout << "this->vectOfLand[i].dim() : " << this->vectOfLand[i].dim() << "\n";
            std::cout << "dim() : " << this->vectOfLand[i].dim() << "\n";
            std::cin.ignore();
        }

        if ( this->vectOfLand[i].dim() >= dim )
        {
            for ( size_t nr = 0 ; nr != numberOfPoints ; ++nr )
            {
                 if (averageLandscpceInDiscreteSetOfPointsBDG)std::cout << "nr : " << this->vectOfLand[i].computeValueAtAGivenPoint( dim , result[nr].first ) << "\n";
                 result[nr].second += this->vectOfLand[i].computeValueAtAGivenPoint( dim , result[nr].first );
            }
        }
    }
    */

    for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
    {
        if (averageLandscpceInDiscreteSetOfPointsBDG) std::cout << "i : " << i << std::endl;
        if ( this->vectOfLand[i].dim() >= dim )
        {
            size_t nr = 0;
            size_t it = 1;
            while ( ( nr != numberOfPoints ) && ( it != this->vectOfLand[i].land[dim].size() )  )
            {
                if ( averageLandscpceInDiscreteSetOfPointsBDG )
                {
                     std::cout << "nr : " << nr << " , it : " << it << " , numberOfPoints : " << numberOfPoints << " , this->vectOfLand[i].land[dim].size() : " << this->vectOfLand[i].land[dim].size() << std::endl;
                     std::cout <<  "this->vectOfLand[i].land[dim][it-1].first : " << this->vectOfLand[i].land[dim][it-1].first << std::endl;
                     std::cout <<  "this->vectOfLand[i].land[dim][it].first : " << this->vectOfLand[i].land[dim][it].first << std::endl;
                     std::cout << "result[nr].first : " << result[nr].first << std::endl;
                     std::cin.ignore();
                }
                 if ( (this->vectOfLand[i].land[dim][it-1].first <= result[nr].first) && (this->vectOfLand[i].land[dim][it].first >= result[nr].first) )
                 {
                    result[nr].second += functionValue( this->vectOfLand[i].land[dim][it-1] , this->vectOfLand[i].land[dim][it] , result[nr].first );
                    ++nr;
                    continue;
                 }
                 if ( result[nr].first > this->vectOfLand[i].land[dim][it].first )
                 {
                     ++it;
                     continue;
                 }
            }
        }
    }

    if ( averageLandscpceInDiscreteSetOfPointsBDG )
    {
        std::cout << "Before averaging \n";
        for ( size_t i = 0 ; i != numberOfPoints ; ++i )
        {
            std::cout << "( " << result[i] << " ) , ";
        }
        std::cin.ignore();
    }


    for ( size_t i = 0 ; i != numberOfPoints ; ++i )
    {
        result[i].second /= (double)this->vectOfLand.size();
    }

    if ( averageLandscpceInDiscreteSetOfPointsBDG )
    {
        std::cout << "After averaging \n";
        for ( size_t i = 0 ; i != numberOfPoints ; ++i )
        {
            std::cout << "( " << result[i] << " ) , ";
        }
        std::cin.ignore();
    }

    return result;
}

PersistenceLandscape vectorSpaceOfPersistenceLandscapes::average()
{
    double dbg = false;
    PersistenceLandscape result;
    if ( this->vectOfLand.size() == 0 )return result;
    if ( useGridInComputations )
    {
        //cerr << "Computer average using grid method\n";
        //finding maxN such that lambda_n exist
        size_t maxN = 0;
        for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
        {
            if ( maxN < this->vectOfLand[i].size() ){maxN = this->vectOfLand[i].size();}
        }
        if ( dbg ){cerr << "Maximal nonzero lambda : " << maxN << endl;}

        //for every lambda:
        for ( size_t lambda = 0 ; lambda != maxN ; ++lambda )
        {
            if ( dbg ){cerr << "Considering lambda : " << lambda << endl;}
            //initialize and set up the counter:
            std::vector< size_t > counter( this->vectOfLand.size() );
            for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i ){counter[i] = 1;}
            //
            std::vector< std::pair<double,double> > thisLambda;
            thisLambda.push_back( std::make_pair( INT_MIN , 0 ) );
            while ( true )
            {
                //now we need to iterate through the counter.
                std::vector< size_t > whichIndicesShouldBeIncremented;
                double valueInThisPoint = 0;
                double min_x = INT_MAX;
                for ( size_t i = 0 ; i != counter.size() ; ++i )
                {
                    if ( lambda >= this->vectOfLand[i].land.size() )continue;
                    if ( counter[i] >= this->vectOfLand[i].land[lambda].size() )continue;
                    if ( this->vectOfLand[i].land[lambda][counter[i]].first < min_x )
                    {
                        //we found new point with smaller x coordinate. Whatever we had before that have the be zeored.
                        whichIndicesShouldBeIncremented.clear();
                        valueInThisPoint = 0;
                        whichIndicesShouldBeIncremented.push_back(i);
                        min_x = this->vectOfLand[i].land[lambda][counter[i]].first;
                        valueInThisPoint += this->vectOfLand[i].land[lambda][counter[i]].second;
                    }
                    else
                    {
                        if ( this->vectOfLand[i].land[lambda][counter[i]].first == min_x  )
                        {
                            valueInThisPoint += this->vectOfLand[i].land[lambda][counter[i]].second;
                            whichIndicesShouldBeIncremented.push_back(i);
                        }
                    }
                }
                //if we cannot find any new point, that means that we are done.
                if ( min_x == INT_MAX ){break;}

                thisLambda.push_back( std::make_pair( min_x , valueInThisPoint/(double)this->vectOfLand.size() ) );
                if ( dbg ){cerr << "Adding to lambda : " << min_x << " , " << valueInThisPoint/(double)this->vectOfLand.size() << endl;getchar();}
                for ( size_t i = 0 ; i != whichIndicesShouldBeIncremented.size() ; ++i )
                {
                    ++counter[ whichIndicesShouldBeIncremented[i] ];
                }
            }
            thisLambda.push_back( std::make_pair( INT_MAX , 0 ) );
            result.land.push_back( thisLambda );
        }
    }
    else
    {
        //compute average as a linear combination of PL functions


        //This is an old version of the procedure, in which I am doing a linear mearging. It is not the most efficient one, therefore, I am replacing it with the binary tree like merging.
        /*
        for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
        {
            result += this->vectOfLand[i];
        }
        result /= this->vectOfLand.size();
        */
        std::vector< PersistenceLandscape > nextLevelMerge( this->vectOfLand.size() );
        for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
        {
            nextLevelMerge[i] = this->vectOfLand[i];
        }
        while ( nextLevelMerge.size() != 1 )
        {
            if ( dbg ){cerr << "nextLevelMerge.size() : " << nextLevelMerge.size() << endl;}
            std::vector< PersistenceLandscape > nextNextLevelMerge;
            for ( size_t i = 0 ; i < nextLevelMerge.size() ; i=i+2 )
            {
                if ( dbg ){cerr << "i : " << i << endl;cerr << "nextLevelMerge.size() : " << nextLevelMerge.size() << endl;}
                PersistenceLandscape l;
                if ( i+1 != nextLevelMerge.size() )
                {
                    l = nextLevelMerge[i]+nextLevelMerge[i+1];
                }
                else
                {
                    l = nextLevelMerge[i];
                }
                nextNextLevelMerge.push_back( l );
            }
            if ( dbg ){cerr << "After this iteration \n";}
            nextLevelMerge = nextNextLevelMerge;
        }
        result = nextLevelMerge[0];
    }
    return result;
}


/*
    //old version of the code to compute average in a grid
    if ( useGridInComputations )
    {
        cerr << "compute average on a grid \n";getchar();
        //compute average on a grid
        //first we need to check what is the maximal number of nonzero functions in the considered collection of landscapes:
        size_t maximumNuberOfFunction = 0;
        for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
        {
            if ( this->vectOfLand[i].size() > maximumNuberOfFunction ){maximumNuberOfFunction = this->vectOfLand[i].size();}
        }
        size_t numberOfBins = this->vectOfLand[0].land[0].size();
        std::vector< std::pair<double,double> > templateVector(numberOfBins);
        for ( size_t i = 0 ; i != numberOfBins ; ++i )
        {

            templateVector[i].first = this->vectOfLand[0].land[0][i].first;
            templateVector[i].second = 0;
        }

        //now create a suitably large Persistence Landscape result:
        for ( size_t lambda = 0 ; lambda != maximumNuberOfFunction ; ++lambda )
        {
            std::vector< std::pair<double,double> > aa(templateVector);
            result.land.push_back(aa);
        }

        //now it come time to average. For every lambda_n
        for ( size_t lambda = 0 ; lambda != maximumNuberOfFunction ; ++lambda )
        {
            for ( size_t nrOfPoint = 0 ; nrOfPoint != numberOfBins ; ++nrOfPoint )
            {
                 for ( size_t numberOfLandscapeToAverage = 0 ; numberOfLandscapeToAverage != this->vectOfLand.size() ; ++numberOfLandscapeToAverage )
                 {
                     if ( this->vectOfLand[numberOfLandscapeToAverage].size() < lambda )continue;
                     //test, remove for the final version
                     if ( result.land[lambda][nrOfPoint].first !=
                         this->vectOfLand[numberOfLandscapeToAverage].land[lambda][nrOfPoint].first )
                     {
                         cerr << "result.land[lambda][nrOfPoint].first : " << result.land[lambda][nrOfPoint].first << endl;
                         cerr << "this->vectOfLand[numberOfLandscapeToAverage][lambda][nrOfPoint].first : " << this->vectOfLand[numberOfLandscapeToAverage].land[lambda][nrOfPoint].first << endl;
                     }
                     //end of test.
                     result.land[lambda][nrOfPoint].second += this->vectOfLand[numberOfLandscapeToAverage].land[lambda][nrOfPoint].second;
                 }
                 result.land[lambda][nrOfPoint].second /= (double)this->vectOfLand.size();
            }
        }
    }
*/

double vectorSpaceOfPersistenceLandscapes::standardDeviation( int whichDistance )
{
    PersistenceLandscape av = this->average();
    double distanceToAverage = 0;
    for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
    {
        double distance;
        if ( whichDistance > 0 )
        {
            //L^whichDistance distance
            distance = ::computeDiscanceOfLandscapes( av , this->vectOfLand[i] , (unsigned)whichDistance );
        }
        else
        {
            //L^infty distance
            distance = ::computeMaxNormDiscanceOfLandscapes( av , this->vectOfLand[i] );
        }
        distanceToAverage += distance*distance;
    }
    double deviation = sqrt(distanceToAverage/this->vectOfLand.size());
    return deviation;
}//standardDeviation


/*
std::pair< double , size_t > vectorSpaceOfPersistenceLandscapes::standardDeviation( int whichDistance )
{
    PersistenceLandscape av = this->average();
    double distanceToAverage = 0;
    for ( size_t i = 0 ; i != this->vectOfLand.size() ; ++i )
    {
        double distance;
        if ( whichDistance > 0 )
        {
            //L^whichDistance distance
            distance = ::computeDiscanceOfLandscapes( av , this->vectOfLand[i] , (unsigned)whichDistance );
        }
        else
        {
            //L^infty distance
            distance = ::computeMaxNormDiscanceOfLandscapes( av , this->vectOfLand[i] );
        }
        distanceToAverage += distance*distance;
    }
    //double deviation = sqrt(distanceToAverage/this->vectOfLand.size());
    return std::make_pair(distanceToAverage,vectOfLand.size());
}//standardDeviation
*/





#endif
