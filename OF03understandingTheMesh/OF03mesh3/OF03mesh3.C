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
    OF03mesh3

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    #include "createMesh.H"

    IOdictionary volumeRatioDict // Define dictionary object
    (
        IOobject
        (
            "volumeRatioDict", // Dictionary nme
            runTime.system(), // Dictionary path
            mesh, // 基于 mesh 构造
            IOobject::MUST_READ, 
            IOobject::NO_WRITE
        )
    );

    const scalar c = mesh.C().size();
    Info << "Number of cells = " << c << endl; // Get the number of mesh cells

    const labelListList& neighbour = mesh.cellCells(); // Return neighbour cells as a new list

    /*
    * Modifying the code
    */
   //label len = mesh.Cf().size();
   //scalar initial = 0;
   //List<scalar> ratios(len, initial); // Create a fixed size list
   //label counter = 0;
    //lines above are added 
    scalar volumeRatio = 0.0;
    scalar currentRatio = 0.0;

    label nFail = 0;

    scalar maxRatio(readScalar(volumeRatioDict.lookup("maxRatio")));
    // read from Dict

     forAll (neighbour, cellI) // Over all neighbour cells
     {
        List<label> n = neighbour[cellI]; 
        // Get access to the neighbour cells for each cell and put in list n

        const scalar cellVolume = mesh.V()[cellI]; // Get the volume of cellI

        forAll (n, i) // 得看一下forAll语句怎么定义的
        {
            label neighbourIndex = n[i];
            scalar neighbourVolume = mesh.V()[neighbourIndex];

            if (neighbourVolume >= cellVolume)
            {
                volumeRatio = neighbourVolume / cellVolume;

                if (volumeRatio > currentRatio)
                {
                    currentRatio = volumeRatio;
                }

                if ( volumeRatio > maxRatio)
                {
                    nFail += 1;
                }
                 //ratios.append(volumeRatio);
                 //ratios[counter] = volumeRatio;
                 //counter += 1;
            }
        }
    }

    Info << "Maximum volume ratio = " << currentRatio << nl 
         << "Number of cell volume ratios exceeding " << maxRatio
         << " = " << nFail << nl
         << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
