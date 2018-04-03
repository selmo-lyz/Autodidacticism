#include <iostream>

using namespace std;

bool EmptyRow(char* str)
{
    int Col = 0;
    for (; !str[Col] && Col < 101; ++Col) {}

    return (Col == 101) ? 1 : 0;
}

bool EmptyCol(char (*str)[101], int totalRow, int Col)
{
    int Row = 0;
    for (; !str[Row][Col] && Row <= totalRow; ++Row) {}

    return (Row > totalRow) ? 1 : 0;
}

int main(void)
{
    int totalRow = 0;
    char str[100][101] = {0};

    do
    {
        cin.getline(str[totalRow], 101);
    }
    while (!EmptyRow(str[totalRow]) && ++totalRow < 100);
    --totalRow;

    for (int Col = 0; Col < 101 && !EmptyCol(str, totalRow, Col); ++Col)
    {
        for (int Row = totalRow; Row >= 0; --Row)
        {
            if (str[Row][Col] == '\00')
            {
                cout << ' ' ;
            }
            else
            {
                cout << str[Row][Col];
            }
        }
        cout << endl;
    }
}
