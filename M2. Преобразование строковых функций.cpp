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
	long n;
	cin >> n;
	long p[n] = {0};
	for (long i = 0; i < n; i++){
		long z;
		cin	>> z;
		if (i == 0) continue;
		for (long j = z-1; j >= 0; j--){
			if (p[i+j] > 0) break;
			p[i+j] = j + 1;
		}
		p[i + z - 1] = max(z, p[i + z - 1]);
	}
	for (long i = 0; i < n; i++){
		cout << p[i] << " ";
	}
    return 0;
}
