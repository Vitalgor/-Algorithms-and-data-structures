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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    if ((x2 == x3) && (y2 == y3)){
        if ((x1 == x2) && (y1 == y2)) cout << "YES";
        else cout << "NO";
        return 0;
    }
    int A = y2 - y3;
    int B = x3 - x2;
    int C = -A*x2 - B*y2;
    if (A*x1 + B*y1 + C != 0) cout << "NO" << endl;
    else{
        vec v = {x3 - x2, y3 - y2};
        vec v2 = {x1 - x2, y1 - y2};
        double lambda;
        if (v.x != 0) lambda = v2.x/v.x;
        else lambda = v2.y/v.y;
        //cout << lambda << " " << v2.x << " " << v.x << endl;
        if ((lambda >= 0) && (lambda <= 1)) cout << "YES";
        else cout << "NO";
        return 0;
    }
    return 0;
}
