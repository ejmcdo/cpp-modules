#pragma once
#include <calc-engine.h>
#include <color-handle.h>
#include <vector>
#include <png.h>
#include <gd-creator-util.h>

enum screenTypes{
    SOLID,
    GRAD,
    IMG
};

fullColor gridNumP(std::vector<std::vector<pixel>> g, point p){
    fullColor p1 = g[floor(p.y)][floor(p.x)].color;
    fullColor p2 = g[floor(p.y)][ceil(p.x)].color;
    fullColor p3 = g[ceil(p.y)][floor(p.x)].color;
    fullColor p4 = g[ceil(p.y)][ceil(p.x)].color;
    double uX = p.x-floor(p.x);
    double uY = p.y-floor(p.y);
    double fR = p1.r*(1-uX)*(1-uY)+p2.r*uX*(1-uY)+p3.r*(1-uX)*uY+p4.r*uX*uY;
    double fG = p1.g*(1-uX)*(1-uY)+p2.g*uX*(1-uY)+p3.g*(1-uX)*uY+p4.g*uX*uY;
    double fB = p1.b*(1-uX)*(1-uY)+p2.b*uX*(1-uY)+p3.b*(1-uX)*uY+p4.b*uX*uY;
    return fullColor(fR,fG,fB,false);
}

struct screen{
    screenTypes type;
    fullColor col1;
    fullColor col2;
    point cent;
    double size;
    double angle;
    ps gradPs;
    std::vector<std::vector<pixel>> imgRef;
    screen(){}
    screen(fullColor c): type(SOLID), col1(c){}
    screen(fullColor c1, fullColor c2, point c, double s, double an): type(GRAD), col1(c1), col2(c2), cent(c), size(s), angle(an), gradPs(ps(cent,point(cent.x+cos(an*pi/180),cent.y+sin(an*pi/180)))){}
    screen(std::vector<std::vector<pixel>> ir): type(IMG), imgRef(ir){}
    fullColor getCol(point p){
        fullColor final;
        double sub=0;
        fullColor subCol;
        switch(type){
        case SOLID:
            final = col1;
            break;
        case GRAD:
            sub = ((gradPs.linDist(p)/size)+1)/2;
            sub = (sub<0) ? 0 : sub;
            sub = (sub>1) ? 1 : sub;
            final = (pixel(col1)+pixel(col2,static_cast<unsigned char>(255*sub))).color;
            break;
        case IMG:
            final = gridNumP(imgRef,point(p.x*(imgRef[0].size()-1),p.y*(imgRef.size()-1)));
            break;
        }
        return final;
    }
};

/*
* stringFuncs - Represents different types of functions that can be used in string notation. Currently unfinished.
*/
enum stringFuncs{
    NONE_SF,
    BEZIER, // be
    BSPLINE, // bs
    SINUSOID, // si
    COMPCURVE, // co
    TRANSFORM, // tr
    EXTEND, // ex
    TANGENT, // ta
    SWARP, // sw
    DWARP, // dw
    QWARP, // qw
    INTERSECTION, // in
    SELECT, // se
    SPLIT, // sp
    PRISM, // pr
    PSWARP, // ps
    CIRC, // ci
    SC_SOLID, // so
    SC_GRAD, // gr
    SC_IMG, // im
    RENDCURVE, // rc
    RENDPRIS, // rp
    DIMENSIONS, // di
    RESNAME, // rn
    STROKE, // st
    STRGROUP, // sg
    LAYOUT, // la
    FILL_SOL, // fs
    FILL_GRAD // fg
};

/*
* snCache - Structure that is used to store the data from a stringNotation parse.
*/
struct snCache{
    std::vector<std::vector<para>> curves;
    std::vector<prism> prisms;
    std::vector<screen> screens;
    std::vector<std::vector<double>> rendOrder;
    point dimensions;
    std::string name;
    std::vector<stroke> strokes;
    std::vector<std::vector<int>> strokeGroups;
    std::string layoutBase;
    std::vector<filler> fillers;
    snCache(){}
    snCache(std::vector<std::vector<para>> c, std::vector<prism> p, std::vector<screen> s, std::vector<std::vector<double>> r, point d, std::string n, std::vector<stroke> st, std::vector<std::vector<int>> sg, std::string lb, std::vector<filler> fi): curves(c), prisms(p), screens(s), rendOrder(r), dimensions(d), name(n), strokes(st), strokeGroups(sg), layoutBase(lb), fillers(fi) {
        if(!name.size())
            name = "pmRender";
    }
};

/*
* stringNotation - Takes a raw string, parses it, and returns the resulting curves and prisms in the form of a snCache.
*/
snCache stringNotation(std::string s){
    std::string vh="";
    stringFuncs func=NONE_SF;
    std::vector<std::vector<para>> finalC;
    std::vector<prism> finalP;
    std::vector<screen> finalS;
    std::vector<std::vector<double>> finalRO;
    point finalD;
    std::string finalN = "";
    std::vector<stroke> finalST;
    std::vector<std::vector<int>> finalSG;
    std::string finalLB;
    std::vector<filler> finalFI;
    std::vector<std::vector<double>> valStore;
    std::vector<int> valStoreInt;
    int valCount = 0;
    std::vector<point> valStorePoint;
    std::vector<para> valStorePara;
    transformType sampTransform;
    std::vector<transformNode> valStoreTN;
    std::vector<intersection> valStoreInter;
    std::vector<intersection> valStoreInter2;
    int levelInd=0;
    bool foundName = false;
    bool read = true;
    bounds sampBounds;
    bool boundsDef = false;
    for(int i=0;i<s.size();i++){

        if(s[i] == '[')
            read = false;
        if(read){
            if(s[i] == '\n')
                vh = "";
            else if(s[i] == ',' || s[i] == ';'){
                if(!(func == SC_IMG or func == RESNAME or func == LAYOUT)){
                    valStore[valStore.size()-1].push_back(str2Doub(vh));
                    vh = "";
                }
            }
            else if(s[i] == '|'){
                if(!(func == SC_IMG or func == RESNAME or func == LAYOUT)){
                    valStore[valStore.size()-1].push_back(str2Doub(vh));
                    valStore.push_back(std::vector<double>({}));
                }
                vh = "";
            }
            else
                vh += s[i];
            if(func == NONE_SF and vh.size() == 2){
                if(vh == "be")
                    func = BEZIER;
                else if(vh == "bs")
                    func = BSPLINE;
                else if(vh == "si")
                    func = SINUSOID;
                else if(vh == "co")
                    func = COMPCURVE;
                else if(vh == "tr")
                    func = TRANSFORM;
                else if(vh == "ex")
                    func = EXTEND;
                else if(vh == "ta")
                    func = TANGENT;
                else if(vh == "sw")
                    func = SWARP;
                else if(vh == "dw")
                    func = DWARP;
                else if(vh == "qw")
                    func = QWARP;
                else if(vh == "in")
                    func = INTERSECTION;
                else if(vh == "se")
                    func = SELECT;
                else if(vh == "sp")
                    func = SPLIT;
                else if(vh == "pr")
                    func = PRISM;
                else if(vh == "ps")
                    func = PSWARP;
                else if(vh == "ci")
                    func = CIRC;
                else if(vh == "so")
                    func = SC_SOLID;
                else if(vh == "gr")
                    func = SC_GRAD;
                else if(vh == "im")
                    func = SC_IMG;
                else if(vh == "rc")
                    func = RENDCURVE;
                else if(vh == "rp")
                    func = RENDPRIS;
                else if(vh == "di")
                    func = DIMENSIONS;
                else if(vh == "rn")
                    func = RESNAME;
                else if(vh == "st")
                    func = STROKE;
                else if(vh == "sg")
                    func = STRGROUP;
                else if(vh == "la")
                    func = LAYOUT;
                else if(vh == "fi")
                    func = FILL_SOL;
                else if(vh == "fg")
                    func = FILL_GRAD;
                valStore.clear();
                valStore.push_back(std::vector<double>({}));
                vh = "";
                }
            if(s[i] == ';'){
                if(func == BEZIER or func == BSPLINE or func == CIRC){
                    for(int j=0;j<valStore[0].size();j+=2)
                        valStorePoint.push_back(point(valStore[0][j],valStore[0][j+1]));
                }
                if(func == TRANSFORM or func == PRISM){
                    for(int j=0;j<valStore[0].size();j+=2)
                        valStorePara.push_back(finalC[valStore[0][j]][valStore[0][j+1]]);
                }
                if(func == SWARP or func == DWARP or func == QWARP or func == PSWARP){
                    for(int j=0;j<valStore[1].size();j+=2)
                        valStorePara.push_back(finalC[valStore[1][j]][valStore[1][j+1]]);
                }
                if(func == SELECT){
                    for(int j=0;j<valStore[1].size();j++)
                        valStorePara.push_back(finalC[valStore[0][0]][valStore[1][j]]);
                }
                switch(func){
                case BEZIER:
                    finalC.push_back(std::vector<para>({bez(valStorePoint)}));
                    valStorePoint.clear();
                    break;
                case BSPLINE:
                    finalC.push_back(std::vector<para>({bSpline(valStorePoint,valStore[1][0],valStore[1][1])}));
                    valStorePoint.clear();
                    break;
                case SINUSOID:
                    finalC.push_back(std::vector<para>({sinusoid(point(valStore[0][0],valStore[0][1]),valStore[0][2],valStore[0][3],valStore[0][4],valStore[0][5])}));
                    break;
                case COMPCURVE:
                    finalC.push_back(std::vector<para>({compCurve(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]])}));
                    break;
                case TRANSFORM:
                    valCount = 0;
                    for(int j=0;j<valStore[1].size();j++){
                        if(valStore[1][j] == 0 || valStore[1][j] == 2){
                            valStoreTN.push_back(transformNode(transformType(valStore[1][j]),valStore[2][valCount],valStore[2][valCount+1]));
                            valCount += 2;
                        }
                        else if (valStore[1][j] == 1){
                            valStoreTN.push_back(transformNode(valStore[2][valCount]));
                            valCount++;
                        }
                        else if (valStore[1][j] == 3 || valStore[1][j] == 5){
                            valStoreTN.push_back(transformNode(transformType(valStore[1][j]),finalC[valStore[2][valCount]][valStore[2][valCount+1]].x,finalC[valStore[2][valCount+2]][valStore[2][valCount+3]].y));
                            valCount += 4;
                        }
                        else if (valStore[1][j] == 4){
                            if(valStore[2][valCount+2])
                                valStoreTN.push_back(transformNode(finalC[valStore[2][valCount]][valStore[2][valCount+1]].y));
                            else
                                valStoreTN.push_back(transformNode(finalC[valStore[2][valCount]][valStore[2][valCount+1]].x));
                            valCount += 3;
                        }
                    }
                    finalC.push_back(transform(valStorePara,valStoreTN));
                    valStoreTN.clear();
                    break;
                case EXTEND:
                    if(valStore[0].size() == 3)
                        finalC.push_back(std::vector<para>({extend(finalC[valStore[0][0]][valStore[0][1]],valStore[0][2])}));
                    if(valStore[0].size() == 4)
                        finalC.push_back(std::vector<para>({extend(finalC[valStore[0][0]][valStore[0][1]],valStore[0][2],valStore[0][3])}));
                    if(valStore[0].size() == 7){
                        if(valStore[0][4])
                            finalC.push_back(std::vector<para>({extend(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]].y,quickTransform(valStore[0][5],valStore[0][6]))}));
                        else
                            finalC.push_back(std::vector<para>({extend(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]].x,quickTransform(valStore[0][5],valStore[0][6]))}));
                    }
                    break;
                case TANGENT:
                    finalC.push_back(std::vector<para>({tangent(finalC[valStore[0][0]][valStore[0][1]],valStore[0][2],finalC[valStore[0][3]][valStore[0][4]],valStore[0][5])}));
                    break;
                case SWARP:
                    finalC.push_back(std::vector<para>({singleWarp(finalC[valStore[0][0]][valStore[0][1]],valStorePara,point(valStore[2][0],valStore[2][1]),point(valStore[2][2],valStore[2][3]))}));
                    break;
                case DWARP:
                    finalC.push_back(std::vector<para>({doubleWarp(valStore[0][2] ? finalC[valStore[0][0]][valStore[0][1]].slice(1,0) : finalC[valStore[0][0]][valStore[0][1]],valStore[0][5] ? finalC[valStore[0][3]][valStore[0][4]].slice(1,0) : finalC[valStore[0][3]][valStore[0][4]],valStorePara,point(valStore[2][0],valStore[2][1]),point(valStore[2][2],valStore[2][3]))}));
                    break;
                case QWARP:
                    finalC.push_back(std::vector<para>({quadWarp(valStore[0][2] ? finalC[valStore[0][0]][valStore[0][1]].slice(1,0) : finalC[valStore[0][0]][valStore[0][1]],valStore[0][5] ? finalC[valStore[0][3]][valStore[0][4]].slice(1,0) : finalC[valStore[0][3]][valStore[0][4]],valStore[0][8] ? finalC[valStore[0][6]][valStore[0][7]].slice(1,0) : finalC[valStore[0][6]][valStore[0][7]],valStore[0][11] ? finalC[valStore[0][9]][valStore[0][10]].slice(1,0) : finalC[valStore[0][9]][valStore[0][10]],valStorePara,point(valStore[2][0],valStore[2][1]),point(valStore[2][2],valStore[2][3]))}));
                    break;
                case INTERSECTION:
                    if(valStore[0].size() == 6){
                        valStoreInter = findInters(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]]);
                        if(valStore[0][5] == 1)
                            finalC.push_back(std::vector<para>({finalC[valStore[0][0]][valStore[0][1]].slice(valStoreInter[valStore[0][4]].prog1,1)}));
                        else
                            finalC.push_back(std::vector<para>({finalC[valStore[0][0]][valStore[0][1]].slice(0,valStoreInter[valStore[0][4]].prog1)}));
                    }
                    if(valStore[0].size() == 8){
                        valStoreInter = findInters(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]]);
                        valStoreInter2 = findInters(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][5]][valStore[0][6]]);
                        finalC.push_back(std::vector<para>({finalC[valStore[0][0]][valStore[0][1]].slice(valStoreInter[valStore[0][4]].prog1,valStoreInter2[valStore[0][7]].prog1)}));
                        valStoreInter2.clear();
                    }
                    valStoreInter.clear();
                    break;
                case SELECT:
                    finalC.push_back(valStorePara);
                    break;
                case SPLIT:
                    finalC.push_back(std::vector<para>({finalC[valStore[0][0]][valStore[0][1]].slice(valStore[1][0],valStore[1][1])}));
                    break;
                case PRISM:
                    finalP.push_back(prism(valStorePara));
                    break;
                case PSWARP:
                    if(valStore[3].size() == 4)
                        finalC.push_back(pointSlopeWarp(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]],valStorePara,point(valStore[2][0],valStore[2][1]),point(valStore[2][2],valStore[2][3]),bounds(valStore[3][0],valStore[3][1],valStore[3][2],valStore[3][3])));
                    else
                        finalC.push_back(pointSlopeWarp(finalC[valStore[0][0]][valStore[0][1]],finalC[valStore[0][2]][valStore[0][3]],valStorePara,point(valStore[2][0],valStore[2][1]),point(valStore[2][2],valStore[2][3]),finalP[valStore[3][0]]));
                    break;
                case CIRC:
                    if(valStore[2].size() == 4)
                        finalC.push_back(circPoints(valStorePoint,valStore[1][0],bounds(valStore[2][0],valStore[2][1],valStore[2][2],valStore[2][3])));
                    else
                        finalC.push_back(circPoints(valStorePoint,valStore[1][0],finalP[valStore[2][0]]));
                    break;
                case SC_SOLID:
                    finalS.push_back(screen(fullColor(valStore[0][0],valStore[0][1],valStore[0][2],false)));
                    break;
                case SC_GRAD:
                    finalS.push_back(screen(fullColor(valStore[0][0],valStore[0][1],valStore[0][2],false),fullColor(valStore[1][0],valStore[1][1],valStore[1][2],false),point(valStore[2][0],valStore[2][1]),valStore[2][2],valStore[2][3]));
                    break;
                case SC_IMG:
                    finalS.push_back(screen(matrix(vh)));
                    break;
                case RENDCURVE:
                    if(!boundsDef){
                        sampBounds = finalC[valStore[0][0]][0].limits;
                        boundsDef = true;
                    }
                    for(int j=0;j<valStore[0].size();j++){
                        finalRO.push_back(std::vector<double>({0,valStore[0][j],valStore[1][0]}));
                        if(valStore.size() == 3)
                            finalRO[finalRO.size()-1].push_back(valStore[2][0]);
                        for(int k=0;k<finalC[valStore[0][j]].size();k++){
                            finalC[valStore[0][j]][k].configure();
                            sampBounds.minX = (finalC[valStore[0][j]][k].limits.minX<sampBounds.minX) ? finalC[valStore[0][j]][k].limits.minX : sampBounds.minX;
                            sampBounds.minY = (finalC[valStore[0][j]][k].limits.minY<sampBounds.minY) ? finalC[valStore[0][j]][k].limits.minY : sampBounds.minY;
                            sampBounds.maxX = (finalC[valStore[0][j]][k].limits.maxX>sampBounds.maxX) ? finalC[valStore[0][j]][k].limits.maxX : sampBounds.maxX;
                            sampBounds.maxY = (finalC[valStore[0][j]][k].limits.maxY>sampBounds.maxY) ? finalC[valStore[0][j]][k].limits.maxY : sampBounds.maxY;
                        }
                    }
                    break;
                case RENDPRIS:
                    if(!boundsDef){
                        sampBounds = finalP[valStore[0][0]].limits;
                        boundsDef = true;
                    }
                    for(int j=0;j<valStore[0].size();j++){
                        finalRO.push_back(std::vector<double>({1,valStore[0][j],valStore[1][0]}));
                        sampBounds.minX = (finalP[valStore[0][j]].limits.minX<sampBounds.minX) ? finalP[valStore[0][j]].limits.minX : sampBounds.minX;
                        sampBounds.minY = (finalP[valStore[0][j]].limits.minY<sampBounds.minY) ? finalP[valStore[0][j]].limits.minY : sampBounds.minY;
                        sampBounds.maxX = (finalP[valStore[0][j]].limits.maxX>sampBounds.maxX) ? finalP[valStore[0][j]].limits.maxX : sampBounds.maxX;
                        sampBounds.maxY = (finalP[valStore[0][j]].limits.maxY>sampBounds.maxY) ? finalP[valStore[0][j]].limits.maxY : sampBounds.maxY;
                    }
                    break;
                case DIMENSIONS:
                    finalD = point(valStore[0][0],valStore[0][1]);
                    break;
                case RESNAME:
                    finalN = vh;
                    break;
                case STROKE:
                    for(int j=0;j<valStore[1].size();j++){
                        if(valStore[1][j])
                            valStoreInt.push_back(int(valStore[1][j]));
                    }
                    finalST.push_back(stroke(strokeTypes(valStore[0][0]), valStore[0][1], valStore[0][2], valStore[0][3], valStore[0][5], valStore[0][4], valStore[0][6], valStoreInt));
                    valStoreInt.clear();
                    break;
                case STRGROUP:
                    for(int j=0;j<valStore[0].size();j++)
                        valStoreInt.push_back(int(valStore[0][j]));
                    finalSG.push_back(valStoreInt);
                    valStoreInt.clear();
                    break;
                case LAYOUT:
                    loadLevelsSuff(false);
                    while(levelInd<levels.size() and !foundName){
                        if(levels[levelInd].getPropString("k2") == vh){
                            foundName = true;
                            finalLB = levels[levelInd].getPropString("k4");
                        }
                        levelInd++;
                    }
                    break;
                case FILL_SOL:
                    for(int j=0;j<valStore[1].size();j++)
                        valStoreInt.push_back(valStore[1][j]);
                    finalFI.push_back(filler(valStore[0][0],int(valStore[0][1]),valStore[0][2],int(valStore[0][3]),int(valStore[0][4]),layers(int(valStore[0][5])),int(valStore[0][6]), valStoreInt));
                    valStoreInt.clear();
                    break;
                case FILL_GRAD:
                    for(int j=0;j<valStore[1].size();j++)
                        valStoreInt.push_back(valStore[1][j]);
                    finalFI.push_back(filler(fullColor(valStore[2][0],valStore[2][1],valStore[2][2],false), fullColor(valStore[3][0],valStore[3][1],valStore[3][2],false), valStore[0][0],int(valStore[0][1]),valStore[0][2],int(valStore[0][3]),int(valStore[0][4]),layers(int(valStore[0][5])),int(valStore[0][6]),valStoreInt));
                    valStoreInt.clear();
                    break;
                }
                if(func == BEZIER or func == BSPLINE or func == CIRC)
                    valStorePoint.clear();
                if(func == TRANSFORM or func == SWARP or func == DWARP or func == QWARP or func == PRISM or func == PSWARP or func == SELECT)
                    valStorePara.clear();
                if(func == SC_IMG or func == RESNAME or func == LAYOUT)
                    vh = "";
                func = NONE_SF;
            }
        }
        if(s[i] == ']')
            read = true;
    }
    if(!finalD.x or !finalD.y){
        sampBounds.minX -= 10;
        sampBounds.minY -= 10;
        sampBounds.maxX += 10;
        sampBounds.maxY += 10;
        finalD = point(ceil(sampBounds.maxX-sampBounds.minX),ceil(sampBounds.maxY-sampBounds.minY));
        for(int i=0;i<finalRO.size();i++){
            if(finalRO[i][0]){
                for(int j=0;j<finalP[finalRO[i][1]].sides.size();j++)
                    valStorePara.push_back(finalP[finalRO[i][1]].sides[j].transform(transformNode(TRANSLATE,-sampBounds.minX,-sampBounds.minY)));
                finalP[finalRO[i][1]] = prism(valStorePara);
                valStorePara.clear();
            }
            else{
                for(int j=0;j<finalC[finalRO[i][1]].size();j++)
                    finalC[finalRO[i][1]][j] = finalC[finalRO[i][1]][j].transform(transformNode(TRANSLATE,-sampBounds.minX,-sampBounds.minY));
            }
        }
    }
    return snCache(finalC,finalP,finalS,finalRO,finalD,finalN,finalST,finalSG,finalLB,finalFI);
}

/*
* pm - Parametric render filetypes. Either an image or a GD level.
*/
enum pmFileTypes{
    PM_IMG,
    PM_GD,
};

/*
* pmRender - Takes a parametric render file, parses it, and renders the result.
*/
void pmRender(std::string fn){
    snCache a = stringNotation(fileCont(fn));
    pmFileTypes fileType;
    std::string fvh;
    for(int i=fn.size()-3;i<fn.size();i++)
        fvh += fn[i];
    if(fvh == "gpm")
        fileType = PM_IMG;
    else if(fvh == "dpm")
        fileType = PM_GD;
    std::vector<std::vector<pixel>> mat;
    std::vector<para> subC;
    prism subP;
    fillSquare subSq;
    curve subCu;
    std::vector<stroke> subSg;
    std::vector<block> objects;
    std::string subNa;
    figure subFi;
    switch(fileType){
    case PM_IMG: // For image types(.gpm files), a .png image is rendered.
        for(int i=0;i<a.dimensions.y;i++){
            mat.push_back(std::vector<pixel>({}));
            for(int j=0;j<a.dimensions.x;j++)
                mat[i].push_back(a.screens[0].getCol(point(j,a.dimensions.y-i-1)));
        }
        for(int i=0;i<a.rendOrder.size();i++){ // For image renders, a specific rending order is used to ensure that layers are represented properly.
            if(a.rendOrder[i][0]){ // A boolean is used to distinguish between prisms and curves.
                subP = a.prisms[a.rendOrder[i][1]];
                subP.fill(); // The default fill algorithm is used, and every pixel included uses its respective screen rules when recoloring.
                for(int j=0;j<subP.squares.size();j++){
                    subSq = subP.squares[j];
                    for(int k=subSq.pos.x-subSq.s/2;k<=subSq.pos.x+subSq.s/2;k++){
                        for(int m=subSq.pos.y-subSq.s/2;m<=subSq.pos.y+subSq.s/2;m++){
                            if(m >= 0 and m < a.dimensions.y and k >= 0 and k < a.dimensions.x)
                                mat[a.dimensions.y-m-1][k] = a.screens[a.rendOrder[i][2]].getCol(point(k/a.dimensions.x,(a.dimensions.y-m-1)/a.dimensions.y));
                        }
                    }
                }
            }
            else{
                subC = a.curves[a.rendOrder[i][1]];
                for(int j=0;j<subC.size();j++){
                    subC[j].generatePixels(a.rendOrder[i][3]); // For curves, the generatePixels algorithm is used, and the pixels are recolored depending on the specified magnitude.
                    for(int k=0;k<subC[j].linePixels.size();k++){
                        if(subC[j].linePixels[k].y >= 0 and subC[j].linePixels[k].y < a.dimensions.y and subC[j].linePixels[k].x >= 0 and subC[j].linePixels[k].x < a.dimensions.x)
                            mat[a.dimensions.y-subC[j].linePixels[k].y-1][subC[j].linePixels[k].x] += pixel(a.screens[a.rendOrder[i][2]].getCol(point(subC[j].linePixels[k].x,a.dimensions.y-subC[j].linePixels[k].y-1)), static_cast<unsigned char>(255*subC[j].linePixels[k].mag));
                    }
                }
            }
        }
        image(mat, a.name+".png");
        break;
    case PM_GD:
        for(int i=0;i<a.rendOrder.size();i++){ // For GD renders, the rending order doesnt necessarily matter, as everything already has custom layers, however, it is still implemented as a way to keep things organized.
            if(a.rendOrder[i][0]){
                subFi = figure(a.prisms[a.rendOrder[i][1]], a.fillers[a.rendOrder[i][2]]); // Figures create their objects based on their filler type.
                subFi.createObjects();
                for(int j=0;j<subFi.objects.size();j++)
                    objects.push_back(subFi.objects[j]);
            }
            else{
                subSg.clear();
                for(int j=0;j<a.strokeGroups[a.rendOrder[i][2]].size();j++)
                    subSg.push_back(a.strokes[a.strokeGroups[a.rendOrder[i][2]][j]]);
                subCu = curve(a.curves[a.rendOrder[i][1]],subSg);
                subCu.configure();
                for(int j=0;j<subCu.objects.size();j++)
                    objects.push_back(subCu.objects[j]);
            }
        }
        subNa = "";
        for(int i=0;i<a.name.size();i++){
            if(a.name[i] == '_')
                subNa += " ";
            else
                subNa += a.name[i];
        }
        if(a.layoutBase.size())
            inject(getLevString(objects,levelStringDecode(a.layoutBase)), a.name);
        else
            inject(objects, a.name);
        break;
    }
}