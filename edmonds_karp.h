#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
using namespace std;

struct edge
{
    int from, to;
    int cap;
    int cost;
    int flow;
    int x, y;
};
int const MAXN = 262144;
int const INF = 1e9 + 1e5;

edge edges[MAXN];
vector<int> g[MAXN];
int sz;

void add_edge(int from, int to, int cost, int cap, int x, int y)
{
    edge e1 = {from, to, cap, cost, 0, x, y};
    g[from].push_back(sz);
    edges[sz] = e1;
    sz++;
    edge e2 = {to, from, 0, -cost, 0, -1, -1};
    g[to].push_back(sz);
    edges[sz] = e2;
    sz++;
}

pair<int, int > mincost(int need_flow, int n, int s, int t)
{
    int cost = 0;
    int flow = 0;
    vector<int> p(n);
    vector<int> _d(n);
    fill(p.begin(), p.end(), -1);
    fill(_d.begin(), _d.end(), 0);
    _d[s] = 0;
    p[s] = s;
    bool used = true;
    while(used)
    {
        used = false;
        for(int i = 0; i < sz; ++i)
        {
            if(edges[i].cap != edges[i].flow && p[edges[i].from] != -1 && (p[edges[i].to] == -1 || _d[edges[i].to] > _d[edges[i].from] + edges[i].cost))
            {
                _d[edges[i].to] = _d[edges[i].from] + edges[i].cost;
                p[edges[i].to] = i;
                used = true;
            }
        }
    }
    vector<int> pi;
    pi = move(_d);
    vector <int> d(n);
    while(flow < need_flow)
    {

        fill(p.begin(), p.end(), -1);
        fill(d.begin(), d.end(), 0);
        int used[n];
        for (int cnt = 0; cnt < n; cnt++)
        {
            int v;
            int mn = INF;
            if (cnt == 0)
            {
                v = s;
                used[v] = 2;
            }
            else
            {
                for (int i = 0; i < n; i++)
                {
                    if (used[i] == 1 && d[i] < mn)
                    {
                        mn = d[i];
                        v = i;
                    }
                }
            }
            used[v] = 2;
            for (int id: g[v])
            {
                if (edges[id].to == s)
                    continue;
                if (edges[id].cap > edges[id].flow)
                {
                    int newd = d[v] + edges[id].cost + pi[edges[id].from] - pi[edges[id].to];
                    if (p[edges[id].to] == -1 || d[edges[id].to] > newd)
                    {
                        d[edges[id].to] = newd;
                        used[edges[id].to] = 1;
                        p[edges[id].to] = id;
                    }
                }
            }
        }

        if (p[t] == -1)
            break;

        if (d[t] + pi[t] >= 0 && !need_flow)
            break;

        int cur = t;
        int mx = INF - flow;
        while (cur != s)
        {
            mx = min(mx, edges[p[cur]].cap - edges[p[cur]].flow);
            cur = edges[p[cur]].from;
        }

        flow += mx;
        if (flow > need_flow)
            break;

        cost += (pi[t] + d[t]) * mx;
        cur = t;
        while (cur != s)
        {
            int id = p[cur];
            edges[id].flow += mx;
            edges[id ^ 1].flow -= mx;
            cur = edges[id].from;
        }

        for (int i = 0; i < n; ++i)
        {
            if (i != s && p[i] == -1)
                pi[i] = INF;
            else
                pi[i] = min(pi[i] + d[i], INF);
        }

    }
    return make_pair(flow, cost);
}
