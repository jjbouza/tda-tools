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
    "This is ComputeAverage program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameter: \n\
A file containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided.";

int main(int argc, char *argv[]) {
  configure();
  std::cout << programInfo << std::endl;

  if (argc != 2) {
    std::cout << std::endl << std::endl;
    std::cout << "Wrong usage of a program. Please call <program name> ";
    std::cout << "<name of the file with names of files with persistence "
                 "landscapes/barcodes> ";
    std::cout << "The program will now terminate." << std::endl;
    return 1;
  }

  char *fileWithInputFiles = argv[1];

  std::cout << "Here are the parameters of the program: " << std::endl;
  std::cout << "Name of the file with barcodes \\ landscapes : "
            << fileWithInputFiles << std::endl;

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
               "found in the file 'distances.txt'"
            << std::endl;

  vectorSpaceOfPersistenceLandscapes v;
  for (size_t i = 0; i != landscapes.size(); ++i) {
    v.addLandscape(landscapes[i]);
  }

  PersistenceLandscape average = v.average();

  std::ostringstream name;
  name << fileWithInputFiles << "_average.lan";
  std::string fName = name.str();
  const char *FName = fName.c_str();
  average.printToFile((char *)FName);

  std::cout << "That is all. Have a good day!" << std::endl;

  return 0;
}
