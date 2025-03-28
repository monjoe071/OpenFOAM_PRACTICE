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
    OF04field4

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

scalar calculatePressure(scalar t, vector x, vector x0, scalar scale);

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    Info << "Reading transportProperties\n" << endl;
    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );
    dimensionedScalar nu
    (
        "nu",
        dimViscosity,
        transportProperties
    );

    Info << "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info << "Reading field T\n" << endl;
    volScalarField T
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info << "Reading field U\n" << endl;
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

    volScalarField zeroScalarField
    (
        IOobject
        (
            "zeroScalarField",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ, // 无需给初始条件的文件
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zeroScalarField", dimless, Zero) // initialize as zero
    );

    volVectorField zeroVectorField
    (
        IOobject
        (
            "zeroVectorField",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector("zeroVectorField", dimless, Zero)
    );

    volVectorField gradT
    (
        IOobject
        (
            "gradT",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        fvc::grad(T) // 因为是从 T 场计算而来，所以不需要再次和 mesh 建立关系
    );

    Info << "Reading/calculating face flux field phi\n" << endl;
    surfaceScalarField phi
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fvc::flux(U)
        // fvc::interpolate(U)*mesh.Sf(); // 等同于此行计算
    );

    // Create a vector object originVector with initial value
    const vector originVector(0.05, 0.05, 0.005);

    const scalar rFarCell =
    max
    (
        mag 
        (
            dimensionedVector("x0", dimLength, originVector)
            // create dimensionedVector with reference of originVector
            - mesh.C() // minus the coordinates of centre point of a cell
        )
    ).value(); // return the value of claculated result

    Info << "\nStarting time loop\n" << endl;

    while (runTime.loop()) // time step goes on
    {
        Info << "Time = " << runTime.timeName() << nl << endl;

        for (label cellI=0; cellI<mesh.C().size(); ++cellI)
        {
            p[cellI] = calculatePressure // call the custom function
            (
                runTime.time().value(),
                mesh.C()[cellI],
                originVector,
                rFarCell
            );
        }

        U = fvc::grad(p) * dimensionedScalar("tmp", dimTime, 1.0); // create U field
        // multiply an extra temporary dimensioned varible "tmp"
        // to ensure the same dimension of both side

        runTime.write();
    }

    Info << "Calculation done." << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}

// definiiton of the custom function
scalar calculatePressure(scalar t, vector x, vector x0, scalar scale)
{
    scalar r(mag(x - x0) / scale); // create a scalar object r and initialize

    scalar rR(1.0 / (r + 1e-12));

    scalar f(1.0);

    return Foam::sin(2.0 * Foam::constant::mathematical::pi * f * t) * rR;

}


// ************************************************************************* //
