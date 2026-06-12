/*
* Simple zlib compression/decompression module that I finally got to work after some 20+ hours of bugfixing.
* I've been spoiled by Python. Credit to the original creators or something.
*/
#pragma once
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

// compTypes - There are only two methods of compression that I use often, one for .png images, and another for Geometry Dash save files(yes, I play that game).
enum compTypes{
    PNG,
    GD
};

/*
* zlibComp - Takes a string and compresses it using zlib. Type(either .png or gd save file) is specified.
*/
std::string zlibComp(std::string x, compTypes type) {
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
        if(type == PNG)
            compTest = compress(dest, &destSize, source, uLong(x.size()), int(type));
        if(type == GD)
            compTest = compress2(dest, &destSize, source, uLong(x.size()), 9, int(type));
        boundOff++;
    }
    std::string final;
    for (unsigned int i = 0; i < destSize; i++) {
        final += (*(dest + i));
    }
    return final;
}

/*
* zlibComp - Takes a compressed string and decompresses it using zlib. Type(either .png or gd save file) is specified.
*/
std::string zlibUncomp(std::string x, compTypes type) {
    Bytef* source = new unsigned char[x.size()];
    uLongf destLen = 0;
    int compTest = -1;
    int boundOff = 0;
    Bytef* dest = new unsigned char[destLen];
    int datMin = 0;
    int datMax = 0;
    if(type == GD){
        datMin = 10;
        datMax = 8;
    }
    while (compTest < 0) {
        for (unsigned int i = 0; i < (x.size() - boundOff); i++) {
            if (i >= datMin && i <= (x.size() - boundOff) - datMax)
                *(source + (i - datMin)) = x[i];
            if (i >= (x.size() - boundOff) - 4)
                destLen += int(static_cast<unsigned char>(x[i])) * int(pow(256, i - (x.size() - boundOff) + 4));
        }
        dest = new unsigned char[destLen];
        compTest = uncompress(dest, &destLen, source, uLong((x.size() - boundOff) - (datMin + datMax)), int(type));
        boundOff++;
    }
    std::string final;
    for (unsigned int i = 0; i < destLen; i++) {
        final += *(dest + i);
    }
    return final;
}