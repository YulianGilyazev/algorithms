#include<iostream>
#include<algorithm>
#include<vector>
#include<list>

namespace constants
{
    const int MAXN = 1e9;
}

struct Graph
{
    int n;
    int s;
    int t;
    std::vector< std::vector< int > > g;
    std::vector< std::vector<bool> > used;
    Graph (int n_init, int s_init, int t_init)
    {
        n = n_init;
        s = s_init;
        t = t_init;
        g = std::vector< std::vector< int > > (n, std::vector< int > (n, 0));
        used = std::vector< std::vector<bool> > (n, std::vector<bool> (n, 0));
    }
    void add_edge (int from, int to, int cap)
    {
        g[from][to] = cap;
        used[from][to] = true;
    }
};

class PushToStart
{

public:

    void init (Graph * gr)
    {
        _g_sz = gr->n;
        _s = gr->s;
        _t = gr->t;
        _g = std::vector< std::vector< int > > (_g_sz);
        _ptr = std::vector<int>(_g_sz, 0);
        _e = std::vector<int>(_g_sz, 0);
        _e[_s] = constants::MAXN;
        _h = std::vector<int>(_g_sz, 0);
        _h[_s] = _g_sz;

        for (int i = 0; i < _g_sz; i++)
            for (int j = 0; j < _g_sz; j++)
                if (gr->used[i][j])
                    _add_edge(i, j, gr->g[i][j]);
    }

    int max_flow()
    {
        _flow = 0;
        _relabel_to_front();
        for (int id : _g[_s])
            _flow += _edges[id].flow;
        return _flow;
    }

private:

    struct Edge
    {
        int from, to, cap, flow;
    };

    int _g_sz;
    int _s;
    int _t;
    int _flow;
    std::vector<Edge> _edges;
    std::vector< std::vector< int > > _g;
    std::vector<int> _ptr;
    std::vector<int> _e;
    std::vector<int> _h;

    void _add_edge(int from, int to, int cap)
    {
        Edge e1 = {from , to, cap, 0};
        Edge e2 = {to, from, 0, 0};
        _g[from].push_back((int) _edges.size());
        _edges.push_back(e1);
        _g[to].push_back((int) _edges.size());
        _edges.push_back(e2);
    }

    void _init_preflow()
    {
        for(int id: _g[_s])
        {
            _edges[id].flow = _edges[id].cap;
            _e[_edges[id].to] = _edges[id].flow;
            _edges[id ^ 1].flow = -_edges[id].flow;
        }
    }

    void _push(int id)
    {
        if (_e[_edges[id].from] > 0 && _h[_edges[id].from] == _h[_edges[id].to] + 1 && _edges[id].cap > _edges[id].flow)
        {
            int cur_flow = std::min(_e[_edges[id].from], _edges[id].cap - _edges[id].flow);
            _edges[id].flow += cur_flow;
            _edges[id^1].flow -= cur_flow;
            _e[_edges[id].from] -= cur_flow;
            _e[_edges[id].to] += cur_flow;
        }
    }

    void _relabel(int u)
    {
        if (_e[u] <= 0)
            return;
        int min_h = constants::MAXN;
        for (int id : _g[u])
        {

            if (_edges[id].cap == _edges[id].flow)
                continue;
            if (_h[_edges[id].to] < _h[u])
                return;
            if (min_h > _h[_edges[id].to] && _edges[id].cap != _edges[id].flow)
                min_h = _h[_edges[id].to];
        }
        _h[u] = 1 + min_h;
    }

    void _discharge(int v)
    {
        while (_e[v] > 0)
        {
            int cur_id = _ptr[v];
            if (_ptr[v] >= _g[v].size())
            {
                _relabel(v);
                _ptr[v] = 0;
            }
            else
            {
                if (_edges[_g[v][cur_id]].flow < _edges[_g[v][cur_id]].cap && _h[v] == _h[_edges[_g[v][cur_id]].to] + 1)
                    _push(_g[v][cur_id]);
                else
                    _ptr[v]++;
            }
        }
    }

    void _relabel_to_front()
    {
        _init_preflow();
        std::list<int> vert_set;
        for (int i = 0; i < _g_sz; i++)
        {
            if (i == _s || i == _t)
                continue;
            vert_set.push_back(i);
        }
        auto it = vert_set.begin();
        while(it != vert_set.end())
        {
            int old_h = _h[*it];
            _discharge(*it);
            if(_h[*it] > old_h)
            {
                int cur = *it;
                vert_set.erase(it);
                vert_set.push_front(cur);
                it = vert_set.begin();
            }
            else
                ++it;
        }
    }
};
