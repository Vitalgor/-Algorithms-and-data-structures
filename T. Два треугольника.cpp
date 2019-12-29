#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

const double eps = 10e-6;

typedef struct vec{
    double x, y;
} vec;

double pseudodot(vec x, vec y){
    return x.x*y.y - y.x*x.y;
}

int sgn(double x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int collinear(vec x, vec y){
    return (fabs(pseudodot(x, y)) < eps);
}

int point_on_segment(vec x, vec a, vec b){
    double x1 = x.x;
    double y1 = x.y;
    double x2 = a.x;
    double y2 = a.y;
    double x3 = b.x;
    double y3 = b.y;
    if ((fabs(x2 - x3) < eps) && (fabs(y2 - y3) < eps)){
        if ((fabs(x1 - x2) < eps) && (fabs(y1 - y2) < eps)) return 1;
        else return 0;
    }
    double A = y2 - y3;
    double B = x3 - x2;
    double C = -A*x2 - B*y2;
    //cout << "reading point" << endl;
    if (fabs(A*x1 + B*y1 + C) > eps) return 0;
    else{
        //cout << "checking point" << endl;
        vec v = {x3 - x2, y3 - y2};
        vec v2 = {x1 - x2, y1 - y2};
        double lambda;
        if (v.x != 0) lambda = v2.x*1.0/v.x;
        else lambda = v2.y*1.0/v.y;
        //cout << "lambda = " << lambda << endl;
        if ((lambda >= 0) && (lambda <= 1)) return 1;
        else return 0;
    }
}

int in_triangle(vec x, vec a, vec b, vec c){
    if (collinear(a, b) && collinear(a, c) && collinear(b, c)){
        return point_on_segment(x, a, b) || point_on_segment(x, a, c) || point_on_segment(x, b, c);
    }
    vec s1 = {a.x - b.x, a.y - b.y};
    vec s2 = {b.x - c.x, b.y - c.y};
    vec s3 = {c.x - a.x, c.y - a.y};
    vec p1 = {x.x - b.x, x.y - b.y};
    vec p2 = {x.x - c.x, x.y - c.y};
    vec p3 = {x.x - a.x, x.y - a.y};
    return (sgn(pseudodot(s1, p1))*sgn(pseudodot(s2, p2)) >= 0) && (sgn(pseudodot(s1, p1))*sgn(pseudodot(s3, p3)) >= 0) && (sgn(pseudodot(s2, p2))*sgn(pseudodot(s3, p3)) >= 0);
}

int line_intersect(vec v1, vec v2, vec u1, vec u2, vec & ans){
    double A1 = v1.y - v2.y;
    double B1 = v2.x - v1.x;
    double C1 = -A1*v1.x - B1*v1.y;
    double A2 = u1.y - u2.y;
    double B2 = u2.x - u1.x;
    double C2 = -A2*u1.x - B2*u1.y;
    //cout << "A1, B1, C1 = " << A1 << " " << B1 << " " << C1 << endl;
    //cout << "A2, B2, C2 = " << A2 << " " << B2 << " " << C2 << endl;
    if (fabs(A1*B2 - A2*B1) < eps) return 0;
    double x = (B1*C2 - C1*B2)*1./(A1*B2 - A2*B1);
    double y = (C1*A2 - A1*C2)*1./(A1*B2 - A2*B1);
    vec p = {x, y};
    /*
    cout << "intersection point = " << p.x << " " << p.y << endl;
    cout << "segment" << endl;
    cout << v1.x << " " << v1.y << endl;
    cout << v2.x << " " << v2.y << endl;
    cout << u1.x << " " << u1.y << endl;
    cout << u2.x << " " << u2.y << endl;
    cout << "----" << endl;
    */
    if (point_on_segment(p, v1, v2) && point_on_segment(p, u1, u2)){
        //cout << "p = " << p.x << " " << p.y << endl;
        ans = {x, y};
        //cout << "Accepted!" << endl;
        return 1;
    }
    else return 0;
}// ќтвет - в ans

double aria(vec a, vec b, vec c){
    double s = 0;
    s += (b.x - a.x)*(b.y + a.y)/2;
    s += (c.x - b.x)*(c.y + b.y)/2;
    s += (a.x - c.x)*(a.y + c.y)/2;
    return fabs(s);
}

double sarrus(vec a, vec b, vec c){
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    std::cout.precision(6);
    std::cout.setf(std::ios::fixed);

    int x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    vec t1[3];
    t1[0] = {x1, y1};
    t1[1] = {x2, y2};
    t1[2] = {x3, y3};
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    vec t2[3];
    t2[0] = {x1, y1};
    t2[1] = {x2, y2};
    t2[2] = {x3, y3};
    if (collinear(t1[0], t1[1]) && collinear(t1[1], t1[2]) && collinear(t1[2], t1[0])){
        cout << aria(t2[0], t2[1], t2[2]);
        return 0;
    } // вырожденный треугольник

    if (collinear(t2[0], t2[1]) && collinear(t2[1], t2[2]) && collinear(t2[2], t2[0])){
        cout << aria(t1[0], t1[1], t1[2]);
        return 0;
    }
    //cout << "here" << endl;
    vector<vec> points2;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            vec ans;
            if (line_intersect(t1[i], t1[(i + 1) % 3], t2[j], t2[(j + 1) % 3], ans)) points2.push_back(ans);
        }
    }
    for (int i = 0; i < 3; i++){
        if (in_triangle(t1[i], t2[0], t2[1], t2[2])) points2.push_back(t1[i]);
        if (in_triangle(t2[i], t1[0], t1[1], t1[2])) points2.push_back(t2[i]);
    } // вершины треуг, лежащие в чужом, других точек, кроме этиз и точек перес не будет

    //cout << "here 2" << endl;
    vector<vec> points;
    for (int i = 0; i < points2.size(); i++){
        int was = 0;
        for (int j = 0; j < i; j++){
            if ((fabs(points2[i].x - points2[j].x) < eps) && (fabs(points2[i].y - points2[j].y) < eps)) was = 1;
        }
        if (!was) points.push_back(points2[i]);
    } // выикдывает повтор€ющиейс€ одинаковые точки
    /*
    cout << "points" << endl;
    for (int i = 0; i < points.size(); i++){
        cout << points[i].x << " " << points[i].y << endl;
    }
    cout << "----" << endl;
    */
    if (points.size() <= 2){
        cout << aria(t1[0], t1[1], t1[2]) + aria(t2[0], t2[1], t2[2]);
        return 0;
    } // точек перес меньше 2 => их общ плоащь - ноль, тк или точка или отрезок


    // джарвис
    vec mx = points[0];
    int imax = 1;
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
        if (sarrus(points[0], r, points[i]) >= 0){
            r = points[i];
            imax = i;
        }
    }
    int n = points.size();
    int used[n] = {0};
    used[imax] = 1;
    vector<vec> Q;
    Q.push_back(points[imax]);
    int iprev = imax;
    //cout << "points[0] = " << points[0].x << " " << points[0].y << endl;
    //cout << "r = " << r.x << " " << r.y << endl;
    while ((fabs(r.x - points[0].x) > eps) || (fabs(r.y - points[0].y) > eps)){
        r = points[0];
        imax = 1;
        for (int i = 1; i < points.size(); i++){
            if ((sarrus(points[iprev], r, points[i]) >= 0) && (used[i] == 0)){
                r = points[i];
                imax = i;
            }
        }
        used[imax] = 1;
        iprev = imax;
        Q.push_back(r);
        //cout << "r = " << r.x << " " << r.y << endl;
    }
    //конец джарвиса
    double s = 0;
    /*
    for (int i = 0; i < Q.size(); i++){
        cout << Q[i].x << " " << Q[i].y << endl;
    }
    cout << "----" << endl;
    */

    // площадь выпуклой оболочки
    for (int i = 0; i < Q.size()-1; i++){
        s += (Q[i+1].x - Q[i].x)*(Q[i+1].y + Q[i].y)/2;
    }
    s += (Q[0].x - Q[Q.size()-1].x)*(Q[0].y + Q[Q.size()-1].y)/2;
    s = fabs(s);
    //cout << s << endl;
    /*
    cout << aria(t1[0], t1[1], t1[2]) << endl;
    */
    cout << aria(t1[0], t1[1], t1[2]) + aria(t2[0], t2[1], t2[2]) - s;
    return 0;
}
