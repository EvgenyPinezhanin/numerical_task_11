#include"numerical.h"

rkm_method::rkm_method(double (*_f1)(double, double, double), bool _linear1, double (*_f2)(double, double, double), bool _linear2,
    double _x0, double _u0, double _ud0, double _h0, int _Nmax, double _b, double _Egr, double _E, double _Emin, CONTROL _control, VNEXT _vnext)
    : f(2), x0(_x0), u0(_u0), h0(_h0), Nmax(_Nmax), b(_b), Egr(_Egr), E(_E), Emin(_Emin), control(_control), vnext(_vnext) {
    f[0] = _f1;
    isLinear[0] = _linear1;
    f[1] = _f2;
    isLinear[1] = _linear2;
    if (isLinear[0] && isLinear[1]) {
        p = 4;
    } else {
        p = 3;
    }
}

void rkm_method::setFunc(double (*_f1)(double, double, double), bool _linear1, double (*_f2)(double, double, double), bool _linear2) {
    f[0] = _f1;
    isLinear[0] = _linear1;
    f[1] = _f2;
    isLinear[1] = _linear2;
    if (isLinear[0] && isLinear[1]) {
        p = 4;
    } else {
        p = 3;
    }
}
void rkm_method::setSC(double _x0, double _u0, double _ud0) {
    x0 = _x0;
    u0 = _u0;
    ud0 = _ud0;
}
void rkm_method::setH0(double _h0) {
    h0 = _h0;
}
void rkm_method::setControl(CONTROL _control, VNEXT _vnext, int _Nmax, double _b, double _Egr, double _E, double _Emin) {
    Nmax = _Nmax;
    b = _b;
    Egr = _Egr;
    E = _E;
    Emin = _Emin;
    control = _control;
    vnext = _vnext;
}

int rkm_method::getP() {
    return p;
}

void rkm_method::solve(vector<double>& X, vector<double>& H, vector<vector<double>>& V, vector<vector<double>>& V_cap,
    vector<double>& OLP, vector<vector<double>>& V_res, vector<int>& step_dec, vector<int>& step_inc) {
    X.clear(); H.clear();
    V.clear(); V.resize(2);
    V_cap.clear(); V_cap.resize(2);
    OLP.clear();
    V_res.clear(); V_res.resize(2);
    step_dec.clear(); step_inc.clear();

    vector<vector<double>> k(5);
    for (int i = 0; i < 5; i++) {
        k[i].resize(2);
    }
    double S[2];
    double Sabs;
    int N = 0;

    double xN = x0, xN_1;
    X.push_back(xN);
    double vN[2] = { u0, ud0 };
    V[0].push_back(vN[0]);
    V[1].push_back(vN[1]);
    double vN_1[2];
    double hN = h0;
    H.push_back(hN);
    double vCapN_1[2];
    double vCorN_1[2];
    int step_d = 0, step_i = 0;
    while (true) {
        xN_1 = xN + hN;
        if (xN_1 > b) {
            hN -= xN_1 - (b - Egr / 2.0);
            H[N] = hN;
            step_d++;
            continue;
        }

        for (int i = 0; i < 2; i++) {
            k[0][i] = f[i](xN, vN[0], vN[1]);
        }
        for (int i = 0; i < 2; i++) {
            k[1][i] = f[i](xN + hN / 3.0, vN[0] + hN / 3.0 * k[0][0], 
                                          vN[1] + hN / 3.0 * k[0][1]);
        }
        for (int i = 0; i < 2; i++) {
            k[2][i] = f[i](xN + hN / 3.0, vN[0] + hN / 6.0 * k[0][0] + hN / 6.0 * k[1][0],
                                          vN[1] + hN / 6.0 * k[0][1] + hN / 6.0 * k[1][1]);
        }
        for (int i = 0; i < 2; i++) {
            k[3][i] = f[i](xN + hN / 2.0, vN[0] + hN / 8.0 * k[0][0] + 3.0 * hN / 8.0 * k[2][0],
                                          vN[1] + hN / 8.0 * k[0][1] + 3.0 * hN / 8.0 * k[2][1]);
        }
        for (int i = 0; i < 2; i++) {
            k[4][i] = f[i](xN_1, vN[0] + hN / 2.0 * k[0][0] - 3.0 * hN / 2.0 * k[2][0] + 2.0 * hN * k[3][0],
                                 vN[1] + hN / 2.0 * k[0][1] - 3.0 * hN / 2.0 * k[2][1] + 2.0 * hN * k[3][1]);
        }

        for (int i = 0; i < 2; i++) {
            vCapN_1[i] = vN[i] + hN / 6.0 * (k[0][i] + 4.0 * k[3][i] + k[4][i]);
        }
        for (int i = 0; i < 2; i++) {
            vN_1[i] = vN[i] + hN / 10.0 * (k[0][i] + 3.0 * k[2][i] + 4.0 * k[3][i] + 2.0 * k[4][i]);
        }

        for (int i = 0; i < 2; i++) {
            S[i] = hN / 30.0 * (2.0 * k[0][i] - 9.0 * k[2][i] + 8.0 * k[3][i] - k[4][i]);
        }
        for (int i = 0; i < 2; i++) {
            vCorN_1[i] = vN_1[i] + S[i];
        }

        Sabs = max(abs(S[0]), abs(S[1]));

        if (control != CONST) {
            if (Sabs > E) {
                hN /= 2.0;
                H[N] = hN;
                step_d++;
                continue;
            }
            if (control == UPDOWN) {
                if (Sabs < Emin) {
                    hN *= 2.0;
                    step_i++;
                }
            }
        }

        X.push_back(xN_1); xN = xN_1;
        V[0].push_back(vN_1[0]);
        V[1].push_back(vN_1[1]);
        H.push_back(hN);
        V_cap[0].push_back(vCapN_1[0]);
        V_cap[1].push_back(vCapN_1[1]);
        OLP.push_back(Sabs);
        step_dec.push_back(step_d); step_d = 0;
        step_inc.push_back(step_i); step_i = 0;
        if (vnext == VN) {
            V_res[0].push_back(vN_1[0]); vN[0] = vN_1[0];
            V_res[1].push_back(vN_1[1]); vN[1] = vN_1[1];
        }
        else if (vnext == VNCAP) {
            V_res[0].push_back(vCapN_1[0]); vN[0] = vCapN_1[0];
            V_res[1].push_back(vCapN_1[1]); vN[1] = vCapN_1[1];
        }
        else if (vnext == VNÑOR) {
            V_res[0].push_back(vCorN_1[0]); vN[0] = vCorN_1[0];
            V_res[1].push_back(vCorN_1[1]); vN[1] = vCorN_1[1];
        }

        if (xN_1 >= b - Egr) {
            N++;
            break;
        }
        N++;
        if (N == Nmax) break;
    }
}