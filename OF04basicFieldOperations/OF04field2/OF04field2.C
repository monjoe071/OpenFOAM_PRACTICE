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
    OF04field2

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// 函数声明：计算给定时间t、位置x、参考点和尺度因子的压力值
// 计算p，参数是参考点x0，位置x，时间t，尺度因子scale
scalar calculatePressure(scalar t, vector x, vector x0, scalar scale);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    #include "createFields4.H" // 读入场的所有参数，包括压力、速度、粘度

    const vector originVector(0.05, 0.05, 0.05); // 不变的参考点

    const dimensionedScalar rFarCell = max(
        mag(
            dimensionedVector("x0", dimLength, originVector) - mesh.C() // mesh.C()储存了网格的中心坐标场
        )
    );
    const scalar farCell = rFarCell.value(); // 有量纲距离转换到无量纲数
    Info << "有量纲长度: " << rFarCell << endl;
    Info << "无量纲长度: " << farCell << endl;

    // 当controlDict里要求开启循环，就执行下面的代码
    Info << "\n开始时间循环\n" << endl;

    while (runTime.loop())
    {
        Info << "时间 = " << runTime.timeName() << nl << endl;
        // 循环所有cell计算压力
        for (label cellI = 0; cellI<mesh.C().size(); cellI++)
        {
            // 调用之前声明的函数
            p[cellI] = calculatePressure(runTime.time().value(), mesh.C()[cellI], originVector, farCell);
        }
        // 用压力梯度冒充速度，由于压力梯度量纲不是m/s
        // 乘以单位时间来获取正确的量纲，不必要，只是为描述正确
        // tmp是给单位时间起的名字
        // p的单位是m^2/s^2, 梯度单位m/s^2
        U = fvc::grad(p)*dimensionedScalar("tmp", dimTime, 1.); //场操作fvc比循坏高效

        runTime.write();
    }

    Info << "结束！画出p的iso-contours范围(-10,10), 并对U使用glyph filter" << endl;



    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}

// 自定义函数定义
scalar calculatePressure(scalar t, vector x, vector x0, scalar scale)
{
    scalar r (mag(x - x0)/scale);
    scalar rR (1./(r+1e-12));
    scalar f (2.); //频率

    return Foam::sin(2.*Foam::constant::mathematical::pi*f*t)*rR;
}


// ************************************************************************* //
