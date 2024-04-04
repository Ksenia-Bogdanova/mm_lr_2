#pragma once
#include <fstream>
//---------------------------------------------------------------------------

#include "linalg.h"
//------------------------------------------------------------------------
//Модель гравитации

class TGravityModel
{
protected:
	double rad;
	const double mu = 398600.4418e9;
	const double ae = 6378136;
	const double OMEGA = 7.292115e-5;
	TVector G; //проекции ускорения на оси x, y, z
public:
	virtual ~TGravityModel() {}
	virtual TVector getAcceleration(TVector X, double t) = 0;
};

//--------------------------------------------------------------------------------
//Модель центрального гравитационного поля Земли

class TCentralGravityModel : public TGravityModel
{
public:
	TVector getAcceleration(TVector X, double t) override;
};

