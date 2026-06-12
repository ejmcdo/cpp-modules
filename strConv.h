#pragma once
#include <string>
#include <math.h>
int decAcc = 3;
std::string doub2Str(double y) {
    if (!y)
        return "0";
    std::string final;
    if (y < 0)
        final += "-";
    double x = abs(y);
    int dPoint = int(log(x) / log(10));
    double sub = x / pow(10, int(log(x) / log(10)));
    if (sub == 10) {
        sub = 1;
        dPoint++;
    }
    if (dPoint < 0) {
        final += "0.";
        for (int i = 1; i < -dPoint; i++) {
            final += "0";
        }
    }
    int trailZero = 0;
    int decDig = 0;
    for (int i = 0; i < 15; i++) {
        if (i >= dPoint && int(sub)) {
            trailZero = int(final.size());
            decDig++;
        }
        final += char(int(sub) + 48);
        if (i == dPoint) {
            trailZero = int(final.size()) - 1;
            final += ".";
        }
        sub = (sub - int(sub)) * 10;
        if (decDig == decAcc)
            break;
    }
    std::string realFinal;
    for (int i = 0; i <= trailZero; i++) {
        realFinal += final[i];
    }
    return realFinal;
}
std::string int2Str(int y) {
    if (!y)
        return "0";
    std::string final;
    if (y < 0)
        final += "-";
    int x = abs(y);
    int len = -1;
    for (int i = 1; i < 10; i++) {
        if (x == pow(10, i))
            len = i;
    }
    if (len == -1)
        len = int(log(x) / log(10));
    for (int i = 0; i < len + 1; i++) {
        final += char((int(x / pow(10, len - i)) % 10) + 48);
    }
    return final;
}
int str2Int(std::string x) {
    int final = 0;
    if (x.size()) {
        bool neg = (x[0] == '-');
        for (unsigned int i = neg; i < x.size(); i++) {
            final += int(pow(10, x.size() - 1 - i)) * (int(x[i]) - 48);
        }
        final *= int(pow(-1, neg));
    }
    return final;
}
double str2Doub(std::string x) {
    double final = 0;
    int dpp = -1;
    if (x.size() > 0) {
        bool neg = (x[0] == '-');
        for (unsigned int i = neg; i < x.size(); i++) {
            if (x[i] != '.') {
                if (dpp == -1) {
                    final *= 10;
                    final += (int(x[i]) - 48);
                }
                else {
                    final += pow(10, int(dpp - i)) * (int(x[i]) - 48);
                }
            }
            else {
                dpp = i;
            }
        }
        final *= pow(-1, neg);
    }
    return final;
}
std::string intConvStr(int v, int n) {
    std::string final;
    for (int j = 0; j < n; j++)
        final += int(v / pow(256, n - j - 1)) % 256;
    return final;
}

