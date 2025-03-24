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
    // #include "createTime.H"
    Foam::Info<< "Create time\n" << Foam::endl;
    Foam::Time runTime(Foam::Time::controlDictName, args);
    // copied form createTime.H

    Info << "Case name    :    " << runTime.caseName() << endl;
    Info << "Root path    :    " << runTime.rootPath() << endl;
    Info << "Path         :    " << runTime.path() << endl;
    Info << "Time path    :    " << runTime.timePath() << endl;
    Info << "Controldict  :    " << runTime.controlDict() << endl;
    Info << "Format       :    " << runTime.writeFormat() << endl;
    Info << "Version      :    " << runTime.writeVersion() << endl;
    Info << "Start time   :    " << runTime.startTime() << endl;
    Info << "End time     :    " << runTime.endTime() << endl;
    Info << "Time step    :    " << runTime.deltaT() << endl;
    
    Info << "# set end time = 10" << endl;
    runTime.setEndTime(10);

    Info << "# set deltaT = 1" << endl;
    runTime.setDeltaT(1);

    Info << "Start time    :    " << runTime.startTime() << endl;
    Info << "End time      :    " << runTime.endTime() << endl;
    Info << "Time tep      :    " << runTime.deltaT() << endl;

    while (runTime.loop())
    {
        Info << "Time: " << runTime.timeName() << endl;
        runTime.write();
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
