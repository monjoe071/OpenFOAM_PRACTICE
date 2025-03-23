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
    OF03mesh2

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    #include "createMesh.H"

    Info << "Time                    : " << runTime.timeName() << nl
         << "Number of mesh cells    : " << mesh.C().size() << nl
         << "Number of internal faces: " << mesh.Cf().size() << nl
         << endl;

    for (label cellI = 0; cellI < mesh.C().size(); ++cellI) //C++原生遍历
    {
        if (cellI%100 == 0)
        {
            Info << "Cell " << cellI
                 << " with centre at " << mesh.C()[cellI] // return the coordinates of cell centres
                 << endl; 
        }
    }
    Info << nl << endl;

    forAll(mesh.owner(),faceI)
    {
        if (faceI%200 == 0)
        {
            Info << "Internal face " << faceI
                 << " with centre at " << mesh.Cf()[faceI]
                 << " with owner cell " << mesh.owner()[faceI]
                 << " and neighbour cell " << mesh.neighbour()[faceI]
                 << endl;
        }
    }
    Info << nl << endl;

    forAll(mesh.boundaryMesh(), patchI)
    {
        Info << "patch " << patchI
             << " is " << mesh.boundary()[patchI].name()
             << " with " << mesh.boundary()[patchI].Cf().size() << " faces."
             << "Start from face " << mesh.boundary()[patchI].start()
             << endl; 
    }
    Info << nl << endl;

    label nIndex(0);
    forAll(mesh.boundaryMesh(),patchI)
    {
        Info << "Patch " << patchI << nl
             << "\tits face " << nIndex
             << " adjacent to cell " << mesh.boundary()[patchI].patch().faceCells()[nIndex] << nl
             //<< "\tits normal vector " << mesh.boundary()[patchI].Sf()[nIndex] << nl
             //<< "\tits surface area " << mag(mesh.boundary()[patchI].Sf()[nIndex]) << nl
             << endl;
    }
    Info << nl << endl;

    const faceList& fcs = mesh.faces();
    const pointField& pts = mesh.points();
    const List<point>& cents = mesh.faceCentres();

    forAll(fcs,faceI)
    {
        if (faceI%200 == 0)
        {
            if (faceI < mesh.Cf().size())
            {
                Info << "Internal face ";
            } else
            {
                forAll(mesh.boundary(),patchI)
                {
                    if ( (mesh.boundary()[patchI].start() <= faceI) &&
                        (faceI < mesh.boundary()[patchI].start() + 
                        mesh.boundary()[patchI].Cf().size()))
                    {
                        Info << "Face on patch " << patchI << ", faceI ";
                        break;
                    }
                }
            }
            Info << faceI << " with centre at " << cents[faceI] 
                 << " has " << fcs[faceI].size() << " vertices: ";
            forAll(fcs[faceI],vertexI)
            {
                Info << " " << pts[fcs[faceI][vertexI]];
            }
            Info << endl;
        }
    }
    Info << nl << endl;

    forAll(mesh.boundaryMesh(),patchI)
    {
        const polyPatch& pp = mesh.boundaryMesh()[patchI];
        if (isA<emptyPolyPatch>(pp))
        {
            Info << "Patch " << patchI
                 << ": " << mesh.boundary()[patchI].name()
                 << " is empty."
                 << endl;
        }
    }
    Info << nl << endl;

    word patchName("movingWall");
    label patchID = mesh.boundaryMesh().findPatchID(patchName);
    Info << "Retrived patch " << patchName
         << " at index " << patchID
         << " using its name only."
         << endl;
    Info << nl << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
