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
#include "PersistenceBarcode.h"

using namespace std;

const char* programInfo =
"This is normalizeBarcodes program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameter: \n\
A name of a file with barcode to normalize.";


int main( int argc, char *argv[] )
{
    configure();
    std::cout << programInfo << endl;
    if ( argc != 2 )
    {
        cout << "Wrong parameter list. Please provide a name of a file with the barcode to normalize. The program will now terminate." << endl;
        return 0;
    }
    char* filename = argv[1];

    PersistenceBarcodes b(filename);

    b.setRangeToMinusOneOne();
    //b.averageBarcodes();

    std::ostringstream name;
    name << filename << "_normalized";
    std::string fName = name.str();
    const char* FName = fName.c_str();
    b.writeToFile( (char*)FName );



	return 0;
}
