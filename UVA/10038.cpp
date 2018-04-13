#include <iostream>

using namespace std;

int main(void)
{
    int n;

    while (cin >> n)
    {
        int flag = 1, i = 0;

        int min, sub;
        int *diff = new int[n];
        cin >> min;
        while (i < n-1)
        {
            cin >> sub;
            diff[i++] = ((min - sub) > 0) ? (min - sub) : -(min - sub);
            min = sub;
        }

        for (i = n-2; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (diff[j] > diff[j+1])
                {
                    int temp = diff[j];
                    diff[j] = diff[j+1];
                    diff[j+1] = temp;
                }
            }
        }

        i = 0;
        for (i = 0; flag && i < n-1; i++)
        {
            if(diff[i] != i + 1)
            {
                flag = 0;
            }
        }

        if (flag)
        {
            cout << "Jolly\n";
        }
        else
        {
            cout << "Not jolly\n";
        }

        delete [] diff;
    }
}
