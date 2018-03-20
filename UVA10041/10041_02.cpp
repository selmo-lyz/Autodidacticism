#include <iostream>

using namespace std;

int main()
{
    int cases;
    cin >> cases;
    for (int i = 0; i < cases; i++)
    {
        int r, num[500] = {0}, mid, Sum = 0;
        cin >> r;
        mid = r/2;

        for (int j = 0; j < r; j++)
            cin >> num[j];

        for (int j = r-1; j > 0; j--)
        {
            for (int k = 0; k < j; k++)
            {
                if (num[k] > num[k+1])
                {
                    int temp = num[k];
                    num[k] = num[k+1];
                    num[k+1] = temp;
                }
            }
        }

        for (int j = 0; j < r; j++)
        {
            if (num[mid] - num[j] > 0)
                Sum += num[mid] - num[j];
            else
                Sum += num[j] - num[mid];
        }

        cout << Sum << endl;
    }
}
