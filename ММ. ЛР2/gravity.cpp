//---------------------------------------------------------

#include "gravity.h"
#include <math.h>
#include <iostream>
using namespace std;
//----------------------------------------------------------
//Центральное ГПЗ

TVector TCentralGravityModel::getAcceleration(TVector X, double t)
{
	rad = sqrt(pow(X[0], 2) + pow(X[1], 2) + pow(X[2], 2));
	G.resize(3);
	G[0] = -mu * X[0] / (pow(rad, 3));
	G[1] = -mu * X[1] / (pow(rad, 3));
	G[2] = -mu * X[2] / (pow(rad, 3));
	return G;
}
