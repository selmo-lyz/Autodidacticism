#include <iostream>

using namespace std;

int main(void)
{
    bool first = 1;
    char input;

    while (cin.get(input))
    {
        if (input == '"')
        {
            if (first)
            {
                cout << "``";
                first = 0;
            }
            else
            {
                cout << "''";
                first = 1;
            }
        }
        else
        {
            cout << input;
        }
    }
}
