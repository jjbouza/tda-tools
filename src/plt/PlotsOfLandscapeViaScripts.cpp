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
"This is PlotsOfLandscapes program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following three parameters: \n\
A file with persistence landscapes (.lan files) or persistence barcode have to be provided.\n\
A positive integer f indicating the first level of the landscape to be plot.\n\
A positive integer s indicating the last level of the landscape to be plot. If you want to plot all the landscapes, put here -1. Note that the landscapes will be created in the same folders where the files are.";

 //think if the input parameter should not be the number representing +infinity in the diagram file.

int main(int argc, char *argv[])
{
    configure();
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != 4 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> " << std::endl;
        std::cout << "<name of the file with persistence landscape/barcode> " << std::endl;
        std::cout << "<A positive integer f indicating the first level of the landscape to be plot>." << std::endl;
        std::cout << "<A positive integer s indicating the last level of the landscape to be plot. If you want to plot all the landscapes, put here -1>." << std::endl;
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    char* inputFile = argv[1];
    int f = atoi( argv[2] );
    int t = atoi( argv[3] );


    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Name of the file with barcode \\ landscape : " << inputFile << std::endl;
    std::cout << "Plot will be done from the level: " << f << std::endl;
    std::cout << "Plot will be done to the level: " << t << std::endl;


    //End of a small talk with the User. Let's get to work.
    //First, read the names of files:

    std::vector<std::string> namesOfFiles;
    string s(inputFile);
    namesOfFiles.push_back( s );

    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< PersistenceLandscape > landscapes = createLandscapesFromTheFiles( namesOfFiles );
    std::cout << "Done." << std::endl;

    //And finally, the computations of integrals:

    landscapes[0].plot( (char*)inputFile , f , t );

    std::cout << "Plot has been prepared." << std::endl;
    std::cout << "That is all. Have a good day!" << std::endl;

	return 0;
}
