/*
* A hub for color/pixel modifications.
*/
#pragma once
#include <vector>
#include <math.h>
#include <iostream>

/*
* fullColor - Represents a color by using both RGB and HSV values.
*/
struct fullColor{
    double r=0;
    double g=0;
    double b=0;
    double h=0;
    double s=0;
    double v=0;
    fullColor(){}
    fullColor(double v1, double v2, double v3, bool hsv){ // Either RGB or HSV values can be passed in the constructor. Boolean hsv is used to differentiate between the two.
        if(hsv){
            h=v1;
            s=v2;
            v=v3;
            checkRGB();
        }
        else{
            r=v1;
            g=v2;
            b=v3;
            checkHSV();
        }
    }
    void checkRGB(){ // In case there are any changes to h, s, or v, values r, g, and b, are recalculated to reflect the changes.
        std::vector<double>final=std::vector<double>({0,0,0});
        if(h>=0&&h<60){
            final[0]=1;
            final[1]=h/60;
        }
        if(h>=60&&h<120){
            final[1]=1;
            final[0]=2-h/60;
        }
        if(h>=120&&h<180){
            final[1]=1;
            final[2]=h/60-2;
        }
        if(h==-180||h==180){
            final[1]=1;
            final[2]=1;
        }
        if(h>=-180&&h<-120){
            final[2]=1;
            final[1]=-2-h/60;
        }
        if(h>=-120&&h<-60){
            final[2]=1;
            final[0]=2+h/60;
        }
        if(h>=-60&&h<0){
            final[0]=1;
            final[2]=-h/60;
        }
        final=std::vector<double>({(final[0]+(1-final[0])*(1-s))*v,(final[1]+(1-final[1])*(1-s))*v,(final[2]+(1-final[2])*(1-s))*v});
        r=round(final[0]*255);// RGB values each have a range of 0 to 255, inclusive.
        g=round(final[1]*255);
        b=round(final[2]*255);
    }
    void checkHSV(){ // In case there are any changes to r, g, or b, values h, s, and v, are recalculated to reflect the changes.
        std::vector<double> final;
        if(r>g&&r>b)
            final=std::vector<double>({(g-b)/(r-((g<b)?g:b))*60,1-((g<b)?g:b)/r,r/255});
        if(g>r&&g>b)
            final=std::vector<double>({(b-r)/(g-((r<b)?r:b))*60+120,1-((r<b)?r:b)/g,g/255});
        if(b>r&&b>g)
            final=std::vector<double>({(r-g)/(b-((r<g)?r:g))*60-120,1-((r<g)?r:g)/b,b/255});
        if(r==g&&r>b)
            final=std::vector<double>({60,1-b/r,r/255});
        if(r==b&&r>g)
            final=std::vector<double>({-60,1-g/r,r/255});
        if(g==b&&b>r)
            final=std::vector<double>({180,1-r/b,g/255});
        if(g==b&&r==b)
            final=std::vector<double>({0,0,r/255});
        h=final[0]; // h has a range of -180 to 180, while s and v have a range of 0 to 1.
        s=final[1];
        v=final[2];
    }
    void print(){
        std::cout << r << " " << g << " " << b << "\n" << h << " " << s << " " << v;
    }
};

/*
* pixel - Represents a single pixel in an image.
*/
struct pixel{
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
    unsigned char a{};
    fullColor color;

    // Can either take 4 raw values, a vector, and fullColor object as a constructor. For fullColor, the a value is set to 255 by default unless specified.
    pixel(){}
    pixel(unsigned char re, unsigned char gr, unsigned char bl, unsigned char al): r(re), g(gr), b(bl), a(al) {color=fullColor(r,g,b,false);}
    pixel(std::vector<unsigned char> vals): r(vals[0]), g(vals[1]), b(vals[2]), a(vals[3]) {color=fullColor(r,g,b,false);}
    pixel(fullColor c): color(c){
        a = 255;
        check();
    }
    pixel(fullColor c, unsigned char al): color(c){
        a = al;
        check();
    }

    // print - Prints the r, g, b, and a values.
    void print(){
        std::cout << int(r) << " " << int(g) << " " << int(b) << " " << int(a) << "\n";
    }

    // check - Checks the fullColor for any changes and sets r, g, and b values accordingly.
    void check(){
        color.checkRGB();
        r = static_cast<unsigned char>(round(color.r));
        g = static_cast<unsigned char>(round(color.g));
        b = static_cast<unsigned char>(round(color.b));
    };

    // Operator overload + - Takes the original pixel and overlaps pixel c to it, taking into account transparency. Returns the result as a new pixel.
    pixel operator+(pixel c){
        double unitA = double(a)/double(255);
        double unitAC = double(c.a)/double(255);
        double rop = 1-((double(1)-unitA)*(double(1)-unitAC));
        if(rop > 0)
            return pixel(fullColor(round((r*unitA*(1-unitAC)+c.r*unitAC)/rop),round((g*unitA*(1-unitAC)+c.g*unitAC)/rop),round((b*unitA*(1-unitAC)+c.b*unitAC)/rop),false),round(rop*255));
        else
            return pixel(0,0,0,0);
    }

    // Operator overload += - Overlaps pixel c on to the original pixel.
    void operator+=(pixel c){*this = (*this)+c;}
};