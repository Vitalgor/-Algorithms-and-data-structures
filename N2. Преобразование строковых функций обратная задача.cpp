#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n;
    cin >> n;
    int P[n];
    int Z[n];
    for (int i = 0; i < n; i++){
        cin >> P[i];
        Z[i] = 0;
    }
    for (int i = 1; i < n; i++){
        if (P[i] > 0){
            Z[i - P[i] + 1] = P[i];
        }
    }
    Z[0] = n;
    int i = 1;
    while (i < n){
        int t = i;
        if (Z[i] > 0){
            for (int j = 1; j < Z[i]; j++){
                if (Z[i + j] > Z[j]) break;
                Z[i + j] = min(Z[j], Z[i] - j);
                t = i + j;
            }
        }
        i = t + 1;
    }
    for (int i = 0; i < n; i++){
        cout << Z[i] << " ";
    }
    return 0;
}
