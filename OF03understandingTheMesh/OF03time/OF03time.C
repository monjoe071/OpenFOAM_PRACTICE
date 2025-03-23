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
    OF03time

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    // 下面几行中的 .timeName(), .C() and .Cf() 都是对象的[方法].
	// mesh.C() 和 .Cf() 返回描述cell和内部面中心的向量场
	// 使用mesh.C().size()方法获取网格大小
    Info << "你好，最新的时间文件夹是 " << runTime.timeName() << nl
         << "网格有 " << mesh.C().size() << " 个cell和 " << mesh.Cf().size()
         << " 个内部面. Wubalubadubdub!" << nl << endl;

    // It's possible to iterate over every cell in a standard C++ loop (not recommend)
    for (label cellI = 0; cellI < mesh.C().size(); cellI++)
        if (cellI%20 == 0) // only show every twentieth cell not to spam the screen too much
            Info << "Cell " << cellI << " with centre at " << mesh.C()[cellI] << endl;
    Info << endl; // spacer

    // Each cell is constructed of faces, which can either be internal faces, or 
    // form a boundary, or a patch in OF terms. 
    // Internal faces have an owner and a neighour 
    for (label faceI = 0; faceI < mesh.owner().size(); faceI++)
        if (faceI%100 == 0)
            Info << "Internal face " << faceI << " with centre at " << mesh.Cf()[faceI]
                 << " with owner cell " << mesh.owner()[faceI]
                 << " and neighbour " << mesh.neighbour()[faceI] << endl;
    Info << endl;

    // Boundary contions are accessed using boungaryMesh object
    // In fact, all the bounary faces are included in constant/polymesh/faces,
    // but in that file, the internal faces are defined first.
    // Besides, the starting faceI indices are defined in constant/polyMesh/boundary file
    // from which boundary face definitions start.
    // OF provides a macro definition 'forAll' for for loops over all entries
    // in a field or a list, saving typing.
    forAll(mesh.boundaryMesh(), patchI)
        Info << "Patch[ " << patchI << " ]: " << mesh.boundary()[patchI].name() << " with "
             << mesh.boundary()[patchI].Cf().size() << " faces. Starts at total face "
             << mesh.boundary()[patchI].start() << endl;
        Info << endl;

    // Faces adjacent to boundaries are acessed as follows.
    // label patchFaceI(0);
    // forAll(mesh.boundaryMesh(), patchI)
    //     Info << "Patch " << patchI << " has its face " << patchFaceI << " adjacent to cell "
    //          << mesh.boundary()[patchI].patch().faceCells()[patchFaceI]
    //          << ". It has normal vector " << mesh.boundary()[patchI].Sf()[patchFaceI] // 法向量，模为面积
    //          << " and surface area " << mag(mesh.boundary()[patchI].Sf()[patchFaceI]) // mag计算模
    //          << endl;
    // Info << endl;

    // 对于内部面，方法.Sf()可以用来直接被mesh对象调用
    // 此外还有一个捷径方法.magSf()来返回一个标量，代表表面面积
    // 内部面的法向量由owner指向neighbour，并且owner的下标比neighbour小
    // 对边界面来说，法向量总是指向计算域之外，他有一个虚拟的neighbour

    // Look at how points to make up each face
    const faceList& fcs = mesh.faces();
    const List<point>& pts = mesh.points();
    const List<point>& cents = mesh.faceCentres();
    // These const varibles are defined as references to some respective objects in the mesh 别名

    forAll(fcs, faceI) // for all faceI in fcs
        if (faceI%80 == 0) // faces that are multiples of 80
        {
            if(faceI<mesh.Cf().size())
                Info << "内部面 ";
            else
            {
                forAll(mesh.boundary(), patchI) // for all patches in boundary
                    // See if faceI is in patchI one by one
                    if ((mesh.boundary()[patchI].start() <= faceI) && 
                         (faceI < mesh.boundary()[patchI].start()+mesh.boundary()[patchI].Cf().size()))
                    {
                        Info << "非内部面在patch " << patchI << ", faceI ";
                        break;
                    }
            }

            Info << faceI << " 中心坐标在 " << cents[faceI]
                 << " 有 " << fcs[faceI].size() << " 顶点：";
            forAll(fcs[faceI], vertexI)
            // Note how fcs[faceI] holds the indices of points whose coordinates
            // are stored in the pts list.
                Info << " " << pts[fcs[faceI][vertexI]];
            Info << endl;
        }
    Info << endl;

    // 原始的cavity算例中，前后面定义为empty，这是一个特殊的边界条件。他的.CF()场大小为零
    label patchID(0);
    const polyPatch& pp = mesh.boundaryMesh()[patchID]; // pp是boundary 0的引用
    if (isA<emptyPolyPatch>(pp)) // pp不是emmpty（是moving wall），因此后面不会打印
    {
        // patch patchID is of type "empty".
        Info << "You will not see this." << endl;
    }

    // 通过patch的名字获取其信息
    word patchName("movingWall");// 要找的patch: movingWall
    patchID = mesh.boundaryMesh().findPatchID(patchName);
    Info << "取回patch " << patchName << " 在下标 " << patchID << " ，只是使用它的名字" << nl << endl;

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
