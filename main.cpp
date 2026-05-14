#include <calc-engine.h>
#include <png.h>
#include <string>
#include <file-handle.h>

using namespace std;
void printBS(string a){
    for(int i=0;i<a.size();i++){
        unsigned char b = a[i];
        cout << int(b) << " ";
    }
}

int main(){
    /*
    vector<para> a = vector<para>({bez(vector<point>({point(9,51),point(66,36),point(111,55)}))});
    a.push_back(a[0].transform(vector<transformNode>({transformNode(SCALE,0.5,0.5)})));

    pixel bgCol = pixel(fullColor(255,255,255,false));
    vector<pixel> strokeCol = vector<pixel>({(fullColor(255,0,0,false)),fullColor(0,0,255,false)});
    vector<vector<pixel>> b;
    int width = 120;
    int height = 120;
    for(int i=0;i<height;i++){
        b.push_back(vector<pixel>({}));
        for(int j=0;j<width;j++)
            b[i].push_back(bgCol);
    }
    for(int i=0;i<a.size();i++){
        a[i].generatePixels(1);
        for(int j=0;j<a[i].linePixels.size();j++)
            b[height-a[i].linePixels[j].y-1][a[i].linePixels[j].x] += pixel(strokeCol[i].color, round(a[i].linePixels[j].mag*double(strokeCol[i].a)));
    }
    image(b, "test3.png");
    */
    para a=sinusoid(point(90,90),60,60,0,180);
    para b=sinusoid(point(90,90),30,30,180,-180);
    para c=sinusoid(point(45,90),15,15,180,180);
    para w=sinusoid(point(135,90),15,15,180,180);

    para d=bez(vector<point>({point(20,20),point(34,25),point(14,2)}));
    para e=quadWarp(a,b,c,w,vector<para>({d}),point(100,100),point(0,0))[0];
    e.printPoint(0.45);
    cout << "\n";
    return 0;
}