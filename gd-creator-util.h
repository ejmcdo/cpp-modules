/*
* (What is supposed to be)a bunch of different creator utilities that I've developed during my time as a Geometry Dash player/creator.
* Currently unfinished.
*/
#pragma once
#include <iostream>
#include <string>
#include <gd-obj.h>
#include <vector>
#include <calc-engine.h>

/*
* strokeTypes - Represents a type of stroke pattern used for curves.
*/
enum strokeTypes {
    NORMAL,
    GLOW,
    PATCH,
};

double defaultThickness = 2;

/*
* stroke - Represents a type of rendering pattern used for curves.
*/
struct stroke {
    double thickness = defaultThickness;
    int color = 1;
    int zOrder = 1;
    strokeTypes type = NORMAL;
    stroke() {};
    stroke(double t, int c, int z, strokeTypes ty) :thickness(t), color(c), zOrder(z), type(ty) {}
};

double defaultOptFact = 0.5;
/*
* curve - Represents a basic curve that renders in the GD editor.
*/
struct curve{
    // Represents the path of the curve.
    std::vector<para> path;

    // Represents all strokes used to draw the curve.
    std::vector<stroke> strokes;

    // After object creation, all objects are stored here.
    std::vector<block> objects;

    // Optimization factor. Used by fullTraverse to determine how large the deviant distance should be for each segment.
    double optFact = defaultOptFact;
    curve (std::vector<para> p): path(p){}
    void createObjects(){
        objects.clear();
        // Blocks to be pushed into the final vector.
        block nodeObj = std::vector<prop>({prop(1,3637)});
        block segObj = std::vector<prop>({prop(1,917)});
        block glowObj = std::vector<prop>({prop(1,1888)});
        for(int i=0;i<path.size();i++){
            for(int j=0;j<strokes.size();j++){
                std::vector<double> points;
                point p;
                point pp;
                point m;
                switch(strokes[j].type){
                case NORMAL: // For normal stroke types, basic nodes and segments are used.
                    nodeObj.setDoub(32,strokes[j].thickness/50);
                    nodeObj.setInt(21,strokes[j].color);
                    nodeObj.setInt(25,strokes[j].zOrder);
                    segObj.setDoub(129,strokes[j].thickness/7.5);
                    segObj.setInt(21,strokes[j].color);
                    segObj.setInt(25,strokes[j].zOrder);
                    points = fullTraverse(path[i], optFact);
                    for(int k=0;k<points.size();k++){
                        p = path[i](points[k]); // Current point. Node object is set to this.
                        nodeObj.setDoub(2,p.x);
                        nodeObj.setDoub(3,p.y);
                        objects.push_back(nodeObj);
                        if(k){
                            m = midpoint(p,pp);
                            segObj.setDoub(2,m.x);
                            segObj.setDoub(3,m.y);
                            segObj.setDoub(6,-angleVector(p,pp)/pi*180);
                            segObj.setDoub(128,dist(p,pp)/7.5);
                            objects.push_back(segObj);
                        }
                        pp = path[i](points[k]); // Previous point. Used for determining segment object data.
                    }
                    break;
                case GLOW: // For glow stroke types, inverse magnitude traversal is used, with the scale factor being the thickness of the stroke.
                    glowObj.setDoub(32,strokes[j].thickness/10);
                    glowObj.setInt(21,strokes[j].color);
                    glowObj.setInt(25,strokes[j].zOrder);
                    points = path[i].imt(strokes[j].thickness).pos;
                    for(int k=0;k<points.size();k++){
                        p = path[i](points[k]);
                        glowObj.setDoub(2,p.x);
                        glowObj.setDoub(3,p.y);
                        objects.push_back(glowObj);
                    }
                    break;
                }
            }
        }
    }
};