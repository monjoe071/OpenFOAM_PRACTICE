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
    OF04field3

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    volScalarField p // pressure field p constructed on IOobject and mesh
    (
        IOobject // create IOobject using following arguments
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    //runTime++; // move to next time step

    //for (int i = 0; i < 3; ++i) // three time step
    while(runTime.loop()) // time step goes on
    {
        //++runTime; // move to next time step
        
        forAll(p,i) 
        {
            if (mesh.C()[i][1] < 0.5)
            {
                p[i] = i*runTime.value();
            }
        }
        //p.write();
        runTime.write(); // write as required by the Dict
    }

    // forAll(p,i) 
    // {
    //     if (mesh.C()[i][1] < 0.5)
    //     {
    //         p[i] = i*runTime.value();
    //     }
    // }

    Info << "Max p = " << max(p).value() << endl;
    Info << "Min p = " << min(p).value() << endl;
    Info << "Average p = " << average(p).value() << endl;
    Info << "Sum p = " << sum(p).value() << endl;

    // p.write();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
