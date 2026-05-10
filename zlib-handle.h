/*
* Simple zlib compression/decompression module that I finally got to work after some 20+ hours of bugfixing.
* I've been spoiled by Python. Credit to the original creators or something.
*/
#include <string>
#include <math.h>

#include <zlib/uncompr.c>
#include <zlib/inflate.c>
#include <zlib/inftrees.c>
#include <zlib/inffast.c>

#include <zlib/adler32.c>
#include <zlib/compress.c>
#include <zlib/crc32.c>
#include <zlib/deflate.h>
#include <zlib/deflate.c>
#include <zlib/trees.c>
#include <zlib/zlib.h>
#include <zlib/zutil.h>
#include <zlib/zutil.c>

/*
* zlibComp - Takes a string and compresses it using zlib. Uses all default settings and will probably stay that way forever. Sorry not sorry.
*/
std::string zlibComp(std::string x) {
    Bytef* source = new unsigned char[x.size()];
    for (unsigned int i = 0; i < x.size(); i++) {
        *(source + i) = x[i];
    }
    int boundOff = 0;
    uLongf destSize;
    Bytef* dest;
    int compTest = -1;
    while (compTest != 0) {
        destSize = compressBound(uLong(x.size())) + boundOff;
        dest = new unsigned char[destSize];
        compTest = compress(dest, &destSize, source, uLong(x.size()));
        boundOff++;
    }
    std::string final;
    for (unsigned int i = 0; i < destSize; i++) {
        final += (*(dest + i));
    }
    return final;
}

/*
* zlibComp - Takes a compressed string and decompresses it using zlib. Uses all default settings and will also probably stay that way forever.
*/
std::string zlibUncomp(std::string x) {
    Bytef* source = new unsigned char[x.size()];
    uLongf destLen = 0;
    int compTest = -1;
    int boundOff = 0;
    Bytef* dest = new unsigned char[destLen];
    while (compTest < 0) {
        for (unsigned int i = 0; i < (x.size() - boundOff); i++) {
            if (i >= 0 and i <= (x.size() - boundOff))
                *(source + (i - 0)) = x[i];
            if (i >= (x.size() - boundOff) - 4){
                unsigned char s=x[i];
                destLen += int(s) * int(pow(256, i - (x.size() - boundOff) + 4));
            }
        }
        dest = new unsigned char[destLen];
        compTest = uncompress(dest, &destLen, source, uLong((x.size() - boundOff)));
        boundOff++;
    }
    std::string final;
    for (unsigned int i = 0; i < destLen; i++) {
        final += *(dest + i);
    }
    return final;
}
