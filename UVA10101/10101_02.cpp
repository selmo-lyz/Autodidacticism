/*
 * Date: 20180320
 * Name: Bangla Numbers
 * 說明:
 *     將輸入的數值轉換為Bangla型式的數值，輸入的數為 non-negative 且 <= 999999999999999
 *
 *     Bangla 型式:
 *         'kuti': 10000000
 *         'lakh':   100000
 *         'hajar':    1000
 *         'shata':     100
 *
 *     輸出格式:
 *         input:
 *             23764
 *             45987458973958
 *         output:
 *             1. 23 hajar 7 shata 64
 *             2. 45 lakh 89 hajar 7 shata 45 kuti 89 lakh 73 hajar 9 shata 58
 * 問題:
 *     1. 要能夠儲存 10^16-1 ，至少需要大約 2^54 位元以上
 *     2. 如何轉換並顯示Bangla
 *     3. 如何符合輸出規則，注意空格且需考慮特殊情形，如：0, 10, 10110,...
 * pesudocode:
 *     func problem(n)
 *         if (n == 0) return
 *
 *         if (n >= 10000000)
 *         {
 *             problem(n/10000000)
 *             output " kuti"
 *             n %= 10000000
 *         }
 *         ...
 *         if (n >= 100)
 *         {
 *             problem(n/100)
 *             output " shata"
 *             n %= 100
 *         }
 *
 *         if (n) output n
 *
 *     input, t = 1
 *     loop (intput input)
 *         problem(input)
 *         if (!input) output " 0"
 *         output newline
 * 參考:
 *     http://knightzone.org
 */
#include <iostream>
#include <iomanip>

using namespace std;

/*
 * 2. 用以下步驟轉換數值，
 *    1. 確認是否為 0
 *    2. 比較大小，確認位數
 *    3. 擷取一定範圍以上的位數，進入遞迴
 *    4. 重複1~3，直到數值為零或不超過最低範圍
 *    5. 輸出當前數值
 *    6. 回到前一層遞迴，輸出位數名稱
 *    7. 更新數值
 *    8. 重複2~7，直到最初遞迴的數值到零或不超過最低範圍
 */
void problem(long long n)
{
    static int digit[4] = {10000000, 100000, 1000, 100};
    static char name[4][7] = {" kuti", " lakh", " hajar", " shata"};

    if (n == 0)  return;

    for (int i = 0; i < 4; i++)
    {
        if (n >= digit[i])
        {
            problem(n/digit[i]);
            cout << name[i];
            n %= digit[i];
        }
    }

    if (n)  cout << " " << n;
}

int main()
{
    /*
     * 1. 需用long long 儲存
     */
    long long input = 1;
    int t = 1;

    while (cin >> input)
    {
        /*
         * 3. 輸出時要注意需輸出序號、空格需佔4格，且需輸出 0
         */
        cout << setw(4) << right << t++ << ".";

        problem(input);
        if (!input) cout << " 0";

        cout << endl;
    }
}
