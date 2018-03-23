/*
 * Date: 20180322
 * Name: What's Cryptanalysis
 * 簡介:
 *     先輸入一正整數代表接下來的輸入行數，
 *     再輸入的文字會包含大小寫與各種字元(包含空白字元)
 *     要輸出字母在輸入文字中的出現次數，格式如: "字母 次數"，
 *     順序為:
 *         1. 次數，由大到小
 *         2. 字母，從A到Z
 * 問題:
 *     1. A, Z, a, z 的 ASCII code ?
 *     2. 需統一輸入的大小寫，以方便比較
 * 說明:
 *     先用陣列儲存A~Z，後用陣列依行儲存各字母次數(循環行數次)，
 *     再用迴圈輸出各字母的數字，(照字母順序輸出且次數為零不輸出)
 * PesudoCode:
 *     count[26] = {0}
 *     alphabet[26] = {0}
 *     loop (i in 0~25)
 *         alphabet[i] = 65 + i
 *
 *     input lines
 *     loop (i in 1~lines)
 *         char temp
 *         input temp
 *         loop (temp != '\n')
 *             loop (j in 0~25)
 *                 if (Uppercase(temp) == alphabet[j])
 *                     count[j]++;
 *                     break;
 *             input temp
 *
 *     loop (i in 0~25)
 *         int max = 0;
 *         loop (0 in 0~25)
 *             if (count[j] > count[max])
 *                 max = j
 *
 *         if (!count[max])
 *             break;
 *
 *         output alphabat[max], count[max]
 *
 *         count[max] = 0
 */
#include <iostream>

using namespace std;

int main()
{
    int count[26] = {0};
    char alphabat[26] = {0};
    for (int i = 0; i < 26; i++)
        alphabat[i] = 65 + i;

    int lines;
    cin >> lines;
    cin.get();    // 用來除去input lines之後的'\n'

    for (int i = 0; i < lines; i++)
    {
        char temp;

        cin.get(temp);
        while (temp != '\n')
        {
            for (int j = 0; j < 26; j++)
            {
                if (toupper(temp) == alphabat[j])
                {
                    count[j]++;
                    break;
                }
            }
            cin.get(temp);
        }
    }

    for (int i = 0; i < 26; i++)
    {
        int max = 0;

        for (int j = 0; j < 26; j++)
        {
            if (count[j] > count[max])
                max = j;
        }

        if (!count[max])
            break;

        cout << alphabat[max] << " " << count[max] << endl;

        count[max] = 0;
    }
}
