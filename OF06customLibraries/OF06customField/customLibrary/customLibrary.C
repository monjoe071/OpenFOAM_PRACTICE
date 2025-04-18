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
    customLibrary

Description

\*---------------------------------------------------------------------------*/

#include "customLibrary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

scalar computeR(const fvMesh& mesh, volScalarField &r, dimensionedVector x0)
{
    r = mag(mesh.C() - x0);
    return max(r).value();
    //return returnReduce(max(r).value(), maxOp<scalar>());
}

void computeU(const fvMesh& mesh, volVectorField& U, word pName)
{
    const volScalarField& pField = mesh.lookupObject<volScalarField>(pName);

    U = fvc::grad(pField)*dimensionedScalar("tmp", dimTime, 1.);
}


// ************************************************************************* //
