//---------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include "custom.h"
#include <math.h>
#include <iostream>

TSatellite::TSatellite() : gm(NULL) {};

TSatellite::TSatellite( long double t0, long double t1, long double SamplingIncrement, TGravityModel* t)
{
	OMEGA = OMEGA * M_PI / 180;
	i = i * M_PI / 180;

	long double cos_i{};

	if (abs(i - M_PI / 2.0) < 1e-12)
		cos_i = 0.0;
	else
		cos_i = cos(i);

	omega = omega * M_PI / 180;
	v = v * M_PI / 180;
	this->t0 = t0;
	this->t1 = t1;
	this->SamplingIncrement = SamplingIncrement;
	X0.resize(6);
	A.resize(3, 3);
	double u = v + omega;
	A(0, 0) = cos(u) * cos(OMEGA) - sin(u) * sin(OMEGA) * cos_i;
	A(0, 1) = -sin(u) * cos(OMEGA) - cos(u) * sin(OMEGA) * cos_i;
	A(0, 2) = sin(i) * sin(OMEGA);
	A(1, 0) = cos(u) * sin(OMEGA) + sin(u) * cos(OMEGA) * cos_i;
	A(1, 1) = -sin(u) * sin(OMEGA) + cos(u) * cos(OMEGA) * cos_i;
	A(1, 2) = -sin(i) * cos(OMEGA);
	A(2, 0) = sin(u) * sin(i);
	A(2, 1) = cos(u) * sin(i);
	A(2, 2) = cos_i;

	double p = a * (1 - e * e);
	TVector X, V;
	X.resize(3);
	V.resize(3);
	X[0] = p / (1 + e * cos(v));
	X[1] = 0;
	X[2] = 0;
	V[0] = sqrt(398600.4418e9 / p) * e * sin(v);
	V[1] = sqrt(398600.4418e9 / p) * (1 + e * cos(v));
	V[2] = 0;
	X = A * X;
	V = A * V;
	X0[0] = X[0];
	X0[1] = X[1];
	X0[2] = X[2];
	X0[3] = V[0];
	X0[4] = V[1];
	X0[5] = V[2];
	gm = t;
}

TVector TSatellite::getRight(const TVector& X, long double t)
{
	TVector Y;
	Y.resize(6);
	Y[0] = X[3];
	Y[1] = X[4];
	Y[2] = X[5];
	//std::cout << X[0] << " " << X[1] << " " << X[2] << std::endl;
	TVector g = gm->getAcceleration(X, t);
	Y[3] = g[0];
	Y[4] = g[1];
	Y[5] = g[2];
	return Y;
}

