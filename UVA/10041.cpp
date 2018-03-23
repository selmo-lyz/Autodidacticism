/*
 * Date: 20180316
 * Name: Vito's Family
 * 說明:
 *     為了要輸出從他家到各親戚家最短的距離和，需將他家設在親戚家的中間，即中位數
 * Tips: 
 *     在數線上，離各點距離的總和最短的點是中點，
 *     若是奇數則是中點，若是偶數則是中點的左右點
 * 步驟:
 *     1. initialize
 *     2. sort
 *     3. summary
 *     4. 1~3 loop cases times
 */

#include <iostream>

using namespace std;

void findMid(void)
{
    // initialize
    int r, num[500] = {0};
    cin >> r;
    for (int i = 0; i < r; i++)
        cin >> num[i];

    // bubble sort
    for (int i = r-1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (num[j] > num[j+1])
            {
                int temp = num[j];
                num[j] = num[j+1];
                num[j+1] = temp;
            }
        }
    }

    // summary
    int Sum = 0;
    for (int i = 0; i < r; i++)
    {
        if ((num[r/2] - num[i]) < 0)
            Sum += num[i] - num[r/2];
        else
            Sum += num[r/2] - num[i];
    }

    cout << Sum << endl;
}

int main()
{
    int cases;
    cin >> cases;

    for (; cases; --cases)
    {
        findMid();
    }
} 
