/*
* For whatever reason, I took it upon myself to study the format of the .png image. As a result, I made this module to easily create and parse .png images from scratch.
*/
#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <file-handle.h>
#include <zlib-handle.h>

/*
* paeth - Predictor model used in the parsing process.
*/
int paeth(int a, int b, int c){
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);
    if(pa <= pb && pa <= pc)
        return a;
    else if(pb <= pc)
        return b;
    else
        return c;
}

/*
* pixel - Represents a single pixel in a .png image.
*/
struct pixel{
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
    unsigned char a{};

    // Can either take 4 raw values or a vector as a constructor.
    pixel(){}
    pixel(unsigned char re, unsigned char gr, unsigned char bl, unsigned char al): r(re), g(gr), b(bl), a(al) {}
    pixel(std::vector<unsigned char> vals): r(vals[0]), g(vals[1]), b(vals[2]), a(vals[3]) {}

    // print - Prints the r, g, b, and a values.
    void print(){
        std::cout << int(r) << " " << int(g) << " " << int(b) << " " << int(a) << "\n";
    }
};

/*
* filter - Represents all of the models used during filtering.
*/
enum filter{
    NONE_FILT,
    SUB,
    ABOVE,
    AVERAGE,
    PAETH
};

/*
*  matrix - Takes a .png image located at filename fn and parses it. Return a pixel matrix.
*/
std::vector<std::vector<pixel>> matrix(std::string fn){
    std::string sb = fileCont(fn);
    int count = 8;
    std::vector<std::vector<int>> blocks;
    std::vector<std::string> blockNames;
    std::vector<int> datBlockIndices;
    while(count < sb.size()){ // Block parser. The only blocks that really matter are the IDAT blocks, where the pixel data is stored.
        int blockLen = 0;
        for (int i=0;i<4;i++)
            blockLen+=sb[count+i]*pow(256,3-i);
        std::string blockName = "";
        for (int i=0;i<4;i++)
            blockName += sb[count+i+4];
        if(blockName == "IDAT")
            datBlockIndices.push_back(blockNames.size());
        blockNames.push_back(blockName);
        count += 8;
        std::vector<int> blockData;
        for(int i=0;i<blockLen;i++)
            blockData.push_back(sb[count+i]);
        count += blockLen+4;
        blocks.push_back(blockData);
    } // The first block is always the header block, which is where the width and height of the image are stored.
    int width = 0;
    int height = 0;
    for(int i=0;i<4;i++){
        width += blocks[0][i]*pow(256,3-i);
        height += blocks[0][i+4]*pow(256,3-i);
    }
    // Compressed data may be stored across IDAT blocks. This ensures that the full data stream is present.
    std::string allData;
    for (int i=0;i<datBlockIndices.size();i++){
        for(int j=0;j<blocks[datBlockIndices[i]].size();j++)
            allData += (blocks[datBlockIndices[i]][j]);
    }
    std::string decompressedData = zlibUncomp(allData);
    std::vector<std::vector<std::vector<unsigned char>>> pixelMat;
    std::vector<filter> filters;
    for(int i=0;i<decompressedData.size();i++){ // Identifying what filter was used for each row.
        if(!(i % (width*4+1))){
            pixelMat.push_back(std::vector<std::vector<unsigned char>>({}));
            filters.push_back(filter(decompressedData[i]));
        }
        else if(!(((i%(width*4+1))-1)%4)){
            pixelMat[pixelMat.size()-1].push_back(std::vector<unsigned char>({}));
            pixelMat[pixelMat.size()-1][pixelMat[pixelMat.size()-1].size()-1].push_back(decompressedData[i]);
        }
        else
            pixelMat[pixelMat.size()-1][pixelMat[pixelMat.size()-1].size()-1].push_back(decompressedData[i]);
    }
    int sp1 = 0; // Sample values used in case a filter was used that exceeded index bounds(for example, a filter of type UP located on the first row, where there are no pixels above the ones being read).
    int sp2 = 0;
    int sp3 = 0;
    std::vector<std::vector<pixel>> finalPixels;
    for(int i=0;i<pixelMat.size();i++){
        finalPixels.push_back(std::vector<pixel>({}));
        for(int j=0;j<pixelMat[i].size();j++){
            for(int k=0;k<4;k++){
                switch(filters[i]){ // Pixels are evaluated taking into account filters.
                case SUB:
                    if(j > 0)
                        pixelMat[i][j][k] = (pixelMat[i][j][k]+pixelMat[i][j-1][k])%256;
                    break;
                case ABOVE:
                    if(i > 0)
                        pixelMat[i][j][k] = (pixelMat[i][j][k]+pixelMat[i-1][j][k])%256;
                    break;
                case AVERAGE:
                    sp1 = 0;
                    if(j > 0)
                        sp1 = pixelMat[i][j-1][k];
                    sp2 = 0;
                    if(i > 0)
                        sp2 = pixelMat[i-1][j][k];
                    pixelMat[i][j][k] = (pixelMat[i][j][k]+int((sp1+sp2)/2))%256;
                case PAETH:
                    sp1 = 0;
                    if(j > 0)
                        sp1 = pixelMat[i][j-1][k];
                    sp2 = 0;
                    if(i > 0)
                        sp2 = pixelMat[i-1][j][k];
                    sp3 = 0;
                    if(i > 0 && j > 0)
                        sp3 = pixelMat[i-1][j-1][k];
                    pixelMat[i][j][k] = (pixelMat[i][j][k]+paeth(sp1,sp2,sp3))%256;
                }
            }
            finalPixels[finalPixels.size()-1].push_back(pixel(pixelMat[i][j]));
        }
    }
    return finalPixels;
}

/*
* instantCrc32 - Takes a data stream, attaches a crc32 checksum at the end of it, and returns it.
*/
std::string instantCrc32(std::string x){
    Bytef b[x.size()];
    for(int i=0;i<x.size();i++)
        b[i] = x[i];
    unsigned int idatCrc = crc32(0L, &b[0], x.size());
    for(int i=0;i<4;i++)
        x += int(idatCrc/pow(256,3-i))%256;
    return x;
}

/*
* image - Takes a pixel matrix and converts it to a .png image named filename.
*/
void image(std::vector<std::vector<pixel>> pixels, std::string fileName){
    std::string rawStream = "";
    for(int i=0;i<pixels.size();i++){
        rawStream += static_cast<char>(0); // No compression used because I quite literally cannot be bothered. Cry about it.
        for(int j=0;j<pixels[i].size();j++){
            rawStream += pixels[i][j].r;
            rawStream += pixels[i][j].g;
            rawStream += pixels[i][j].b;
            rawStream += pixels[i][j].a;
        }
    }

    // Only three blocks are required, IHDR, IDAT, and IEND. The rest is optional metadata that was never researched.
    std::string compStr = zlibComp(rawStream);
    std::string idatStr = "IDAT";
    for(int i=0;i<compStr.size();i++)
        idatStr += compStr[i];
    idatStr = instantCrc32(idatStr);
    
    std::string ihdrStr = "IHDR";
    for(int i=0;i<4;i++)
        ihdrStr += int(pixels[0].size()/pow(256,3-i))%256;
    for(int i=0;i<4;i++)
        ihdrStr += int(pixels.size()/pow(256,3-i))%256;
    std::vector<char> headerBytes = std::vector<char>({8, 6, 0, 0, 0}); // Something related to bit depth and color if I remember correctly.
    for(int i=0;i<5;i++)
        ihdrStr += headerBytes[i];
    ihdrStr = instantCrc32(ihdrStr);

    std::string iendStr = instantCrc32("IEND");

    std::vector<std::string> blocks = std::vector<std::string>({ihdrStr, idatStr, iendStr});
    std::vector<unsigned char> startBytes = std::vector<unsigned char>({137,80,78,71,13,10,26,10}); // 8-byte signature used in all pngs. We love to see hard-coded values here.
    std::string finalString;
    for(int i=0;i<8;i++)
        finalString += startBytes[i];
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++)
            finalString += (int((blocks[i].size()-8)/pow(256,3-j)))%256;
        for(int j=0;j<blocks[i].size();j++)
            finalString += blocks[i][j];
    }
    fileWrite(fileName, finalString);
}