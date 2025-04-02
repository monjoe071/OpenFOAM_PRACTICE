#include "computeVelocityPressure.H"

scalar computeR(const fvMesh& mesh, volScalarField& r, dimensionedVector x0)
{
    r = mag(mesh.C() - x0);

    return max(r).value();
}

volScalarField computePressure
(
    const fvMesh& mesh,
    scalar t,
    dimensionedVector x0,
    scalar scale,
    scalar f
)
{
    volScalarField r(mag(mesh.C()-x0)/scale);

    volScalarField rR(1.0 / (r + dimensionedScalar("tmp", dimLength, 1e-12)));

    return Foam::volScalarField(Foam::sin(2.0*Foam::constant::mathematical::pi*f*t)*rR*dimensionedScalar("tmp", dimensionSet(0, 3, -2, 0, 0),1.0));
}

void computeVelocity(const fvMesh& mesh, volVectorField& U, word pName)
{
    const volScalarField& pField = mesh.lookupObject<volScalarField>(pName);

    U = fvc::grad(pField) * dimensionedScalar("tmp", dimTime, 1.0);
}