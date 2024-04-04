#pragma once
#include "gravity.h"
#include <math.h>
#include "ephaccess.h"

//-----------------------------------------------------------------------
//Добавление внешних возмущений 

class Decorator : public TGravityModel
{
protected:
	TGravityModel* grav;
public:
	Decorator(TGravityModel* gravity) : grav(gravity) {}
	TVector getAcceleration(TVector X, double t) override
	{
		return this->grav->getAcceleration(X, t);
	}
};

//------------------------------------------------------------------------
//Влияние луны

class MoonDecorator : public Decorator
{
protected:
	//double rad1, rad2;
	double mu_moon = 4902.8001e9;
	int _object;
	int _reference;

public:
	MoonDecorator(TGravityModel* gravity) : Decorator(gravity) {}
	TVector getAcceleration(TVector X, double t) override
	{
		EphAccess* eph = ephCreate();

		ephLoadFile(eph, "epm2021.bsp");
		_object = ephObjectByName("moon");
		_reference = ephObjectByName("earth");

		double pos[3];
		ephCalculateRectangular(eph, _object, _reference, t / 86400.0, 0.0, pos, NULL);
		TVector moonX(3);
		moonX[0] = pos[0] * 1000; moonX[1] = pos[1] * 1000; moonX[2] = pos[2] * 1000;

		TVector Xpos(3);
		Xpos[0] = X[0]; Xpos[1] = X[1]; Xpos[2] = X[2];
		auto r = moonX - Xpos;

		TVector moon_grav(3);

		moon_grav[0] = mu_moon * (r[0] * 1 / pow(r.length(), 3)) - moonX[0] / pow(moonX.length(), 3);
		moon_grav[1] = mu_moon * (r[1] * 1 / pow(r.length(), 3)) - moonX[1] / pow(moonX.length(), 3);
		moon_grav[2] = mu_moon * (r[2] * 1 / pow(r.length(), 3)) - moonX[2] / pow(moonX.length(), 3);
		/*rad = sqrt(pow(X[0], 2) + pow(X[1], 2) + pow(X[2], 2));
		rad2 = sqrt(pow(X[0] - moonX[0], 2) + pow(X[1] - moonX[1], 2) + pow(X[2] - moonX[2], 2));
		rad1 = sqrt(pow(moonX[0], 2) + pow(moonX[1], 2) + pow(moonX[2], 2));
		moon_grav[0] = mu_moon * ((moonX[0] - X[0]) / pow(rad2, 3) - moonX[0] / pow(rad1, 3));
		moon_grav[1] = mu_moon * ((moonX[1] - X[1]) / pow(rad2, 3) - moonX[1] / pow(rad1, 3));
		moon_grav[2] = mu_moon * ((moonX[2] - X[2]) / pow(rad2, 3) - moonX[2] / pow(rad1, 3));*/
		return (Decorator::getAcceleration(X, t) + moon_grav);
	}
};

//-------------------------------------------------------------------------
//Влияние Солнца

class SunDecorator : public Decorator
{
protected:
	double mu_sun = 1.32718e20;
	int _object;
	int _reference;
public:
	SunDecorator(TGravityModel* gravity) : Decorator(gravity) {}
	TVector getAcceleration(TVector X, double t) override
	{
		EphAccess* eph = ephCreate();

		ephLoadFile(eph, "epm2021.bsp");
		_object = ephObjectByName("sun");
		_reference = ephObjectByName("earth");

		double pos[3];
		ephCalculateRectangular(eph, _object, _reference, t / 86400.0, 0.0, pos, NULL);

		TVector sunX(3);
		sunX[0] = pos[0] * 1000; sunX[1] = pos[1] * 1000; sunX[2] = pos[2] * 1000;

		TVector Xpos(3);
		Xpos[0] = X[0]; Xpos[1] = X[1]; Xpos[2] = X[2];
		auto r = sunX - Xpos;

		TVector sun_grav(3);
		sun_grav[0] = mu_sun * (r[0] * 1 / pow(r.length(), 3)) - sunX[0] / pow(sunX.length(), 3);
		sun_grav[1] = mu_sun * (r[1] * 1 / pow(r.length(), 3)) - sunX[1] / pow(sunX.length(), 3);
		sun_grav[2] = mu_sun * (r[2] * 1 / pow(r.length(), 3)) - sunX[2] / pow(sunX.length(), 3);
		
		return (Decorator::getAcceleration(X, t) + sun_grav);
	}
};