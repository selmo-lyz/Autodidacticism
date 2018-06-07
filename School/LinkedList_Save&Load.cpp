#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class node
{
public:
    void InData(int arg)
    {
        data = arg;
    }

    void InGrade_m(int arg)
    {
        grade_m = arg;
    }

    void InGrade_f(int arg)
    {
        grade_f = arg;
    }

    void InPtr(node* p)
    {
        ptr = p;
    }

    void InName(char* str)
    {
        strcpy(name, str);
    }

    int OutData(void)
    {
        return data;
    }

    int OutGrade_m(void)
    {
        return grade_m;
    }

    int OutGrade_f(void)
    {
        return grade_f;
    }

    char* OutName(void)
    {
        return name;
    }

    node* OutPtr(void)
    {
        return ptr;
    }

private:
    int data, grade_m, grade_f;
    char name[11];
    node* ptr;
};

class Menu
{
public:
    int display();

    Menu(node* ht = NULL)
    {
        int data, grade_m, grade_f;
        char name[11];
        ifstream fin("log.txt");

        if (!fin)
        {
            cerr << "錯誤: 檔案開啟失敗 !\n";
        }
        else
        {
            fin.get();      // 因為儲存格式，故需去除儲存文件最前面的 \n
            while (!fin.getline(name, 11).eof())
            {
                fin >> data >> grade_m >> grade_f;
                fin.get();      // 去除 fin >> 後的 \n

                node* pt = new node;
                pt->InData(data);
                pt->InGrade_m(grade_m);
                pt->InGrade_f(grade_f);
                pt->InName(name);
                pt->InPtr(NULL);

                if (!ht)
                {
                    ht = pt;
                }
                else
                {
                    node *p = ht;
                    while (p->OutPtr())
                    {
                        p = p->OutPtr();
                    }
                    p->InPtr(pt);
                }
            }
        }

        fin.close();
        h = ht;
    }

    ~Menu()
    {
        ofstream fout("log.txt", ios_base::trunc);

        if (!fout)
        {
            cerr << "錯誤: 檔案開啟失敗 !\n";
        }
        else
        {
            node* p = h;
            while (p)
            {
                fout << endl << p->OutName() << endl << p->OutData()
                    << endl << p->OutGrade_m() << endl << p->OutGrade_f();

                p = p->OutPtr();
            }
            fout.close();
        }

        while (h)
        {
            node *q = h, *p = h;

            while (q->OutPtr())
            {
                p = q;
                q = q->OutPtr();
            }

            if (p != q)
            {
                p->InPtr(NULL);
                delete q;
            }
            else
            {
                delete h;
                break;
            }
        }
    }

private:
    node* h;

    int getselection();
    int selection(int option);
    void print(node* h);
    void search(node* h);
    node* add(node* h);
    node* del(node* h);
};

int Menu::display()
{
    cout << "\n===============\n"
         << "選單\n"
         << "1. 增新\n"
         << "2. 刪除\n"
         << "3. 查詢\n"
         << "4. 列印\n"
         << "5. 離開\n"
         << "請選擇功能： ";

    return selection(getselection());
}

int Menu::getselection()
{
    char option;
    cin >> option;

    while (cin.get() != '\n') {}

    return option;
}

int Menu::selection(int option)
{
    switch (option)
    {
        case '1':
            cout << "\nNew----------\n";
            h = add(h);
            cout << "\n-------------\n";
            break;
        case '2':
            cout << "\nDelete----------\n";
            h = del(h);
            cout << "\n----------------\n";
            break;
        case '3':
            cout << "\nSearch----------\n";
            search(h);
            cout << "\n----------------\n";
            break;
        case '4':
            cout << "\nPrint----------\n";
            print(h);
            cout << "\n---------------\n";
            break;
        case '5'  :
            cout << "\n-----Exit-----\n";
            return 0;
        default :
            cout << "請輸入數字 1 ~ 5 ， 選擇功能 ";
    }

    return 1;
}


void Menu::print(node* p)
{
    cout << "成績單---------\n";
    while (p)
    {
        cout << "座號: " << p->OutData() << endl
             << "姓名: " << p->OutName() << endl
             << "期中考成績: " << p->OutGrade_m() << endl
             << "期末考成績: " << p->OutGrade_f() << endl;

        p = p->OutPtr();
    }
}

void Menu::search(node* p)
{
    if (!h)
    {
        cout << "錯誤: 串列已無資料 !";
        return;
    }

    cout << "請輸入要查詢的資料(請輸入座號-整數): ";
    int arg;
    cin >> arg;

    while (p)
    {
        if (arg == p->OutData())
        {
            cout << "\n座號: " << p->OutData()
                 << "\n姓名: " << p->OutName()
                 << "\n期中考成績: " << p->OutGrade_m()
                 << "\n期末考成績: " << p->OutGrade_f();
            return;
        }
        p = p->OutPtr();
    }

    cout << "注意: 查無此資料 !";
}

node* Menu::add(node* h)
{
    int arg, grade_m, grade_f;
    char str[11];
    cout << "請輸入要增新的資料: \n座號 (整數): ";
    cin >> arg;
    while (cin.get() != '\n') {}
    cout << "名字 (字串): ";
    cin.getline(str, 11);
    cout << "期中考成績 (整數): ";
    cin >> grade_m;
    cout << "期末考成績 (整數): ";
    cin >> grade_f;


    node* p = h;
    while (p)
    {
        if (arg == p->OutData() || !strcmp(str, p->OutName()))
        {
            cout << "錯誤: 資料重複 !";
            return h;
        }
        p = p->OutPtr();
    }

    node* pt = new node;
    pt->InData(arg);
    pt->InGrade_m(grade_m);
    pt->InGrade_f(grade_f);
    pt->InName(str);

    if (h == NULL)
    {
        pt->InPtr(NULL);
        h = pt;
    }
    else
    {
        node* p = h, * q = h;
        while (q != NULL && arg > q->OutData())
        {
            p = q;
            q = q->OutPtr();
        }

        pt->InPtr(q);
        if (p == q)  h = pt;
        else  p->InPtr(pt);
    }

    cout << "\n已增新資料:\n座號: " << arg
         << "\n姓名:" << str
         << "\n期中考成績: " << grade_m
         << "\n期末考成績: " << grade_f;

    return h;
}

node* Menu::del(node* h)
{

    if (!h)
    {
        cout << "錯誤: 串列已無資料 !";
        return h;
    }
    else
    {
        cout << "請輸入要刪除的資料(請輸入座號-整數): ";
        int arg;
        cin >> arg;

        char option;
        node* q = h, * p = h;
        while (q != NULL && q->OutData() != arg)
        {
            p = q;
            q = q->OutPtr();
        }

        if (!q)
        {
            cout << "注意: 查無資料 !";
            return h;
        }
        else
        {
            cout << "請問要刪除該資料嗎? (Y/N) : ";
            while (cin >> option)
            {
                if (option == 'y' || option == 'Y')
                {
                    if (p == q)
                    {
                        h = q->OutPtr();
                    }
                    else
                    {
                        p->InPtr(q->OutPtr());
                    }
                    delete q;
                    cout << "已刪除資料: " << arg;
                    break;
                }
                else if (option == 'n' || option == 'N')
                {
                    return h;
                }
                else
                {
                    cout << "錯誤: 請輸入 y/Y 或 n/N !";
                }
            }
        }
    }

    return h;
}

int main(void)
{
    Menu m;

    while (m.display()) {}
}
