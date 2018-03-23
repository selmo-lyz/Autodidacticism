/*
 * Date: 20180319
 * Name: You can say 11
 * 說明:
 *     判斷輸入的正整數 N，是否為 11 的倍數，當輸入 0 時停止
 *     輸入的值可以到 1000 位數
 * 問題:
 *     1. 輸入值需為 1000 位數，任何整數皆不可能
 *     2. 如何判斷 11 的倍數
 *     3. 如何將ASCII字元編碼轉換為Integer
 *     4. 如何使用 0 結束程式
 * pesudocode:
 * loop (1)
 *      char input[1001] = {0};
 *      input input
 *
 *      int i = 0, sumOdd = 0, sumEven = 0;
 *      loop (input[i] != '\n')
 *          if (i % 2 = 1)
 *              sumOdd += (int)input[i];
 *          else
 *              sumEven += (int)input[i];
 *          i++;
 *
 *      if ((sumOdd + sumEven) == 0)
 *          break;
 *
 *      loop (input[i] != '\n')
 *          output input[i]
 *          i++;
 *
 *      if ((sumOdd - sumEven) % 11 == 0)
 *          output " is a multiple of 11."
 *      else
 *          output " is not a multiple of 11."
 */
#include <iostream>

using namespace std;

int main()
{
    while (1)
    {
        // 初始化
        /*
         * 1. 使用字元陣列儲存輸入數字
         */
        char input[1001] = {0};

        // 輸入字串
        cin >> input;

        // 處理題目
        /*
         * 2. 使用 奇位數-偶位數 % 11 的值為 0，判斷其為11的倍數
         */
        int sumOdd = 0, sumEven = 0;
        for (int i = 0; input[i] != '\0'; i++)
        {
            /*
             * 3. 使用下面被註解的程式碼後，可知 0 的ASCII值為 48
             *    故在字元轉換成整數時，要減去 48
             */
            if (i % 2 == 1)
                sumOdd += input[i]-48;
            else
                sumEven += input[i]-48;
            /*
             * 用來測試上面 input[i] 的轉換是否為ASCII，
             * 0 的ASCII值為 48
             *  cout << "SumOdd: " << sumOdd << endl;
             *  cout << "SumEven: " << sumEven << endl;
            */
        }

        // 跳出條件
        /*
         * 4. 在輸出之前，用sumOdd+sumEven判斷輸入值為零，結束程式
         */
        if ((sumOdd + sumEven) == 0)
            return 0;

        // 輸出
        for (int i = 0; input[i] != '\0'; i++)
            cout << input[i];

        if ((sumOdd - sumEven) % 11 == 0)
            cout << " is a multiple of 11.\n";
        else
            cout << " is not a multiple of 11.\n";
    }
}
