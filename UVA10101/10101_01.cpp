/*
 * Date: 20180320
 * Name: Bangla Numbers
 * 說明:
 *     將輸入的數值轉換為Bangla型式的數值，輸入的數為 non-negative 且 <= 999999999999999
 *     Bangla 型式:
 *         'kuti': 10000000
 *         'lakh':   100000
 *         'hajar':    1000
 *         'shata':     100
 *     各位數對應的單位
 *         15: kuti kuti
 *         14, 13: lakh kuti
 *         12, 11: hajar kuti
 *         10: shata kuti
 *         9, 8: kuti
 *         7, 6: lakh
 *         5, 4: hajar
 *         3: shata
 *         2, 1:
 *         註：陣列是從0開始
 *     輸出格式:
 *         "case number. Bangla型式數值"，如:
 *         input:
 *             23764
 *             45987458973958
 *         output:
 *             1. 23 hajar 7 shata 64
 *             2. 45 lakh 89 hajar 7 shata 45 kuti 89 lakh 73 hajar 9 shata 58
 * 問題:
 *     1. 要能夠儲存 10^16-1 ，至少需要大約 2^54 位元以上
 *     2. 如何轉換並顯示Bangla
 *     3. 如何符合輸出規則，需考慮特殊情形，如：0, 10, 10110,...
 * pesudocode:
 * loop (1)
 *     input,  output[15]
 *     input input
 *
 *     i = 0
 *     loop (input)
 *         output[i] = input % 10
 *         intput /= 10
 *         i =i + 1
 *
 *     我心已死。
 *     if (output[15])
 *         cout << output[15] << " kuti "
 *     ......
 *     if (output[2] || output[1])
 *         cout << output[2] << output[1]
 *     cout << endl
 */
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    unsigned long long input = 1, temp = 1;
    int t = 1;

    while (cin >> temp)
    {
        input = temp;
        int output[15] = {0};
        for (int i = 0; input; i++)
        {
            output[i] = input % 10;
            input /= 10;
        }


        /*
         * 我心已死。
         */
        cout << setw(4) << right << t++ << ".";
        if (output[14])
            cout << " " << output[14] << " kuti";

        if (output[13])
            cout << " " << output[13] << output[12];
        else if (output[12])
            cout << " " << output[12];
        if (output[13] || output[12])
            cout << " lakh";

        if (output[11])
            cout << " " << output[11] << output[10];
        else if (output[10])
            cout << " " << output[10];
        if (output[11] || output[10])
            cout << " hajar";

        if (output[9])
            cout << " " << output[9] << " shata";

        if (output[8])
            cout << " " << output[8] << output[7];
        else if (output[7])
            cout << " " << output[7];

        if (output[14] || output[13] || output[12] || output[11] ||
            output[10] || output[9] || output[8] || output[7])
            cout << " kuti";

        if (output[6])
            cout << " " << output[6] << output[5];
        else if (output[5])
            cout << " " << output[5];
        if (output[6] || output[5])
            cout << " lakh";

        if (output[4])
            cout << " " << output[4] << output[3];
        else if (output[3])
            cout << " " << output[3];
        if (output[4] || output[3])
            cout << " hajar";;

        if (output[2])
            cout << " " << output[2] << " shata";

        if (output[1])
            cout << " " << output[1] << output[0];
        else if (output[0])
            cout << " " << output[0];

        if (temp == 0)
            cout << " 0";
        cout << endl;
    }
}
