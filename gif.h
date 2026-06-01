/*
* Video files were way to difficult to decipher, so I had to settle for a .gif encoder. Such is life.
*/
#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <file-handle.h>
#include <color-handle.h>

/*
* rBinStr - Takes a number n and stores it in l bits in little-endian order.
*/
std::string rBinStr(int n,int l){
    std::string final = "";
    for(int i=0;i<l;i++)
        final += int((int(n/pow(2,i)))%2) ? "1" : "0";
    return final;
}

/*
* rStrDec - Takes a string of 8 bits, stores them in little-endian order, and returns a char as a result.
*/
unsigned char rStrDec(std::string s){
    int final = 0;
    for(int i=0;i<8;i++)
        final += (s[i] == '1')*pow(2,i);
    return static_cast<unsigned char>(final);
}

/*
* frame - Takes a char matrix(because .gif files only have 256 colors) and encodes it as a single frame in a .gif file. Int time refers to the time in 1/100ths of a second before the next frame.
*/
std::string frame(std::vector<std::vector<unsigned char>> mags, int time){
    // Lots and lots of metadata. Serves a purpose. Probably.
    std::string final = "!";
    final += "\xf9\x04\x04";
    final += static_cast<unsigned char>(time%256);
    final += static_cast<unsigned char>(time/256);
    final += char(0);
    final += char(0);
    final += ",";
    for(int i=0;i<4;i++)
        final += char(0);
    final += static_cast<unsigned char>(mags[0].size()%256);
    final += static_cast<unsigned char>(mags[0].size()/256);
    final += static_cast<unsigned char>(mags.size()%256);
    final += static_cast<unsigned char>(mags.size()/256);
    final += char(0);
    final += "\x08";

    // Values stored in a straight vector starting from the top-left corner for encoding.
    std::vector<unsigned char> vals;
    for(int i=0;i<mags.size();i++){
        for(int j=0;j<mags.size();j++)
            vals.push_back(mags[i][j]);
    }

    // LZW Encoding algorithm. I'm not explaining this here. Wikipedia is right around the corner.
    std::vector<std::vector<int>> codes = std::vector<std::vector<int>>({std::vector<int>({256,9})});
    std::vector<std::vector<int>> dict;
    std::vector<int> testCode;
    int cs = 0;
    for(int i=0;i<vals.size();i++){
        testCode.push_back(vals[i]);
        int co = 0;
        int fo = -1;
        while(co < dict.size() and fo == -1){
            if(testCode == dict[co])
                fo = co;
            co += 1;
        }
        if(fo == -1){
            if(testCode.size() > 1){
                dict.push_back(testCode);
                codes.push_back(std::vector<int>({cs,int(ceil(log2(dict.size()+257)))}));
            }
            testCode = std::vector<int>({vals[i]});
            cs = vals[i];
        }
        else
            cs = fo+258;
    }
    codes.push_back(std::vector<int>({cs,int(ceil(log2(dict.size()+257)))}));
    codes.push_back(std::vector<int>({257,int(ceil(log2(dict.size()+257)))}));

    // Bit codes are also packed in a weird way. Fun.
    std::string vh = "";
    std::vector<int> bs;
    for(int i=0;i<codes.size();i++){
        std::string bits = rBinStr(codes[i][0],codes[i][1]);
        for(int j=0;j<bits.size();j++){
            vh += bits[j];
            if(vh.size() == 8){
                bs.push_back(rStrDec(vh));
                vh = "";
            }
        }
    }
    while(vh.size() < 8)
        vh += "0";
    bs.push_back(rStrDec(vh));

    for(int i=0;i<bs.size();i++){ // Final byte stream in blocks of 255 bytes until the end.
        if(!(i%255))
            final += ((bs.size()-i<255) ? bs.size()-i : 255);
        final += bs[i];
    }
    final += char(0); // Learned the hard way the C++ doesnt allow direct "\x00" addition.
    return final;
}

/*
* animation - Takes a vector of char matrices and packs them all into a .gif animation.
* Due the limited nature of the color table, only a background color, stroke color, as well as all the colors directly between them, are used. Int time has the same purpose as frame, and the result is written into file fn.
*/
void animation(std::vector<std::vector<std::vector<unsigned char>>> frames, fullColor bgColor, fullColor strokeColor, int time, std::string fn){
    // Even more metadata.
    std::string final = "GIF89a";
    final += static_cast<unsigned char>(frames[0][0].size()%256);
    final += static_cast<unsigned char>(frames[0][0].size()/256);
    final += static_cast<unsigned char>(frames[0].size()%256);
    final += static_cast<unsigned char>(frames[0].size()/256);
    final += "\xf7";
    final += char(0);
    final += char(0);
    // Color table initializes values to be used. Index 0 is the background color and Index 255 is the stroke color. All other values lie inbetween the two.
    // Example: For a background color red(#FF0000) and a stroke color yellow(#FFFF00), index 128 in the table would refer to orange(#FF8000)
    for(int i=0;i<256;i++){
        final += static_cast<unsigned char>(bgColor.r+double(strokeColor.r-bgColor.r)/255*i);
        final += static_cast<unsigned char>(bgColor.g+double(strokeColor.g-bgColor.g)/255*i);
        final += static_cast<unsigned char>(bgColor.b+double(strokeColor.b-bgColor.b)/255*i);
    }
    // Extension stuff.
    final += "!";
    final += "\xff\x0b";
    final += "NETSCAPE2.0";
    final += "\x03\x01\xff\xff";
    final += char(0);
    // All frames are encoded and packed.
    for(int i=0;i<frames.size();i++)
        final += frame(frames[i],time);
    final += ";";
    fileWrite(fn,final);
}