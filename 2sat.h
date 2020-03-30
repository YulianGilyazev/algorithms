#include <bits/stdc++.h>

using namespace std;
const int MAXN = 4e4; //max count of variable

int n;
vector<int> g[2 * MAXN], gt[2 * MAXN];
bool used[2 * MAXN];
int comp[2 * MAXN];
vector<int> order;

void dfs (int v) {
	used[v] = true;
	for (int i = 0; i < g[v].size(); ++i) {
		int to = g[v][i];
		if (!used[to])
			dfs (to);
	}
	order.push_back (v);
}

void dfst (int v, int cl) {
	comp[v] = cl;
	for (int i = 0; i < gt[v].size(); ++i) {
		int to = gt[v][i];
		if (comp[to] == -1)
			dfst (to, cl);
	}
}

int solve() {

    /* g and gt initialization*/
	fill(used, used + n, false);
	for (int i = 0; i < n; ++i)
		if (!used[i])
			dfs (i);

	fill(comp, comp + n, -1);
	for (int i = 0, j = 0; i < n; ++i) {
		int v = order[n - i - 1];
		if (comp[v] == -1)
			dfst (v, j++);
	}

	for (int i = 0; i < n; ++i) {
		if (comp[i] == comp[i ^ 1]) {
			cout << "-1\n"; // doesn't have solution
			return 0;
		}
    }
}

