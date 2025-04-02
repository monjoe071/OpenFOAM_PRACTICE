#include "fvCFD.H"

scalar computeR(const fvMesh& mesh, volScalarField& r, dimensionedVector x0);

volScalarField computePressure(const fvMesh& mesh, scalar t, dimensionedVector x0, scalar scale, scalar f);

void computeVelocity(const fvMesh& mesh, volVectorField& U, word pName = "p");