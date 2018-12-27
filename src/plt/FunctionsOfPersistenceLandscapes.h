#pragma once
#include "PersistenceLandscape.h"

using namespace std;

// functions of landscape:
// integral
// maximum
// moments
// number of nonzero landscapes

double computeIntegral(const PersistenceLandscape &l) {
  return l.computeIntegralOfLandscape();
} // computeIntegral

double maximum(const PersistenceLandscape &l) {
  return l.computeMaximum();
} // maximum

double
firstMomentOfFirstLandscapeCenteredAtZero(const PersistenceLandscape &l) {
  return l.computeNthMoment(1, 0, 0);
} // firstMomentCenteredAtZero

double
secondMomentOfFirstLandscapeCenteredAtZero(const PersistenceLandscape &l) {
  return l.computeNthMoment(2, 0, 0);
} // firstMomentCenteredAtZero

double
thirdMomentOfFirstLandscapeCenteredAtZero(const PersistenceLandscape &l) {
  return l.computeNthMoment(3, 0, 0);
} // firstMomentCenteredAtZero

double
fourthMomentOfFirstLandscapeCenteredAtZero(const PersistenceLandscape &l) {
  return l.computeNthMoment(4, 0, 0);
} // firstMomentCenteredAtZero

double numberOfNonzeroLandscapes(const PersistenceLandscape &l) {
  return l.size();
} // numberOfNonzeroLandscapes

double userFunction(const PersistenceLandscape &l) {
  // Please implement your function here:
  return 3.141592;
} // numberOfNonzeroLandscapes

double averageBarcodeLength(PersistenceBarcodes &b) {
  double av = 0;
  for (PersistenceBarcodes::bIterator i = b.bBegin(); i != b.bEnd(); ++i) {
    av += fabs(i->second - i->first);
  }
  av = av / b.size();
  return av;
}

double maxLengthBarcode(PersistenceBarcodes &b) {
  double maxL = 0;
  for (PersistenceBarcodes::bIterator i = b.bBegin(); i != b.bEnd(); ++i) {
    if (maxL < fabs(i->second - i->first))
      maxL = fabs(i->second - i->first);
  }
  return maxL;
}

// TODO -- after adding any function to the above collection, please add it to
// the string and a function below that allows to choose suitable function by the
// user:
const char *listOfAvailableFunctions = "The available functions are:\n\
1 - computeIntegral\n\
2 - maximum\n\
3 - firstMomentOfFirstLandscapeCenteredAtZero\n\
4 - secondMomentOfFirstLandscapeCenteredAtZero\n\
5 - thirdMomentOfFirstLandscapeCenteredAtZero\n\
6 - fourthMomentOfFirstLandscapeCenteredAtZero\n\
7 - numberOfNonzeroLandscapes\n\
8 - user defined function, please go to the file functionsOfPersistenceLandscapes.h to define it\n";

typedef double (*fptr)(const PersistenceLandscape &);

size_t numberOfFunctions() { return (size_t)8; }

fptr gimmeFunctionOfANumnber(int numberOfFunction) {
  if ((numberOfFunction <= 0) || (numberOfFunction > 7)) {
    std::cout << "Wrong number of function : " << numberOfFunction
              << ".The program will now terminate." << std::endl;
    throw("Wrong number of function, the program will now terminate.\n");
  }
  switch (numberOfFunction) {
  case 1: {
    std::cout << "Using function computeIntegral." << std::endl;
    return computeIntegral;
    break;
  }
  case 2: {
    std::cout << "Using function maximum." << std::endl;
    return maximum;
    break;
  }
  case 3: {
    std::cout << "Using function firstMomentCenteredAtZero." << std::endl;
    return firstMomentOfFirstLandscapeCenteredAtZero;
    break;
  }
  case 4: {
    std::cout << "Using function secondMomentOfFirstLandscapeCenteredAtZero."
              << std::endl;
    return secondMomentOfFirstLandscapeCenteredAtZero;
    break;
  }
  case 5: {
    std::cout << "Using function thirdMomentthirdMomentCenteredAtZero."
              << std::endl;
    return thirdMomentOfFirstLandscapeCenteredAtZero;
    break;
  }
  case 6: {
    std::cout << "Using function fourthMomentthirdMomentCenteredAtZero."
              << std::endl;
    return fourthMomentOfFirstLandscapeCenteredAtZero;
    break;
  }
  case 7: {
    std::cout << "Using function numberOfNonzeroLandscapes." << std::endl;
    return numberOfNonzeroLandscapes;
    break;
  }
  case 8: {
    std::cout << "Using the function defined by the used in a file "
                 "functionsOfPersistenceLandscapes.h."
              << std::endl;
    return userFunction;
    break;
  }
  default: {
    std::cout << "Unknown function, the program will now terminate."
              << std::endl;
    throw("Unknown function, the program will now terminate.\n");
    return 0;
  }
  }
}
