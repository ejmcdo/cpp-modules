/*
* You know that game you played on your phone 10 years ago? The one with the cube and the spikes? This is a module that allows manipulation of its objects.
*/
#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <zlib-handle.h>
#include <b64.h>
#include <file-handle.h>
#include <string-handle.h>

// propType - All the different types of properties that an object can have.
enum propType {
    INT_PROP,
    DOUBLE_PROP,
    BOOL_PROP,
    STRING_PROP,
    HSV_PROP,
    INTVEC,
    PARTICLE,
};

// printIntV - Takes an integer vector and converts it to a storable string
std::string printIntV(std::vector<int> a) {
    std::string final;
    for (int i = 0; i < a.size(); i++) {
        final += int2Str(a[i]);
        if (i < a.size() - 1)
            final += ".";
    }
    return final;
}

// printDoubleV - Takes a double vector and converts it to a storable string.
std::string printDoubleV(std::vector<double> a) {
    std::string final;
    for (int i = 0; i < a.size(); i++) {
        final += doub2Str(a[i]);
        if (i < a.size() - 1)
            final += "a";
    }
    return final;
}

// hsv - Represents an hsv modification used on an object.
struct hsv {
    int hue = 0;
    double sat = 1;
    double val = 1;
    bool addSat = 0;
    bool addVal = 0;
    hsv() {}
    hsv(std::vector<double> f) :hue(int(f[0])), sat(f[1]), val(f[2]), addSat(f[3]), addVal(f[4]) {}
    std::string getString() {
        return printDoubleV(std::vector<double>({ double(hue),sat,val,double(addSat),double(addVal) }));
    }
    void print() {
        std::cout << hue << " " << sat << " " << val << " " << addSat << " " << addVal << std::endl;
    }
    bool equal(hsv x) {
        return (hue == x.hue&&sat == x.sat&&val == x.val&&addSat == x.addSat&&addVal == x.addVal);
    }
    void reset() {
        hue = 0;
        sat = 1;
        val = 1;
        addSat = false;
        addVal = false;
    }
};

// propSpec - Represents a property's name and type.
struct propSpec {
    std::string name;
    propType type = INT_PROP;
    propSpec() {}
    propSpec(std::string n, propType t) : name(n), type(t) {}
};

// parseIntV - Takes a string for an integer vector and parses it.
std::vector<int> parseIntV(std::string a) {
    std::vector<int> final;
    std::string vh;
    for (int i = 0; i < a.size() + 1; i++) {
        if (a[(i < int(a.size())) ? i : int(a.size())] == '.'||i == a.size()) {
            final.push_back(str2Int(vh));
            vh = "";
        }
        else
            vh += a[i];
    }
    return final;
}

// parseDoubleV - Takes a string for a double vector and parses it.
std::vector<double> parseDoubleV(std::string a) {
    std::vector<double> final;
    std::string vh;
    for (int i = 0; i < a.size() + 1; i++) {
        if (a[(i < int(a.size())) ? i : int(a.size())] == 'a'||i == a.size()) {
            final.push_back(str2Doub(vh));
            vh = "";
        }
        else
            vh += a[i];
    }
    return final;
}

// All the properties that currently exist. There's probably a better way to do this, but oh well.
std::vector<propSpec> propSpecs = std::vector<propSpec>({
    propSpec("id", INT_PROP), // 1
    propSpec("x", DOUBLE_PROP),
    propSpec("y", DOUBLE_PROP),
    propSpec("hFlip", BOOL_PROP),
    propSpec("vFlip", BOOL_PROP),
    propSpec("rotation", DOUBLE_PROP),
    propSpec("red", INT_PROP),
    propSpec("green", INT_PROP),
    propSpec("blue", INT_PROP),
    propSpec("duration", DOUBLE_PROP),
    propSpec("touch triggered", BOOL_PROP), // 11
    propSpec("coin id", INT_PROP),
    propSpec("edp", BOOL_PROP),
    propSpec("tint ground", BOOL_PROP),
    propSpec("player color 1", BOOL_PROP),
    propSpec("player color 2", BOOL_PROP),
    propSpec("blending", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("1.9 color", INT_PROP),
    propSpec("editor level", INT_PROP),
    propSpec("color 1", INT_PROP), // 21
    propSpec("color 2", INT_PROP),
    propSpec("target color", INT_PROP),
    propSpec("z layer", INT_PROP),
    propSpec("z order", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("offsetX", INT_PROP),
    propSpec("offsetY", INT_PROP),
    propSpec("easing", INT_PROP),
    propSpec("text", STRING_PROP), // 31
    propSpec("scale", DOUBLE_PROP),
    propSpec("single group", INT_PROP),
    propSpec("group parent", INT_PROP),
    propSpec("opacity", DOUBLE_PROP),
    propSpec("interactable", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("col1 hsv enabled", BOOL_PROP), // 41
    propSpec("col2 hsv enabled", BOOL_PROP),
    propSpec("col1 hsv", HSV_PROP),
    propSpec("col2 hsv", HSV_PROP),
    propSpec("fade in", DOUBLE_PROP),
    propSpec("hold", DOUBLE_PROP),
    propSpec("fade out", DOUBLE_PROP),
    propSpec("hsv enabled", BOOL_PROP),
    propSpec("hsv", HSV_PROP),
    propSpec("copy color", INT_PROP),
    propSpec("group id", INT_PROP), // 51
    propSpec("group mode", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("orange teleportation portal distance", DOUBLE_PROP),
    propSpec("smooth ease", BOOL_PROP),
    propSpec("enable group", BOOL_PROP),
    propSpec("groups", INTVEC),
    propSpec("lock x(player)", BOOL_PROP),
    propSpec("lock y(player)", BOOL_PROP),
    propSpec("copy opacity", BOOL_PROP),
    propSpec("editor layer 2", INT_PROP), // 61
    propSpec("spawn triggered", BOOL_PROP),
    propSpec("delay(spawn)", DOUBLE_PROP),
    propSpec("dont fade", BOOL_PROP),
    propSpec("main only", BOOL_PROP),
    propSpec("detail only", BOOL_PROP),
    propSpec("dont enter", BOOL_PROP),
    propSpec("degrees", DOUBLE_PROP),
    propSpec("x360", INT_PROP),
    propSpec("lock obj rot", BOOL_PROP),
    propSpec("center id", INT_PROP), // 71
    propSpec("x mod", DOUBLE_PROP),
    propSpec("y mod", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("strength", DOUBLE_PROP),
    propSpec("animation id", INT_PROP),
    propSpec("target count", INT_PROP),
    propSpec("sub count", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("item id", INT_PROP),
    propSpec("item id", BOOL_PROP), // 81
    propSpec("toggle on/off", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("interval", DOUBLE_PROP),
    propSpec("easing rate", DOUBLE_PROP),
    propSpec("exclusive", BOOL_PROP),
    propSpec("multi triggered", BOOL_PROP),
    propSpec("equals/smaller/larger", INT_PROP),
    propSpec("dual mode", BOOL_PROP),
    propSpec("speed(follow player y)", DOUBLE_PROP),
    propSpec("delay(follow player y)", DOUBLE_PROP), // 91
    propSpec("offset(follow player y)", DOUBLE_PROP),
    propSpec("trigger on exit", BOOL_PROP),
    propSpec("dynamic block", BOOL_PROP),
    propSpec("block b id", INT_PROP),
    propSpec("no glow", BOOL_PROP),
    propSpec("rotation speed", DOUBLE_PROP),
    propSpec("disable rotation", BOOL_PROP),
    propSpec("multi activate option", BOOL_PROP),
    propSpec("aim mode", BOOL_PROP),
    propSpec("x/y mode", INT_PROP), // 101
    propSpec("preview disable", BOOL_PROP),
    propSpec("high detail", BOOL_PROP),
    propSpec("multi activate", BOOL_PROP),
    propSpec("max speed", DOUBLE_PROP),
    propSpec("randomize start", BOOL_PROP),
    propSpec("speed", DOUBLE_PROP),
    propSpec("linked group", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("exit static", BOOL_PROP),
    propSpec("free mode", BOOL_PROP), // 111
    propSpec("edit camera settings", BOOL_PROP),
    propSpec("easing", INT_PROP),
    propSpec("padding", DOUBLE_PROP),
    propSpec("ord", INT_PROP),
    propSpec("no effects", BOOL_PROP),
    propSpec("reverse", BOOL_PROP),
    propSpec("reversed", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("time mod", DOUBLE_PROP),
    propSpec("no touch", BOOL_PROP), // 121
    propSpec("use speed", BOOL_PROP),
    propSpec("animate on trigger", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("disable delayed loop", BOOL_PROP),
    propSpec("disable animshine", BOOL_PROP),
    propSpec("scale X", DOUBLE_PROP),
    propSpec("scale Y", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("warp Y angle", DOUBLE_PROP), // 131
    propSpec("warp X angle", DOUBLE_PROP),
    propSpec("only move", BOOL_PROP),
    propSpec("passable", BOOL_PROP),
    propSpec("hide", BOOL_PROP),
    propSpec("non stick X", BOOL_PROP),
    propSpec("ice block", BOOL_PROP),
    propSpec("P1", BOOL_PROP),
    propSpec("override", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("lock X camera", BOOL_PROP), // 141
    propSpec("lock Y camera", BOOL_PROP),
    propSpec("modX", DOUBLE_PROP),
    propSpec("modY", DOUBLE_PROP),
    propSpec("particle properties", PARTICLE),
    propSpec("use obj color", BOOL_PROP),
    propSpec("uniform obj color", BOOL_PROP),
    propSpec("gravity", DOUBLE_PROP),
    propSpec("force", DOUBLE_PROP),
    propSpec("scale X", DOUBLE_PROP),
    propSpec("scale Y", DOUBLE_PROP), // 151
    propSpec("list", INTVEC),
    propSpec("div X", BOOL_PROP),
    propSpec("div Y", BOOL_PROP),
    propSpec("color 1 index", INT_PROP),
    propSpec("color 2 index", INT_PROP),
    propSpec("reference only", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("streak additive", INT_PROP),
    propSpec("unlink dual gravity", INT_PROP),
    propSpec("hide ground", INT_PROP), // 161
    propSpec("hide p1", INT_PROP),
    propSpec("hide p2", INT_PROP),
    propSpec("edge", INT_PROP),
    propSpec("disable p1 controls", INT_PROP),
    propSpec("gravity dir", INT_PROP),
    propSpec("arrow dir", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("edit velocity", BOOL_PROP),
    propSpec("channel", INT_PROP),
    propSpec("change channel", BOOL_PROP), // 171
    propSpec("channel only", BOOL_PROP),
    propSpec("target channel", INT_PROP),
    propSpec("blending", INT_PROP),
    propSpec("speed", DOUBLE_PROP),
    propSpec("strength", DOUBLE_PROP),
    propSpec("time off", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("wave w", DOUBLE_PROP),
    propSpec("thickness", DOUBLE_PROP),
    propSpec("fade", DOUBLE_PROP), // 181
    propSpec("fade out", DOUBLE_PROP),
    propSpec("inner", DOUBLE_PROP),
    propSpec("invert", DOUBLE_PROP),
    propSpec("flip", BOOL_PROP),
    propSpec("rotate", BOOL_PROP),
    propSpec("dual", BOOL_PROP),
    propSpec("target", BOOL_PROP),
    propSpec("target Y", DOUBLE_PROP),
    propSpec("use Y", BOOL_PROP),
    propSpec("outer", DOUBLE_PROP), // 191
    propSpec("disable all", BOOL_PROP),
    propSpec("grip slope", BOOL_PROP),
    propSpec("relative pos", BOOL_PROP),
    propSpec("hide mg", INT_PROP),
    propSpec("lowest layer", INT_PROP),
    propSpec("highest layer", INT_PROP),
    propSpec("p1/p2 only", INT_PROP),
    propSpec("disable P2 controls", INT_PROP),
    propSpec("P2", BOOL_PROP),
    propSpec("follow C", BOOL_PROP), // 201
    propSpec("layer", INT_PROP),
    propSpec("U/BL", INT_PROP),
    propSpec("D/BR", INT_PROP),
    propSpec("L/TL", INT_PROP),
    propSpec("R/TR", INT_PROP),
    propSpec("vertex mode", BOOL_PROP),
    propSpec("disable", BOOL_PROP),
    propSpec("id", INT_PROP),
    propSpec("legacy hsv off", BOOL_PROP),
    propSpec("quick start", BOOL_PROP), // 211
    propSpec("follow", BOOL_PROP),
    propSpec("follow easing", DOUBLE_PROP),
    propSpec("only if active", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("enter/exit only", INT_PROP),
    propSpec("move dist", INT_PROP),
    propSpec("move dist +-", INT_PROP),
    propSpec("offset", INT_PROP),
    propSpec("offset +-", INT_PROP), // 221
    propSpec("length", INT_PROP),
    propSpec("length +-", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("effect id", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("move angle", INT_PROP), // 231
    propSpec("move angle +-", INT_PROP),
    propSpec("scale X", DOUBLE_PROP),
    propSpec("scale X +-", DOUBLE_PROP),
    propSpec("scale Y", DOUBLE_PROP),
    propSpec("scale Y +-", DOUBLE_PROP),
    propSpec("moveX", INT_PROP),
    propSpec("moveX +-", INT_PROP),
    propSpec("moveY", INT_PROP),
    propSpec("moveY +-", INT_PROP),
    propSpec("x/y mode", BOOL_PROP), // 241
    propSpec("easing", INT_PROP),
    propSpec("easing rate", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("easing2", INT_PROP),
    propSpec("easing2 rate", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP), // 251
    propSpec("offset Y", INT_PROP),
    propSpec("offset Y +-", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("color channel", INT_PROP),
    propSpec("ease out", BOOL_PROP), // 261
    propSpec("dir buttons DIR", INT_PROP),
    propSpec("mod front", DOUBLE_PROP),
    propSpec("mod back", DOUBLE_PROP),
    propSpec("tint", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("rotation", DOUBLE_PROP),
    propSpec("rotation +-", DOUBLE_PROP), // 271
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("group parent ids", INTVEC),
    propSpec("to opacity", DOUBLE_PROP),
    propSpec("dir buttons INWARDS", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("hsv on", BOOL_PROP),
    propSpec("area parent", BOOL_PROP),
    propSpec("ignore gparent", BOOL_PROP),
    propSpec("ignore linked", BOOL_PROP), // 281
    propSpec("deadzone", DOUBLE_PROP),
    propSpec("dir buttons SINGLE ARROW", BOOL_PROP),
    propSpec("single ptouch", BOOL_PROP),
    propSpec("UNKNOWN", DOUBLE_PROP),
    propSpec("from opacity", DOUBLE_PROP),
    propSpec("relative", BOOL_PROP),
    propSpec("rfade", DOUBLE_PROP),
    propSpec("non stick y", BOOL_PROP),
    propSpec("screen off X", DOUBLE_PROP),
    propSpec("screen off Y", DOUBLE_PROP), // 291
    propSpec("delay", DOUBLE_PROP),
    propSpec("delay +-", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("max speed", DOUBLE_PROP),
    propSpec("max speed +-", DOUBLE_PROP),
    propSpec("start speed", DOUBLE_PROP),
    propSpec("start speed +-", DOUBLE_PROP), // 301
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("target dir", BOOL_PROP),
    propSpec("x only", BOOL_PROP),
    propSpec("y only", BOOL_PROP),
    propSpec("max range", INT_PROP),
    propSpec("max range +-", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP), // 311
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("steer force", DOUBLE_PROP),
    propSpec("steer force +-", DOUBLE_PROP),
    propSpec("steer force low", DOUBLE_PROP),
    propSpec("steer force low +-", DOUBLE_PROP),
    propSpec("steer force high", DOUBLE_PROP),
    propSpec("steer force high +-", DOUBLE_PROP), // 321
    propSpec("speed range low", DOUBLE_PROP),
    propSpec("speed range low +-", DOUBLE_PROP),
    propSpec("speed range high", DOUBLE_PROP),
    propSpec("speed range high +-", DOUBLE_PROP),
    propSpec("break force", DOUBLE_PROP),
    propSpec("break force +-", DOUBLE_PROP),
    propSpec("break angle", DOUBLE_PROP),
    propSpec("break angle +-", DOUBLE_PROP),
    propSpec("break steer force", DOUBLE_PROP),
    propSpec("break steer force +-", DOUBLE_PROP), // 331
    propSpec("break steer speed limit", DOUBLE_PROP),
    propSpec("break steer speed limit +-", DOUBLE_PROP),
    propSpec("acceleration", DOUBLE_PROP),
    propSpec("acceleration +-", DOUBLE_PROP),
    propSpec("ignore disabled", BOOL_PROP),
    propSpec("steer force low checkbox", BOOL_PROP),
    propSpec("steer force high checkbox", BOOL_PROP),
    propSpec("rotate dir", BOOL_PROP),
    propSpec("rot offset", INT_PROP),
    propSpec("priority", INT_PROP), // 341
    propSpec("", INT_PROP),
    propSpec("enter channel", INT_PROP),
    propSpec("target enter channel", INT_PROP),
    propSpec("static force", BOOL_PROP),
    propSpec("static force value", DOUBLE_PROP),
    propSpec("redirect force", BOOL_PROP),
    propSpec("redirect force min", DOUBLE_PROP),
    propSpec("redirect force max", DOUBLE_PROP),
    propSpec("redirect force mod", DOUBLE_PROP),
    propSpec("save offset", BOOL_PROP), // 351
    propSpec("ignore x", BOOL_PROP),
    propSpec("ignore y", BOOL_PROP),
    propSpec("exit gravity", INT_PROP),
    propSpec("use eid", BOOL_PROP),
    propSpec("scale stick", BOOL_PROP),
    propSpec("near accel", DOUBLE_PROP),
    propSpec("near accel +-", DOUBLE_PROP),
    propSpec("near dist", DOUBLE_PROP),
    propSpec("slow dist", DOUBLE_PROP),
    propSpec("easing", DOUBLE_PROP), // 361
    propSpec("easing +-", DOUBLE_PROP),
    propSpec("rot easing", INT_PROP),
    propSpec("rot deadZ", INT_PROP),
    propSpec("priority", INT_PROP),
    propSpec("max range unmarked?", INT_PROP),
    propSpec("page 2 1/2/3", INT_PROP),
    propSpec("instant offset", BOOL_PROP),
    propSpec("center effect", BOOL_PROP),
    propSpec("disable gridsnap", BOOL_PROP),
    propSpec("zoom", DOUBLE_PROP), // 371
    propSpec("no audio scale", BOOL_PROP),
    propSpec("anim id?", INT_PROP),
    propSpec("order index", INT_PROP),
    propSpec("ref only", BOOL_PROP),
    propSpec("close loop", BOOL_PROP),
    propSpec("prox", BOOL_PROP),
    propSpec("curve", BOOL_PROP),
    propSpec("time/even/dist", INT_PROP),
    propSpec("preview art", BOOL_PROP),
    propSpec("pickup item", BOOL_PROP), // 381
    propSpec("toggle trigger", BOOL_PROP),
    propSpec("points", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("xref pos", INT_PROP),
    propSpec("yref pos", INT_PROP),
    propSpec("xref relative", BOOL_PROP),
    propSpec("yref relative", BOOL_PROP),
    propSpec("seconds only", BOOL_PROP),
    propSpec("maintimes/attempts/points", INT_PROP),
    propSpec("align", INT_PROP), // 391
    propSpec("song", INT_PROP),
    propSpec("small step", BOOL_PROP),
    propSpec("direction mode", BOOL_PROP),
    propSpec("target/dir mode centerGroup", INT_PROP),
    propSpec("direction mode distance", INT_PROP),
    propSpec("dynamic mode", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("prep", BOOL_PROP),
    propSpec("load prep", BOOL_PROP),
    propSpec("rot target id", INT_PROP), // 401
    propSpec("rot offset", DOUBLE_PROP),
    propSpec("dynamic mode easing", INT_PROP),
    propSpec("speed", INT_PROP),
    propSpec("pitch", INT_PROP),
    propSpec("volume", INT_PROP),
    propSpec("reverb", BOOL_PROP),
    propSpec("start", INT_PROP),
    propSpec("fade in", INT_PROP),
    propSpec("end", INT_PROP),
    propSpec("fade out", INT_PROP), // 411
    propSpec("FFT", BOOL_PROP),
    propSpec("loop", BOOL_PROP),
    propSpec("stop loop", BOOL_PROP),
    propSpec("is unique", BOOL_PROP),
    propSpec("unique id", INT_PROP),
    propSpec("stop", BOOL_PROP),
    propSpec("change volume", BOOL_PROP),
    propSpec("change speed", BOOL_PROP),
    propSpec("override", BOOL_PROP),
    propSpec("vol near", DOUBLE_PROP), // 421
    propSpec("vol med", DOUBLE_PROP),
    propSpec("vol far", DOUBLE_PROP),
    propSpec("min dist", INT_PROP),
    propSpec("dist 2", INT_PROP),
    propSpec("dist 3", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("CAM", BOOL_PROP),
    propSpec("", INT_PROP),
    propSpec("events", INTVEC),
    propSpec("", INT_PROP), // 431
    propSpec("channel", INT_PROP),
    propSpec("pre load", BOOL_PROP),
    propSpec("min interval", DOUBLE_PROP),
    propSpec("sequence", INTVEC),
    propSpec("mode", INT_PROP),
    propSpec("min int", DOUBLE_PROP),
    propSpec("reset", DOUBLE_PROP),
    propSpec("reset full/step", BOOL_PROP),
    propSpec("particle", INT_PROP),
    propSpec("spawn ordered", BOOL_PROP), // 441
    propSpec("group remaps", INTVEC),
    propSpec("static force additive", BOOL_PROP),
    propSpec("no multi activate(plat)", BOOL_PROP),
    propSpec("claim touch", BOOL_PROP),
    propSpec("material", INT_PROP),
    propSpec("extra id", INT_PROP),
    propSpec("respawn id", INT_PROP),
    propSpec("modifier", DOUBLE_PROP),
    propSpec("", INT_PROP),
    propSpec("", INT_PROP), // 451
    propSpec("relative rot", BOOL_PROP),
    propSpec("smooth velocity", BOOL_PROP),
    propSpec("smooth velocity modifier", DOUBLE_PROP),
    propSpec("sfx group", INT_PROP),
    propSpec("preview opacity", DOUBLE_PROP),
    propSpec("group id", INT_PROP),
    propSpec("dir buttons", INT_PROP),
    propSpec("auto layer", BOOL_PROP),
    propSpec("no effects", BOOL_PROP),
    propSpec("no sfx", BOOL_PROP), // 461
    propSpec("single frame", INT_PROP),
    propSpec("no anim", BOOL_PROP),
    propSpec("instant camera", BOOL_PROP),
    propSpec("exit instant", BOOL_PROP),
    propSpec("time counter", BOOL_PROP),
    propSpec("start time", DOUBLE_PROP),
    propSpec("dont override", BOOL_PROP),
    propSpec("ignore timewarp", BOOL_PROP),
    propSpec("time mod", DOUBLE_PROP),
    propSpec("start paused", BOOL_PROP), // 471
    propSpec("start/stop", BOOL_PROP),
    propSpec("stop time", DOUBLE_PROP),
    propSpec("stop checked", BOOL_PROP),
    propSpec("multi activate", BOOL_PROP),
    propSpec("typ 1", INT_PROP),
    propSpec("typ 2", INT_PROP),
    propSpec("target typ", INT_PROP),
    propSpec("mod", DOUBLE_PROP),
    propSpec("assign op", INT_PROP),
    propSpec("op1", INT_PROP), // 481
    propSpec("op2", INT_PROP),
    propSpec("mod2", DOUBLE_PROP),
    propSpec("tol +-", DOUBLE_PROP),
    propSpec("rnd/flr/cei 1", INT_PROP),
    propSpec("rnd/flr/cei 2", INT_PROP),
    propSpec("instant", BOOL_PROP),
    propSpec("kerning", INT_PROP),
    propSpec("ignore volume test", BOOL_PROP),
    propSpec("sound duration", DOUBLE_PROP),
    propSpec("persistent", BOOL_PROP), // 491
    propSpec("target all", BOOL_PROP),
    propSpec("reset", BOOL_PROP),
    propSpec("timer", BOOL_PROP),
    propSpec("extra sticky", BOOL_PROP),
    propSpec("dont boost y", BOOL_PROP),
    propSpec("single color type", INT_PROP),
    propSpec("bpm", INT_PROP),
    propSpec("speed", INT_PROP),
    propSpec("disable", BOOL_PROP),
    propSpec("bpb", INT_PROP), // 501
    propSpec("reverb type", INT_PROP),
    propSpec("reverb enable", BOOL_PROP),
    propSpec("spawn only", BOOL_PROP),
    propSpec("UNKNOWN", BOOL_PROP),
    propSpec("preview opacity", DOUBLE_PROP),
    propSpec("no particle", BOOL_PROP),
    propSpec("disable all", BOOL_PROP),
    propSpec("dont boost x", BOOL_PROP),
    propSpec("snap ground", BOOL_PROP),
    propSpec("extended collision", BOOL_PROP), // 511
    propSpec("max size", DOUBLE_PROP),
    propSpec("animate", BOOL_PROP),
    propSpec("relative", BOOL_PROP),
    propSpec("empty only", BOOL_PROP),
    propSpec("minX id", INT_PROP),
    propSpec("minY id", INT_PROP),
    propSpec("maxX id", INT_PROP),
    propSpec("maxY id", INT_PROP),
    propSpec("time mod", DOUBLE_PROP),
    propSpec("position x mod", DOUBLE_PROP), // 521
    propSpec("rotation mod", DOUBLE_PROP),
    propSpec("scale x mod", DOUBLE_PROP),
    propSpec("line opacity", DOUBLE_PROP),
    propSpec("extra id 2", INT_PROP),
    propSpec("min force", DOUBLE_PROP),
    propSpec("max force", DOUBLE_PROP),
    propSpec("relative", BOOL_PROP),
    propSpec("range", BOOL_PROP),
    propSpec("force id", INT_PROP),
    propSpec("shader disable", BOOL_PROP), // 531
    propSpec("hide attempts", INT_PROP),
    propSpec("background", INT_PROP),
    propSpec("control id", INT_PROP),
    propSpec("target control id", BOOL_PROP),
    propSpec("cw/ccw", INT_PROP),
    propSpec("x360", INT_PROP),
    propSpec("special center", INT_PROP),
    propSpec("dont edit area parent", BOOL_PROP),
    propSpec("stop jump", BOOL_PROP),
    propSpec("stop move", BOOL_PROP), // 541
    propSpec("stop rot", BOOL_PROP),
    propSpec("stop slide", BOOL_PROP),
    propSpec("silent", BOOL_PROP),
    propSpec("position y mod", DOUBLE_PROP),
    propSpec("scale y mod", DOUBLE_PROP),
    propSpec("offset X", INT_PROP),
    propSpec("offset Y", INT_PROP),
    propSpec("offvar X", INT_PROP),
    propSpec("offvar Y", INT_PROP),
    propSpec("match rot", BOOL_PROP), // 551
    propSpec("rotation", INT_PROP),
    propSpec("rotation +-", INT_PROP),
    propSpec("scale", DOUBLE_PROP),
    propSpec("scale +-", DOUBLE_PROP),
    propSpec("delay +-", DOUBLE_PROP),
    propSpec("spawn delay", DOUBLE_PROP),
    propSpec("friction", DOUBLE_PROP),
    propSpec("friction +-", DOUBLE_PROP),
    propSpec("start speed unmarked?", BOOL_PROP),
    propSpec("near friction", DOUBLE_PROP), // 561
    propSpec("near friction +-", DOUBLE_PROP),
    propSpec("start dir", INT_PROP),
    propSpec("start dir +-", INT_PROP),
    propSpec("start dir unmarked?", INT_PROP),
    propSpec("mod X", DOUBLE_PROP),
    propSpec("mod X +-", DOUBLE_PROP),
    propSpec("mod Y", DOUBLE_PROP),
    propSpec("mod Y +-", DOUBLE_PROP),
    propSpec("redirect dir", BOOL_PROP),
    propSpec("exclusive", BOOL_PROP), // 571
    propSpec("init set add", INT_PROP),
    propSpec("edit respawn time", INT_PROP),
    propSpec("respawn time", DOUBLE_PROP),
    propSpec("audio on death", INT_PROP),
    propSpec("no death sfx", INT_PROP),
    propSpec("relative scale", BOOL_PROP),
    propSpec("abs/neg 1", INT_PROP),
    propSpec("abs/neg 2", INT_PROP),
    propSpec("stop/pause/resume", INT_PROP),
    propSpec("reset remap", BOOL_PROP), // 581
    propSpec("velocity mod x", DOUBLE_PROP),
    propSpec("velocity mod y", DOUBLE_PROP),
    propSpec("override velocity", BOOL_PROP),
    propSpec("dont slide", BOOL_PROP),
    propSpec("speed", DOUBLE_PROP),
    propSpec("allow collide", BOOL_PROP),
    propSpec("end boost", DOUBLE_PROP),
    propSpec("stop slide", BOOL_PROP),
    propSpec("max duration", DOUBLE_PROP),
    propSpec("redirect dash", BOOL_PROP), // 591
    propSpec("offset anim", BOOL_PROP),
    propSpec("boost slide", INT_PROP),
    propSpec("", INT_PROP),
    propSpec("dont reset", BOOL_PROP),
    propSpec("speed +-", INT_PROP),
    propSpec("pitch +-", INT_PROP),
    propSpec("volume +-", INT_PROP),
    propSpec("pitch steps", BOOL_PROP),
    });
//int highestProp = 0;

//particle - Represent a particle object's properties.
struct particle {
    std::vector<double> props;
    particle() {}
    particle(std::vector<double> p) :props(p) {}
    void print() {
        printDoubleV(props);
    }
};

// prop - Represents a single property an object can hold.
struct prop {
    propSpec spec; // Used to determine what data type the property is.
    int num = 0;
    int val0 = 0;
    double val1 = 0;
    bool val2 = false;
    std::string val3 = "";
    hsv val4 = hsv();
    std::vector<int> val5;
    particle val6 = particle();
    prop() {}
    prop(std::string n, std::string p) { // The most common case of property initialization is during level string parsing. Properties are stored as strings in the format "[id],[value]".
        if(n[0] != 'k'){ // Some special objects (e.g. start position) can contain properties that start with k.
            num = str2Int(n);
            spec = propSpecs[num - 1];
            switch (spec.type) { // By means of the propSpec, the propety value type is determined.
            case INT_PROP:
                val0 = str2Int(p);
                break;
            case DOUBLE_PROP:
                val1 = str2Doub(p);
                break;
            case BOOL_PROP:
                val2 = str2Int(p);
                break;
            case STRING_PROP:
                val3 = b64dec(p);
                break;
            case HSV_PROP:
                val4 = hsv(parseDoubleV(p));
                break;
            case INTVEC:
                val5 = parseIntV(p);
                break;
            case PARTICLE:
                val6 = particle(parseDoubleV(p));
                break;
            }
        }
    }

    // All properties are constructed with an ID and their value.
    prop(int n, int v) : num(n), spec(propSpecs[n - 1]) { // An integer could mean either a prop type of INT, DOUBLE, or BOOL. This constuctor differentiates between the three.
        switch (spec.type) {
        case INT_PROP:
            val0 = v;
            break;
        case DOUBLE_PROP:
            val1 = double(v);
            break;
        case BOOL_PROP:
            val2 = bool(v);
            break;
        }
    }
    prop(int n, double v) : num(n), spec(propSpecs[n - 1]) { // A double could mean either a prop type of DOUBLE, or BOOL. This constuctor differentiates between the two.
        switch (spec.type) {
        case INT_PROP:
            val0 = int(v);
            break;
        case DOUBLE_PROP:
            val1 = v;
            break;
        }
    }
    // All other property types require no differentiation.
    prop(int n, bool v) : num(n), val2(v), spec(propSpecs[n - 1]) {}
    prop(int n, std::string v) : num(n), val3(v), spec(propSpecs[n - 1]) {}
    prop(int n, hsv v) : num(n), val4(v), spec(propSpecs[n - 1]) {}
    prop(int n, std::vector<int> v) : num(n), val5(v), spec(propSpecs[n - 1]) {}
    prop(int n, particle v) : num(n), val6(v), spec(propSpecs[n - 1]) {}

    // print - Prints out the property name and value.
    void print() {
        std::cout << spec.name << ": ";
        switch (spec.type) {
        case INT_PROP:
            std::cout << val0;
            break;
        case DOUBLE_PROP:
            std::cout << val1;
            break;
        case BOOL_PROP:
            std::cout << val2;
            break;
        case STRING_PROP:
            std::cout << val3;
            break;
        case HSV_PROP:
            std::cout << "\n  hue: " << val4.hue << "\n  saturation: " << (val4.addSat ? "+" : "x") << val4.sat << "\n  value: " << (val4.addVal ? "+" : "x") << val4.val;
            break;
        case INTVEC:
            std::cout << printIntV(val5);
            break;
        case PARTICLE:
            val6.print();
            break;
        }
        std::cout << "\n";
    }

    // Get/set for type int.
    int getInt() {
        return val0;
    }
    void setInt(int n) {
        val0 = n;
    }

    // Get/set for type double.
    double getDoub() {
        return val1;
    }
    void setDoub(double n) {
        val1 = n;
    }

    // Get/set for type bool.
    bool getBool() {
        return val2;
    }
    void setBool(bool n) {
        val2 = n;
    }

    // Get/set for type string.
    std::string getString() {
        return val3;
    }
    void setString(std::string n) {
        val3 = n;
    }

    // Get/set for type HSV.
    hsv getHSV() {
        return val4;
    }
    void setHSV(hsv n) {
        val4 = n;
    }

    // Get/set for type int vector, plus a function to push an integer.
    std::vector<int> getIntVec() {
        return val5;
    }
    void setIntVec(std::vector<int> n) {
        val5 = n;
    }
    void pushIntVec(int n) {
        val5.push_back(n);
    }

    // Get/set for type particle.
    particle getParticle() {
        return val6;
    }
    void setParticle(particle n) {
        val6 = n;
    }

    // expString - Returns the string for the property as it would appear in a level string.
    std::string expString() {
        std::string final;
        switch (spec.type) {
        case INT_PROP:
            final = int2Str(val0);
            break;
        case DOUBLE_PROP:
            final = doub2Str(val1);
            break;
        case BOOL_PROP:
            final = val2 ? "1" : "0";
            break;
        case STRING_PROP:
            final = b64enc(val3);
            break;
        case HSV_PROP:
            final = val4.getString();
            break;
        case INTVEC:
            final = printIntV(val5);
            break;
        case PARTICLE:
            final = printDoubleV(val6.props);
            break;
        }
        return final;
    }
};

// block - Represents an in-game object.
struct block {
    std::vector<prop> props;
    std::string present = ""; // 600-bit bit field that represents each property's presence.
    block() {}
    block(std::string r) { // The most common construction of a block is during level string parsing.
        for (int i = 0; i < 75; i++)
            present += char(0);
        bool readProp = true;
        std::string vh1 = "";
        std::string vh2 = "";
        for (int i = 0; i < r.size() + 1; i++) {
            if (r[(i < int(r.size())) ? i : int(r.size())] == ','||i == r.size()) { // For every two commas that are read, a property is parsed and added into the list. Also adds in the final property after having reached the end of the string.
                readProp = !readProp;
                if (readProp) {
                    prop sub = prop(vh1, vh2);
                    props.push_back(sub);
                    setPresent(sub.num);
                    vh1 = "";
                    vh2 = "";
                }
            }
            else {
                if (readProp)
                    vh1 += r[i];
                else
                    vh2 += r[i];
            }
        }
        if (isPresent(1)) { // Because a lot of properties are reused between different objects, this ensures that the contextully correct property name is used when the block is printed.
            int id = props[getPNum(1)].val0;
            if (isPresent(10)&&id == 1912)
                props[getPNum(10)].spec.name = "chance"; // Most obvious example: property 10 for a random trigger(id 1912) would be named "chance" as opposed to the more common "duration".
                // And no, I'm not commenting the rest of these.
            if (id == 901) {
                if (isPresent(28))
                    props[getPNum(28)].spec.name = "moveX";
                if (isPresent(29))
                    props[getPNum(29)].spec.name = "moveY";
            }
            if (isPresent(51)) {
                switch (id) {
                case 901:
                    props[getPNum(51)].spec.name = "target group id";
                    break;
                case 3024:
                    props[getPNum(51)].spec.name = "effect id";
                    break;
                case 3619:
                    props[getPNum(51)].spec.name = "target item id";
                    break;
                case 3608:
                    props[getPNum(51)].spec.name = "particle group";
                    break;
                case 3640:
                    props[getPNum(51)].spec.name = "state on";
                    break;
                case 2919:
                    props[getPNum(51)].spec.name = "tint channel";
                    break;
                case 3600:
                    props[getPNum(51)].spec.name = "spawn id";
                    break;
                case 1006:
                    if (isPresent(52)) {
                        if (!props[getPNum(52)].val2)
                            props[getPNum(51)].spec.name = "channel id";
                    }
                    else
                        props[getPNum(51)].spec.name = "channel id";
                    break;
                }
                if (id == 1346||id == 2067||id == 1347||id == 1814||id == 3016||id == 3660||id == 3661||(id >= 3006&&id <= 3010)||id == 3022)
                    props[getPNum(51)].spec.name = "target group id";
                if (id == 1616||id == 3033||id == 1347||id == 1611||id == 1811||id == 2062||id == 3614||id == 3615||id == 1815||id == 2915)
                    props[getPNum(51)].spec.name = "target id";
                if (id >= 3011&&id <= 3015) {
                    if (isPresent(355)) {
                        if (props[getPNum(355)].val2)
                            props[getPNum(51)].spec.name = "effect id";
                    }
                }
                if (id == 3602||id == 1912||id == 3605||id == 3603)
                    props[getPNum(51)].spec.name = "group id 1";
                if (id == 3620||id == 3609)
                    props[getPNum(51)].spec.name = "true id";
                if (id == 2905||id == 2907||id == 2913||id == 2914||id == 2916)
                    props[getPNum(51)].spec.name = "center id";
            }
            if (isPresent(56)) {
                if (id == 1049||id == 1611||id == 1811||id == 1815||id == 1812||id == 3645)
                    props[getPNum(56)].spec.name = "activate group";
            }
            if (isPresent(59)&&id == 1931)
                props[getPNum(59)].spec.name = "lock y";
            if (isPresent(66)&&id == 3010)
                props[getPNum(66)].spec.name = "secondary only";
            if (isPresent(70)&&id == 2015)
                props[getPNum(70)].spec.name = "add";
            if (isPresent(71)) {
                switch (id) {
                case 901:
                    props[getPNum(71)].spec.name = "target pos group id";
                    break;
                case 3033:
                    props[getPNum(71)].spec.name = "parent id";
                    break;
                case 1914:
                    props[getPNum(71)].spec.name = "target group id";
                    break;
                case 3608:
                    props[getPNum(71)].spec.name = "spawn particle";
                    break;
                case 3613:
                    props[getPNum(71)].spec.name = "ui target";
                    break;
                case 3640:
                    props[getPNum(71)].spec.name = "state off";
                    break;
                case 2913:
                    props[getPNum(71)].spec.name = "tint channel";
                    break;
                case 3600:
                    props[getPNum(71)].spec.name = "target pos";
                    break;
                }
                if (id == 1346||id == 2067)
                    props[getPNum(71)].spec.name = "center group id";
                if (id == 1347||id == 3016||id == 3661)
                    props[getPNum(71)].spec.name = "follow group id";
                if (id == 3620||id == 3609)
                    props[getPNum(71)].spec.name = "false id";
                if (id == 3602||id == 1912||id == 3605||id == 3603)
                    props[getPNum(71)].spec.name = "group id 2";
                if (id == 2914||id == 2915)
                    props[getPNum(71)].spec.name = "ref channel";
            }
            if (isPresent(76)&&id == 3033)
                props[getPNum(76)].spec.name = "animation group id";
            if (isPresent(77)&&id == 1817)
                props[getPNum(77)].spec.name = "count";
            if (isPresent(80)) {
                int prop80 = getPNum(80);
                if (id == 3619||id == 3620)
                    props[prop80].spec.name = "item id 1";
                if (id == 1815||id == 3609)
                    props[prop80].spec.name = "block a id";
                if (id == 1816)
                    props[prop80].spec.name = "block id";
            }
            if (isPresent(88)) {
                if (id == 1817)
                    props[getPNum(88)].spec.name = "multiply/divide";
            }
            if (isPresent(95)) {
                if (id == 3619||id == 3620)
                    props[getPNum(95)].spec.name = "item id 2";
            }
            if (isPresent(100)&&id == 901)
                props[getPNum(100)].spec.name = "target mode";
            if (isPresent(138)) {
                if (id == 901||id == 1346)
                    props[getPNum(138)].spec.name = "target P1";
                if (id == 3016||id == 3661)
                    props[getPNum(138)].spec.name = "follow P1";
            }
            if (isPresent(175)&&id == 2923)
                props[getPNum(138)].spec.name = "cb";
            if (isPresent(176)) {
                switch (id) {
                case 2923:
                    props[getPNum(176)].spec.name = "cr";
                    break;
                case 2916:
                    props[getPNum(176)].spec.name = "bulge";
                    break;
                case 2922:
                    props[getPNum(176)].spec.name = "degrees";
                    break;
                }
                if (id == 2914||id == 2915)
                    props[getPNum(176)].spec.name = "intensity";
                if (id == 2919||id == 2920||id == 2921)
                    props[getPNum(176)].spec.name = "target";

            }
            if (isPresent(179)) {
                switch (id) {
                case 2909:
                    props[getPNum(179)].spec.name = "slice height";
                    break;
                case 2910:
                    props[getPNum(179)].spec.name = "line thickness";
                    break;
                case 2917:
                    props[getPNum(179)].spec.name = "modifier";
                    break;
                case 2921:
                    props[getPNum(179)].spec.name = "r";
                    break;
                case 2923:
                    props[getPNum(179)].spec.name = "br";
                    break;
                }
                if (id == 2913||id == 2914)
                    props[getPNum(179)].spec.name = "size";

            }
            if (isPresent(180)) {
                switch (id) {
                case 2911:
                    props[getPNum(180)].spec.name = "rgb off";
                    break;
                case 2916:
                    props[getPNum(180)].spec.name = "radius";
                    break;
                case 2921:
                    props[getPNum(180)].spec.name = "g";
                    break;
                case 2923:
                    props[getPNum(180)].spec.name = "bg";
                    break;
                }
                if (id == 2912||id == 2915||id == 2917||id == 2910)
                    props[getPNum(180)].spec.name = "target X";

            }
            if (isPresent(181)) {
                switch (id) {
                case 2909:
                    props[getPNum(181)].spec.name = "max col X off";
                    break;
                }
                if (id == 2905||id == 2907)
                    props[getPNum(181)].spec.name = "fade in";

            }
            if (isPresent(182)&&id == 2923)
                props[getPNum(182)].spec.name = "max col Y off";
            if (isPresent(188)) {
                switch (id) {
                case 2904:
                    props[getPNum(188)].spec.name = "no player particles";
                    break;
                case 2919:
                    props[getPNum(188)].spec.name = "use lum";
                    break;
                case 2921:
                    props[getPNum(188)].spec.name = "edit rgb";
                    break;
                }
                if (id == 2910||id == 2912||id == 2915)
                    props[getPNum(188)].spec.name = "use X";

            }
            if (isPresent(189)) {
                switch (id) {
                case 2911:
                    props[getPNum(189)].spec.name = "segment h";
                    break;
                case 2921:
                    props[getPNum(189)].spec.name = "b";
                    break;
                case 2923:
                    props[getPNum(189)].spec.name = "bb";
                    break;
                }

            }
            if (isPresent(190)) {
                switch (id) {
                case 2917:
                    props[getPNum(190)].spec.name = "use X";
                    break;
                case 2919:
                    props[getPNum(190)].spec.name = "tint channel enabled";
                    break;
                case 2921:
                    props[getPNum(190)].spec.name = "tween rgb";
                    break;
                }
                if (id == 2905||id == 2907)
                    props[getPNum(190)].spec.name = "follow";
            }
            if (isPresent(191)) {
                switch (id) {
                case 2909:
                    props[getPNum(191)].spec.name = "max slice X off";
                    break;
                case 2911:
                    props[getPNum(191)].spec.name = "line strength";
                    break;
                case 2915:
                    props[getPNum(191)].spec.name = "follow ease";
                    break;
                case 2923:
                    props[getPNum(191)].spec.name = "cg";
                    break;
                }
            }
            if (isPresent(192)&&id == 2911)
                props[getPNum(192)].spec.name = "disable";
            if (isPresent(194)) {
                switch (id) {
                case 2912:
                    props[getPNum(194)].spec.name = "snap grid";
                    break;
                case 2915:
                    props[getPNum(194)].spec.name = "dual dir";
                    break;
                case 2917:
                    props[getPNum(194)].spec.name = "use Y";
                    break;
                case 2921:
                    props[getPNum(194)].spec.name = "clamp rgb";
                    break;
                }
            }
            if (isPresent(200)) {
                if (id == 901||id == 1346)
                    props[getPNum(200)].spec.name = "target P2";
                if (id == 3016||id == 3661)
                    props[getPNum(200)].spec.name = "follow P2";
            }
            if (isPresent(201)) {
                switch (id) {
                case 2066:
                    props[getPNum(201)].spec.name = "PT";
                    break;
                case 2915:
                    props[getPNum(201)].spec.name = "C";
                    break;
                }
                if (id == 1815||id == 3609)
                    props[getPNum(201)].spec.name = "PP";
            }
            if (isPresent(210)&&id == 1006)
                props[getPNum(210)].spec.name = "static hsv off";
            if (isPresent(214)&&id == 2065)
                props[getPNum(214)].spec.name = "animate active only";
            if (isPresent(290)&&id == 2907)
                props[getPNum(290)].spec.name = "screen off";
            if (isPresent(300)&&id == 3660)
                props[getPNum(300)].spec.name = "speed";
            if (isPresent(301)&&id == 3660)
                props[getPNum(301)].spec.name = "speed +-";
            if (isPresent(363)&&id == 3660)
                props[getPNum(363)].spec.name = "dir";
            if (isPresent(364)&&id == 3660)
                props[getPNum(364)].spec.name = "dir +-";
            if (isPresent(392)&&id == 3660)
                props[getPNum(392)].spec.name = "sfx id";
            if (isPresent(394)) {
                switch (id) {
                case 1346:
                    props[getPNum(394)].spec.name = "follow mode";
                    break;
                case 2015:
                    props[getPNum(394)].spec.name = "snap360";
                    break;
                }
            }
            if (isPresent(473)&&id == 3615)
                props[getPNum(473)].spec.name = "target time";
            if (isPresent(479)&&id == 3620)
                props[getPNum(479)].spec.name = "mod1";
            if (isPresent(480)&&id == 3620)
                props[getPNum(480)].spec.name = "op1";
            if (isPresent(481)&&id == 3620)
                props[getPNum(481)].spec.name = "op2";
            if (isPresent(482)&&id == 3620)
                props[getPNum(482)].spec.name = "compare op";
            if (isPresent(512)&&id == 2917)
                props[getPNum(512)].spec.name = "radius";
            if (isPresent(515)&&id == 2912)
                props[getPNum(515)].spec.name = "hard edges";
            if (isPresent(533)) {
                switch (id) {
                case 3030:
                    props[getPNum(3030)].spec.name = "ground";
                    break;
                case 3031:
                    props[getPNum(3031)].spec.name = "middle ground";
                    break;
                }
            }
            if (isPresent(535)&&id == 1616)
                props[getPNum(535)].spec.name = "use control id";
            if (isPresent(560)&&id == 3660)
                props[getPNum(560)].spec.name = "speed unmarked?";
            if (isPresent(565)&&id == 3660)
                props[getPNum(565)].spec.name = "dir unmarked?";
        }
    }

    // Alternatively, a block can be constructed directly from a property vector.
    block(std::vector<prop> p) :props(p) {
        for (int i = 0; i < 75; i++)
            present += char(0);
        for (int i = 0; i < props.size(); i++)
            setPresent(props[i].num);
    }

    // disp - Displays the object using sprite data(only used in tandem with SFML).
    void disp();

    // getString - Returns the string of the block as it would appear in a level string.
    std::string getString() {
        std::string final;
        for (unsigned int i = 0; i < props.size(); i++) {
            final += int2Str(props[i].num) + "," + props[i].expString();
            if (i < props.size() - 1)
                final += ",";
        }
        return final;
    }

    // isPresent - Returns true if the respective property bit is true.
    bool isPresent(int n) {
        return int(static_cast<unsigned char>(present[int((n - 1) / 8)]) / pow(2, 7 - ((n - 1) % 8))) % 2;
    }

    // setPresent - Sets the property bit true if it was false already.
    void setPresent(int n) {
        if (!isPresent(n))
            present[int((n - 1) / 8)] += static_cast<unsigned char>(pow(2, 7 - ((n - 1) % 8)));
    }

    // print - Prints each property.
    void print() {
        for (int i = 0; i < props.size(); i++)
            props[i].print();
    }

    // getPNum - Because the properties of an object can be defined in any order, this function is used to find its index in the property list.
    int getPNum(int n) {
        int final = 0;
        while (props[final].num != n&&final < props.size() - 1)
            final++;
        return final;
    }

    // For gets and sets in blocks, the property is first checked if its present. If present, getPNum is used to find the index for further evaulation.
    // If not present, a default value is returned in get functions, while the property is added to the list in set functions.

    // Get/set for type int.
    int getInt(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getInt();
        else
            return 0; // Integers have a default value of 0.
    }
    void setInt(int n, int v) {
        if (isPresent(n))
            props[getPNum(n)].setInt(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }

    // Get/set for type double.
    double getDoub(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getDoub();
        else {
            if (n == 32||n == 128||n == 129)
                return 1; // For properties scale(id 32), scaleX(id 128), and scaleY(id 129), their default value is 1.
            else
                return 0;
        }
    }
    void setDoub(int n, double v) {
        if (isPresent(n))
            props[getPNum(n)].setDoub(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }

    // Get/set for type bool.
    bool getBool(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getBool();
        else
            return false;
    }
    void setBool(int n, bool v) {
        if (isPresent(n))
            props[getPNum(n)].setBool(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }

    // Get/set for type string.
    std::string getString(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getString();
        else
            return "";
    }
    void setString(int n, std::string v) {
        if (isPresent(n))
            props[getPNum(n)].setString(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }

    // Get/set for type HSV.
    hsv getHSV(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getHSV();
        else
            return hsv();
    }
    void setHSV(int n, hsv v) {
        if (isPresent(n))
            props[getPNum(n)].setHSV(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }

    // Get/set for type integer vector, with an additional push function.
    std::vector<int> getIntVec(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getIntVec();
        else
            return std::vector<int>({});
    }
    void setIntVec(int n, std::vector<int> v) {
        if (isPresent(n))
            props[getPNum(n)].setIntVec(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }
    void pushIntVec(int n, int v) {
        if (isPresent(n))
            props[getPNum(n)].pushIntVec(v);
        else {
            props.push_back(prop(n, std::vector<int>({ v })));
            setPresent(n);
        }
    }

    // Get/set for type particle.
    particle getParticle(int n) {
        if (isPresent(n))
            return props[getPNum(n)].getParticle();
        else
            return particle();
    }
    void setParticle(int n, particle v) {
        if (isPresent(n))
            props[getPNum(n)].setParticle(v);
        else {
            props.push_back(prop(n, v));
            setPresent(n);
        }
    }
};

// Used for research.
bool enableUnknownProps = false;

/*
* color - Represent a color used in a level. Holds basic RGB values, as well as some other ones I never looked into.
*/
struct color {
    int red = 0;
    int green = 0;
    int blue = 0;
    int id = 0;
    bool blend = false;
    color() {}
    color(std::string iRaw) {
        std::string raw = iRaw + ",";
        int currProp = 0;
        bool readProp = true;
        std::string vh = "";
        std::string vh2 = "";
        for (unsigned int i = 0; i < raw.size(); i++) {
            if (raw[i] == '_') {
                if (readProp) {
                    currProp = str2Int(vh);
                }
                else {
                    if (currProp == 1)
                        red = str2Int(vh);
                    else if (currProp == 2)
                        green = str2Int(vh);
                    else if (currProp == 3)
                        blue = str2Int(vh);
                    else if (currProp == 6)
                        id = str2Int(vh);
                    else if (currProp == 5)
                        blend = true;
                    else {
                        if (enableUnknownProps)
                            std::cout << "unknown color property (" << currProp << ")\n";
                    }
                }
                readProp = !readProp;
                vh = "";
            }
            else
                vh += raw[i];
        }
    }
    void setCol(int r, int g, int b) {
        red = r;
        green = g;
        blue = b;
    }
};

// xor11 - Encoding used exclusively in the game data files.
std::string xor11(std::string x) {
    std::string final;
    for (unsigned int i = 0; i < x.size(); i++)
        final += x[i]^11;
    return final;
}

// levelStringEncode - Encodes a level string.
std::string levelStringEncode(std::string x) {
    return b64enc(zlibComp(x, GD));
}

// levelStringEncode - Decodes a level string.
std::string levelStringDecode(std::string x) {
    if (x.size())
        return zlibUncomp(b64dec(x), GD);
    else
        return "";
}

// dataFileEncode - Encodes a game data file.
std::string dataFileEncode(std::string x) {
    return xor11(b64enc(zlibComp(x, GD)));
}

// dataFileEncode - Decodes a game data file.
std::string dataFileDecode(std::string x) {
    return zlibUncomp(b64dec(xor11(x)), GD);
}

/*
* levelSpec - Basic level specification that only contains colors and blocks.
*/
struct levelSpec {
    std::vector<block> blocks;
    std::vector<color> colors;
    levelSpec() {}
    levelSpec(std::vector<block> b, std::vector<color> c) : blocks(b), colors(c) {}
};

// levelStringParse - Takes a level string(encoded or decoded) and parses it. Returns the blocks and colors in the form of a levelSpec. raw is used to differentiate between an encoded and decoded string.
levelSpec levelStringParse(std::string s, bool raw) {
    std::string sub;    
    if(raw)
        sub = s;
    else
        sub = levelStringDecode(s);
    unsigned int count = 0;
    bool readBlock = false;
    bool readCol = false;
    std::string vh = "";
    std::vector<block> finalBlocks;
    std::vector<color> finalCols;
    for (int i = 0; i < 1100; i++) {
        finalCols.push_back(color());
    }
    while (count < sub.size()) {
        if (sub[count] == ';') {
            if (readBlock) {
                finalBlocks.push_back(block(vh));
                vh = "";
            }
            else {
                readBlock = true;
                readCol = false;
            }
            if (vh.size() == 1) {
                if (vh[0] == '0') {
                    vh = "";
                }
            }
        }
        else if (sub[count] == '|') {
            if (readCol) {
                color subCol = color(vh);
                finalCols[subCol.id] = subCol;
                vh = "";
            }
        }
        else if (sub[count] == ','&&!readBlock)
            readCol = true;
        else {
            if (readBlock||readCol)
                vh += sub[count];
        }
        if (sub[count] == 'k') {
            readCol = false;
            if(!readBlock)
                vh = "";
        }
        count++;
    }
    return levelSpec(finalBlocks, finalCols);
}

// getLevString(no prefix) - Takes a vector of objects and returns an encoded level string with them. Uses a default level prefix.
std::string getLevString(std::vector<block> b) {
    std::string fStr = "kS38,1_40_2_125_3_255_11_255_12_255_13_255_4_-1_6_1000_7_1_15_1_18_0_8_1|1_0_2_102_3_255_11_255_12_255_13_255_4_-1_6_1001_7_1_15_1_18_0_8_1|1_0_2_102_3_255_11_255_12_255_13_255_4_-1_6_1009_7_1_15_1_18_0_8_1|1_255_2_255_3_255_11_255_12_255_13_255_4_-1_6_1002_5_1_7_1_15_1_18_0_8_1|1_40_2_125_3_255_11_255_12_255_13_255_4_-1_6_1013_7_1_15_1_18_0_8_1|1_40_2_125_3_255_11_255_12_255_13_255_4_-1_6_1014_7_1_15_1_18_0_8_1|1_125_2_255_3_0_11_255_12_255_13_255_4_-1_6_1005_5_1_7_1_15_1_18_0_8_1|1_0_2_255_3_255_11_255_12_255_13_255_4_-1_6_1006_5_1_7_1_15_1_18_0_8_1|1_255_2_255_3_255_11_255_12_255_13_255_4_-1_6_1004_7_1_15_1_18_0_8_1|,kA13,0,kA15,0,kA16,0,kA14,,kA6,0,kA7,0,kA25,0,kA17,0,kA18,0,kS39,0,kA2,0,kA3,0,kA8,0,kA4,0,kA9,0,kA10,0,kA22,0,kA23,0,kA24,0,kA27,1,kA40,1,kA41,1,kA42,1,kA28,0,kA29,0,kA31,1,kA32,1,kA36,0,kA43,0,kA44,0,kA45,1,kA46,0,kA33,1,kA34,1,kA35,0,kA37,1,kA38,1,kA39,1,kA19,0,kA26,0,kA20,0,kA21,0,kA11,0;";
    for (int i = 0; i < b.size(); i++)
        fStr += b[i].getString() + ";";
    return levelStringEncode(fStr);
}

// getLevString(prefix) - Takes a vector of objects and returns an encoded level string that inlcudes prefix p.
std::string getLevString(std::vector<block> b, std::string p) {
    std::string fStr = p;
    for (int i = 0; i < b.size(); i++)
        fStr += b[i].getString() + ";";
    return levelStringEncode(fStr);
}

// xmlNode - Represents a single node in an XML file. Used exclusively in GD data file encoding/decoding.
struct xmlNode {
    std::string name = "";
    std::string type = "";
    std::string data = "";
    std::vector<xmlNode> children = std::vector<xmlNode>({}); // Inception.
    bool bNode = false;
    std::string pathName = ""; // A pathname system is implemented for node parsing.
    xmlNode() {};
    xmlNode(std::vector<std::string> x, std::string pn) {
        int closures = 0;
        bool readName = true;
        bool addSubNode = false;
        std::vector<std::string> sub5;
        int startPoint = 0;
        std::string subName;
        int tabs = 0;
        for (unsigned int j = 0; j < x.size(); j++) {
            if (!addSubNode)
                subName = "";
            else
                sub5.push_back(x[j]);
            closures = 0;
            for (unsigned int i = 0; i < x[j].size(); i++) {
                if (x[j][i] == '>')
                    closures++;
                if (i > 0) {
                    if (x[j][i] == '>'||x[j][i] == ' ')
                        readName = false;
                    if (readName)
                        subName += x[j][i];
                }
            }
            if (closures == 1) {
                if (x[j] != "<t />"&&x[j] != "<d />") {
                    if (!addSubNode)
                        name = subName;
                    addSubNode = true;
                    if (x[j][1] == '/')
                        tabs--;
                    else {
                        if (x[j] != "<t />"&&x[j] != "<d />")
                            tabs++;
                    }
                    if (x[j] == ("</" + name + ">")&&tabs == 0) {
                        std::vector<std::string> sub6;
                        for (unsigned int k = 0; k < sub5.size() - 1; k++) {
                            sub6.push_back(sub5[k]);
                        }
                        xmlNode sub7 = xmlNode(sub6, pn + "/" + name);
                        for (unsigned int k = 0; k < sub7.children.size(); k++) {
                            children.push_back(sub7.children[k]);
                        }
                        sub5.clear();
                        addSubNode = false;
                        name = "";
                    }
                }
                else if (x[j] == "<t />") {
                    if (!addSubNode) {
                        children.push_back(xmlNode(x[j], pn + "/" + name));
                        readName = true;
                    }
                }
            }
            if (closures == 2&&!addSubNode) {
                children.push_back(xmlNode(x[j], pn + "/" + name));
                readName = true;
            }
        }
    };
    xmlNode(std::string x, std::string pn) {
        pathName = pn;
        bool readType = true;
        bool readData = false;
        if (x != "<t />") {
            for (unsigned int i = 1; i < x.size(); i++) {
                if (x[i] == '>')
                    readType = false;
                if (x[i] == '<')
                    readData = false;
                if (readType)
                    type += x[i];
                if (readData)
                    data += x[i];
                if (x[i] == '>')
                    readData = true;
            }
        }
        else {
            type = "b";
            data = "t";
        }
    }
    xmlNode(std::string pn, std::string t, std::string d) : pathName(pn), type(t), data(d) {}
    void print(bool n) {
        std::cout << pathName << " " << type << " " << data;
        if (n)
            std::cout << std::endl;
    }
};

// xmlParse - Parses an XML file.
xmlNode xmlParse(std::string x) {
    std::string sub;
    std::string sub2;
    std::vector<std::string> sub3;
    for (unsigned int i = 1; i < x.size(); i++) {
        if (x[i] == '<') {
            if (sub.size())
                sub2 += sub;
            sub = "";
        }
        else if (x[i] == '>') {
            if (sub.size()) {
                sub2 += "<" + sub + ">";
                if (i < x.size() - 1) {
                    if (x[i + 1] == '<') {
                        sub3.push_back(sub2);
                        sub2 = "";
                    }
                }
            }
            sub = "";
        }
        else
            sub += x[i];
    }
    sub3.push_back(sub2);
    std::vector<std::string> sub4;
    for (unsigned int i = 1; i < sub3.size(); i++) {
        sub4.push_back(sub3[i]);
    }
    return(xmlNode(sub4, ""));
}

// kPropTypes - Represent a property type used in level storage.
enum kPropTypes {
    K_INT,
    K_STRING,
    K_FLOAT,
    K_BOOL
};

// kPropTypes - Represent a property used in level storage.
struct kProp {
    std::string title = "";
    kPropTypes type = K_INT;
    int intData = 0;
    std::string stringData = "";
    double floatData = 0;
    bool boolData = false;
    kProp() {}
    kProp(xmlNode t, xmlNode v) { // For each property, there exists a title and some data.
        title = t.data;
        if (v.type == "i") {
            type = K_INT;
            intData = str2Int(v.data);
        }
        else if (v.type == "s") {
            type = K_STRING;
            stringData = v.data;
        }
        else if (v.type == "r") {
            type = K_FLOAT;
            floatData = str2Doub(v.data);
        }
        else if (v.type == "b") {
            type = K_BOOL;
            boolData = true;
        }
    }

    // Data can be set directly.
    kProp(std::string t, int v) : type(K_INT), title(t), intData(v) {}
    kProp(std::string t, std::string v) : type(K_STRING), title(t), stringData(v) {}
    kProp(std::string t, double v) : type(K_FLOAT), title(t), floatData(v) {}
    kProp(std::string t, bool v, bool f) : type(K_BOOL), title(t), boolData(v) {}

    // print - Prints the title and data of the property.
    void print() {
        std::cout << title + " " + getType() + " ";
        switch (type) {
        case K_INT:
            std::cout << intData;
            break;
        case K_STRING:
            std::cout << stringData;
            break;
        case K_FLOAT:
            std::cout << floatData;
            break;
        case K_BOOL:
            std::cout << boolData;
            break;
        }
        std::cout << std::endl;
    }

    // getType - Returns a string used to specify what type the kProp is in XML format.
    std::string getType(){
        switch (type) {
        case K_INT:
            return "i";
            break;
        case K_STRING:
            return "s";
            break;
        case K_FLOAT:
            return "r";
            break;
        case K_BOOL:
            return "b";
            break;
        }
        return "";
    }

    // getData - Returns the data associated with the kProp.
    std::string getData() {
        switch (type) {
        case K_INT:
            return int2Str(intData);
            break;
        case K_STRING:
            return stringData;
            break;
        case K_FLOAT:
            return doub2Str(floatData);
            break;
        case K_BOOL:
            return "t";
            break;
        }
        return "";
    }
};

// level - Like levelSpec, except more thorough. Stores all properties of a level. Used for data file encoding/decoding.
struct level {
    std::vector<kProp> props;
    std::vector<kProp> kI6props; // Special set of properties that mess with decoding.
    std::vector<block> objects;
    std::vector<color> colors;
    level() {}
    level(std::vector<xmlNode> x) { // Parses all xmlNodes and stores them in their respective vectors.
        bool kI6Passed = false;
        for (unsigned int i = 0; i < x.size(); i++) {
            if (x[i].data == "kI6") {
                kI6Passed = true;
            }
            else {
                if ((i + kI6Passed) % 2) {
                    if (kI6Passed)
                        kI6props.push_back(kProp(x[i - 1], x[i]));
                    else
                        props.push_back(kProp(x[i - 1], x[i]));
                }
            }
        }
    }

    //Get/set for property type int.
    int getPropInt(std::string t) {
        int final = 0;
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                final = props[i].intData;
        }
        return final;
    }
    void setPropInt(std::string t, int v) {
        int ind = -1;
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                ind = i;
        }
        if (ind > -1)
            props[ind].intData = v;
    }

    //Get/set for property type float.
    double getPropFloat(std::string t) {
        double final = 0;
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                final = props[i].floatData;
        }
        return final;
    }
    void setPropFloat(std::string t, double v) {
        int ind = -1;
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                ind = i;
        }
        if (ind > -1)
            props[ind].floatData = v;
    }

    //Get/set for property type string.
    std::string getPropString(std::string t) {
        std::string final = "";
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                final = props[i].stringData;
        }
        return final;
    }
    void setPropString(std::string t, std::string v) {
        int ind = -1;
        for (unsigned int i = 0; i < props.size(); i++) {
            if (props[i].title == t)
                ind = i;
        }
        if (ind > -1)
            props[ind].stringData = v;
    }
    
    //getNodes - Returns the xmlNodes used to store level data in XML format.
    std::vector<xmlNode> getNodes() {
        std::vector<xmlNode> final;
        for (unsigned int i = 0; i < props.size(); i++) {
            final.push_back(xmlNode("/plist/dict/d/d/", "k", props[i].title)); // For each property, a title node and a data node pushed to the list.
            final.push_back(xmlNode("/plist/dict/d/d/", props[i].getType(), props[i].getData()));
        }
        final.push_back(xmlNode("/plist/dict/d/d/", "k", "kI6"));
        if (kI6props.size())
            for (unsigned int i = 0; i < kI6props.size(); i++) {
                final.push_back(xmlNode("/plist/dict/d/d/d/", "k", kI6props[i].title));
                final.push_back(xmlNode("/plist/dict/d/d/d/", kI6props[i].getType(), kI6props[i].getData()));
            }
        else
            final.push_back(xmlNode("/plist/dict/d/d/", "n", "n"));
        return final;
    }

    // print - Prints all properties.
    void print() {
        for (unsigned int i = 0; i < props.size(); i++) {
            props[i].print();
        }
        for (unsigned int i = 0; i < kI6props.size(); i++) {
            kI6props[i].print();
        }
    }
};

// defaultLevel - Returns some default level data.
level defaultLevel() {
    level final;
    final.props.push_back(kProp("kCEK", 4));
    final.props.push_back(kProp("k2", ""));
    final.props.push_back(kProp("k4", ""));
    final.props.push_back(kProp("k5", "Player"));
    final.props.push_back(kProp("k101", "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));
    final.props.push_back(kProp("k13", true, 0));
    final.props.push_back(kProp("k21", 2));
    final.props.push_back(kProp("k16", 1));
    final.props.push_back(kProp("k50", 45));
    final.props.push_back(kProp("k47", true, 0));
    final.props.push_back(kProp("kI1", 0));
    final.props.push_back(kProp("kI2", 0));
    final.props.push_back(kProp("kI3", 1));
    final.props.push_back(kProp("kI7", 0));
    return final;
}

// xmlFormat - Returns all xmlNodes in proper XML format as a string.
std::string xmlFormat(std::vector<xmlNode> x) {
    std::string final;
    std::string currPath = "/";
    bool pathMatch = (currPath == x[0].pathName);
    std::string dirName = "";
    for (unsigned int i = 0; i < x.size(); i++) {
        pathMatch = (currPath == x[i].pathName);
        if (!pathMatch) {
            if (currPath.size() < x[i].pathName.size()) {
                unsigned int count = int(currPath.size());
                while (count < x[i].pathName.size()&&!pathMatch) {
                    currPath += x[i].pathName[count];
                    if (x[i].pathName[count] != '/')
                        dirName += x[i].pathName[count];
                    else {
                        std::string sub = dirName;
                        if (sub == "plist")
                            sub += " version=\"1.0\" gjver=\"2.0\""; // Hard coded in for GD purposes.
                        final += "<" + sub + ">";
                        dirName = "";
                    }
                    pathMatch = (currPath == x[i].pathName);
                    count++;
                }
            }
            else {
                std::vector<std::string> closedPaths;
                for (unsigned int j = int(x[i].pathName.size()); j < currPath.size(); j++) {
                    if (currPath[j] != '/')
                        dirName += currPath[j];
                    else {
                        closedPaths.push_back(dirName);
                        dirName = "";
                    }
                }
                for (int j = int(closedPaths.size() - 1); j > -1; j--) {
                    final += "</" + closedPaths[j] + ">";
                }
                currPath = x[i].pathName;
                pathMatch = true;
            }
        }
        if (x[i].type == "b")
            final += "<t />";
        else if (x[i].type == "n")
            final += "<d />";
        else
            final += "<" + x[i].type + ">" + x[i].data + "</" + x[i].type + ">";
    }
    std::vector<std::string> closedPaths;
    for (unsigned int j = 1; j < currPath.size(); j++) {
        if (currPath[j] != '/')
            dirName += currPath[j];
        else {
            closedPaths.push_back(dirName);
            dirName = "";
        }
    }
    for (int j = int(closedPaths.size() - 1); j > -1; j--) {
        final += "</" + closedPaths[j] + ">";
    }
    return "<?xml version=\"1.0\"?>" + final;
}

// Used to store all of the user's unfinished levels.
std::vector<level> levels;

// Used to store the full file name of the data file.
std::string fullFileName = "";

// getFileName - Gets the file name of the game data file and stores it.
void getFileName() {
    LPSTR a = new char[150];
    DWORD b = 150;
    DWORD c = GetModuleFileNameA(NULL, a, b);
    int bsCount = 0;
    int count = 0;
    std::string filePrefix;
    while (count < 150&&bsCount < 3) {
        filePrefix += a[count];
        if (a[count] == '\\')
            bsCount++;
        count++;
    }
    fullFileName = filePrefix + "AppData\\Local\\GeometryDash\\CCLocalLevels.dat";
}

// loadLevelsSuff - Loads in the user's unfinished levels with some sort of suffix(whose purpose I forgot).
std::vector<xmlNode> loadLevelsSuff(bool getSuff) {
    if (!fullFileName.size())
        getFileName();
    levels.clear();
    std::vector<xmlNode> test = xmlParse(dataFileDecode(fileCont(fullFileName))).children;
    bool readingLevels = false;
    bool listStop = false;
    std::vector<xmlNode> sub7;
    std::vector<xmlNode> final;
    for (unsigned int i = 0; i < test.size(); i++) {
        if (test[i].data == "LLM_02") {
            if (sub7.size()) {
                levels.push_back(level(sub7));
            }
            listStop = true;
        }
        if (listStop) {
            if (getSuff)
                final.push_back(test[i]);
        }
        else {
            if (test[i].pathName == "/plist/dict/d/") {
                readingLevels = (test[i].data[0] == 'k');
                if (sub7.size()) {
                    levels.push_back(level(sub7));
                    sub7.clear();
                }
            }
            else {
                if (readingLevels&&test[i].pathName.size() > 14)
                    sub7.push_back(test[i]);
            }
        }
    }
    return final;
}

// inject - Takes a level string and "injects" it into the user's unfinished levels. If a level already exists with the specified name, its overwritten. Otherwise, a new level is created.
void inject(std::string levString, std::string name) {
    std::vector<xmlNode> suffNodes = loadLevelsSuff(true);
    level newLevel = defaultLevel();
    newLevel.setPropString("k2", name);
    newLevel.setPropString("k4", levString);
    std::vector<level> newLevels;
    int targInd = -1;
    for (unsigned int i = 0; i < levels.size(); i++) {
        if (levels[i].getPropString("k2") == name)
            targInd = i;
    }
    if (targInd == -1) {
        newLevels.push_back(newLevel);
        for (unsigned int i = 0; i < levels.size(); i++)
            newLevels.push_back(levels[i]);
    }
    else {
        for (unsigned int i = 0; i < levels.size(); i++) {
            if (i == targInd)
                newLevels.push_back(newLevel);
            else
                newLevels.push_back(levels[i]);
        }
    }
    std::vector<xmlNode> totalNodes = std::vector<xmlNode>({ xmlNode("/plist/dict/","k","LLM_01"),xmlNode("/plist/dict/d/","k","_isArr") ,xmlNode("/plist/dict/d/","b","t") });
    for (unsigned int i = 0; i < newLevels.size(); i++) {
        totalNodes.push_back(xmlNode("/plist/dict/d/", "k", "k_" + int2Str(i)));
        std::vector<xmlNode> sub = newLevels[i].getNodes();
        for (unsigned int j = 0; j < sub.size(); j++) {
            totalNodes.push_back(sub[j]);
        }
    }
    for (unsigned int i = 0; i < suffNodes.size(); i++) {
        totalNodes.push_back(suffNodes[i]);
    }
    fileWrite(fullFileName, dataFileEncode(xmlFormat(totalNodes)));
}

// inject - Takes a vector of blocks, converts it to a level string, and injects it into the user's unfinished levels.
void inject(std::vector<block> a, std::string name) {
    inject(getLevString(a), name);
}