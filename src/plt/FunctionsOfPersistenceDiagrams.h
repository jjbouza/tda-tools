#pragma once
#include "PersistenceBarcode.h"

using namespace std;

//
//
//
//non parametric
//
//




double averageLengthsOfInterval( const PersistenceBarcodes& b )
{
    double result = 0;
    for ( size_t i = 0 ; i != b.size() ; ++i )
    {
        result += (b.barcodes[i].second - b.barcodes[i].first);
    }
    return result/(double)b.size();
}

double medianLengthsOfInterval( const PersistenceBarcodes& b )
{
    if ( b.size() == 0 )return 0;
    std:vector<double> result( b.size() );
    for ( size_t i = 0 ; i != b.size() ; ++i )
    {
        result[i] = (b.barcodes[i].second - b.barcodes[i].first);
    }
    nth_element( result.begin(), result.begin()+result.size()/2,result.end() );
    return result[result.size()/2];
}

double maxLengthsOfInterval( const PersistenceBarcodes& b )
{
    double maxLength = 0;
    for ( size_t i = 0 ; i != b.barcodes.size() ; ++i )
    {
        if ( maxLength < fabs( b.barcodes[i].second - b.barcodes[i].first ) )maxLength = fabs( b.barcodes[i].second - b.barcodes[i].first );
    }
    return maxLength;
}

//
//
//
//parametric
//
//
//
double lengthIfIthBar( const PersistenceBarcodes& b , size_t position )
{
    if ( b.size() > position ) return (b.barcodes[position].second - b.barcodes[position].first);
    return 0;
}

double medianLengthsOfDominantIntervals( const PersistenceBarcodes& b , size_t numberOfDominantIntervals )
{
    if ( b.size() == 0 )return 0;
    //cerr << "medianLengthsOfDominantIntervals \n";
    std:vector<double> result( std::min(b.size(),numberOfDominantIntervals) );
    for ( size_t i = 0 ; i != std::min(b.size(),numberOfDominantIntervals) ; ++i )
    {
        result[i] = (b.barcodes[i].second - b.barcodes[i].first);
    }
    nth_element( result.begin(), result.begin()+result.size()/2,result.end() );
    return result[result.size()/2];
}


double averageLengthsOfDominantIntervals( const PersistenceBarcodes& b , size_t numberOfDominantIntervals )
{
    if ( b.size() == 0 )return 0;
    //cerr << "averageLengthsOfDominantIntervals \n";
    double result = 0;
    for ( size_t i = 0 ; i != std::min(b.size(),numberOfDominantIntervals) ; ++i )
    {
        result += (b.barcodes[i].second - b.barcodes[i].first);
    }
    return result/(double)b.size();
}


