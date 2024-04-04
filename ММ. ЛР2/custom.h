#pragma once
//---------------------------------------------------------------------------

#ifndef customH
#define customH

#include "model.h"
#include "gravity.h"


//----------------------------------------------------------------------------
//Движение спутника

class TSatellite : public TModel
{
protected:
	TGravityModel* gm;
	double OMEGA = 0.0l;
	double i = 90.0l;
	double omega = 0.0l;
	double v = 0.0l;
	double a = 6'500'000;
	double e = 0.1l;
	TMatrix A;
	TVector R, V, R_, V_;
	double julian_date;
public:
	TSatellite();
	//TSatellite(double theta0, double w0, double omega0, double i0, double p0, double a0, double mu0, double e0);
	TSatellite( long double t0, long double t1, long double SamplingIncrement, TGravityModel* T);//TGravityModel* t);
	//TVector coordinates(double theta, double i, double omega, double a, double e, double v);
	TVector getRight(const TVector& X, long double t);
};
#endif