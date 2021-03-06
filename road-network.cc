#include <algorithm>
#include <climits>
#include <cstdio>
using namespace std;

typedef long long ll;
#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#define REP(i, n) for (int i = 0; i < (n); i++)

int ri()
{
  int x;
  scanf("%d", &x);
  return x;
}

const int N = 500, M = 10000, MOD = 1000000007;
bool S[N];
int p[N], cut[N][N], h[N], nh[N+1];
struct Edge { int v, c; Edge *next, *twain; } *es[N], pool[2*M];

void dfs(int u)
{
  S[u] = true;
  for (auto e = es[u]; e; e = e->next)
    if (e->c > 0 && ! S[e->v])
      dfs(e->v);
}

int augment(int n, int u, int d, int src, int sink)
{
  if (u == sink)
    return d;
  int old = d, mn = n-1;
  for (auto e = es[u]; e; e = e->next)
    if (e->c > 0) {
      if (h[e->v]+1 == h[u]) {
        int dd = augment(n, e->v, min(d, e->c), src, sink);
        e->c -= dd;
        e->twain->c += dd;
        if (! (d -= dd)) return old-d;
        if (h[src] >= n) break;
      }
      mn = min(mn, h[e->v]);
    }
  if (old == d) {
    if (! --nh[h[u]])
      h[src] = n;
    nh[h[u] = mn+1]++;
  }
  return old-d;
}

int maxFlow(int n, int src, int sink)
{
  int flow = 0;
  fill_n(h, n, 0);
  fill_n(nh, n+1, 0);
  do flow += augment(n, src, INT_MAX, src, sink);
  while (h[src] < n);
  return flow;
}

void gomoryHu(int n, int m)
{
  fill_n(p, n, 0);
  REP(i, n)
    fill_n(cut[i], n, INT_MAX);
  FOR(i, 1, n) {
    REP(j, m) {
      int t = pool[2*j].c+pool[2*j+1].c >> 1;
      pool[2*j].c = pool[2*j+1].c = t;
    }
    int flow = maxFlow(n, i, p[i]);
    fill_n(S, n, false);
    dfs(i);
    FOR(j, i+1, n)
      if (S[j] && p[j] == p[i])
        p[j] = i;
    cut[i][p[i]] = cut[p[i]][i] = flow;
    REP(j, i)
      cut[j][i] = cut[i][j] = min(flow, cut[p[i]][j]);
  }
}

int main()
{
  int n = ri(), m = ri();
  REP(i, m) {
    int u = ri()-1, v = ri()-1, w = ri();
    pool[2*i] = {v, w, es[u], &pool[2*i+1]};
    pool[2*i+1] = {u, w, es[v], &pool[2*i]};
    es[u] = &pool[2*i];
    es[v] = &pool[2*i+1];
  }
  gomoryHu(n, m);
  ll ans = 1;
  REP(i, n)
    FOR(j, i+1, n)
      ans = ans*cut[i][j]%MOD;
  printf("%lld\n", ans);
}
