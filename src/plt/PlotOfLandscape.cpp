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
"This is PlotsOfLandscape program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the name of a file with landscape or barcode and two integer parameters indicating what range of barcodes is to be plotted.";

 //think if the input parameter should not be the number representing +infinity in the diagram file.

int main(int argc, char *argv[])
{
    configure();
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != 4 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> ";
        std::cout << "<name of the file with persistence landscapes/barcodes> ";
        std::cout << "<A positive integer f indicating the first level of the landscape to be plot>.";
        std::cout << "<A positive integer s indicating the last level of the landscape to be plot. If you want to plot all the landscapes, put here -1>.";
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    char* fileWithLandscape = argv[1];
    int f = atoi( argv[2] );
    int t = atoi( argv[3] );


    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Name of the file with barcode \\ landscape : " << fileWithLandscape << std::endl;
    std::cout << "Plot will be done from the level: " << f << std::endl;
    std::cout << "Plot will be done to the level: " << t << std::endl;


    //End of a small talk with the User. Let's get to work.
    //First, read the names of files:


    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscape." << std::endl;
    std::vector< string > names;
    names.push_back( (string)fileWithLandscape );
    std::vector< PersistenceLandscape > landscapes = createLandscapesFromTheFiles( names );
    std::cout << "Done." << std::endl;


    //And finally, the computations of integrals:
    std::cout << "Preparing plots." << std::endl;

    if ( t != -1 )
    {
        landscapes[0].printToFiles( fileWithLandscape , (unsigned)f , (unsigned)t );
        landscapes[0].generateGnuplotCommandToPlot( fileWithLandscape , (unsigned)f , (unsigned)t );
    }
    else
    {
        landscapes[0].printToFiles( fileWithLandscape , (unsigned)f , (unsigned)landscapes[0].size() );
        if ( f == 0 )
        {
            landscapes[0].generateGnuplotCommandToPlot( fileWithLandscape );
        }
        else
        {
            landscapes[0].generateGnuplotCommandToPlot( fileWithLandscape , (unsigned)f , (unsigned)landscapes[0].size() );
        }
    }

    std::cout << "Plots has been prepared." << std::endl;
    std::cout << "That is all. Have a good day!" << std::endl;
	return 0;
}
