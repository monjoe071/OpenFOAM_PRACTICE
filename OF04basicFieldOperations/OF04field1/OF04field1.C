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
    OF04field1

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// A function with arguments of time,t, sptial coordinates, x, reference point, x0
// and scale factor, scale
scalar calculatedPressure(scalar t, vector x, vector x0, scalar scale);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    // This reads a dictionary file
    Info << "Reading transportProperties" << endl; 
    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties", // name
            runTime.constant(), // path
            mesh,
            IOobject::MUST_READ_IF_MODIFIED, // the file will be re-read if it is modified
            IOobject::NO_WRITE // read-only
        )
    );

    // Create a scalar constant for kinematic viscosity, nu
    dimensionedScalar nu
    (
        "nu", // name of the varible
        dimViscosity, // dimensions
        // TIP: to check how this is defined, run:
		// grep -r dimViscosity $FOAM_SRC/OpenFOAM/
        transportProperties.lookup("nu") // this takes the value from the dictionary and returns it, passing it to the object constructor as an argument
    );
    Info << "nu: " << nu << endl;

    // Read the field p from the time folders
    // Don't need keywords
    Info << "rading filed p\n" << endl;
    // Create a scalar field p
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(), // name of the current time
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh // initialises the field to match the size of the mesh with default (0) values
    );

    // Read the field U from the file
    Info << "Reading field U\n" << endl;
    // Create a vector field U
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    // Define a constant vector field that will not change
    const vector originVector(0.05, 0.05, 0.05);

    // Calculate the max distance from the origin to the mesh centre
    // The .value() method converts a dimensionedScalar to a regular scalar
    const scalar rFarCell = max( // find the max distance
        // 向量相减后取模
        mag(dimensionedVector("x0", dimLength, originVector) - mesh.C())
    ).value();

    Info << "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {
        Info << "Time = " << runTime.timeName() << nl << endl;

        for (label cellI = 0; cellI<mesh.C().size(); cellI++)
        {
            p[cellI] = calculatedPressure(runTime.time().value(), mesh.C()[cellI], originVector, rFarCell);
        }

        U = fvc::grad(p)*dimensionedScalar("tmp", dimTime, 1.0);

        runTime.write();
    }

    Info << "Finished! Best to visualise the results by plooting p iso-contours with range (-10, 10)"
         << " and applying a glyph filter to the Ufield in paraview." << endl;


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// Definition of the custom function declared at the top of this file
scalar calculatedPressure(scalar t, vector x, vector x0, scalar scale)
{
    scalar r (mag(x-x0)/scale);

    scalar rR (1.0 / (r + 1e-12));

    scalar f (1.0);

    return Foam::sin(2.0*Foam::constant::mathematical::pi*f*t)*rR*100;
}

// ************************************************************************* //
