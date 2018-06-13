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






#include <iostream>
#include <cstdlib>
#include <string>
#include "PersistenceBarcode.h"
#include "PersistenceLandscape.h"
#include "FilesReader.h"
#include "PermutationTest.h"

using namespace std;

const char* programInfo =
"This is NthMoment program which is a part of Peristence Landscape Toolbox by Pawel Dlotko.\
 It takes as input the name of a file with landscape or barcode, an integer n so that the n-th moment \
 is to be computed and a number k so that the moment of the n-th moment of the k-th landscape centered in \
 c is to be computed. The 1-th moment (mean) is centered at zero. The second moment, variance (n=2), is centered \
 at the mean. All higher moments are centered at the mean and normalized by the variance to the power n/2,where n is the number of moment to compute.";

int main(int argc, char *argv[])
{
    configure();
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != 4 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> "<< std::endl;
        std::cout << "<name of the file with persistence landscapes/barcodes> "<< std::endl;
        std::cout << "<A positive integer indicating which moment we are to compute>."<< std::endl;
        std::cout << "<A positive integer l indicating which landscape's moment we are to compute>."<< std::endl;
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    char* fileWithLandscape = argv[1];
    int n = atoi( argv[2] );
    int lambda = atoi( argv[3] );


    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Name of the file with persistence landscapes/barcodes : " << fileWithLandscape << std::endl;
    std::cout << "Which moment: " << n << std::endl;
    std::cout << "Which landscape: " << lambda << std::endl;

    if ( n < 1 )
    {
        cerr << "Cannot define a " << n << "-th moment." << endl;
        return 1;
    }

    //End of a small talk with the User. Let's get to work.
    //First, read the names of files:


    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscape." << std::endl;
    std::vector< string > names;
    names.push_back( (string)fileWithLandscape );
    std::vector< PersistenceLandscape > landscapes = createLandscapesFromTheFiles( names );
    std::cout << "Done." << std::endl;

    cerr << "Landscapes[0] : " << landscapes[0] << endl;
    getchar();


    //And finally, the computations of moment:
    ofstream out;
    std::ostringstream name;
    name << fileWithLandscape << "_" << n << "_moment";
    std::string fName = name.str();
    const char* FName = fName.c_str();
    out.open((char*)FName);

    double center = 0;
    double normalizingFactor = 1;
    if ( n == 1 )
    {
        //mean
        cerr << "Mean \n";
        center = 0;
        normalizingFactor = 1;
    }
    else
    {
        if ( n == 2 )
        {
            //variance
            center = landscapes[0].computeNthMoment(  1 , 0 , lambda );
            normalizingFactor = 1;
            cerr << "Variance \n";
        }
        else
        {
            //n-th moment
            cerr << n << "-th moment \n";
            center = landscapes[0].computeNthMoment(  1 , 0 , lambda );
            normalizingFactor = landscapes[0].computeNthMoment(  2 , center , lambda );
            normalizingFactor = pow( normalizingFactor , (n/2.0) );
        }
    }

    //cerr << "center : " << center <<endl;
    //cerr << "normalizingFactor : " << normalizingFactor << endl;

    double moment = normalizingFactor*landscapes[0].computeNthMoment(  n , center , lambda );

    out << moment;
    cout << "The " << n  << "-th moment is : " << moment << endl;
    out.close();

    std::cout << "Moment has been computed." << std::endl;
    std::cout << "That is all. Have a good day!" << std::endl;
	return 0;
}


