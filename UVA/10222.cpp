/*
 * Date: 20180323
 * Name: Decode the Mad Man
 * 簡介:
 *     這次只會有一次輸入測試，且此測試會包括一或數個字，可能包含各種字元
 *     需將輸入代換成"該輸入值在鍵盤上左邊兩個的值"，並以此作為輸出
 * 問題:
 *     1. 如何代換字元
 *     2. 如何儲存輸入
 * 說明:
 *     使用對照表將輸入一一對換成字元
 * PesudoCode:
 *     alphabet[2][num]
 *     input
 *
 *     input input
 *     loop (input)
 *         loop (i in 0~num)
 *             if (input == alphabet[1][i])
 *                 output alphabet[0][i]
 *                 break
 *             else if (input == ' ')
 *                 output ' '
 *                 break
 *             else if (input == '\n')
 *                 output ' '
 *                 break
 *         input input
 */
#include <iostream>

using namespace std;

int main()
{
    /*
     * 1. 在此使用對照表進行尋找後代換，0列: 輸入值表; 1列: 輸出值表
     */
    char alphabet[2][40] = 
    {
        {
            '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
            'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
            'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
            'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        },
        {
            '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', 
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
            'z', 'x', 'c', 'v', 'b', 'n', 'm', ','
        }
    };

    /*
     * 2. 因為不知道輸入的個數有多大，故直接一個個代換
     *
     * 註: 不能在尋找字元的條件中增加 else ，
     *     這會使該次沒有符合上列條件的情形都套用 else
     */
    char input = '1';
    cin.get(input);
    while (input)
    {
        for (int i = 0; i < 40; i++)
        {
            if (tolower(input) == alphabet[0][i])
            {
                cout << alphabet[1][i];
                break;
            }
            else if (input == ' ')
            {
                cout << ' ';
                break;
            }
            else if (input == '\n')
            {
                cout << '\n';
                break;
            }
        }
        cin.get(input);
    }
}
