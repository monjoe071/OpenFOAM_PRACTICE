// Definiiton of the custom function
scalar calculatePressure(scalar t, vector x, vector x0, scalar scale)
{
    scalar r(mag(x - x0) / scale); // create a scalar object r and initialize

    scalar rR(1.0 / (r + 1e-12));

    scalar f(1.0);

    return Foam::sin(2.0 * Foam::constant::mathematical::pi * f * t) * rR;

}