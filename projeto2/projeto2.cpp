#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// n - número de estações (n ≥ 2)
// m - número de ligações entre estações (m ≥ 0)
// l - número de linhas da rede de metro (l ≥ 0)
int n, m, l;
vector<vector<int>> grafo;
vector<int> colors;
vector<int> distances;

#define WHITE 0
#define GRAY 1
#define BLACK 2

void read() // m
{
    cin >> n >> m >> l;

    grafo = vector<vector<int>>(n + l); // tamanho maximo de (n+l)n ou (n+l)l

    for (int i = 0; i < m; i++) // m
    {
        int estacao_x;
        int estacao_y;
        int linha;

        cin >> estacao_x >> estacao_y >> linha;

        int linhas = linha + n - 1;

        if (find(grafo[estacao_x - 1].begin(), grafo[estacao_x - 1].end(), linhas) == grafo[estacao_x - 1].end())
        {
            grafo[estacao_x - 1].push_back(linhas);
        }
        if (find(grafo[estacao_y - 1].begin(), grafo[estacao_y - 1].end(), linhas) == grafo[estacao_y - 1].end())
        {
            grafo[estacao_y - 1].push_back(linhas);
        }

        grafo[linhas].push_back(estacao_x - 1);
        grafo[linhas].push_back(estacao_y - 1);
    }
}

int bfs(const vector<vector<int>> &grafo, int s) // (n*l)+(l*n)+n => 2nl + n
{
    colors = vector<int>(n + l, WHITE);
    distances = vector<int>(n + l, -1);
    int maxima = 0;
    queue<int> fila;
    colors[s] = GRAY;
    distances[s] = 0;
    fila.push(s);

    while (!fila.empty()) // n+l
    {
        int v = fila.front();
        fila.pop();
        for (int w : grafo[v]) // n ou l
        {
            if (colors[w] == WHITE)
            {
                fila.push(w);
                colors[w] = GRAY;
                distances[w] = (distances[v] + 1);
                if (w < n)
                {
                    maxima = max(maxima, distances[w]);
                }
            }
        }
        colors[v] = BLACK;

        if (maxima == (l + 1) * 2)
        {
            return maxima / 2 - 1;
        }
    }

    for (int i = 0; i < n; i++) // n
    {
        if (distances[i] == -1)
        {
            return -1;
        }
    }

    return maxima / 2 - 1;
}

int snailCity() // l
{
    int maximo = -1;
    int dist;
    for (int i = n; i < n + l; i++) // l
    {
        if (grafo[i].empty())
        {
            continue;
        }
        int done = 0;
        for (size_t j = 0; j < grafo[i].size(); j++)
        {
            if (grafo[grafo[i][j]].size() == 1)
            {
                dist = bfs(grafo, grafo[i][j]);
                done = 1;
                break;
            }
        }
        if (!done)
        {
            dist = bfs(grafo, grafo[i].front());
        }
        if (dist == -1)
        {
            return -1;
        }
        maximo = max(dist, maximo);
        if (maximo == l - 1)
        {
            return maximo;
        }
    }
    return maximo;
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    read();
    int hiSnail = snailCity();
    cout << hiSnail << endl;
    return 0;
}