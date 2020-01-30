#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


namespace constants
{
    const int alpha = 10;
    const int MAXN = 150020;
    const char min_symb = '0';
}

class Automaton
{
protected:

    long long len[2 * constants::MAXN];
    int link[2 * constants::MAXN];
    long long weight[2 * constants::MAXN];
    int par[2 * constants::MAXN];
    char par_symb[2 * constants::MAXN];
    bool is_term[2 * constants::MAXN];
    int nxt[2 * constants::MAXN][10];
    std::string s;
    int sz, last;

private:

    void _init()
    {
        for (int i = 0; i < 2 * constants::MAXN; i++)
            for (int j = 0; j < constants::alpha - 1; j++)
                nxt[i][j] = 0;
        std::fill(weight, weight + constants::MAXN * 2, 0);
        sz = last = 0;
        len[0] = 0;
        link[0] = -1;
        ++sz;
    }

    void _addSymb (char c)
    {
        int cur = sz++;
        len[cur] = len[last] + 1;
        par[cur] = last;
        par_symb[cur] = c;
        int p;
        for (p = last; p != -1 && !nxt[p][c - constants::min_symb]; p = link[p])
            nxt[p][c - constants::min_symb] = cur;
        if (p == -1)
            link[cur] = 0;
        else
        {
            int q = nxt[p][c - constants::min_symb];
            if (len[p] + 1 == len[q])
                link[cur] = q;
            else
                _clone(p, c, q, cur);
        }
        last = cur;
    }

    void _clone(int p, char c, int q, int cur)
    {
        int clone = sz++;
        len[clone] = len[p] + 1;
        par[clone] = p;
        par_symb[clone] = c;
        for (int j = 0; j < 10; j++)
            nxt[clone][j] = nxt[q][j];
        link[clone] = link[q];
        for (; p!=-1 && nxt[p][c - constants::min_symb]==q; p=link[p])
            nxt[p][c - constants::min_symb] = clone;
        link[q] = link[cur] = clone;
    }

    void _calcTerminal()
    {
        int cur = last;
        while (cur != 0)
        {
            is_term[cur] = true;
            cur = link[cur];
        }
    }

    void _calcWeight(int a)
    {
        int ans = 0;
        for (int i = 0; i < constants::alpha; i++)
            if (nxt[a][i] > 0)
            {
                if (weight[nxt[a][i]] == 0)
                    _calcWeight(nxt[a][i]);
                ans += weight[nxt[a][i]];
            }
        if (is_term[a])
            ans++;
        weight[a] = ans;
    }

public:

    void build(std::string str)
    {
        s = str;
        _init();
        for (int i = 0; i < (int)s.length(); i++)
            _addSymb(s[i]);
        _calcTerminal();
        _calcWeight(0);
    }
};
