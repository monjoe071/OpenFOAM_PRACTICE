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
    OF02args

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    argList::addNote(
        "Demonstrates how to handle command line options.\n"
        "\n"
        "Input argyments:\n"
        "----------------\n"
        "   someWord - does stuff\n"
        "   someScalar - does more things\n");

    argList::noParallel();
    argList::validArgs.append("someWord");
    argList::validArgs.append("someScalar");

    argList::addOption
        (
            "dict",
            "word",
            "Path to an addtional dictionary (not really used now)");

    argList::addBoolOption
        (
            "someSwitch",
            "Switches from A to B");

    argList::addOption
        (
            "someInt",
            "label",
            "Optional integer");


    // create argument list
    // This is normally defined inside setRootCase.H
    // #include "setRootCase.H"
    Foam::argList args(argc, argv);
    if (!args.checkRootCase())
    {
        Foam::FatalError.exit();
    }


    const word someWord = args[1];
    const scalar someScalar = args.argRead<scalar>(2);

    Info << "Got argument: " << nl
         << "word: " <<someWord << nl 
         << "scalar: " << someScalar << endl;

    
    fileName dictPath("./system/defaultDict");

    if (args.optionFound("dict"))
    {
        args.optionReadIfPresent("dict", dictPath);
        Info << "Got an override flag for dictionary path" << endl;
    }
    Info << "Would read dict from " << dictPath << endl;

    const bool someConstBool = args.optionFound("someSwitch");
    Info << "Boolean switch set to " << someConstBool << endl;

    label someInt(0);
    args.optionReadIfPresent("someInt", someInt);
    Info << "Integer option value " << someInt << endl;

    // #include "setRootCase.H"
    // #include "createTime.H"

    // // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    // Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
    //     << "  ClockTime = " << runTime.elapsedClockTime() << " s"
    //     << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
