#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <math.h>
using namespace std;

typedef struct vec{
    long long x, y;
} vec;

long long sarrus(vec a, vec b, vec c){
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

double dist(vec a, vec b){
    return sqrt(0.0 + (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    std::cout.precision(12);
    std::cout.setf(std::ios::fixed);
    long n, l;
    cin >> n >> l;
    vector<vec> points;
    for (int i = 0; i < n; i++){
        long long a, b;
        cin >> a >> b;
        points.push_back(vec{a, b});
    }
    vec mx = points[0];
    int imax = 0;
    for (int i = 1; i < points.size(); i++){
        if ((points[i].x < mx.x) || ((points[i].x == mx.x) && (points[i].y < mx.y))){
            mx = points[i];
            imax = i;
        }
    }
    swap(points[0], points[imax]);
    vec r = points[1];
    imax = 1;
    for (int i = 2; i < points.size(); i++){
        if (sarrus(points[0], r, points[i]) > 0){
            r = points[i];
            imax = i;
        }
        else{
            if ((sarrus(points[0], r, points[i]) == 0) && (dist(r, points[0]) < dist(points[i], points[0]))){
                r = points[i];
                imax = i;
            }
        }
    }
    int used[n] = {0};
    used[imax] = 1;
    vector<vec> Q;
    Q.push_back(points[imax]);
    int iprev = imax;
    //cout << "points[0] = " << points[0].x << " " << points[0].y << endl;
    //cout << "r = " << r.x << " " << r.y << endl;
    while ((r.x != points[0].x) || (r.y != points[0].y)){
        r = points[0];
        imax = 1;
        for (int i = 1; i < points.size(); i++){
            if ((sarrus(points[iprev], r, points[i]) > 0) && (used[i] == 0)){
                r = points[i];
                imax = i;
            }
            else{
                if ((sarrus(points[iprev], r, points[i]) == 0) && (used[i] == 0) && (dist(r, points[iprev]) < dist(points[i], points[iprev]))){
                    r = points[i];
                    imax = i;
                }
            }
        }
        used[imax] = 1;
        iprev = imax;
        Q.push_back(r);
        //cout << "r = " << r.x << " " << r.y << endl;
    }
    //конец джарвиса
    long double ans = 2 * atan(1) * 4 * l;
    for (long i = 0; i < Q.size()-1; i++){
        ans += dist(Q[i], Q[i+1]);
    }
    ans += dist(Q[Q.size()-1], Q[0]);
    cout << ans << endl;
    return 0;
}
