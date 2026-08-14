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

/*
* layers - The different layers that are used for GD objects.
*/
enum layers{
    B5,
    B4,
    B3,
    B2,
    B1,
    T1,
    T2,
    T3,
    T4,
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
    int editorLayer = 0;
    layers layer;
    std::vector<int> groups;

    // For use in stroke type patch.
    bool patchWest = false;
    bool patchKill = false;
    bool patchSpike = false;

    stroke() {};
    stroke(double t, int c, int z, strokeTypes ty) :thickness(t), color(c), zOrder(z), type(ty) {}
    stroke(strokeTypes ty, double t, int c, int z, int el, int la, int pbf, std::vector<int> g): type(ty), thickness(t), color(c), zOrder(z), editorLayer(el), layer(layers(la)), patchWest(pbf/4), patchKill((pbf/2)%2), patchSpike(pbf%2){}
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

    std::vector<std::vector<double>> points;

    std::vector<std::vector<point>> positions;
    // Optimization factor. Used by fullTraverse to determine how large the deviant distance should be for each segment.

    double optFact = defaultOptFact;
    curve () {}
    curve (std::vector<para> p): path(p){}
    curve (std::vector<para> p, std::vector<stroke> s): path(p), strokes(s){}
    void configure(){
        objects.clear();
        points.clear();
        positions.clear();

        // Blocks to be pushed into the final vector depending on stroke type.
        block nodeObj = std::vector<prop>({prop(1,3637)});
        block segObj = std::vector<prop>({prop(1,917)});
        block glowObj = std::vector<prop>({prop(1,1888)});
        block slopeObj = std::vector<prop>({prop(1,1743),prop(135,1)});
        block spikeObj = std::vector<prop>({prop(1,39)});
        block hitBObj = std::vector<prop>({prop(1,3610),prop(135,1)});
        block solObj = std::vector<prop>({prop(1,1),prop(135,1)});
        std::vector<double> subPoints;
        for(int i=0;i<path.size();i++){
            positions.push_back(std::vector<point>({}));
            subPoints = fullTraverse(path[i], 0.5); // For every subcurve, a set of points are created for general use/display/positioning.
            for(int k=0;k<subPoints.size();k++)
                positions[i].push_back(path[i](subPoints[k]));
            for(int j=0;j<strokes.size();j++){
                point p;
                point pp;
                point m;
                double an;
                double dis;
                double sX;
                double sY;
                switch(strokes[j].type){
                case NORMAL: // For normal stroke types, basic nodes and segments are used.
                    nodeObj.setDoub(32,strokes[j].thickness/50);
                    nodeObj.setInt(21,strokes[j].color);
                    nodeObj.setInt(25,strokes[j].zOrder);
                    nodeObj.setInt(20,strokes[j].editorLayer);
                    nodeObj.setInt(24,int(strokes[j].layer) * 2 - 5);
                    segObj.setDoub(129,strokes[j].thickness/7.5);
                    segObj.setInt(21,strokes[j].color);
                    segObj.setInt(25,strokes[j].zOrder);
                    segObj.setInt(20,strokes[j].editorLayer);
                    segObj.setInt(24,int(strokes[j].layer) * 2 - 5);
                    subPoints = fullTraverse(path[i], optFact);
                    for(int k=0;k<subPoints.size();k++){
                        p = path[i](subPoints[k]); // Current point. Node object is set to this.
                        nodeObj.setDoub(2,p.x);
                        nodeObj.setDoub(3,p.y);
                        objects.push_back(nodeObj);
                        if(k){
                            m = midpoint(p,pp);
                            an = -angleVector(p,pp)/pi*180;
                            dis = dist(p,pp);
                            segObj.setDoub(2,m.x);
                            segObj.setDoub(3,m.y);
                            segObj.setDoub(6,an);
                            segObj.setDoub(128,dis/7.5);
                            objects.push_back(segObj);
                        }
                        pp = path[i](subPoints[k]); // Previous point. Used for determining segment object data.
                    }
                    break;
                case GLOW: // For glow stroke types, inverse magnitude traversal is used, with the scale factor being the thickness of the stroke.
                    glowObj.setDoub(32,strokes[j].thickness/10);
                    glowObj.setInt(21,strokes[j].color);
                    glowObj.setInt(25,strokes[j].zOrder);
                    glowObj.setInt(20,strokes[j].editorLayer);
                    glowObj.setInt(24,int(strokes[j].layer) * 2 - 5);
                    subPoints = path[i].imt(strokes[j].thickness).pos;
                    for(int k=0;k<subPoints.size();k++){
                        p = path[i](subPoints[k]);
                        glowObj.setDoub(2,p.x);
                        glowObj.setDoub(3,p.y);
                        objects.push_back(glowObj);
                    }
                    break;
                case PATCH: // For patch stroke types, a number of different options and configurations can be used to render curves.
                    subPoints = path[i].imt(15).pos;
                    for(int k=0;k<subPoints.size();k++){
                        p = path[i](subPoints[k]);
                        if(k){
                            m = midpoint(p,pp);
                            an = angleVector(p,pp);
                            dis = dist(p,pp);
                            if(strokes[j].patchKill){ // For patches with hitboxes. Can render either or a spike pattern or solid white hitboxes.
                                if(strokes[j].patchSpike){
                                    spikeObj.setDoub(2, pp.x + sqrt(261) * cos(-atan(0.4)*pow(-1,strokes[j].patchWest) + an) * dis / 30);
                                    spikeObj.setDoub(3, pp.y + sqrt(261) * sin(-atan(0.4)*pow(-1,strokes[j].patchWest) + an) * dis / 30);
                                    spikeObj.setDoub(6,-an/pi*180);
                                    spikeObj.setDoub(32, dis/30);
                                    objects.push_back(spikeObj);
                                }
                                else{
                                    hitBObj.setDoub(2, m.x + 15 * cos(an - pi/2*pow(-1,strokes[j].patchWest)));
                                    hitBObj.setDoub(3, m.y + 15 * sin(an - pi/2*pow(-1,strokes[j].patchWest)));
                                    hitBObj.setDoub(6,-an/pi*180);
                                    hitBObj.setDoub(128, dis/30);
                                    objects.push_back(hitBObj);
                                }
                            }
                            else{ // For platform patches. Used for gameplay structuring.
                                sX = round((p.x - pp.x) / 0.3) / 100;
                                sY = round((p.y - pp.y) / 0.3) / 100;
                                if (abs(sX) > 0 && abs(sY) > 0) {
                                    slopeObj.setDoub(2, m.x);
                                    slopeObj.setDoub(3, m.y);
                                    slopeObj.setBool(4, ((sY < 0) == strokes[j].patchWest));
                                    slopeObj.setBool(5, ((sX < 0) == strokes[j].patchWest));
                                    slopeObj.setDoub(128, abs(sX));
                                    slopeObj.setDoub(129, abs(sY));
                                    objects.push_back(slopeObj);
                                }
                                else if (!(sX == 0 && sY == 0)) {
                                    if (sY == 0) {
                                        solObj.setDoub(2, m.x);
                                        solObj.setDoub(3, m.y - 15 * pow(-1, (sX < 0) == strokes[j].patchWest));
                                        solObj.setDoub(128, abs(sX));
                                        solObj.setDoub(129, 1);
                                        objects.push_back(solObj);
                                    }
                                    else if (sX == 0) {
                                        solObj.setDoub(2, m.x + 15 * pow(-1, (sY < 0) == strokes[j].patchWest));
                                        solObj.setDoub(3, m.y);
                                        solObj.setDoub(128, 1);
                                        solObj.setDoub(129, abs(sY));
                                        objects.push_back(solObj);
                                    }
                                }
                            }
                        }
                        pp = path[i](subPoints[k]);
                    }
                    break;
                }
            }
            points.push_back(subPoints);
        }
    }
};

/*
* inscPoint - Represents an x point in a prism inscription. Boolean tang is set to true in case the point is a tangent point.
*/
struct inscPoint{
    double p=0;
    bool tang=0;
    inscPoint(){}
    inscPoint(double po, bool t): p(po), tang(t){}
};

/*
* inscLine - Represents a line at value y used in a prism inscription.
*/
struct inscLine{
    double y=0;
    std::vector<inscPoint> p;
    inscLine(){}
    inscLine(double yp): y(yp){}
};

/*
* sortInsert - Inserts an inscPoint in a vector while keeping it sorted from least to greatest in terms of their p values.
*/
std::vector<inscPoint> sortInsert(std::vector<inscPoint> l, inscPoint n){
    std::vector<inscPoint> final;
    bool placed=0;
    if(l.size()){ // Only time sorting is necessary is when there are already values in the list.
        if(!equal(n.p,l[0].p)&&n.p<l[0].p){ // Cases for when inscPoint n is either less than the first value or greater than the last value.
            final.push_back(n);
            for(int i=0;i<l.size();i++)
                final.push_back(l[i]);
        }
        else if(!equal(n.p,l[l.size()-1].p)&&n.p>l[l.size()-1].p){
            for(int i=0;i<l.size();i++)
                final.push_back(l[i]);
            final.push_back(n);
        }
        else{ // For cases where value n lies somewhere in between, every value is evaluated.
            placed = false;
            for(int i=0;i<l.size();i++){
                if(equal(l[i].p,n.p)) // Case to prevent repeat values.
                    placed = true;
                if(l[i].p>n.p&&!placed){
                    final.push_back(n);
                    placed = true;
                }
                final.push_back(l[i]);
            }
        }
    }
    else
        final.push_back(n);
    return final;
}

/*
* fillerTypes - Represents a type of rendering pattern for prisms.
*/
enum fillerTypes{
    FI_SOL,
    FI_GRAD,
};

/*
* filler - Represents all the rules and properties of a type of filler used.
*/
struct filler{
    fillerTypes type;
    double targSize = 30;
    int obj;
    double rot=0;
    int color=0;
    int zOrder=1;
    layers layer;
    int editorLayer=0;
    std::vector<int> groups;
    fullColor col1;
    fullColor col2;
    filler() {}
    filler(double ts, int o, double r, int c, int z, layers l, int el, std::vector<int> g): type(FI_SOL), targSize(ts), obj(o), rot(r), color(c), zOrder(z), layer(l), editorLayer(el), groups(g) {}
    filler(fullColor c1, fullColor c2, double ts, int o, double r, int c, int z, layers l, int el, std::vector<int> g): type(FI_GRAD), col1(c1), col2(c2), targSize(ts), obj(o), rot(r), color(c), zOrder(z), layer(l), editorLayer(el), groups(g) {}
};

/*
* figure - Similar to curve, but for prisms.
*/
struct figure{
    prism space;
    filler fRule;
    std::vector<block> objects;
    figure() {}
    figure(prism s, filler fr):space(s), fRule(fr) {}
    void createObjects(){
        objects.clear();
        switch(fRule.type){
        case FI_SOL: // For cases of FI_SOL(solid filler), the space's fill is called and the squares are used for the filling process.
            space.fill(fRule.targSize ? fRule.targSize : 30);
            for(int i=0;i<space.squares.size();i++){
                objects.push_back(std::vector<prop>({
                    prop(1,fRule.obj),
                    prop(2,space.squares[i].pos.x),
                    prop(3,space.squares[i].pos.y),
                    prop(6,-fRule.rot),
                    prop(32,space.squares[i].s/30),
                    prop(21,fRule.color),
                    prop(25,fRule.zOrder),
                    prop(24,int(fRule.layer) * 2 - 5),
                    prop(57,fRule.groups),
                    prop(20, fRule.editorLayer),
                }));
            }
            break;
        case FI_GRAD: // For cases of gradients, the prism inscription algorithm is used.
            curve sideObj = space.sides;
            sideObj.configure(); // General use points from the side curves are used.
            std::vector<point> allPoints = std::vector<point>({});
            double pMin = space.sides[0](0).y; // Prism minimums and maximums.
            double pMax = space.sides[0](0).y;
            std::vector<double> dPoints = std::vector<double>({}); // Derivative points use to identify tangents.
            for(int i=0;i<space.sides.size();i++){
                expr sub4=space.sides[i].derive().y;
                for(int j=0;j<sideObj.positions[i].size();j++){
                    allPoints.push_back(sideObj.positions[i][j]);
                    pMin = std::min(pMin,allPoints[allPoints.size()-1].y);
                    pMax = std::max(pMax,allPoints[allPoints.size()-1].y);
                    dPoints.push_back(sub4(sideObj.points[i][j]));
                }
            }
            pMin+=fRule.targSize; // The targSize offset is added to identify the initial and terminal points for the lines.
            pMax-=fRule.targSize;
            double lineCount=ceil((pMax-pMin)/fRule.targSize); // Total amount of lines.
            double lineSpace=(pMax-pMin)/lineCount; // Space between the lines.
            double lineOff=fmod(pMin,lineSpace); // Offset for the start of the lines.
            std::vector<inscLine> allLines;
            for(int i=0;i<=lineCount;i++)
                allLines.push_back(inscLine(i*lineSpace+pMin));
            for(int i=0;i<allPoints.size();i++){
                ps sub = ps(allPoints[i],allPoints[(i+1)%allPoints.size()]); // Bounds are set by means of a point-slope intersection between the horizontal line and border line.
                for(int k=ceil((std::min(allPoints[i].y,allPoints[(i+1)%allPoints.size()].y)-lineOff)/lineSpace);k<=floor((std::max(allPoints[i].y,allPoints[(i+1)%allPoints.size()].y)-lineOff)/lineSpace);k++){
                    ps sub2=ps(point(0,k*lineSpace+lineOff),point(1,k*lineSpace+lineOff));
                    psInterResult sub3 = psInter(sub,sub2);
                    int ind=k-floor(pMin/lineSpace);
                    if(ind>=0&&ind<allLines.size()){
                        bool tang = false;
                        if((sub3.result == allPoints[i]) or (sub3.result == allPoints[(i+1)%allPoints.size()])) // Checks to see if the intersection is negligibliy close enough to a border point.
                            tang = equal(dPoints[i],0) or equal(dPoints[(i+1)%allPoints.size()],0); // Checks to see if the y derivative of the point is close to 0. If so, the tang boolean for the inscPoint is set to true.
                        allLines[ind].p=sortInsert(allLines[ind].p,inscPoint(sub3.result.x, tang));
                    }
                }
            }
            for(int i=0;i<allLines.size();i++){
                for(int j=allLines[i].p.size()-1;j>-1;j--){
                    if(allLines[i].p[j].tang and j>0 and j<allLines[i].p.size()-1)
                        allLines[i].p.erase(allLines[i].p.begin()+j,allLines[i].p.begin()+j+1); // Every tangent point is discarded.
                }
            }
            for(int i=0;i<allLines.size();i++){ // For every line, an object is created using the specifications in fRule.
                for(int j=0;j<allLines[i].p.size();j+=2){
                    double sp = allLines[i].p[j].p;
                    double ep = allLines[i].p[j+1].p;
                    double sLen = ep-sp;
                    int sAmt = floor(sLen/240)+1; // Protocol to prevent any object from being too large(8 blocks maximum).
                    double sSize = sLen/sAmt;
                    fullColor sc = gravRGB(fRule.col1,fRule.col2,double(i)/(allLines.size()-1)); // Gravitation between two colors to give a gradient effect within the figure.
                    for(int k=0;k<sAmt;k++){
                        objects.push_back(block(std::vector<prop>({
                            prop(1,fRule.obj),
                            prop(2,sp+sSize/2+sSize*k),
                            prop(3,allLines[i].y),
                            prop(128,sSize/7.5),
                            prop(129,ceil(fRule.targSize/7.5*100)/100),
                            prop(25,fRule.zOrder),
                            prop(20,fRule.editorLayer),
                            prop(21,fRule.color),
                            prop(6,-fRule.rot),
                            prop(41,1),
                            prop(43,hsv(std::vector<double>({sc.h,sc.s,sc.v,0,0}))),
                        })));
                    }
                }
            }
            break;
        }
    }
};