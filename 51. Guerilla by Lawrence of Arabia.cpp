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
    long n, m;
    cin >> n >> m;
    while (n > 0){
        long long A[n], dp[n];
        long long C[n][n];
        for (long i = 0; i < n; i++){
            for (long j = 0; j < n; j++) {
                C[i][j] = 0;
            }
        }
        for (long i = 0; i < n; i++){
            cin >> A[i];
            long long sum = A[i];
            for (long j = i - 1; j >= 0; j--){
                C[i][j] = C[i][j+1] + sum * A[j];
                sum += A[j];
                //cout << "i, j, sum = " << i << " " << j << " " << C[i][j] << endl;
            }
            dp[i] = C[i][0];
        }
        for (long k = 1; k <= m; k++) {
            long long dp2[n];
            for (long i = 0; i < n; i++) {
                dp2[i] = 0;
            }
            dp[0] = 0;
            for (long i = 1; i < n; i++) {
                long long m = -1;
                for (long j = 1; j <= i; j++) {
                    if (m == -1)
                        m = dp[j-1] + C[i][j];
                    m = min(m, dp[j-1] + C[i][j]);
                    //cout << "i, j, m = " << i << " " << j << " " << m << " " << dp[j-1] << endl;
                }
                if (i > 1)
                    dp2[i] = m;
                else
                    dp2[i] = 0;
            }
            for (long i = 0; i < n; i++) {
                dp[i] = dp2[i];
            }
        }
        cout << dp[n-1] << endl;
        cin >> n >> m;
    }
    return 0;
}
