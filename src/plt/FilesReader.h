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



#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include "PersistenceBarcode.h"
#include "PersistenceLandscape.h"


//This procedure is designed to read file names from a file provided by user and return vector of those names.
std::vector<std::string> readFileNames( char* filenameWithFilenames )
{
    bool dbg = false;

    std::vector<std::string> result;
    std::ifstream in;
    in.open( filenameWithFilenames );
    std::string line;
    while (!in.eof())
    {
        getline(in,line);
        line.erase( std::remove_if( line.begin(), line.end(), ::isspace) , line.end() );

        if (dbg){std::cerr << "line : " << line << std::endl;}

        if ( (line.length() == 0) || (line[0] == '#') )
        {
            //in this case we have a file name. First we should remove all the white spaces.
            if ( dbg ){std::cerr << "This is a line with comment, it will be ignored n";}
        }
        else
        {
            result.push_back( line );
            if (dbg){std::cerr << "Line after removing white spaces : " << line << std::endl;}
        }
	}
    in.close();

    /*
    std::cout << "Read the following files from the file : " << filenameWithFilenames << std::endl;
    for ( size_t i = 0 ; i != result.size() ; ++i )
    {
        std::cerr << result[i] << std::endl;
    }
    std::cerr << std::endl;
    */

    return result;
}//readFileNames


//This procedure reads all the files named in the vector<string>. It can read both barcode files and landscape files. But in the case of landsape files, we assume that the extension is
//'.lan'.
std::vector< PersistenceLandscape > createLandscapesFromTheFiles( std::vector<std::string> filenames )
{
    bool dbg = true;

    std::vector< PersistenceLandscape > result;
    for ( size_t fileNo = 0 ; fileNo != filenames.size() ; ++fileNo )
    {
        //now reading file filenames[fileNo]
        char* filename = (char*)filenames[fileNo].c_str();

        //first we need to check if it is a barcode, or .lan file.
        bool isThisBarcodeFile = true;
        if ( filenames[fileNo].size() > 4 )
        {
            //check if last four characters in the name of the file is '.lan'.
            size_t l = filenames[fileNo].size();
            if ( (filenames[fileNo][l-1] == 'n') && (filenames[fileNo][l-2] == 'a') && (filenames[fileNo][l-3] == 'l') && (filenames[fileNo][l-4] == '.') )
            {
                isThisBarcodeFile = false;
            }
        }

        if ( dbg ){std::cerr << "Considering a file : " << filename << std::endl;}
        if ( dbg ){std::cerr << "isThisBarcodeFile : " << isThisBarcodeFile << std::endl;}

        if ( isThisBarcodeFile )
        {
            if ( dbg ){std::cerr << "This is a barcode file" << std::endl;}
            PersistenceBarcodes b( filename );
            PersistenceLandscape l(b);
            result.push_back(l);
        }
        else
        {
            if ( dbg ){std::cerr << "This is a landscape file" << std::endl;}
            PersistenceLandscape l( filename );
            result.push_back(l);
        }
    }
    return result;
}//createLandscapesFromTheFiles



//This procedure reads all the files named in the vector<string>. They all asume to store barcode
std::vector< PersistenceBarcodes > createBarcodesFromTheFiles( std::vector<std::string> filenames )
{
    bool dbg = true;

    std::vector< PersistenceBarcodes > result;
    for ( size_t fileNo = 0 ; fileNo != filenames.size() ; ++fileNo )
    {
        //now reading file filenames[fileNo]
        char* filename = (char*)filenames[fileNo].c_str();

        //first we need to check if it is a barcode, or .lan file.
        PersistenceBarcodes b( filename );
        result.push_back(b);
    }
    return result;
}//createLandscapesFromTheFiles
