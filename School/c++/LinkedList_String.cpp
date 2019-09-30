/*
 * Name: LinkedList_String.cpp
 * 說明：
 *     使用Menu類別操作node類別，以實作Linked List
 *     使用字元陣列name作搜尋的依據
 */
#include <iostream>
#include <cstring>

using namespace std;

class node
{
public:
    void InData(int arg)
    {
        data = arg;
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

    char* OutName(void)
    {
        return name;
    }

    node* OutPtr(void)
    {
        return ptr;
    }

private:
    int data;
    char name[11];
    node* ptr;
};

class Menu
{
public:
    int display();

    Menu(node* p = NULL)
    {
        h = p;
    }
    ~Menu()
    {
        while (h)
        {
            node* p = h, *q = h;
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
            cout << "\n-----New-----\n";
            h = add(h);
            cout << "\n-----New-----\n";
            break;
        case '2':
            cout << "\n-----Delete-----\n";
            h = del(h);
            cout << "\n-----Delete-----\n";
            break;
        case '3':
            cout << "\n-----Search-----\n";
            search(h);
            cout << "\n-----Search-----\n";
            break;
        case '4':
            cout << "\n-----Print-----\n";
            print(h);
            cout << "\n-----Print-----\n";
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
    while (p)
    {
        cout << p->OutData() << ":" << p->OutName() << " -> ";

        p = p->OutPtr();
    }
    cout << "|";
}

void Menu::search(node* p)
{
    if (!h)
    {
        cout << "錯誤: 串列已無資料 !";
        return;
    }

    cout << "請輸入要查詢的資料: ";
    char str[11];
    cin.getline(str, 10);

    while (p)
    {
        if (!strcmp(str, p->OutName()))
        {
            cout << "\nData: " << p->OutData()
                 << "\nName: " << p->OutName()
                 << "\nAddress: " << p;
            return;
        }
        p = p->OutPtr();
    }

    cout << "注意: 查無此資料 !";
}

node* Menu::add(node* h)
{
    int arg;
    char str[11];
    cout << "請輸入要增新的資料: \ndate: ";
    cin >> arg;
    while (cin.get() != '\n') {}
    cout << "name: ";
    cin.getline(str, 11);


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

    cout << "已增新資料: " << arg << ":" << str;

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
        cout << "請輸入要刪除的資料: ";
        char str[11];
        cin.getline(str, 10);

        char option;
        node* q = h, * p = h;
        while (q != NULL && strcmp(str, q->OutName()))
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
                    cout << "已刪除資料: " << str;
                    break;
                }
                else if (option == 'n' || option == 'N')
                    return h;
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
