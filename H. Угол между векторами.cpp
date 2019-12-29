#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

typedef struct vec{
    double x, y;
} vec;

double dot(vec x, vec y){
    return x.x*y.x + x.y*y.y;
}

double pseudodot(vec x, vec y){
    return x.x*y.y - y.x*x.y;
}

int main(){
    ios::sync_with_stdio(false);
    std::cout.precision(6);
    std::cout.setf(std::ios::fixed);

    cin.tie(NULL);
    cout.tie(NULL);
    double a, b, c, d;
    cin >> a >> b >> c >> d;
    vec x = {a, b};
    vec y = {c, d};
    double angle = acos(dot(x, y)/(sqrt(dot(x, x))*sqrt(dot(y,y))));
    cout << angle << endl;
    return 0;
}
