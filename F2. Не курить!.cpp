#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;

typedef struct point{
    long x, y;
    long num;
    bool b;
}point;

struct PointCompare
{
    bool operator()(const point& a, const point& b)
    {
        if (a.x != b.x) return a.x > b.x;
        if (a.y != b.y) return a.y > b.y;
        return b.b;
    }
};

typedef struct segment{
	point s, f;
} segment;

typedef struct vec{
    long double x, y;
} vec;
int line_intersect(vec v1, vec v2, vec u1, vec u2, vec & ans);

struct SegmentCompare
{
    bool operator()(const segment& a, const segment& b)
    {
    	vec v1 = {a.s.x, a.s.y};
    	vec v2 = {a.f.x, a.f.y};
    	vec u1 = {b.s.x, b.s.y};
    	vec u2 = {b.f.x, b.f.y};
    	vec ans;
        if (line_intersect(v1, v2, u1, u2, ans)){
			if (a.s.y != b.s.y) return a.s.y < b.s.y;
			return a.s.x < b.s.x;
        }
        else{
            long x = max(a.s.x, b.s.x);
			vec v1 = {a.s.x, a.s.y};
			vec v2 = {a.f.x, a.f.y};
			vec u1 = {x, -100000};
			vec u2 = {x, 100000};
			vec ans;
			line_intersect(v1, v2, u1, u2, ans);
            vec ans2;
            v1 = {b.s.x, b.s.y};
			v2 = {b.f.x, b.f.y};
			line_intersect(v1, v2, u1, u2, ans2);
			return ans.y < ans2.y;
        }
	}
};

const long double eps = 10e-9;

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
    long n;
    cin >> n;
    segment A[n];
    set<segment, SegmentCompare> T;
    priority_queue<point, vector<point>, PointCompare> Q;
    set<pair<long, long>> S;
    for (long i = 0; i < n; i++){
        cin >> A[i].s.x >> A[i].s.y >> A[i].f.x >> A[i].f.y;
		A[i].s.num = i + 1;
		A[i].f.num = i + 1;
		if ((A[i].s.x == A[i].f.x) && (A[i].s.y == A[i].f.y)) return 1;
		PointCompare cmp = PointCompare();
		if (cmp(A[i].s, A[i].f)) swap(A[i].s, A[i].f);
		A[i].s.b = true;
		A[i].f.b = false;
		if (S.find(make_pair(A[i].s.x, A[i].s.y)) != S.end()){
            cout << "YES" << endl;
            for (long k = 0; k < i; k++){
				if ((A[i].s.x == A[k].s.x) && (A[i].s.y == A[k].s.y)){
					cout << i+1 << " " << k+1 << endl;
					return 0;
				}
				if ((A[i].s.x == A[k].f.x) && (A[i].s.y == A[k].f.y)){
					cout << i+1 << " " << k+1 << endl;
					return 0;
				}
            }
		}

		if (S.find(make_pair(A[i].f.x, A[i].f.y)) != S.end()){
            cout << "YES" << endl;
            for (long k = 0; k < i; k++){
				if ((A[i].f.x == A[k].s.x) && (A[i].f.y == A[k].s.y)){
					cout << i+1 << " " << k+1 << endl;
					return 0;
				}
				if ((A[i].f.x == A[k].f.x) && (A[i].f.y == A[k].f.y)){
					cout << i+1 << " " << k+1 << endl;
					return 0;
				}
            }
		}
		S.insert(make_pair(A[i].f.x, A[i].f.y));
		S.insert(make_pair(A[i].s.x, A[i].s.y));
		Q.push(A[i].s);
		Q.push(A[i].f);
    }
    while (!Q.empty()){
		point cur = Q.top();
		//cout << "cur = "  << cur.x << " " << cur.y << endl;
		Q.pop();
		if (cur.b){
			T.insert(A[cur.num-1]);
			set<segment, SegmentCompare>::iterator it = T.find(A[cur.num-1]);
			//cout << "it = " << it->s.x << " " << it->s.y << endl;
			if (it != T.begin()){
				auto it2 = prev(it, 1);
                vec v1 = {it->s.x, it->s.y};
                vec v2 = {it->f.x, it->f.y};
                vec u1 = {it2->s.x, it2->s.y};
                vec u2 = {it2->f.x, it2->f.y};
                vec ans;
                if (line_intersect(v1, v2, u1, u2, ans)){
					cout << "YES" << endl;
					//cout << it->s.x << " " << it->s.x << endl;
					cout << it->s.num << " " << it2->s.num;
					return 0;
                }
                set<segment>::iterator it3 = next(it, 1);
                if (it3 != T.end()){
					vec v1 = {it3->s.x, it3->s.y};
					vec v2 = {it3->f.x, it3->f.y};
					vec u1 = {it2->s.x, it2->s.y};
					vec u2 = {it2->f.x, it2->f.y};
					vec ans;
					if (line_intersect(v1, v2, u1, u2, ans)){
						cout << "YES" << endl;
						cout << it3->s.num << " " << it2->s.num;
						return 0;
					}
                }
			}
			auto it2 = next(it, 1);
			if (it2 != T.end()){
                vec v1 = {it->s.x, it->s.y};
                vec v2 = {it->f.x, it->f.y};
                vec u1 = {it2->s.x, it2->s.y};
                vec u2 = {it2->f.x, it2->f.y};
                vec ans;
                if (line_intersect(v1, v2, u1, u2, ans)){
					cout << "YES" << endl;
					cout << it->s.num << " " << it2->s.num;
					return 0;
                }
			}
		}
		else{
			segment p = A[cur.num-1];
            set<segment, SegmentCompare>::iterator it = T.find(p);
            if (it != T.begin()){
				auto it2 = prev(it, 1);
                vec v1 = {it->s.x, it->s.y};
                vec v2 = {it->f.x, it->f.y};
                vec u1 = {it2->s.x, it2->s.y};
                vec u2 = {it2->f.x, it2->f.y};
                vec ans;
                if (line_intersect(v1, v2, u1, u2, ans)){
					cout << "YES" << endl;
					cout << it->s.num << " " << it2->s.num;
					return 0;
                }
                set<segment>::iterator it3 = next(it, 1);
                if (it3 != T.end()){
					vec v1 = {it3->s.x, it3->s.y};
					vec v2 = {it3->f.x, it3->f.y};
					vec u1 = {it2->s.x, it2->s.y};
					vec u2 = {it2->f.x, it2->f.y};
					vec ans;
					if (line_intersect(v1, v2, u1, u2, ans)){
						cout << "YES" << endl;
						cout << it3->s.num << " " << it2->s.num;
						return 0;
					}
                }
			}
			set<segment>::iterator it2 = next(it, 1);
			if (it2 != T.end()){
                vec v1 = {it->s.x, it->s.y};
                vec v2 = {it->f.x, it->f.y};
                vec u1 = {it2->s.x, it2->s.y};
                vec u2 = {it2->f.x, it2->f.y};
                vec ans;
                if (line_intersect(v1, v2, u1, u2, ans)){
					cout << "YES" << endl;
					cout << it->s.num << " " << it2->s.num;
					return 0;
                }
			}
			T.erase(p);
		}
    }
    cout << "NO" << endl;
    return 0;
}
