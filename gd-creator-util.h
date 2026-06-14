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

struct curve{
    std::vector<para> path;
    std::vector<block> objects;
    curve (std::vector<para> p): path(p){}
    void createObjects(){
        objects.clear();
        for(int i=0;i<path.size();i++){
            point p;
            for(int j=0;j<10;j++){
                p=path[i](double(j)/10);
                objects.push_back(block(std::vector<prop>({
                    prop(1,1764),
                    prop(2,p.x),
                    prop(3,p.y),
                })));
                point p2 = path[i](double(j+1)/10);
                point mp = midpoint(p,p2);
                double d=dist(p,p2);
                double an=angleVector(p,p2);
                objects.push_back(block(std::vector<prop>({
                    prop(1,917),
                    prop(2,mp.x),
                    prop(3,mp.y),
                    prop(6,-an*180/pi),
                    prop(128,d/7.5),
                })));
            }
            p=path[i](1);
            objects.push_back(block(std::vector<prop>({
                prop(1,1764),
                prop(2,p.x),
                prop(3,p.y),
            })));
        }
    }
};