//    Copyright 2013-2014 University of Pennsylvania
//    Created by Pawel Dlotko
//
//    This file is part of Persistence Landscape Toolbox (PLT).
//
//    PLT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or (at
//    your option) any later version.
//
//    PLT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with PLT.  If not, see <http://www.gnu.org/licenses/>.

#include "FilesReader.h"
#include "VectorSpaceOfPersistenceLandscapes.h"
#include <cstdlib>
#include <iostream>

const char *programInfo =
    "This is StandardDeviation program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following two parameters: \n\
A file containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided.\n\
A positive integer p such that the program will compute pairwise L^p between landscapes (set -1 for bottleneck distance).";

// think if the input parameter should not be the number representing +infinity
// in the diagram file.

int main(int argc, char *argv[]) {
  configure();
  std::cout << programInfo << std::endl;
  size_t numberOfInputFiles = atoi(argv[1]);

  if (argc != 3) {
    std::cout << std::endl << std::endl;
    std::cout << "Wrong usage of a program. Please call <program name> ";
    std::cout << "<name of the file with names of files with persistence "
                 "landscapes/barcodes> ";
    std::cout << "<an integer p determining which distance will be used in the "
                 "procedure (set -1 for the bottleneck distance)>. ";
    std::cout << "The program will now terminate." << std::endl;
    return 1;
  }

  char *fileWithInputFiles = argv[1];
  int whichDistance = atoi(argv[2]);

  std::cout << "Here are the parameters of the program: " << std::endl;
  std::cout << "Name of the file with barcodes \\ landscapes : "
            << fileWithInputFiles << std::endl;
  std::cout << "The distance used : " << whichDistance << std::endl;

  // End of a small talk with the User. Let's get to work.
  // First, read the names of files:
  std::vector<std::string> namesOfFiles = readFileNames(fileWithInputFiles);

  // now generate landscapes based on that files:
  std::cout << "Creating Persistence Landscapes." << std::endl;
  std::vector<PersistenceLandscape> landscapes =
      createLandscapesFromTheFiles(namesOfFiles);
  std::cout << "Done." << std::endl;

  // And finally, do the permutation test:
  std::cout << "Computing avereage landscape. When finished, they will be "
               "found in the file 'StandardDeviation.txt'"
            << std::endl;

  vectorSpaceOfPersistenceLandscapes v;
  for (size_t i = 0; i != landscapes.size(); ++i) {
    v.addLandscape(landscapes[i]);
  }

  std::ofstream out;
  out.open("StandardDeviation.txt");

  double standardDeviation = v.standardDeviation(whichDistance);
  out << standardDeviation << std::endl;
  std::cout << "Standard deviation is : " << standardDeviation << std::endl;

  out.close();
  std::cout << "That is all. Have a good day!" << std::endl;

  return 0;
}
