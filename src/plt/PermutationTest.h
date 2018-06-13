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

#include <cmath>
#include "PersistenceBarcode.h"
#include "PersistenceLandscape.h"
#include "VectorSpaceOfPersistenceLandscapes.h"

using namespace std;


std::vector< std::pair< std::pair<size_t , size_t> , size_t > > permutationTest( const std::vector< std::vector<PersistenceLandscape> > data , size_t numberOfTries , unsigned whichDistance , bool setThisToTrueIfYouWantBottleneck = false )
{
    bool dbg = false;

    std::vector< std::pair< std::pair<size_t , size_t> , size_t > > result;

    if ( dbg ){cerr << "Entering permutationTest \n";}

    //first, average the data in the external vector:
    std::vector<PersistenceLandscape> averages(data.size());
    for ( size_t i = 0 ; i != data.size() ; ++i )
    {
        vectorSpaceOfPersistenceLandscapes v;
        for ( size_t j = 0 ; j != data[i].size() ; ++j )
        {
            v.addLandscape( data[i][j] );
        }
        PersistenceLandscape av = v.average();
        averages[i] = av;
    }
    if ( dbg ){cerr << "Averages computed \n";}
    //now the main bootstraping:
    for ( size_t i = 0 ; i != data.size() ; ++i )
    {
        for ( size_t j = i+1 ; j != data.size() ; ++j )
        {
            cerr << "Comparing the landscapes from the file number : " << i+1 << " , and the file number: " << j+1 << ". " << numberOfTries << " permutations will be made." << endl;
            std::vector< PersistenceLandscape > toMix;
            toMix.insert( toMix.end() , data[i].begin() , data[i].end() );
            toMix.insert( toMix.end() , data[j].begin() , data[j].end() );

            double distanceBetweenOriginal;
            if ( setThisToTrueIfYouWantBottleneck = false )
            {
                //in this case we compute L^p distance
                distanceBetweenOriginal = computeDiscanceOfLandscapes( averages[i] , averages[j] , whichDistance );
            }
            else
            {
                //in this case we compute bottleneck distance.
                distanceBetweenOriginal = computeMaxNormDiscanceOfLandscapes( averages[i] , averages[j] );
            }

            size_t howManyPermuttedAveragesIsGreaterThanOriginal = 0;
            for ( size_t tryy = 0 ; tryy != numberOfTries ; ++tryy )
            {
                //cerr << "tryy : " << tryy << endl;
                random_shuffle( toMix.begin() , toMix.end() );
                vectorSpaceOfPersistenceLandscapes v0;
                vectorSpaceOfPersistenceLandscapes v1;
                for ( size_t a = 0 ; a != data[i].size() ; ++a )
                {
                    v0.addLandscape(toMix[a]);
                }
                for ( size_t a = data[i].size() ; a != data[i].size() + data[j].size() ; ++a )
                {
                    v1.addLandscape(toMix[a]);
                }
                PersistenceLandscape avv0 = v0.average();
                PersistenceLandscape avv1 = v1.average();

                double distanceBetweenPermutted;
                if ( setThisToTrueIfYouWantBottleneck = false )
                {
                    //in this case we compute L^p distance
                    distanceBetweenPermutted = computeDiscanceOfLandscapes( avv0 , avv1 , whichDistance );
                }
                else
                {
                    //in this case we compute bottleneck distance.
                    distanceBetweenPermutted = computeMaxNormDiscanceOfLandscapes( avv0 , avv1 );
                }

                if ( distanceBetweenPermutted > distanceBetweenOriginal )
                {
                    ++howManyPermuttedAveragesIsGreaterThanOriginal;
                }
            }
            std::cerr << "How many averages of permutted sets are farther away than the averages of the original sets: " << howManyPermuttedAveragesIsGreaterThanOriginal << std::endl;
            result.push_back( std::make_pair( std::make_pair(i,j) , howManyPermuttedAveragesIsGreaterThanOriginal ) );
        }
    }

    cerr << "Exiting bootsreapping procedure \n";

    return result;
}//bootstrap



template <typename T>
void writeToFileAsMatrix( std::vector< std::pair< std::pair<size_t , size_t> , T > > b , char* filename )
{
    ofstream out;
    out.open( filename );

    //b.size is a sum of a decreasig arythmetic sequence. Find the first term

    double sqrtDelta = sqrt( 1+8*b.size() );
    double root = 0.5*(sqrtDelta-1);
    size_t numberOfInitialries = (size_t)root;

    size_t counter = 0;
    for ( size_t i = 0 ; i != root ; ++i )
    {
        for ( size_t j = 0 ; j != root-i ; ++j )
        {
            out << b[counter].second << " ";
            ++counter;
        }
        out << endl;
    }
    out.close();
}


template <typename T>
void writeToFileAsAList( std::vector< std::pair< std::pair<size_t , size_t> , T > > b , char* filename )
{
    ofstream out;
    out.open( filename );
    for ( size_t i = 0 ; i != b.size() ; ++i )
    {
        out << "(" << b[i].first.first+1 << "," << b[i].first.second+1 << ") = " << b[i].second << std::endl;
    }
    out.close();
}
