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
    OF01inputOutput

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote ("This is a directions");
    
    //Initialize OF case
    #include "setRootCase.H"

    //These two create the time system (instance named runTime) and the fvMesh (instance named mesh)
    #include "createTime.H"
    #include "createMesh.H"

    //Get access to a custom dictionary
    dictionary customDict;
    const word dictName("customProperties"); //assignment, be like const word dictName = ...
    
    //create an input-output object - this holds the path to the dict and its name
    const word Path = "./constant/"; //relative path to the case
    IOobject dictIO
    (
        dictName, //name of the dictionary, "customProperties"
        Path, //equivalent to mesh.time().constant();
        mesh, //objectRegistry is mesh, which needed by the constructor
        IOobject::MUST_READ //conpulsory to read the dictionary
    );

    //check if the dictionary is present and follows the OF format
    if (!dictIO.typeHeaderOk<dictionary>(true))
    {
        FatalErrorIn(args.executable()) << "Cannot open specifired refinement dictionary "
            << dictName << exit(FatalError);
    }
    
    //Initialise the dictionary object
    customDict = IOdictionary(dictIO);

    //Read various information from the dictionary

    //lookup() does not need to be told the type of the variable, it will automatically deduce it
    //The following lines will grab the value of "someWord" then assign it to the word variable someWord
    //word is a class derived from string, so it is a string
    word someWorld;
    customDict.lookup("someWorld") >> someWorld;

    // 这个方法需要让lookup知道变量类型，并且如果值没有找到，你可以指定一个默认值
    //此例中就指定了一个默认值1.0
    //find a scalar value named someScalar
    scalar someScalar( customDict.lookuporDefault<scalar>("someScalar", 1.0) );

    //find a boolean value
    // A switch is a neat feature allowing boolean values to be read from a dict,
    // it supports the OpenFOAM yes/on/true/1 and no/off/false/0 values automatically.
    bool someBool( customDict.lookuporDefault<bool>("someBool", true) );

    //find a array/List in OF format
    List<scalar> someList ( customDict.lookup("someList") );

    //read a HashTable
    HashTable<vector, word> someHashTable ( customDict.lookup("someHashTable") );

    //Summarise what's been read and print in the console window
    Info << nl << "Read the following:" << nl << nl
         << "someWorld " << someWorld << nl << nl
         << "someScalar " << someScalar << nl << nl
         << "someList " << someList << nl << nl
         << "someHashTable " << someHashTable << nl << nl
         << "someBool " << someBool << nl << nl
         << endl;

    
    //Create a custom directory and write an output file
    
    //Create the output path
    fileName outputDir = mesh.time().path()/"postProcessing";
    //Create the directory
    mkdir(outputDir);

    autoPtr<OFstream> outputFilePtr;

    outputFilePtr.reset(new OFstream(outputDir/"customOutputFile.dat"));

    outputFilePtr() << "BSZDSRC" << endl;
    outputFilaPtr() << "0 1 2 3 4 5" << endl;

    someHashTable.insert("newNEWKey", vector(1., 0., 0.));
    outputFilePtr() << someHashTable << endl;
    Info << "Written done, find the file at postProcessing/customOutputFile.dat\n" << endl;


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
