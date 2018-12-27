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

#include "PersistenceBarcode.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

const char *programInfo =
    "This is BinningOnDiagram program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following two parameters: \n\
A file with a persistence diagram.\n\
A positive integer n indicating how many bins we want to have.";

// think if the input parameter should not be the number representing +infinity
// in the diagram file.

int main(int argc, char *argv[]) {
  configure();

  std::cout << programInfo << std::endl;
  size_t numberOfInputFiles = atoi(argv[1]);

  if (argc != 3) {
    std::cout << std::endl << std::endl;
    std::cout << "Wrong usage of a program. Please call <program name> ";
    std::cout << "<Name of the file persistence diagram> ";
    std::cout
        << "<A positive integer n indicating how many bins we want to have>. ";
    std::cout << "The program will now terminate." << std::endl;
    return 1;
  }

  PersistenceBarcodes b(argv[1]);

  b.putToBins(atoi(argv[2]));

  ofstream out;
  ostringstream name;
  name << argv[1] << "_bins_" << atoi(argv[2]);
  std::string nameStr = name.str();
  const char *filename1 = nameStr.c_str();

  b.writeToFile((char *)filename1);

  clock_t end = clock();

  return 0;
}
