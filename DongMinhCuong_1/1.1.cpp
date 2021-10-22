#include <iostream>
using namespace std;

int main()
{
    string input;
    cout << "Please enter something: \n";
    getline(cin, input);

    int letter = 0, digit = 0;

    for (int i = 0; i < input.length(); i++)
    {
        if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            letter++;
        if (input[i] >= '0' && input[i] <= '9')
            digit++;
    }

    cout << "Number of letter: " << letter << endl;
    cout << "Number of digit: " << digit << endl;
    cout << "Number of symbol: " << input.length() - letter - digit << endl;

    return 0;
}