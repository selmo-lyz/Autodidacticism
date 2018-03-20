/*
 * Date: 20180318
 * Name: Hashmat the Brave Warrior
 * 說明:
 *     程式的目的是算出差值，輸入必定不超過2^32且輸出永遠 >= 0，
 *     輸入直到讀入 'End of File'
 * 問題:
 *     1. 要如何判別輸入為'End of File'
 *     2. 變數需能儲存 <= 2^32 的數值
 *     3. 需要判別哪一個數為 Hashmat 的軍隊
 */
#include <iostream>

using namespace std;

int main()
{
    /* 
     * 2. long long int: 64 bits, -2^63 ~ 2^63-1
     *    因為需判別正負值，故無法用 unsigned
     */
    long long num01 = 0, num02 = 0;

    /*
     * 1. 直接使用 cin ，當讀取到 EOF 時，會回傳error flag
     *    使輸入終止，並跳出迴圈執行後來的工作，
     *
     *    關於 error flag 的知識，見 C++PrimerPlus C06S07.
     */
    while (cin >> num01 >> num02)
    {
        /*
         * 3. 因為Hashmat從未打過仗，故他的兵永遠較小
         */
        if (num02 - num01 > 0)
            cout << num02 - num01 << endl;
        else
            cout << num01 - num02 << endl;
    }
}
