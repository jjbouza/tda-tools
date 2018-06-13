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


#include "Anova.h"
#include "FunctionsOfPersistenceLandscapes.h"
#include "FilesReader.h"

using namespace std;

const char* programInfo =
"This is anova program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameter: \n\
A positive integer N indicating the number of classes that are to be tested,\n\
N files containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided. The next parameter is a function\n\
the average value of which will be compared by using anova. We have provided here some basic functions, but uses should feel free to implement his own function\n\
in a file functionsOfPersistenceLandscapes.h. In this file it is indicated how to use that function.\n\
You should provide number of a function as a parameter of this program. The list of available functions is given below\n\
In this case the program returns the value of F-statistics and the user have to verify the corresponding p-value in the appropriate table.";


int main(int argc, char **argv)
{
    configure();
    std::cout << programInfo << std::endl << listOfAvailableFunctions << std::endl;

    int numberOfClasses = atoi( argv[1] );
    if ( numberOfClasses < 1 )
    {
        std::cout << "Number of classes is : " << numberOfClasses << " which is not a correct value. The program will terminate now." << std::endl;
        return 1;
    }
    std::vector<  std::vector<std::string>  > classes;
    for ( size_t classNo = 0 ; classNo != numberOfClasses ; ++classNo )
    {
        //cerr << "Reading : " << argv[2+classNo] << endl;
        std::vector<std::string> namesOfFilesFirstFamilly = readFileNames( argv[2+classNo] );
        if ( namesOfFilesFirstFamilly.size() == 0 )
        {
            std::cerr << "Class number : " << classNo << " is empty. This is not a correct input to the program. Please remove the class or correct the file name. The program will terminate now." << std::endl;
            return 1;
        }
        classes.push_back( namesOfFilesFirstFamilly );
    }
    int numberOfFunction = atoi( argv[2+numberOfClasses] );
    double confidenceValue = atof( argv[3+numberOfClasses] );

    std::cerr << "Number of function : " << numberOfFunction << std::endl;
    std::cerr << "Confidence value : " << confidenceValue << std::endl;

    std::cerr << "The parameter of the program are : " << std::endl;
    std::cerr << "Number of classes : " << numberOfClasses << std::endl;
    std::cerr << "Names of files with different landscapes or barcodes classes:" << std::endl;
    for ( size_t classNo = 0 ; classNo != classes.size() ; ++classNo )
    {
        std::cout << "Class number : " << classNo+1 << std::endl;
        for ( size_t fileNo = 0 ; fileNo != classes[classNo].size() ; ++fileNo )
        {
            std::cout << classes[classNo][fileNo] << std::endl;
        }
    }




    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< std::vector<PersistenceLandscape> > landscapes;
    for ( size_t classNo = 0 ; classNo != classes.size() ; ++classNo )
    {
        std::vector< PersistenceLandscape > landscapesThisFamilly = createLandscapesFromTheFiles( classes[classNo] );
        landscapes.push_back(landscapesThisFamilly);
    }
    std::cout << "Landscapes has been created." << std::endl;

    double (*functionToUse)(const PersistenceLandscape&) = gimmeFunctionOfANumnber(numberOfFunction);

    cerr << "Constructing anova \n";

    anova<PersistenceLandscape> k(landscapes ,functionToUse );
    k.FCharacteristic();

    //for debug only
    //k.printStatistics();

    std::cout << "That is all. Have a good day!" << std::endl;
	return 1;
}
