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
"This is HistigramsOfBarcodesLength program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the name of a file with a barcoes. It outputs min and max range of involved intervals.";

int main(int argc, char *argv[])
{
    std::cout << programInfo << std::endl;
    size_t numberOfInputFiles = atoi( argv[1] );

    if ( argc != 2 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> "<< std::endl;
        std::cout << "<name of the file with persistence barcodes> "<< std::endl;
        return 1;
    }

    char* fileWithBarcode = argv[1];

    std::cout << "Here are the parameters of the program: " << std::endl;
    std::cout << "Name of the file with persistence landscapes/barcodes : " << fileWithBarcode << std::endl;

    //now generate landscapes based on that files:
    PersistenceBarcodes b(fileWithBarcode);

    std::pair<double,double> mM = b.minMax();

    std::ostringstream name0;
    name0 << fileWithBarcode << "_minMax";
    std::string fName0 = name0.str();
    const char* FName0 = fName0.c_str();
    ofstream out;
    out.open( (char*)FName0 );
    out << mM.first << " " << mM.second;
    out.close();

    std::cout << "Min and max has been found." << std::endl;
    std::cout << "That is all. Have a good day!" << std::endl;
	return 0;
}


