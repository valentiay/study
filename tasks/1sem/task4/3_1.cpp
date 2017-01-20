#include <iostream>

using std::cin;
using std::cout;

int main(){
    int num;
    cin >> num;

    long ** dp = new long * [num + 1];
    for(int i = 0; i <= num; i++)
        dp[i] = new long[num + 1];

    for(int i = 0; i <= num; i++)
        dp[1][i] = 1;

    for(int i = 2; i <= num; i++)
        for(int j = 0; j <= num; j++)
            dp[i][j] = dp[i - 1][j] + ((j - i + 1 > 0)?(dp[i][j - i]):0);

    cout << dp[num][num];

    for(int i = 0; i <= num; i++)
        delete[] dp[i];
    delete[] dp;
}
