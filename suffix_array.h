#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


namespace constants
{
    const int MAXN = 150500;
    char const min_symb = '0';
    char const terminal = '$';
    char const max_symb = ':';
}

class SuffixArray
{
private:
    std::string _s;
    int _n;
    int _sa[constants::MAXN], _new_sa[constants::MAXN], _cls[constants::MAXN];
    int _new_cls[constants::MAXN], _cnt[constants::MAXN], _lcp[constants::MAXN];
    int _n_cls;

    void _update_ar (int d)
    {
        for(int i = 0; i < _n; i++)
            _new_sa[i] = (_sa[i] - d + _n) % _n;
        std::fill(_cnt, _cnt + _n_cls, 0);
        for(int i = 0; i < _n; i++)
            ++_cnt[_cls[i]];
        for(int i = 0; i < _n_cls; i++)
            _cnt[i + 1] += _cnt[i];
        for (int i = _n - 1; i >= 0; --i)
            _sa[--_cnt[_cls[_new_sa[i]]]] = _new_sa[i];
        _n_cls = 0;
        for(int i = 0; i < _n; i++)
        {
            if (i && (_cls[_sa[i]] != _cls[_sa[i - 1]] || _cls[(_sa[i] + d) % _n] != _cls[(_sa[i - 1] + d) % _n]))
                ++_n_cls;
            _new_cls[_sa[i]] = _n_cls;
        }
        ++_n_cls;
        for(int i = 0; i < _n; i++)
            _cls[i] = _new_cls[i];
    }

public:
    void build(std::string str)
    {
        _s = str;
        _n = _s.length();
        _n_cls = constants::max_symb;
        for(int i = 0; i < _n; i++)
        {
            _sa[i] = i;
            _cls[i] = _s[i];
        }
        for (int d = 0; d < _n; d = d ? d*2 : 1)
            _update_ar(d);
        int val = 0;
        for(int i = 0; i < _n; i++)
        {
            if (val) --val;
            if (_cls[i] == _n - 1)
                continue;
            int j = _sa[_cls[i] + 1];
            while (i + val != _n && j + val != _n && _s[i + val] == _s[j + val])
                ++val;
            _lcp[_cls[i]] = val;
        }
    }

    std::vector <int> getLcp()
    {
        std::vector <int> Lcp(_lcp + 1, _lcp + _n - 1);
        return Lcp;
    }
    std::vector <int> getSa()
    {
        std::vector <int> Sa(_sa, _sa + _n);
        return Sa;
    }
};

class segmentTree
{
private:
    int t[constants::MAXN * 4];
public:
    void update (int v, int tl, int tr, int l, int r, int val)
    {
        if (l > r)
            return;
        if (l == tl && tr == r)
            t[v] += val;
        else
        {
            int tm = (tl + tr) / 2;
            update (v * 2, tl, tm, l, std::min(r,tm), val);
            update (v * 2 + 1, tm + 1, tr, std::max(l,tm + 1), r, val);
        }
    }

    int get (int v, int tl, int tr, int pos)
    {
        if (tl == tr)
            return t[v];
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            return t[v] + get (v * 2, tl, tm, pos);
        else
            return t[v] + get (v * 2 + 1, tm + 1, tr, pos);
    }
};

