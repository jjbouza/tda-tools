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
#include "PersistenceBarcode.h"
#include "PersistenceLandscape.h"
#include "FilesReader.h"
#include "PermutationTest.h"

const char* programInfo =
"This is comparePersistenceFromTwoSets program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameters: \n\
A positive integer N equal to the number of files with names (or paths) of files with barcodes or landscapes,\n\
N files each of which contains a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided.\n\
The last but one parameter of a program is a positive integer equal to the number of tries and the last parameter is L^p distance we are to use in the procedure (set -1 for bottleneck distance)\
 in a permutation test. Note that the permutation tests usually takes long time. The progress of the computations will be displayed, but it may take a while for the information to appear.";

 //think if the input parameter should not be the number representing +infinity in the diagram file.

int main(int argc, char *argv[])
{
    configure();
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != numberOfInputFiles + 4 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> <positive integer N equal to the number of files with input files names>";
        std::cout << "N times <name of the file with names of files with persistence landscapes/barcodes> ";
        std::cout << "<positive integer equal to the number of permutations in the permutation test> ";
        std::cout << "<an integer p determining which distance will be used in the procedure (set -1 for the bottleneck distance)>. ";
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    std::vector< char* > inputFiles;
    for ( size_t i = 0 ; i != numberOfInputFiles ; ++i )
    {
        char* nameOfFile = argv[2+i];
        inputFiles.push_back( nameOfFile );
    }
    size_t numberOfPermutations = atoi( argv[2+numberOfInputFiles] );
    int whichDistance = atoi( argv[3+numberOfInputFiles] );

    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Number of files with list of files with barcodes and landscapes : " << numberOfInputFiles << std::endl;
    for ( size_t i = 0 ; i != inputFiles.size() ; ++i )
    {
        std::cout << inputFiles[i] << std::endl;
    }
    std::cout << "Number of permutations with the permutation test : " << numberOfPermutations << std::endl;
    std::cout << "The distance used : " << whichDistance << std::endl;

    //End of a small talk with the User. Let's get to work.
    //First, read the names of files:
    std::cout << "Reading the names of files with barcodes \\ landscapes." << std::endl;
    std::vector< std::vector<std::string> > namesOfFiles;
    for ( size_t i = 0 ; i != inputFiles.size() ; ++i )
    {
        std::vector<std::string> s = readFileNames( inputFiles[i] );
        namesOfFiles.push_back(s);
    }
    std::cout << "Done." << std::endl;


    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< std::vector< PersistenceLandscape > > landscapes;
    for ( size_t i = 0 ; i != namesOfFiles.size() ; ++i )
    {
        std::vector< PersistenceLandscape > lands = createLandscapesFromTheFiles( namesOfFiles[i] );
        landscapes.push_back( lands );
    }
    std::cout << "Done." << std::endl;

    //And finally, do the permutation test:
    std::cout << "Now beginning the permutation test." << std::endl;
    std::vector< std::pair< std::pair<size_t , size_t> , size_t > > resultOfPermutationTest;
    if ( whichDistance != -1 )
    {
        //L^p distance
        resultOfPermutationTest = permutationTest( landscapes , numberOfPermutations , whichDistance  );
    }
    else
    {
        //bottleneck distance
        resultOfPermutationTest = permutationTest( landscapes , numberOfPermutations , whichDistance , true );
    }
    std::cout << "Done with the permutation test, writing p-values to the file 'p-values.txt." << std::endl;

    //now changing numbers of occurences to p-values:
    std::vector< std::pair< std::pair<size_t , size_t> , double > > pValues;
    for ( size_t i = 0 ; i != resultOfPermutationTest.size() ; ++i )
    {
        pValues.push_back( std::make_pair( resultOfPermutationTest[i].first , resultOfPermutationTest[i].second/(double)numberOfPermutations ) );
    }

    std::cout << "Done. Now writing results to a file" << std::endl;
    writeToFileAsAList<double>( pValues , (char*)"p-values.txt" );
    std::cout << "That is all. Have a good day!" << std::endl;

	return 0;
}
