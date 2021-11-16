#pragma once

#include<vector>

using namespace std;

enum CONTROL { UPDOWN, UP, CONST };
enum VNEXT { VN, VNCAP, VNÑOR };

class rkm_method {
private:
	vector<double (*)(double, double, double)> f;

	double x0, u0, ud0;
	double h0;
	int Nmax;
	double b, Egr, E, Emin;

	CONTROL control;
	VNEXT vnext;

	bool isLinear[2];
	int p;

	bool isCheck(double xn, double vn0, double vn1, double h);

public:
	rkm_method(double (*_f1)(double, double, double), bool _linear1, double (*_f2)(double, double, double), bool _linear2, double _x0 = 0.0,
		double _u0 = 0.0, double _ud0 = 0.0, double _h0 = 0.001, int _Nmax = 10000, double _b = 1.0,
		double _Egr = 0.000001, double _E = 0.00000001, double _Emin = 0.0000000001, CONTROL _control = UPDOWN, VNEXT _vnext = VN);

	void setFunc(double (*_f1)(double, double, double), bool _linear1, double (*_f2)(double, double, double), bool _linear2);
	void setSC(double _x0, double _u0, double _ud0);
	void setH0(double _h0);
	void setControl(CONTROL _control, VNEXT _vnext, int _Nmax, double _b, double _Egr, double _E = 0.0, double _Emin = 0.0);

	int getP();

	void solve(vector<double>& X, vector<double>& H, vector<vector<double>>& V, vector<vector<double>>& V_cap,
		vector<double>& OLP, vector<vector<double>>& V_res, vector<int>& step_dec = vector<int>(), vector<int>& step_inc = vector<int>());
};