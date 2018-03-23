/*
 * Date: 20180323
 * Name: Summing Digits
 * 簡介:
 *     函式f(x)的功能為使x的各位數相加，若今輸入一數n(<= 2,000,000,000)作為 x，
 *     並重複使其結果作為f(x)的x，直到結果為個位數，可重複輸入 n
 *     若n = 0，則不處理，直接結束
 *
 *     在每次輸入處理完後，需輸出每次的最終結果
 * 問題:
 *     1. 儲存位數的陣列需要多少空間
 * 說明:
 *     將f(x)看成一個函式，並按照題目條件安排，
 *     輸入0時，結束; 輸入或f(x)的回傳值<10時，跳出執行f(x)的迴圈，
 *     而f(x)函式是將參數值做"取得各位數二部曲"，再將各位數相加回傳
 * PesudoCode:
 *     func problem(arg)
 *         int digits[10] = {0}
 *         loop (arg)
 *             digits[i] = arg % 10
 *             arg /= 10
 *
 *         loop (i in 0~10-1)
 *             arg += digits[i]
 *
 *         return arg
 *
 *     int input = 1;
 *     input input
 *     loop (input)
 *         if (input == 0)
 *             break
 *
 *         loop (input >= 10)
 *             input = problem(input)
 *
 *         output input
 *
 *         input input
 */
#include <iostream>

using namespace std;

int problem(int arg)
{
    /*
     * 1. 因為輸入上限是 2,000,000,000，故空間可為10即可
     */
    int digits[10] = {0};

    for (int i = 0; arg; i++)
    {
        digits[i] = arg % 10;
        arg /= 10;
    }

    for (int i = 0; i < 10; i++)
        arg += digits[i];

    return arg;
}

int main()
{
    int input;
    cin >> input;
    while (input)
    {
        if (!input)
            break;

        while (input >= 10)
            input = problem(input);

        cout << input << endl;

        cin >> input;
    }
}
