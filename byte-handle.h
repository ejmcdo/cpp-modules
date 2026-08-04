#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <iostream>
struct bs {
    std::string stream;
    std::string bl;
    bs() {};
    bs(std::string s) : stream(s) {}
    void lInt(int v, int l) {
        if (abs(v)) {
            int sub;
            if (l == -1)
                sub = int(128 * (v < 0) + ceil(log(abs(v)) / log(256)));
            else
                sub = 128 * (v < 0) + l;
            stream += sub;
            for (int i = 0; i < (sub % 128); i++) {
                stream += static_cast<unsigned char>(int(abs(v) / pow(256, (sub % 128) - i - 1)) % 256);
            }

        }
        else
            stream += char(0);
    }
    void byte(unsigned char x) {
        stream += x;
    }
    void bits(int n, int nb) {
        for (int i = 0; i < nb; i++) {
            bl += int(n / pow(2, nb - i - 1)) % 2;
            if (bl.size() == 8) {
                int sub = 0;
                for (int j = 0; j < 8; j++) {
                    sub += int(int(bl[j]) * pow(2, 7 - j));
                }
                byte(sub);
                bl = "";
            }
        }
    }
    void doub(double v) {
        bool close0 = (log10(abs((int(abs(v)) - abs(v)))) < -6);
        bool close1 = (log10(abs((int(abs(v)) - abs(v)) - 1)) < -6);
        if (int(v)) {
            int sub3 = int(abs(v));
            if (close1)
                sub3 += 1;
            int sub4 = int(ceil(log(sub3) / log(256)));
            if (sub4 == log(sub3) / log(256))
                sub4++;
            int sub = int(128 * (!close0&&!close1) + 64 * (v < 0) + sub4);
            stream += sub;
            for (int i = 0; i < (sub % 64); i++) {
                stream += static_cast<unsigned char>(int(abs(v) / pow(256, (sub % 64) - 1 - i)) % 256);
            }
        }
        else
            stream += static_cast<unsigned char>(128 * (!close0&&!close1) + 64 * (v < 0));
        if (!close0&&!close1) {
            double sub2 = (abs(v) - int(abs(v))) * pow(2, 32);
            for (int i = 0; i < 4; i++) {
                stream += static_cast<unsigned char>(static_cast<unsigned int>(sub2 / pow(256, 3 - i)) % 256);
            }
        }
    }
    void fill() {
        if (bl.size()) {
            while (bl.size() < 8) {
                bl += char(0);
            }
            int sub = 0;
            for (int j = 0; j < 8; j++) {
                sub += int(int(bl[j]) * pow(2, 7 - j));
            }
            byte(sub);
            bl = "";
        }
    }
    void rawString(std::string s){
        stream += s;
        stream += char(0);
    }
};
struct par {
    std::string stream;
    std::string bl;
    int bc = 0;
    int count = 0;
    par() {}
    par(std::string s) : stream(s) {}
    int lInt() {
        int l = byte();
        int final = 0;
        for (int i = 0; i < l % 128; i++) {
            final += int(byte() * pow(256, (l % 128) - 1 - i));
        }
        return final;
    }
    int byte() {
        count++;
        return int(static_cast<unsigned char>(stream[count - 1]));
    }
    int bits(int nb) {
        int sub = int(static_cast<unsigned char>(stream[count]));
        int final = 0;
        for (int i = 0; i < nb; i++) {
            final += (int(sub / pow(2, 7 - bc)) % 2) * int(pow(2, nb - i - 1));
            bc++;
            if (bc == 8) {
                bc = 0;
                count++;
                sub = int(static_cast<unsigned char>(stream[count]));
            }
        }
        return final;
    }
    double doub() {
        int sub = byte();
        int wnLen = sub % 64;
        bool isFloat = int(sub / 128);
        bool isNeg = int(sub / 64) % 2;
        double final = 0;
        for (int i = 0; i < wnLen; i++) {
            final += byte() * pow(256, wnLen - i - 1);
        }
        if (isFloat) {
            unsigned int sub2 = 0;
            for (int i = 0; i < 4; i++) {
                sub2 += static_cast<unsigned int>(byte() * pow(256, 3 - i));
            }
            final += sub2 / pow(2, 32);
        }
        return final * pow(-1, isNeg);
    }
    void pass() {
        if (bc) {
            bc = 0;
            count++;
        }
    }
    std::string rawString(){
        std::string final = "";
        while(stream[count] != char(0)){
            final += stream[count];
            count++;
        }
        count++;
        return final;
    }
};
bs encCurveInd(bs st, std::vector<int> ind) {
    bs final = st;
    final.byte(int(ind[0] / 256));
    final.byte(ind[0] % 256);
    final.byte(ind[1]);
    return final;
}
std::vector<int> decCurveInd(par p) {
    int sub = p.byte() * 256;
    sub += p.byte();
    std::vector<int> final;
    final.push_back(sub);
    final.push_back(p.byte());
    return final;
}
void printBits(unsigned char x) {
    for (int i = 0; i < 8; i++) {
        std::cout << int(x / pow(2, 7 - i)) % 2;
    }
}
void printStringInt(std::string x, int f) {
    switch (f) {
    case 0:
        for (unsigned int i = 0; i < x.size(); i++) {
            std::cout << int(static_cast<unsigned char>(x[i])) << " ";
        }
        break;
    case 1:
        for (unsigned int i = 0; i < x.size(); i++) {
            printBits(x[i]);
            std::cout << " ";
        }
        break;
    }
}