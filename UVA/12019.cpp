#include <iostream>

using namespace std;

int main()
{
    int times, dates[12] = {10, 21, 7, 4, 9, 6, 11, 8, 5, 10, 7, 12};
    const char *days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    cin >> times;
    for (int i = 0; i < times; i++)
    {
        int M, D, diff_module7;
        cin >> M >> D;

        diff_module7 = (D - dates[M-1]) % 7;
        if (diff_module7 >= 0)
        {
            cout << days[diff_module7] << endl;
        }
        else
        {
            cout << days[7 + diff_module7] << endl;
        }
    }
}
