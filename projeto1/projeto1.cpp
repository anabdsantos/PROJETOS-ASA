#include <iostream>
#include <vector>
#include <list>

using namespace std;

int n, m, numero;
vector<int> sequence;
vector<vector<int>> table;
vector<vector<vector<pair<int, vector<int>>>>> calculator;

void read()
{
    cin >> n >> m;
    table = vector<vector<int>>(n, vector<int>(n));
    calculator = vector<vector<vector<pair<int, vector<int>>>>>(m, vector<vector<pair<int, vector<int>>>>(m));
    sequence.resize(m);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> table[i][j];
        }
    }

    for (int k = 0; k < m; k++)
    {
        cin >> sequence[k];
        calculator[k][k] = std::vector<pair<int, vector<int>>>{make_pair(sequence[k], vector<int>())};
    }

    cin >> numero;
}

std::vector<pair<int, vector<int>>> johnSnailRecursiva(int l, int r)
{
    if (l == r || !calculator[l - 1][r - 1].empty())
    {
        return calculator[l - 1][r - 1];
    }

    int min = l;
    int max = r;
    vector<pair<int, vector<int>>> values;
    bool sair = false;
    while (r > min && !sair)
    {
        std::vector<pair<int, vector<int>>> x = johnSnailRecursiva(min, r - 1);
        std::vector<pair<int, vector<int>>> y = johnSnailRecursiva(r, max);
        r--;
        for (size_t i = 0; i < x.size(); i++)
        {
            for (size_t j = 0; j < y.size(); j++)
            {
                int val = table[x[i].first - 1][y[j].first - 1];
                bool continuar = false;
                for (size_t k = 0; k < values.size(); k++)
                {
                    if (val == values[k].first)
                    {
                        continuar = true;
                        break;
                    }
                }
                if (continuar)
                {
                    continue;
                }

                vector<int> valoresbolinha(3);
                valoresbolinha[0] = r;
                valoresbolinha[1] = x[i].first;
                valoresbolinha[2] = y[j].first;
                values.push_back(make_pair(val, valoresbolinha));

                if (values.size() >= (size_t)n || (val == numero && min == 1 && max == m))
                {
                    sair = true;
                    break;
                }
            }
            if (sair)
            {
                break;
            }
        }
    }

    calculator[l - 1][max - 1] = values;

    return values;
}

string divide(int valor, int l, int r)
{
    vector<pair<int, vector<int>>> values = calculator[l - 1][r - 1];
    if (r - l <= 0)
    {
        return to_string(values[0].first);
    }
    for (std::pair<int, std::vector<int>> v : values)
    {
        if (v.first == valor)
        {
            int div = v.second[0];
            int val1 = v.second[1];
            int val2 = v.second[2];
            string result1 = divide(val1, l, div);
            string result2 = divide(val2, div + 1, r);
            if (result2 == "")
            {
                return result1;
            }
            if (result1 == "")
            {
                return result2;
            }
            string result = "(" + result1 + " " + result2 + ")";
            return result;
        }
    }
    return "";
}

int johnSnail()
{
    std::vector<pair<int, vector<int>>> val = johnSnailRecursiva(1, m);
    int last = (int)val.size() - 1;
    if (val[last].first != numero)
    {
        cout << 0 << endl;
        return 0;
    }
    string exp = divide(numero, 1, m);
    cout << 1 << endl;
    cout << exp << endl;
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    read();
    johnSnail();
    return 0;
}
