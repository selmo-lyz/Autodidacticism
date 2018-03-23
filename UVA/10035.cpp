/*
 * Date: 20180318
 * Name: Primary Arithmetic
 * 說明:
 *     輸出進位的次數，且輸入的數 < 10位數
 * 問題:
 *     1. 如何計算進位的次數
 *     2. 如何將進位加到下次計算
 */
#include <iostream>

using namespace std;

int main()
{
    int input01 = 1, input02 = 1;

    // 當輸入0 0時，結束程式
    while (input01 != 0 && input02 != 0)
    {
        int num01, num02, count = 0, ctemp = 0, carry;
        cin >> input01 >> input02;
        num01 = input01, num02 = input02;

        while (num01 != 0 || num02 != 0)
        {
            // 若計數與上次不同，即前一位進位，則將 carry = 1
            if (count == ctemp)  carry = 0;
            else  carry = 1;

            // 若尾數超過10，就計一次數
            if (num01 % 10 + num02 % 10 + carry >= 10)
                ++count;

            // 更新數值
            num01 /= 10, num02 /= 10;

            // 若下輪不可能進位，則將上次計數設為當前計數
            if (num01 % 10 + num02 % 10 + 1 < 10)  ctemp = count;
        }

        // 視題目條件控制輸出
        if (!count && input01 != 0 && input02 != 0)
            cout << "No carry operations\n";
        else if (input01 != 0 && input02 != 0 && count == 1)
            cout << "1 carry operation.\n";
        else if (input01 != 0 && input02 != 0)
            cout << count << " carry operations.\n";
    }
}
