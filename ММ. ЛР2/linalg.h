#pragma once
#include <fstream>
#include <fstream>
#include <fstream>
#include <cstring>
// ����������� ����������
class TMatrix;
class TQuaternion;
// ���������� ������ ��������
class TVector {
protected:
	// ����������� �������
	int n;
	// �������� �������
	double* data;
public:
	// ����������� �� ���������
	TVector();
	// ����������� � �������� ���-��� ���������
	TVector(int n);
	// ����������� �����
	TVector(const TVector& rvalue);
	// �������� ������������
	TVector& operator = (const TVector& rvalue);
	// ����������
	virtual ~TVector();
	// ������� ��������� ���-�� ��������� �������
	inline int size() const { return n; }
	// ������� ��������� ������� ���������� ��������
	inline int high() const { return n - 1; }
	// ������� ������� ���-�� ��������� �������
	void resize(int n);
	// �������� ������� � ��������� �������
	inline double& operator[](int i) { return data[i]; }
	// �������� ������������ ������� � ��������� �������
	inline const double& operator[](int i) const { return data[i]; }
	// �������� - ������� �����
	TVector operator - () const;
	// �������� ��������� ��������
	TVector operator - (const TVector& arg) const;
	// �������� �������� ��������
	TVector operator + (const TVector& arg) const;
	// �������� ��������� ������� �� �����
	TVector operator * (double arg) const;
	// �������� ���������� ��������� ��������
	double operator * (const TVector& arg) const;
	// �������� ��������� ������� �� �������
	TVector operator * (const TMatrix& arg) const;
	// �������� ��������� ������� �� ����������
	//TQuaternion operator * (const TQuaternion& arg) const;
	// �������� ���������� ��������� ��������
	TVector operator ^ (const TVector& arg) const;
	// ������������� ������� - �������� ��������� ����� �� ������
	friend TVector operator * (double lvalue, const TVector& rvalue);
	// ������� ��������� ������ �������
	double length() const;
	// ������� ������������ �������
	TVector norm();
	void print();
	// ������� ������� ������ �������� ��� �� �������� ���� ��� ������ ������� �������
		//TVector rotateByRodrigFormula(double phi, const TVector& axis) const;
	// ������� ������� ������ �������� ��� �� �������� ���� ��� ������ �����������
		//TVector rotate(double phi, const TVector& axis) const;
	// ������� ������ ��� ������ ��������� �����������
	//TVector rotateByQuaternion(const TQuaternion& L) const;
};
// ���������� ������ ������
class TMatrix {
protected:
	// ����������� ������� (����� ����� � ��������)
	int n, m;
	// �������� �������
	double** data;
public:
	// ����������� �� ���������
	TMatrix();
	// ����������� � �������� ������������
	TMatrix(int n, int m);
	// ����������� �����
	TMatrix(const TMatrix& rvalue);
	// �������� ������������
	TMatrix& operator = (const TMatrix& rvalue);
	// ����������
	virtual ~TMatrix();
	// ������� ��������� ���������� �����
	inline int rowCount() const { return n; }
	// ������� ��������� ���-�� ��������
	inline int colCount() const { return m; }
	// ������� ��������� ������� ��������� ������
	inline int rowHigh() const { return n - 1; }
	// ������� ��������� ������� ���������� �������
	inline int colHigh() const { return m - 1; }
	// ������� ������� �����������
	void resize(int n, int m);
	// �������� ������� � ��������� �������
	inline double& operator()(int i, int j) { return data[i][j]; }
	// �������� ������������ ������� � ��������� �������
	inline const double& operator()(int i, int j) const { return data[i][j]; }
	// �������� - ������� �����
	TMatrix operator - () const;
	// �������� ��������� ������
	TMatrix operator - (const TMatrix& arg) const;
	// �������� �������� ������
	TMatrix operator + (const TMatrix& arg) const;
	// �������� ��������� ������� �� �����
	TMatrix operator * (double arg) const;
	// �������� ��������� ������
	TMatrix operator * (const TMatrix& arg) const;
	// �������� ��������� ������� �� ������
	TVector operator * (const TVector& arg) const;
	// ������������� ������� - �������� ��������� ����� �� �������
	friend TMatrix operator * (double lvalue, const TMatrix& rvalue);
	// �������� ��������� ������ (����� ������)
	TMatrix appeal() const;
	// ������� ���������� ������������
	double det() const;
	// ������� ����������������
	TMatrix t() const;
	// ������� ������������ ��������� �������
	static TMatrix E(int n);
	// ������� ������������ �����
	TMatrix swapRows(int i, int j, const TMatrix& arg);
	// ������� ����� �������
	void print();
	// ������� ���������� ����� � �������
	void push(const TVector& arg);
	void print_col(int number);
	void go_to_file(std::string name);
};
