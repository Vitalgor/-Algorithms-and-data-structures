#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

double eps = 1e-9;
double eps2 = 1e-8;
double eps3 = 1e-7;

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
    vec v1 = {x.x - a.x, x.y - a.y};
    vec v2 = {b.x - a.x, b.y - a.y};
    long double xmin = min(a.x, b.x);
    long double xmax = max(a.x, b.x);
    long double ymin = min(a.y, b.y);
    long double ymax = max(a.y, b.y);
    if (fabs(pseudodot(v1, v2)) < eps){
        if (((x.x <= xmax + eps2) && (x.x >= xmin - eps2)) && ((x.y >= ymin - eps2) && (x.y <= ymax + eps2))) return 1;
        else return 0;
    }
    else return 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    std::cout.precision(12);
    std::cout.setf(std::ios::fixed);
    long double x0, y0, r, x1, y1, x2, y2;
    while (cin >> x0 >> y0 >> r >> x1 >> y1 >> x2 >> y2){
        vec c, v1, v2;
        c = vec{x0, y0};
        v1 = vec{x1, y1};
        v2 = vec{x2, y2};
        if ((fabs(v1.x - v2.x) < eps2) && (fabs(v1.y - v2.y) < eps2)){
            if ((fabs(v1.x - c.x) < eps2) && (fabs(v1.y - c.y) < eps2) && (fabs(r) < eps)){
                cout << 1 << endl;
                cout << v1.x << " " << v1.y << endl;
                cout << endl;
            }
            else if (fabs((v1.x - c.x)*(v1.x - c.x) + (v1.y - c.y)*(v1.y - c.y) - r*r) < eps3){
                cout << 1 << endl;
                cout << v1.x << " " << v1.y << endl;
                cout << endl;
            }
            else{
                cout << 0 << endl;
                cout << endl;
            }
            continue;
        }
        v1.x -= c.x;
        v1.y -= c.y;
        v2.x -= c.x;
        v2.y -= c.y;
        long double A = v1.y - v2.y;
        long double B = v2.x - v1.x;
        long double C = -A*v1.x - B*v1.y;
        long double dist = fabs(C)/ sqrt(A*A + B*B);
        if (C*C > r*r*(A*A + B*B) + eps){
            cout << 0 << endl;
            cout << endl;
            continue;
        }
        vec p = vec{A, B};// нормальный вектор
        long double lambda = -C/(A*A + B*B);
        vec t = vec{c.x + lambda*A, c.y + lambda*B};
        v1.x += c.x;
        v1.y += c.y;
        v2.x += c.x;
        v2.y += c.y;
        if (fabs(C*C - r*r*(A*A+B*B)) < eps){
            if (point_on_segment(t, v1, v2)){
                cout << 1 << endl;
                cout << t.x << " " << t.y << endl;
                cout << endl;
            }
            else{
                cout << 0 << endl;
                cout << endl;
            }
            continue;
        }
        lambda = sqrt((r*r - C*C/(A*A + B*B))/(A*A + B*B));
        vec p1 = vec{-B, A}; //направляющий вектор
        //cout << "p = " << p.x << " " << p.y << " " << lambda << " " << dist << endl;
        vec t1 = vec{t.x + lambda*p1.x, t.y + lambda*p1.y};
        vec t2 = vec{t.x - lambda*p1.x, t.y - lambda*p1.y};
        int cnt = 0;
        cnt += point_on_segment(t1, v1, v2);
        cnt += point_on_segment(t2, v1, v2);
        cout << cnt << endl;
        if (cnt == 2){
            if ((t1.x > t2.x) || ((fabs(t1.x - t2.x) < eps2) && (t1.y > t2.y))) swap(t1, t2);
        }
        if (point_on_segment(t1, v1, v2)){
            cout << t1.x << " " << t1.y << endl;
        }
        if (point_on_segment(t2, v1, v2)){
            cout << t2.x << " " << t2.y << endl;
        }
        cout << endl;
    }
    return 0;
}
