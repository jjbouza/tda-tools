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


#include "T-StudentTest.h"
#include "FunctionsOfPersistenceLandscapes.h"
#include "FilesReader.h"

using namespace std;

const char* programInfo =
"This is t-StudentTest program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameter: \n\
Two files containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided. The second parameter is a function\n\
the average value of which will be compared by using t-test. We have provided here some basic functions, but uses should feel free to implement his own function\n\
in a file functionsOfPersistenceLandscapes.h. In this file it is indicated how to use that function.\n\
You should provide number of a function as a parameter of this program. The list of available functions is given below\n\
The last parameter is the confidence value (typically it should be choose from the set:\n\
For one-sided test : 0.75 (=75%), 0.8 (=80%), 0.85 (=85%), 0.9 (=90%), 0.95 (=95%), 0.975 (=97.5%), 0.99 (=99%), 0.995 (=99.5%), 0.9975 (=99.75%), 0.99 (=99.9%), 0.9995 (=99.95%)\n\
For two-sided test : 0.5 (=50%), 0.6 (=60%), 0.7 (=70%), 0.8 (=80%), 0.9 (=90%), 0.95 (=95%), 0.98 (=98%), 0.99 (=99%), 0.995 (=99.5%), 0.998 (=99.8%), 0.999 (=99.9%)\n";


int main(int argc, char **argv)
{
    configure();
    std::cout << programInfo << std::endl << listOfAvailableFunctions << std::endl;
    if ( argc != 5 )
    {
        std::cout << "Wrong number of parameters of the program. The parameters should be:" << std::endl;
        std::cout << "Name of file with landscapes or barcodes from the first population" << std::endl;
        std::cout << "Name of file with landscapes or barcodes from the second population" << std::endl;
        std::cout << "Number of a function (see program info)" << std::endl;
        std::cout << "Confidence value" << std::endl;
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }
    std::vector<std::string> namesOfFilesFirstFamilly = readFileNames( argv[1] );
    std::vector<std::string> namesOfFilesSecondFamilly = readFileNames( argv[2] );

    int numberOfFunction = atoi( argv[3] );
    double confidenceValue = atof( argv[4] );

    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< PersistenceLandscape > landscapesFirstFamilly = createLandscapesFromTheFiles( namesOfFilesFirstFamilly );
    std::vector< PersistenceLandscape > landscapesSecondFamilly = createLandscapesFromTheFiles( namesOfFilesSecondFamilly );
    std::cout << "Landscapes has been created." << std::endl;



    double (*functionToUse)(const PersistenceLandscape&) = gimmeFunctionOfANumnber(numberOfFunction);

    tStudentTest<PersistenceLandscape> k(landscapesFirstFamilly , landscapesSecondFamilly ,functionToUse );

    k.verifyNullHypotheris( confidenceValue );

    //for debug only
    //k.printStatistics();

    std::cout << "That is all. Have a good day!" << std::endl;
	return 1;
}
