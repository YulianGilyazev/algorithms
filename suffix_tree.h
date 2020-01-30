#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>


namespace constants
{
    const int MAXN = 5e5;
    const int alpha = 11;
    const char min_symb = '0';
    const char terminal = ':';
}

class SuffixTree
{
private:
    struct _node
    {
        int l, r, par, link;
        long long depth;
        long long weight;
        std::map<char,int> next;
        _node (int l = 0, int r = 0, int par = -1) : l(l), r(r), par(par), link(-1), weight(0), depth(0) {}
        int len()
        {
            return r - l;
        }
        int &get (char c)
        {
            if (!next.count(c))  next[c] = -1;
            return next[c];
        }
    };

    struct _state
    {
        int v, pos;
        _state (int v, int pos) : v(v), pos(pos)  {}
    };

    _state _go (_state st, int l, int r)
    {
        while (l < r)
            if (st.pos == t[st.v].len())
            {
                st = _state (t [st.v].get( s[l] ), 0);
                if (st.v == -1)
                    return st;
            }
            else
            {
                if (s[ t[st.v].l + st.pos ] != s[l])
                    return _state (-1, -1);
                if (r-l < t[st.v].len() - st.pos)
                    return _state (st.v, st.pos + r - l);
                l += t[st.v].len() - st.pos;
                st.pos = t[st.v].len();
            }
        return st;
    }

    int _split (_state st)
    {
        if (st.pos == t[st.v].len())
            return st.v;
        if (st.pos == 0)
            return t[st.v].par;
        _node v = t[st.v];
        int id = sz++;
        t[id] = _node (v.l, v.l + st.pos, v.par);
        t[v.par].get( s[v.l] ) = id;
        t[id].get( s[v.l + st.pos] ) = st.v;
        t[st.v].par = id;
        t[st.v].l += st.pos;
        return id;
    }

    int _get_link (int v)
    {
        if (t[v].link != -1)
            return t[v].link;
        if (t[v].par == -1)
            return 0;
        int to = _get_link (t[v].par);
        return t[v].link = _split (_go (_state(to,t[to].len()), t[v].l + (t[v].par==0), t[v].r));
    }

    void _add_symb (int pos)
    {
        while (true)
        {
            _state nptr = _go (ptr, pos, pos+1);
            if (nptr.v != -1)
            {
                ptr = nptr;
                return;
            }
            int mid = _split (ptr);
            int leaf = sz++;
            t[leaf] = _node (pos, n, mid);
            t[mid].get(s[pos]) = leaf;
            ptr.v = _get_link (mid);
            ptr.pos = t[ptr.v].len();
            if (!mid)
                break;
        }
    }

    int _calc_weight(int a)
    {
        int ans = 0;
        for (int i = 0; i < constants::alpha; i++)
            if (t[a].next[i + constants::min_symb] > 0)
                ans += _calc_weight(t[a].next[i + constants::min_symb]);
        if (ans == 0)
            ans = 1;

        t[a].weight = ans;

        return ans;
    }

    void _calc_depth(int a)
    {
        if (a == 0)
        {
            t[a].depth = 0;
        }
        for (int i = 0; i < constants::alpha - 1; i++)
        {
            if (t[a].next[i + constants::min_symb] > 0)
            {
                t[t[a].next[i + constants::min_symb]].depth = t[a].depth + t[t[a].next[i + constants::min_symb]].len() ;
                _calc_depth(t[a].next[i + constants::min_symb]);
            }
        }
    }

protected:
    int n;
    std::string s;
    _state ptr;
    _node t[constants::MAXN];
    int sz;
    std::vector <char> findPath(int ind)
    {
        std::vector <char> ans;
        for (int i = t[ind].r - 1; i >= t[ind].l; i--)
        {
            ans.push_back(s[i]);
        }
        return ans;
    }
public:
    SuffixTree () : sz(0), ptr(_state(0, 0)) {}

    void buildTree(std::string str)
    {
        ptr = _state(0, 0);
        s = str;
        n = s.length();
        sz = 1;
        for (int i=0; i<n; ++i)
            _add_symb (i);
        _calc_weight(0);
        t[0].weight--;
        _calc_depth(0);
    }
};
