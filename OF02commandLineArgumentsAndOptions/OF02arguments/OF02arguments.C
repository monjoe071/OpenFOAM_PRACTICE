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
    OF02arguments

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Demonstrates how to handle command line options. \n\n"
        "Application arguments:\n"
        "----------------------\n"
        "       mathLib - Eigen/GSLArmodillo/BLAS\n"
        "       level   - computation speedup\n"
    );

    // 应用参数要在 setRootCase.H 构造 args 之前
    argList::validArgs.append("mathLib");
    argList::validArgs.append("level");
    
    // 应用选项也要在 setRootCase.H 构造 args 之前
    argList::addOption
    (
        "dict",
        "word",
        "Use addtional dictionary (just for example)"
    );

    argList::addOption
    (
        "nPrecision",
        "label",
        "Set the precision level (just for example)"
    );

    argList::addBoolOption
    (
        "log",
        "output the log"
    );

    #include "setRootCase.H" // 构造OpenFOAM的argList类型的args对象
    #include "createTime.H"

    #include "createMesh.H"

    
    const word args1 = args[1];
    const scalar args2 = args.argRead<scalar>(2); 

    Info << "Solver setup: " << nl
         << "    use     : " << args1 << nl
         << "    speedup : " << args2 << nl
         << nl << endl;

    // 创建文件设置默认路径（初始值）
    fileName _dict("./system/myDict");
     
    if (args.optionFound("dict"))
    {
        args.optionReadIfPresent("dict", _dict);
        Info << "Reading myDict " << endl;
    }
    Info << "Dictionary from " << _dict << nl << endl;

    // 创建label类型的对象并给默认初始值
    label _nPrecision(6);
    args.optionReadIfPresent("nPrecision", _nPrecision);
    Info << "Precision is " << _nPrecision << nl << endl;

    const bool _log = args.optionFound("log");
    if (_log)
    {
        Info << "Output the logs" << endl;
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
