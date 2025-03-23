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
    OF03mesh1

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

#include "IOmanip.H" // 输出格式控制

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    /*
    * About createMesh.H
    */
   //#include "createMesh.H"
   // main part in createMesh.H
   Foam::Info
    << "Create mesh for time = "
    << runTime.timeName() << Foam::nl << Foam::endl;

Foam::fvMesh mesh
(
    Foam::IOobject
    (
        Foam::fvMesh::defaultRegion,
        runTime.timeName(),
        runTime,
        Foam::IOobject::MUST_READ
    )
);
//    Info << "Default region: " << fvMesh::defaultRegion << endl;
//    Info << "Mesh sudirectory: " << fvMesh::meshSubDir << endl;

//    Foam::autoPtr<Foam::fvMesh> meshPtr(nullptr);
//    Foam::word regionName(Foam::polyMesh::defaultRegion);

//    Foam::Info << "Create mesh";
//    Foam::Info << " for time " << runTime.timeName() << Foam::nl;

//    meshPtr.reset
//    (
//         new Foam::fvMesh
//         (
//             Foam::IOobject
//             (
//                 regionName,
//                 runTime.timeName(),
//                 runTime,
//                 Foam::IOobject::MUST_READ
//             ),
//             false
//         )
//    );
//    meshPtr().init(true);
//    Foam::fvMesh& mesh = meshPtr();
//    Foam::Info << Foam::endl;
   //lines above copied from createMesh.H
   
   Info << "Mesh directory: " << mesh.meshDir() << endl; //methods from polyMesh
    // print file path where mesh is stored

    const pointField& p = mesh.points(); // points
    Info << "Number of points: " << p.size() << endl;
    for (int i=0; i<3; ++i)
    {
        Info << "("
             << setf(ios_base::scientific)
             << setw(15)
             << p[i][0];
        Info << ", "
             << setf(ios_base::scientific)
             << setw(15)
             << p[i][1];
        Info << ", "
             << setf(ios_base::scientific)
             << setw(15)
             << p[i][2]
             << ")" << endl;
    }
    // print coordinates of first 3 points

    const faceList& f = mesh.faces(); // faces
    Info << "Number of faces: " << f.size() << endl;
    forAll(f,i) // forAll loop: iteration method, check one by one
    {
        Info << "("
             << setw(6) << f[i][0]
             << setw(6) << f[i][1]
             << setw(6) << f[i][2]
             << setw(6) << f[i][3]
             << ")" << endl;
    }
    // print the infomation of points making up the ith face
    // compare with testCase/constant/polyMesh/faces

    const labelList& fOwner = mesh.faceOwner();
    Info << "Number of face owner: " << fOwner.size() << endl;
    forAll(fOwner,i)
    {
        Info << setw(6) << fOwner[i] << endl;
    }
    // 输出自然列表序号的面的owner的单元序号
	// 例如输出
	// 0                    // 0号面的owner是单元0
	// 0                    // 1号面的owner是单元0
	// 1                    // 2号面的owner是单元1
	// 2                    // 3号面的owner是单元2
	// ...
	// 以此类推

    const labelList& fNeigh = mesh.faceNeighbour();
    Info << "Number of face neighbour: " << fNeigh.size() << endl;
    forAll(fNeigh,i)
    {
        Info << setw(6) << fNeigh[i] << endl;
    }

    const polyBoundaryMesh& bm = mesh.boundaryMesh();
    Info << "Number of boundary mesh: " << bm.size() << endl;
    forAll(bm,i)
    {
        Info << "Boundary name: " << bm[i].name()
             << "\tBoundary type: " << bm[i].type()
             << endl;
    }
    // print boundaries

    Info << nl << endl;

    Info << "bounding box: " << mesh.bounds() << endl;
    Info << "Mesh volume: " << sum(mesh.V()).value() << endl;

    // Foam::word regionName(Foam::polyMesh::defaultRegion);

    // Foam::polyMesh mesh
    // (
    //     IOobject
    //     (
    //         regionName,
    //         runTime.timeName(),
    //         runTime,
    //         IOobject::MUST_READ
    //     )
    // );

    // Info << "Max cell centre: " << max(mesh.C()) << endl;
    // Info << "Max cell volumes: " << max(mesh.V()) << endl;
    // Info << "Max cell face centres: " << max(mesh.Cf()) << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
