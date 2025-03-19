/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2025 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    OF01IO

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    /*
        Reading from the dictionary
    */
   const word dictName("customProperties"); //Create a word object to store the name of the dictionary
   IOobject dictIO //Create an IOobject object
   (
        dictName, //Name of the dictionary
        mesh.time().constant(), //Path of the dictionary
        mesh, //relavant mesh
        IOobject::MUST_READ, //conpulsory to read from the dictionary
        IOobject::NO_WRITE //No need to write to the dictionary
   );


   //Error if the file is not defined as class dictionary
   if (!dictIO.typeHeaderOk<dictionary>(true))
   {
        FatalErrorIn(args.executable()) << "Cannot open specified dictionary"
            << dictName << exit(FatalError);
   }
   

   dictionary myDictionary;
   myDictionary = IOdictionary(dictIO); //Create a dictionary object from IOobject

   Info << "Reading myProperties\n" << endl;
   IOdictionary myProperties //字典变量名和字典文件名取相同
   (
        IOobject
        (
            "myProperties",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
   );

   word solver; //Create a word variable
   myProperties.lookup("application") >> solver;
   //Read the value of the key "application" from myProperties and store it in the variable solver

   word format(myProperties.lookup("writeFormat"));

   scalar timeStep(myProperties.lookupOrDefault("deltaT", scalar(0.01)));
   //or
   //scalar timeStep(myProperties.lookupOrDefault<scalar>("deltaT", 0.01));
   // 如果字典中没有提供这一关键词，则使用此句提供的默认值

   bool ifPurgeWrite(myProperties.lookupOrDefault<Switch>("purgeWrite", 0));

   List<scalar> pointList(myProperties.lookup("point"));
   //Read a List

   HashTable<vector, word> sourceField(myProperties.lookup("source"));
   //Read a HashTable

   vector myVec = vector(myProperties.subDict("subDict").lookup("myVec"));
   //Read a sub-dictionary
   //Note the syntax of the sub-dictionary

   //output
   Info << nl
        << "application: " << solver << nl << nl
        << "writeFormat: " << format << nl << nl
        << "deltaT: " << timeStep << nl << nl
        << "purgeWrite: " << ifPurgeWrite << nl << nl
        << "point: " << pointList << nl << nl
        << "source: " << sourceField << nl << nl
        << "myVec: " << myVec << nl << nl
        << endl;

    
    /*
    * Writing to files
    */
    fileName outputDir = runTime.path()/"processing"; //Create varible outputDir and its path
    mkDir(outputDir); //Createthe the directory

    autoPtr<OFstream> outputFilePtr; //Create a pointer to the OFstream
    outputFilePtr.reset(new OFstream(outputDir/"myOutPut.dat")); //给指针定向

    //Write to the file using the pointer
    outputFilePtr() << "processing/myOutPut.dat" << endl;
    outputFilePtr() << "0 1 2 3 ..." << endl;
    sourceField.insert("U", vector(1, 0.0, 0.0));
    outputFilePtr() << sourceField << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
