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
#include "PermutationTest.h"
#include "PersistenceBarcode.h"
#include "PersistenceClassifier.h"
#include "PersistenceLandscape.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

bool compareSecondElementsOfPairs(const std::pair<size_t, double> &f,
                                  const std::pair<size_t, double> &s) {
  return f.second < s.second;
}

const char *programInfo =
    "This is ClassifierBasedOnSingleDimension program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following three parameters: \n\
A string -construct if the classifier is to be constructed, -classify if the classification is to be made based on already constructed classifier or -both if one want to construct classifier and perform the classification at the same time.\n\
It the first parameter is -construct the parameters of the program are:\n\
A positive integer N indicating how many classes there are in the considered data.\n\
A number of N files containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided. We assume that in each of those N files there are landscapes from the same class, and our data is divided to N class\n\
In this case the program create N files each of which contains averaged landscape of a class. The names of the files are class_<number>.lan. Please make sure that in the folder there are no files having the same name.\n\n\
If the first parameter is -classify then the parameters of the program are:\n\
A positive integer N indicating how many classes there are in the considered data.\n\
A file with names of files which are to be classified.\n\
An positive real number p indicating which distance is to be used in classification (set -1 for the bottleneck distance).\n\
A 0-1 variable q. If q = 0 for each input barcode or landscape the best matching cluster will be computed. If q = 1, the distances to all averages will be given.\n\
In that case, the program produce a file classification.txt. In each line of the consist either of a number of a cluster of of a sequence of numbers of clusters with weights\n\n\
If the first parameter is -both then the parameters of the program are:\n\
A positive integer N indicating how many classes there are in the considered data.\n\
A number of N files containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided. We assume that in each of those N files there are landscapes from the same class, and our data is divided to N class\n\
A file with names of files which are to be classified.\n\
An integer p indicating which distance is to be used in classification (set -1 for the bottleneck distance).\n\
A 0-1 variable q. If q = 0 for each input barcode or landscape the best matching cluster will be computed. If q = 1, the distances to all averages will be given.\n\
In that case, the program produce a file classification.txt. In each line of the consist either of a number of a cluster of of a sequence of numbers of clusters with weights.";

// think if the input parameter should not be the number representing +infinity
// in the diagram file.

void construct(std::vector<char *> filesWithClasses) {
  std::vector<std::vector<PersistenceLandscape>> landscapes;
  for (size_t classNo = 0; classNo != filesWithClasses.size(); ++classNo) {
    std::vector<std::string> namesOfFiles =
        readFileNames(filesWithClasses[classNo]);
    std::vector<PersistenceLandscape> landscape =
        createLandscapesFromTheFiles(namesOfFiles);
    landscapes.push_back(landscape);
  }
  persistenceClassifierSingleDimension c(landscapes);
  c.writeToFile((char *)"class");
  std::cout << "Averages computed and written to the folder. The program will "
               "now terminate."
            << std::endl;
}

void classify(size_t numberOfClassesToClassify, char *fileWithNamesOfFiles,
              double usedDistance, int q) {
  persistenceClassifierSingleDimension c;
  c.readFromFile((char *)"class", numberOfClassesToClassify, usedDistance);

  std::cout << "Reading barcodes / lanscapes." << std::endl;
  std::vector<PersistenceLandscape> landscapes =
      createLandscapesFromTheFiles(readFileNames(fileWithNamesOfFiles));
  std::cout << "Done." << std::endl;

  if (q == 0) {
    // only the best matching. If there are a number of equivalently good
    // matchings, all of them will be displayed.
    std::vector<std::vector<size_t>> v = c.classifySimpleVector(landscapes);
    ofstream out;
    out.open("classification.txt");
    for (size_t i = 0; i != v.size(); ++i) {
      // cerr << "i : " << i << endl;
      for (size_t j = 0; j != v[i].size(); ++j) {
        // cerr << "j : " << j << endl;
        out << v[i][j] << " ";
      }
      out << std::endl;
    }
    out.close();
  } else {
    // all the matchings
    std::vector<std::vector<std::pair<size_t, double>>> v =
        c.classifyVector(landscapes);
    ofstream out;
    out.open("classification.txt");
    for (size_t i = 0; i != v.size(); ++i) {
      for (size_t j = 0; j != v[i].size(); ++j) {
        out << "(" << v[i][j].first + 1 << "," << v[i][j].second << ") ";
      }
      out << std::endl << std::endl;
    }
    out.close();
  }
}

int main(int argc, char *argv[]) {
  configure();
  std::cout << programInfo << std::endl;

  if (argc == 1) {
    std::cout << "Wrong parameter list of the program. The program will now "
                 "terminate."
              << std::endl;
    return 1;
  }

  if ((strcmp(argv[1], "-classify") != 0) &&
      (strcmp(argv[1], "-construct") != 0) && (strcmp(argv[1], "-both") != 0)) {
    std::cout
        << "Wrong first parameter of the call of the program. It should be "
           "either -classify or -construct. The program will now terminate."
        << std::endl;
    return 1;
  }

  int action;
  if (strcmp(argv[1], "-construct") == 0) {
    action = 1;
  }
  if (strcmp(argv[1], "-classify") == 0) {
    action = 2;
  }
  if (strcmp(argv[1], "-both") == 0) {
    action = 3;
  }

  if (action == 1) {
    cerr << "The program will construct classifier and write it to your HD \n";
  }
  if (action == 2) {
    cerr << "The program will read the existing classifier from HD and perform "
            "classification \n";
  }
  if (action == 3) {
    cerr << "The program will construct classifier and perform the "
            "classification \n";
  }

  if (action == 1) {
    // in this case we will construct the classifier.
    size_t numberOfClassesToClassify = atoi(argv[2]);
    std::cout << "Number of classes in the data: " << numberOfClassesToClassify
              << std::endl;
    std::vector<char *> filesWithClasses;
    std::cout << "The files to classify are : " << std::endl;
    for (size_t i = 0; i != numberOfClassesToClassify; ++i) {
      std::cout << argv[3 + i] << std::endl;
      filesWithClasses.push_back(argv[3 + i]);
    }
    construct(filesWithClasses);
  } else {
    if (action == 2) {
      // in this case we will do the classification based on the averages of
      // clusters that already exist.
      if (argc != 6) {
        std::cout << "Wrong parameters of the program call. In this mode the "
                     "program needs:"
                  << std::endl;
        std::cout << "A positive integer N indicating how many classes there "
                     "are in the considered data,"
                  << std::endl;
        std::cout
            << "A name of file with names of files which are to be classified,"
            << std::endl;
        std::cout
            << "A positive real number p indicating which distance is to be "
               "used in classification (set -1 for the bottleneck distance),"
            << std::endl;
        std::cout << "A 0-1 variable q. If q = 0 for each input barcode or "
                     "landscape the best matching cluster will be computed. If "
                     "q = 1, the distances to all averages will be given,"
                  << std::endl;
        std::cout << "The program will now terminate." << std::endl;
      }
      size_t numberOfClassesToClassify = atoi(argv[2]);
      char *fileWithNamesOfFiles = argv[3];
      double usedDistance = atof(argv[4]);
      int q = atoi(argv[5]);

      std::cout << "This are the parameters of the program : " << std::endl;
      std::cout << "Number of classes to classify : "
                << numberOfClassesToClassify << std::endl;
      std::cout << "File with the names of files with landscapes or barcodes : "
                << fileWithNamesOfFiles << std::endl;
      std::cout << "Distance used in classification : " << usedDistance
                << std::endl;
      if (q == 0) {
        std::cout << "As a result of classification only the best matching "
                     "will be given."
                  << std::endl;
      } else {
        std::cout << "As a result of classification the similarity to all "
                     "clusters will be given."
                  << std::endl;
      }
      classify(numberOfClassesToClassify, fileWithNamesOfFiles, usedDistance,
               q);
    } else {
      // in this case action == 3, so we both construct a classifier and
      // classify
      size_t numberOfClassesToClassify = atoi(argv[2]);
      std::cout << "Number of classes in the data: "
                << numberOfClassesToClassify << std::endl;
      std::vector<char *> filesWithClasses;
      for (size_t i = 0; i != numberOfClassesToClassify; ++i) {
        filesWithClasses.push_back(argv[3 + i]);
      }
      char *fileWithNamesOfFiles = argv[3 + numberOfClassesToClassify];
      double usedDistance = atof(argv[4 + numberOfClassesToClassify]);
      int q = atoi(argv[5 + numberOfClassesToClassify]);

      std::cout << "This are the parameters of the program : " << std::endl;
      std::cout << "Number of classes to classify : "
                << numberOfClassesToClassify << std::endl;

      std::cout << "The files to classify are : " << std::endl;
      for (size_t i = 0; i != filesWithClasses.size(); ++i) {
        std::cout << filesWithClasses[i] << " ";
      }
      std::cout << std::endl;
      std::cout << "File with the names of files with landscapes or barcodes : "
                << fileWithNamesOfFiles << std::endl;
      std::cout << "Distance used in classification : " << usedDistance
                << std::endl;
      if (q == 0) {
        std::cout << "As a result of classification only the best matching "
                     "will be given."
                  << std::endl;
      } else {
        std::cout << "As a result of classification the similarity to all "
                     "clusters will be given."
                  << std::endl;
      }
      construct(filesWithClasses);
      classify(numberOfClassesToClassify, fileWithNamesOfFiles, usedDistance,
               q);

      /*
      persistenceClassifierSingleDimension c;
      std::vector< std::vector<PersistenceLandscape> > landscapes;
      for ( size_t classNo = 0 ; classNo != filesWithClasses.size() ; ++classNo
      )
      {
          std::vector<std::string> namesOfFiles = readFileNames(
      filesWithClasses[classNo] ); std::vector< PersistenceLandscape > landscape
      = createLandscapesFromTheFiles( namesOfFiles ); landscapes.push_back(
      landscape );
      }
      cerr << "ENTER\n";getchar();
      c.readFromFile( (char*)"class" , numberOfClassesToClassify  , usedDistance
      ); cerr << "EXIT\n";getchar();

      std::cout << "Reading barcodes / lanscapes." << std::endl;
      std::vector< PersistenceLandscape > landscapesToClassify =
      createLandscapesFromTheFiles( readFileNames( fileWithNamesOfFiles ) );
      std::cout << "Done." << std::endl;



      if ( q == 0 )
      {
          //only the best matching. If there are a number of equivalently good
      matchings, all of them will be displayed. std::vector< std::vector<size_t>
      > v = c.classifySimpleVector( landscapesToClassify ); ofstream out;
          out.open("classification.txt");
          for ( size_t i = 0 ; i != v.size() ; ++i )
          {
              for ( size_t j = 0 ; j != v[i].size() ; ++j )
              {
                  out << v[i][j] << " ";
              }
              out << std::endl;
          }
          out.close();
      }
      else
      {
          //all the matchings
          std::vector< std::vector< std::pair< size_t , double > > > v =
      c.classifyVector( landscapesToClassify ); ofstream out;
          out.open("classification.txt");
          for ( size_t i = 0 ; i != v.size() ; ++i )
          {
              for ( size_t j = 0 ; j != v[i].size() ; ++j )
              {
                  out << "(" << v[i][j].first+1 << "," << v[i][j].second << ")
      ";
              }
              out << std::endl << std::endl;
          }
          out.close();
      }
      */
    }
  }
  std::cout << "That is all. Have a good day!" << std::endl;
  return 0;
}
