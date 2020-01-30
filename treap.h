#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<math.h>


namespace constants
{
    const long double EPS = 1e-6;
    const int MAXN = 1e5+10;
    const int coeff = 1009;
}


class Treap
{
protected:
    struct _node
    {
        int cnt;
        int prior;
        int val;
        _node* l;
        _node* r;
        _node(int val) :val(val), l(nullptr), r(nullptr), cnt(1) { prior = (rand() << 16) | rand(); }
        void recalc()
        {
            cnt = 1 + Cnt(l) + Cnt(r);
        }
        static int Cnt(_node* v)
        {
            if (!v)
                return 0;
            return v->cnt;
        }
    };
    typedef _node *node;

    static int _cnt(node v)
    {
        if (!v)
            return 0;
        return v->cnt;
    }
    node _root;

    size_t Size;

    node _merge(node l, node r)
    {
        if (!l)
            return r;
        if (!r)
            return l;
        if (l->prior < r->prior)
        {
            l->r = _merge(l->r, r);
            l->recalc();
            return l;
        }
        else
        {
            r->l = _merge(l, r->l);
            r->recalc();
            return r;
        }
    }

    void _split(node v, int idx, node& l, node& r)
    {
        l = r = nullptr;
        if (!v)
            return;
        if (_cnt(v->l) < idx)
        {
            l = v;
            _split(l->r, idx - _cnt(v->l) - 1, l->r, r);
            l->recalc();
        }
        else
        {
            r = v;
            _split(r->l, idx, l, r->l);
            r->recalc();
        }
    }

public:
    Treap()
    {
        _root = nullptr;
        Size = 0;
    }

    size_t get_sz() const
    {
        return Size;
    }

    void add(int idx, int val)
    {
        node l = nullptr, r = nullptr;
        _split(_root, idx, l, r);
        node cur_node = new _node(val);
        _root = _merge(_merge(l, cur_node), r);
        ++Size;
    }

    void erase(int idx)
    {
        node l = nullptr, m = nullptr, r = nullptr;
        _split(_root, idx, l, r);
        _split(r, 1, m, r);
        _root = _merge(l, r);
        --Size;
    }

    int operator [] (int idx)
    {
        node l = nullptr, m = nullptr, r = nullptr;
        _split(_root, idx, l, r);
        _split(r, 1, m, r);
        _root = _merge(_merge(l, m), r);
        return m->val;
    }
};