#include <iostream>

using namespace std;

int main(void)
{
    unsigned long long N_M;
    cin >> N_M;
    cin.get();

    while (N_M)
    {
        unsigned long long N = N_M*10 / 9;

        if (N%10 == 0) cout << N-1 << " ";
        cout << N;
        cout << endl;

        cin >> N_M;
        cin.get();
    }
}
