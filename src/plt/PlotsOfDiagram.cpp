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
"This is PlotsOfDiagram program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the name of a file with barcode.";

int main(int argc, char *argv[])
{
    configure();
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != 2 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> ";
        std::cout << "<name of the file with persistence barcodes> ";
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    char* fileWithBarcode = argv[1];

    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Name of the file with barcode \\ landscape : " << fileWithBarcode << std::endl;

    //End of a small talk with the User. Let's get to work.
    //First, read the names of files:


    //now generate barcodes based on that files:
    std::cout << "Creating Persistence Landscape." << std::endl;
    std::vector< string > names;
    names.push_back( (string)fileWithBarcode );
    std::vector< PersistenceBarcodes > barcodes = createBarcodesFromTheFiles( names );
    std::cout << "Done." << std::endl;


    //And finally, the computations of integrals:
    std::cout << "Preparing plots." << std::endl;

    barcodes[0].plot( fileWithBarcode );

    std::cout << "Plots has been prepared." << std::endl;
    std::cout << "That is all. Have a good day!" << std::endl;
	return 0;
}
