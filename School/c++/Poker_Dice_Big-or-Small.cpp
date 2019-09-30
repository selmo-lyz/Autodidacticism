#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Dice
{
public:
    int throw_dice(void)
    {
        int data = rand() % 6 + 1;
        return data;
    }

    Dice()
    {
        srand(time(NULL));
    }
};

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

void Game01(void)
{
    const char suits[4][7] = {"梅花", "方塊", "紅心", "黑桃"};
    int wins = 0, ties = 0, total = 0;
    bool choice = 0;        // 0 : Dice, 1 : Poker
    Poker* p;
    Dice* d;
    char option = 'y';

    // 選擇遊戲 (目前不開放選擇)
    if (choice) p = new Poker;
    else d = new Dice;

    while (option == 'y' || option == 'Y')
    {
        ++total;

        // 洗牌
        if (choice && p->empty())
        {
            p->shuffle();
            cout << "!NOTICE: 牌庫已重新洗牌!\n";
        }
        else if (choice)
        {
            while (1)
            {
                cout << "請問是否要洗牌?(y/N): ";
                cin >> option;
                cin.get();
                if (option == 'y' || option == 'Y')
                {
                    p->shuffle();
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

        // 發牌 或 擲骰子
        int point_player, point_computer,
            suit_player, suit_computer;
        if (choice)
        {
            cout << "\n開始發牌!\n";
            int originalP_player = p->deal(), originalP_computer = p->deal();
            point_player = (originalP_player - 1) % 13 + 1;
            point_computer = (originalP_computer - 1) % 13 + 1;
            suit_player = (originalP_player - 1) / 13;
            suit_computer = (originalP_computer - 1) / 13;
        }
        else
        {
            cout << "\n開始擲骰子!\n";
            point_player = d->throw_dice();
            point_computer = d->throw_dice();
        }

        // 選擇 比大 或 比小
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

        // 比較大小
        switch (option)
        {
        case '1':
            if (point_player > point_computer)
            {
                cout << "!Result: WIN!\n";
                ++wins;
            }
            else if (point_player < point_computer)
            {
                cout << "!Result: LOSE!\n";
            }
            else if (choice)
            {
                if (suit_player > suit_computer)
                {
                    cout << "!Result: WIN!\n";
                    ++wins;
                }
                else
                {
                    cout << "!Result: LOSE!\n";
                }
            }
            else
            {
                cout << "!Result: TIE!\n";
                ++ties;
            }
            break;

        case '2':
            if (point_player > point_computer)
            {
                cout << "!Result: LOSE!\n";
            }
            else if (point_player < point_computer)
            {
                cout << "!Result: WIN!\n";
                ++wins;
            }
            else if (choice)
            {
                if (suit_player > suit_computer)
                {
                    cout << "!Result: LOSE!\n";
                }
                else
                {
                    cout << "!Result: WIN!\n";
                    ++wins;
                }
            }
            else
            {
                cout << "!Result: TIE!\n";
                ++ties;
            }
            break;

        default:
            break;
        }

        // 結果
        if (choice)
        {
            cout << "Player: " << suits[suit_player] << point_player
                << endl
                << "Computer: " << suits[suit_computer] << point_computer
                << endl << endl;
        }
        else
        {
            cout << "Player: " << point_player
                << endl
                << "Computer: " << point_computer
                << endl << endl;
        }
        cout << "是否在來一局(y/N): ";
        cin >> option;
        cin.get();
        cout << endl;

        // 統計
        if (option == 'n' || option == 'N')
        {
            cout << "比賽統計:\n"
                 << "勝場數: " << wins << endl
                 << "敗場數: " << total - wins - ties << endl;
            
            if (!choice)
            {
                cout << "平局數: " << ties << endl;
            }
        }
    }
}

int main(void)
{
    Game01();
}