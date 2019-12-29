#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

const long double eps = 10e-5;

typedef struct vec{
    long double x, y;
} vec;

long double pseudodot(vec x, vec y){
    return x.x*y.y - y.x*x.y;
}

int sgn(long double x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int collinear(vec x, vec y){
    return (fabs(pseudodot(x, y)) < eps);
}

int point_on_segment(vec x, vec a, vec b){
    long double x1 = x.x;
    long double y1 = x.y;
    long double x2 = a.x;
    long double y2 = a.y;
    long double x3 = b.x;
    long double y3 = b.y;
    if ((fabs(x2 - x3) < eps) && (fabs(y2 - y3) < eps)){
        if ((fabs(x1 - x2) < eps) && (fabs(y1 - y2) < eps)) return 1;
        else return 0;
    }
    long double A = y2 - y3;
    long double B = x3 - x2;
    long double C = -A*x2 - B*y2;
    //cout << "reading point" << endl;
    if (fabs(A*x1 + B*y1 + C) > eps) return 0;
    else{
        //cout << "checking point" << endl;
        vec v = {x3 - x2, y3 - y2};
        vec v2 = {x1 - x2, y1 - y2};
        long double lambda;
        if (v.x != 0) lambda = v2.x*1.0/v.x;
        else lambda = v2.y*1.0/v.y;
        //cout << "lambda = " << lambda << endl;
        if ((lambda >= 0) && (lambda <= 1)) return 1;
        else return 0;
    }
}

int line_intersect(vec v1, vec v2, vec u1, vec u2, vec & ans){
    if ((v1.x == v2.x) && (v1.y == v2.y)){
        if ((u1.x == u2.x) && (u1.y == u2.y)){
            return ((v1.x == u1.x) && (v1.y == u1.y));
        }
        else{
            return point_on_segment(v1, u1, u2);
        }
    }
    if ((u1.x == u2.x) && (u1.y == u2.y)){
        return point_on_segment(u1, v1, v2);
    }
    long double A1 = v1.y - v2.y;
    long double B1 = v2.x - v1.x;
    long double C1 = -A1*v1.x - B1*v1.y;
    long double A2 = u1.y - u2.y;
    long double B2 = u2.x - u1.x;
    long double C2 = -A2*u1.x - B2*u1.y;
    //cout << "A1, B1, C1 = " << A1 << " " << B1 << " " << C1 << endl;
    //cout << "A2, B2, C2 = " << A2 << " " << B2 << " " << C2 << endl;
    if (fabs(A1*B2 - A2*B1) < eps){
        return (point_on_segment(v1, u1, u2) || point_on_segment(v2, u1, u2) || point_on_segment(u1, v1, v2) || point_on_segment(u2, v1, v2));
    }
    long double x = (B1*C2 - C1*B2)*1./(A1*B2 - A2*B1);
    long double y = (C1*A2 - A1*C2)*1./(A1*B2 - A2*B1);
    vec p = {x, y};
    if (point_on_segment(p, v1, v2) && point_on_segment(p, u1, u2)){
        //cout << "p = " << p.x << " " << p.y << endl;
        ans = {x, y};
        //cout << "Accepted!" << endl;
        return 1;
    }
    else return 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n;
    cin >> n;
    vector<pair<vec, vec>> V;
    long cnt = 0;
    for (int i = 0; i < n; i++){
        long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        V.push_back(make_pair(vec{x1, y1}, vec{x2, y2}));
        for (int j = 0; j < i; j++){
            vec ans;
            cnt += line_intersect(V[i].first, V[i].second, V[j].first, V[j].second, ans);
        }
    }
    cout << cnt << endl;
    return 0;
}
