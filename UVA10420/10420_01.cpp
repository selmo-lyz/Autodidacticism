/*
 * Date: 20180321
 * Name: List of Conquests
 * 說明:
 * pesudocode:
 *     先輸入個數，後輸入每個人的國家與姓名，輸出各國家名稱與人數
 *     輸入最多可以有2000行，每一行最多75個字元，可以假設國家名稱只有一個word
 *     輸出需依照字母順序輸出，格式如:"國家名 人數"
 * 問題:
 *     1. 如何排列字串
 *     2. 如何刪除輸入時的空白字元
 * pesudocode:
 *     times = 0
 *     country[2000][76] = {0}
 *
 *     intput times
 *     loop (i in 0 ~ times-1)
 *         input country[i]
 *
 *     loop (i in times-1 ~ 1)
 *         loop (j in 0 ~ i-1)
 *             loop (k in 0 ~ 76-1)
 *                 if (country[j][k] > country[j+1][k])
 *                     swap(country[j], country[j+1])
 *
 *     count = 1
 *     output country[0]
 *     loop (i in 1 ~ times-1)
 *         if (country[i] == country[i-1])
 *             count++;
 *         else
 *             output count
 *             count = 1
 *             output country[i]
 *     output count
 */
#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    int times = 0;
    char country[2000][76] = {0};

    // input string
    cin >> times;
    for (int i = 0; i < times; i++)
    {
        cin >> country[i];
        while (cin.get() != '\n') {}
    }

    // delet space before each word
    /*
     * 2. 因為空白字元在字前會妨礙比對，故需移除
     */
    for (int i = 0; i < times; i++)
    {
        int j = 0;
        while (country[i][j++] == ' ') {}

        int k = 0;
        while (k != j-1)
        {
            country[i][k++] = country[i][j-1];
            country[i][j-1] = ' ';
            ++j;
        }
    }

    // sort
    /*
     * 1. 除了Bubble Sort外，要處理3件事，
     *    1) 因為優先比對叫前面字元，故交換後，不能繼續比對
     *    2) 當優先比對的字元較小時，需防止繼續比對
     *    3) 要防止比對字元超過字串範圍
     */
    for (int i = times-1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            for (int k = 0; k < 76; k++)
            {
                if (k >= strlen(country[j]) || k >= strlen(country[j+1]))
                    break;

                if (country[j][k] < country[j+1][k])
                    break;

                if (country[j][k] > country[j+1][k])
                {
                    char temp[76] = {0};
                    strcpy(temp, country[j]);
                    strcpy(country[j], country[j+1]);
                    strcpy(country[j+1], temp);
                    break;
                }
            }
        }
    }

    // count & output
    /*
     * 1. 符合格式的輸出步驟：
     *    1) 先輸出一個字串，並初始化該字串(國家)數目count為1
     *    2) 比對當前字串與前一字串
     *    3.1) 若相同，則++count
     *    3.2) 若不同，則輸出當前count，並輸出當前字串，且需初始化count = 1
     *    4) 重複2~3，直到字串陣列皆trace完
     */
    int count = 1;

    cout << country[0];
    for (int i = 1; i < times; i++)
    {
        if (!strcmp(country[i], country[i-1]))
            ++count;
        else
        {
            cout << " " << count << endl;
            count = 1;
            cout << country[i];
        }
    }
    cout << " " << count << endl;
}
