#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

typedef struct vec{
    long x, y;
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
    return (pseudodot(x, y) == 0);
}

int inter_vector(vec x, vec y, vec z){
    return sgn(pseudodot(x, y))*sgn(pseudodot(x,z)) <= 0;
}

int point_on_segment(vec x, vec a, vec b){
    long x1 = x.x;
    long y1 = x.y;
    long x2 = a.x;
    long y2 = a.y;
    long x3 = b.x;
    long y3 = b.y;
    if ((x2 == x3) && (y2 == y3)){
        if ((x1 == x2) && (y1 == y2)) return 1;
        else return 0;
    }
    long A = y2 - y3;
    long B = x3 - x2;
    long C = -A*x2 - B*y2;
    if (A*x1 + B*y1 + C != 0) return 0;
    else{
        vec v = {x3 - x2, y3 - y2};
        vec v2 = {x1 - x2, y1 - y2};
        double lambda;
        if (v.x != 0) lambda = v2.x*1.0/v.x;
        else lambda = v2.y*1.0/v.y;
        //cout << lambda << " " << v2.x << " " << v.x << endl;
        if ((lambda >= 0) && (lambda <= 1)) return 1;
        else return 0;
    }
}

int in_triangle(vec x, vec a, vec b, vec c){
    /*
    cout << "trianlge" << endl;
    cout << a.x << " " << a.y << endl;
    cout << b.x << " " << b.y << endl;
    cout << c.x << " " << c.y << endl;
    cout << "point" << endl;
    cout << x.x << " " << x.y << endl;
    cout << "-----" << endl;
    */
    if (collinear(a, b) && collinear(a, c) && collinear(b, c)){
        return point_on_segment(x, a, b) || point_on_segment(x, a, c) || point_on_segment(x, b, c);
    }
    vec s1 = {a.x - b.x, a.y - b.y};
    vec s2 = {b.x - c.x, b.y - c.y};
    vec s3 = {c.x - a.x, c.y - a.y};
    vec p1 = {x.x - b.x, x.y - b.y};
    vec p2 = {x.x - c.x, x.y - c.y};
    vec p3 = {x.x - a.x, x.y - a.y};
    //cout << "dots" << endl;
    //cout << pseudodot(s1, p1) << " " << pseudodot(s2, p2) << " " << pseudodot(s3, p3) << endl;
    return (sgn(pseudodot(s1, p1))*sgn(pseudodot(s2, p2)) >= 0) && (sgn(pseudodot(s1, p1))*sgn(pseudodot(s3, p3)) >= 0) && (sgn(pseudodot(s2, p2))*sgn(pseudodot(s3, p3)) >= 0);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n, m, k;
    cin >> n >> m >> k;
    vector<vec> coord(n);
    long x0 = 0;
    long y0 = 0;
    for (int i = 0; i < n; i++){
        long a, b;
        cin >> a >> b;
        if (i == 0){
            x0 = a;
            y0 = b;
        }
        coord[i] = {a, b};
        coord[i].x -= x0;
        coord[i].y -= y0;
    }
    int cnt = 0;
    for (int i = 0; i < m; i++){
        long a, b;
        cin >> a >> b;
        a -= x0;
        b -= y0;
        vec point = {a, b};
        int l = 1;
        int r = n-1;
        while (l != r-1){
            int m = (l + r) / 2;
            //cout << "m = " << m << " " << l << " " << r << endl;
            if (inter_vector(point, coord[m], coord[l])){
                r = m;
            }
            else{
                l = m;
            }
        }
        //cout << "l = " << l << " " << r << endl;
        //cout << coord[l].x << " " << coord[l].y << endl;
        //cout << coord[r].x << " " << coord[r].y << endl;
        if (in_triangle(point, coord[0], coord[l], coord[r])) cnt++;
        //cout << "cnt = " << cnt << endl;
        //cout << "------" << endl;
        if (cnt == k) break;
    }
    if (cnt == k) cout << "YES";
    else cout << "NO";
    return 0;
}
