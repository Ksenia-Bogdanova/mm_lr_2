#include "integrator.h"
#include "custom.h"
#include "ephaccess.h"
#include "pattern.h"
#include<iostream>
#include <iomanip>

using namespace std;

double measure_julian_date(double year, double month, double day, double hour, double minute, double second)
{
	//double year = 2023, month = 3, day = 21, hour = 0, minute = 0, second = 0;
	int M, Y, a;
	a = (14 - month) / 12;
	Y = year + 4800 - a;
	M = month + 12 * a - 3;
	double JDN;
	JDN = day + (153 * M + 2) / 5 + 365 * Y + Y / 4 - Y / 100 + Y / 400 - 32045;
	double JD = JDN + (hour - 12) / 24 + minute / 1440 + second / 86400;
	return JD;
}

int main()
{

	/*int _object;
	int _reference;

	EphAccess* eph = ephCreate();

	ephLoadFile(eph, "epm2021.bsp");
	_object = ephObjectByName("moon");
	_reference = ephObjectByName("earth");*/

	
	TVector X(3);


	// Модель центрального гравитационного поля
	TGravityModel* mod = new TCentralGravityModel;
	
	// Модель гравитационного поля с учетом влияния только луны
	TGravityModel* decorator1 = new MoonDecorator(mod);

	// Модель гравитационного поля с учетом влияния только луны
	TGravityModel* decorator2 = new SunDecorator(mod);

	// Модель гравитационного поля с учетом влияния луны и солнца
	TGravityModel* decorator3 = new SunDecorator(decorator1);

	double t0 = measure_julian_date(2024, 1, 1, 0, 0, 0) * 86400.0;
	double t_end = measure_julian_date(2024, 1, 1, 3, 0, 0) * 86400.0;
	
	TIntegrator* Integrator = new TDormandPrinceIntegrator();
	Integrator->setPrecision(1e-19);
	TSatellite* Model1 = new TSatellite(t0, t_end, 60, mod);
	TSatellite* Model2 = new TSatellite(t0, t_end, 60, decorator1);
	TSatellite* Model3 = new TSatellite(t0, t_end, 60, decorator2);
	TSatellite* Model4 = new TSatellite(t0, t_end, 60, decorator3);
	Integrator->Run(Model1);
	Integrator->Run(Model2);
	Integrator->Run(Model3);
	Integrator->Run(Model4);
	TMatrix Result = Model1->getResult();
	TMatrix Result1 = Model2->getResult();
	TMatrix Result2 = Model3->getResult();
	TMatrix Result3 = Model4->getResult();

	string mas = "gr1.txt";
	string mas1 = "gr2.txt";
	string mas2 = "gr3.txt";
	string mas3 = "gr4.txt";

	Result.go_to_file(mas);
	Result1.go_to_file(mas1);
	Result2.go_to_file(mas2);
	Result3.go_to_file(mas3);

	delete Model1;
	delete Model2;
	delete Model3;
	delete Model4;
	delete mod;
	delete decorator1;
	delete decorator2;
	delete decorator3;


	/*TIntegrator* Integrator = new TDormandPrinceIntegrator();
	Integrator->setPrecision(1e-19);
  	TSatellite* Model1 = new TSatellite(1e3, 20e3, 60, mod, measure_julian_date());

	Integrator->Run(Model1);

	TMatrix Result = Model1->getResult();

	string mas = "graf1.txt";

	Result.go_to_file(mas);


	delete Model1;

	delete mod;*/

	return 0;
}