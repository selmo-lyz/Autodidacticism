/*
 * Date: 20180319
 * Name: The 3n+1 problem
 * 說明:
 *     輸入兩個數值，將兩數值範圍內(i <= x <= j)的數值依照"3n+1問題"的規則處理，
 *     每次處理都紀錄步驟數，將最初輸入的兩數值與最大步驟數作為輸出
 *     輸入的範圍為 0 < x < 1,000,000，且可假設運算時不會有overflow a 32-bit integer的問題
 * 問題:
 *     1. 輸入的順序不一定是由小到大
 *     2. 輸出時要依照原輸入順序
 * pesudocode:
 *     func 3n+1 Problem
 *         int count = 0
 *
 *         loop (n != 1)
 *             count + 1
 *
 *             if (n % 2 == 1)  n = 3*n+1
 *             else n = n / 2
 *          count + 1
 *
 *         return count
 *
 *     loop (1)
 *         input input01, input02
 *
 *         int numstart, numfinal
 *         if (input01 > input02)
 *             numstart = input02
 *             numfinal = input01
 *         else
 *             numstart = input01
 *             numfinal = input02
 *
 *         int num, count, max = 0
 *         loop (num in numstart, numfinal)
 *             count = func 3n+1 Problem(num)
 *             if (count > max)  max = count
 *
 *         cout << input01 << " " << input02 << " " << max << endl;
 */
#include <iostream>

using namespace std;

int problem(int n)
{
    int count = 0;

    while (n != 1)
    {
        count++;
        if (n % 2 == 1)  n = 3 * n + 1;
        else n /= 2;
    }
    count ++;

    return count;
}

int main()
{
    while (1)
    {
        int input01, input02, numstart, numfinal;
        cin >> input01 >> input02;

        // 1. & 2. 藉由將輸入交給另一組變數避免順序錯亂
        if (input01 > input02)
        {
            numstart = input02;
            numfinal = input01;
        }
        else
        {
            numstart = input01;
            numfinal = input02;
        }

        int num, count, max = 0;
        for (num = numstart; num <= numfinal; num++)
        {
            count = problem(num);
            if (count > max)  max = count;
        }

        cout << input01 << " " << input02 << " " << max << endl;
    }
}
