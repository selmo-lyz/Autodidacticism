#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Poker
{
public:

    void shuffle(void)
    {
        srand(time(NULL));

        for (int i = 0; i < 52; i++)
        {
            dealed[i] = 0;
        }
        count = 0;
    }

    int deal(void)
    {
        int data = rand() % 52 + 1;

        while (repeat(data))
        {
            data = rand() % 52 + 1;
        }
        ++count;
        dealed[data-1] = 1;

        return data;
    }

    bool empty(void)
    {
        if (count == 52)
        {
            cout << "!ERROR: 牌庫已空\n";
            return 1;
        }
        return 0;
    }

    void Game01(void);

    Poker()
    {
        srand(time(NULL));
        count = 0;
        for (int i = 0; i < 52; i++)
        {
            dealed[i] = 0;
        }
    }

private:
    int count, dealed[52];

    bool repeat(int data)
    {
        if (dealed[data-1] == 1)
            return 1;
        else
            return 0;
    }
};

void Poker::Game01(void)
{
    const char suits[4][7] = {"梅花", "方塊", "紅心", "黑桃"};
    int wins = 0, total = 0;
    char option = 'y';

    while (option == 'y' || option == 'Y')
    {
        ++total;

        if (empty())
        {
            shuffle();
            cout << "!NOTICE: 牌庫已重新洗牌!\n";
        }
        else
        {
            while (1)
            {
                cout << "請問是否要洗牌?(y/N): ";
                cin >> option;
                cin.get();
                if (option == 'y' || option == 'Y')
                {
                    shuffle();
                    cout << "!NOTICE:洗牌已完成!\n";
                    break;
                }
                else if (option == 'n' || option == 'N')
                {
                    break;
                }

                cout << "請輸入 y(Yes) 或 N(No) 來做選擇\n";
            }
        }

        cout << "\n開始發牌!\n";
        int originalP_player = deal(), originalP_computer = deal();
        int point_player = (originalP_player - 1) % 13 + 1,
            point_computer = (originalP_computer - 1) % 13 + 1;
        int suit_player = (originalP_player - 1) / 13,
            suit_computer = (originalP_computer - 1) / 13;

        cout << "請選擇要 (1)比大 或 (2)比小: ";
        cin >> option;
        while (option != '1' && option != '2')
        {
            cout << "!ERROR: 輸入錯誤,在輸入一次!\n";
            cout << "請選擇要 (1)比大 或 (2)比小: ";
            cin >> option;
        }
        cin.get();
        cout << endl;

        if (point_player > point_computer)
        {
            if (option == '1')
            {
                cout << "!Result: WIN!\n";
                ++wins;
            }
            if (option == '2')
                cout << "!Result: LOSE!\n";
        }
        else if (point_player < point_computer)
        {
            if (option == '1')
                cout << "!Result: LOSE!\n";
            if (option == '2')
            {
                cout << "!Result: WIN!\n";
                ++wins;
            }
        }
        else
        {
            if (suit_player > suit_computer)
            {
                if (option == '1')
                {
                    cout << "!Result: WIN!\n";
                    ++wins;
                }
                if (option == '2')
                    cout << "!Result: LOSE!\n";
            }
            else
            {
                if (option == '1')
                    cout << "!Result: LOSE!\n";
                if (option == '2')
                {
                    cout << "!Result: WIN!\n";
                    ++wins;
                }
            }
        }

        cout << "Player: " << suits[suit_player] << point_player
             << endl
             << "Computer: " << suits[suit_computer] << point_computer
             << endl << endl
             << "是否在來一局(y/N): ";
        cin >> option;
        cin.get();
        cout << endl;

        if (option == 'n' || option == 'N')
        {
            cout << "勝場統計:\n"
                 << "勝場數: " << wins << endl
                 << "敗場數: " << total - wins << endl;
        }
    }
}

int main(void)
{
    Poker c;
    c.Game01();
}
