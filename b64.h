/*
* base64 Encoder/Decoder made from scratch.
*/
#pragma once
#include <string>

// Alphabet for base64 encodings/decodings.
std::string b64alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// getLetter - Reverse mapping for decoding. Takes a character in b64 and returns its associated value.
int getLetter(unsigned char x) {
    if (int(x) >= 65&&int(x) <= 90)
        return int(x) - 65;
    else if (int(x) >= 97&&int(x) <= 122)
        return int(x) - 71;
    else if (int(x) >= 48&&int(x) <= 57)
        return int(x) + 4;
    else if (int(x) == 45)
        return 62;
    else if (int(x) == 95)
        return 63;
    else if (int(x) == 61)
        return 0;
    else
        return -1;
}

// b64enc - Takes a string and encodes it in b64.
std::string b64enc(std::string st) {
    std::string final;
    int sub = 0;
    for (unsigned int i = 0; i < st.size(); i += 3) {
        sub = 0;
        if (i + 3 < st.size()) {
            for (int j = 0; j < 3; j++)
                sub += int(pow(256, 2 - j)) * int(static_cast<unsigned char>(st[i + j]));
            for (int j = 0; j < 4; j++) {
                final += b64alp[int(sub / pow(64, 3 - j)) % 64];
            }
        }
        else {
            for (int j = 0; j < 3; j++) {
                if (i + j < st.size())
                    sub += int(pow(256, 2 - j)) * int(static_cast<unsigned char>(st[i + j]));
            }
            for (unsigned int j = 0; j < 4; j++) {
                if (j < 1 + (st.size() % 3) || st.size() % 3 == 0)
                    final += b64alp[int(sub / pow(64, 3 - j)) % 64];
                else
                    final += "=";
            }
        }
    }
    return final;
}

// b64dec - Takes a string encoded in b64 and decodes it.
std::string b64dec(std::string st) {
    std::string final = "";
    int count = 0;
    while (st[st.size() - count - 1] == '=')
        count++;
    int sub = 0;
    for (unsigned int i = 0; i < st.size(); i += 4) {
        if (i + 4 <= st.size()) {
            sub = 0;
            for (int j = 0; j < 4; j++) {
                sub += getLetter(static_cast<unsigned char>(st[i + j])) * int(pow(64, 3 - j));
            }
            if (i < st.size() - 4) {
                for (int j = 0; j < 3; j++) {
                    final += int(sub / pow(256, 2 - j)) % 256;
                }
            }
            else {
                for (int j = 0; j < 3; j++) {
                    if (j < 3 - count)
                        final += int(sub / pow(256, 2 - j)) % 256;
                }
            }
        }
    }
    return final;
}

