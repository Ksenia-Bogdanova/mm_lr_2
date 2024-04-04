#pragma once
#include <fstream>
//---------------------------------------------------------------------------

#include "linalg.h"
//------------------------------------------------------------------------
//������ ����������

class TGravityModel
{
protected:
	double rad;
	const double mu = 398600.4418e9;
	const double ae = 6378136;
	const double OMEGA = 7.292115e-5;
	TVector G; //�������� ��������� �� ��� x, y, z
public:
	virtual ~TGravityModel() {}
	virtual TVector getAcceleration(TVector X, double t) = 0;
};

//--------------------------------------------------------------------------------
//������ ������������ ��������������� ���� �����

class TCentralGravityModel : public TGravityModel
{
public:
	TVector getAcceleration(TVector X, double t) override;
};

