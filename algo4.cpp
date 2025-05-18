#include <bits/stdc++.h>
using namespace std;
const double INF = 1e18;
struct Edge { int to, rev; double cap; };
vector<vector<Edge>> G;
vector<int> level_, ptr_;
int N, S, T;
void addEdge(int u, int v, double c) {
    G[u].push_back({v, (int)G[v].size(), c});
    G[v].push_back({u, (int)G[u].size()-1, 0.0});
}
bool bfs() {
    fill(level_.begin(), level_.end(), -1);
    queue<int> q;
    level_[S] = 0; q.push(S);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &e : G[u]) {
            if (level_[e.to] < 0 && e.cap > 1e-12) {
                level_[e.to] = level_[u] + 1;
                q.push(e.to);
            }
        }
    }
    return level_[T] >= 0;
}
double dfs(int u, double f) {
    if (u == T || f < 1e-15) return f;
    for (int &cid = ptr_[u]; cid < (int)G[u].size(); ++cid) {
        auto &e = G[u][cid];
        if (level_[e.to] == level_[u] + 1 && e.cap > 1e-15) {
            double pushed = dfs(e.to, min(f, e.cap));
            if (pushed > 1e-15) {
                e.cap -= pushed;
                G[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
    }
    return 0.0;
}
double maxflow() {
    double flow = 0;
    while (bfs()) {
        fill(ptr_.begin(), ptr_.end(), 0);
        while (true) {
            double pushed = dfs(S, INF);
            if (pushed < 1e-15) break;
            flow += pushed;
        }
    }
    return flow;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line[0] != '#') lines.push_back(line);
    }
    if (lines.empty()) return 0;
    int h = stoi(lines.back());
    lines.pop_back();
    vector<pair<int,int>> edges;
    int maxNode = -1;
    for (auto &l : lines) {
        stringstream ss(l);
        int u, v;
        if (ss >> u >> v) {
            edges.emplace_back(u, v);
            maxNode = max(maxNode, max(u, v));
        }
    }
    int n = maxNode + 1;
    vector<vector<int>> adj(n);
    for (auto &e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    for (auto &nbrs : adj) sort(nbrs.begin(), nbrs.end());
    vector<vector<int>> psi;
    vector<int> comb(h>0 ? h-1 : 0);
    function<void(int,int)> dfsPsi = [&](int start,int depth){
        if (depth == (int)comb.size()) {
            psi.push_back(comb);
            return;
        }
        for (int u = start; u < n; u++) {
            bool ok = true;
            for (int j = 0; j < depth; j++) {
                if (!binary_search(adj[comb[j]].begin(), adj[comb[j]].end(), u)) {
                    ok = false; break;
                }
            }
            if (!ok) continue;
            comb[depth] = u;
            dfsPsi(u+1, depth+1);
        }
    };
    if (h > 1) dfsPsi(0,0);
    else {
        psi.clear();
        for (int i = 0; i < n; i++) psi.push_back({i});
    }
    vector<vector<int>> v2psi(n);
    for (int i = 0; i < (int)psi.size(); i++)
        for (int v : psi[i])
            v2psi[v].push_back(i);
    vector<vector<int>> fullC;
    vector<int> deg(n,0);
    for (auto &p : psi) {
        int last = p.back();
        for (int cand = last+1; cand < n; cand++) {
            bool ok = true;
            for (int v : p) {
                if (!binary_search(adj[v].begin(), adj[v].end(), cand)) {
                    ok = false; break;
                }
            }
            if (!ok) continue;
            vector<int> C = p;
            C.push_back(cand);
            fullC.push_back(C);
            for (int v : C) deg[v]++;
        }
    }
    if (fullC.empty()) {
        cout << "\n0.00 0 0\n";
        return 0;
    }
    double low = 0.0, high = *max_element(deg.begin(), deg.end());
    double eps = 1.0 / (n * (n - 1.0));
    vector<int> bestSet;
    double bestD = 0.0;
    while (high - low > eps) {
        double alpha = (low + high) / 2.0;
        int P = psi.size();
        S = 0; T = 1 + n + P;
        N = T + 1;
        G.assign(N, {});
        level_.assign(N, -1);
        ptr_.assign(N, 0);
        for (int v = 0; v < n; v++) {
            addEdge(S, 1+v, deg[v]);
            addEdge(1+v, T, alpha * h);
        }
        for (int i = 0; i < P; i++) {
            int uNode = 1 + n + i;
            for (int v : psi[i]) {
                addEdge(uNode, 1+v, INF);
                addEdge(1+v, uNode, 1.0);
            }
        }
        maxflow();
        vector<char> seen(N,0);
        queue<int>q; q.push(S); seen[S]=1;
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(auto &e: G[u]){
                if (!seen[e.to] && e.cap>1e-12){
                    seen[e.to]=1;
                    q.push(e.to);
                }
            }
        }
        vector<int> sel;
        for (int v = 0; v < n; v++)
            if (seen[1+v]) sel.push_back(v);
        if (sel.empty()) {
            high = alpha;
        } else {
            sort(sel.begin(), sel.end());
            double cnt = 0.0;
            for (auto &C : fullC) {
                bool ok = true;
                for (int v : C) {
                    if (!binary_search(sel.begin(), sel.end(), v)) {
                        ok = false; break;
                    }
                }
                if (ok) cnt += 1.0;
            }
            double dens = cnt / sel.size();
            if (dens > bestD) {
                bestD = dens;
                bestSet = sel;
            }
            low = alpha;
        }
    }
    sort(bestSet.begin(), bestSet.end());
    vector<char> inBest(n,0);
    for (int v : bestSet) inBest[v] = 1;
    int edgeCount = 0;
    for (int u = 0; u < n; u++) if (inBest[u]) {
        for (int v : adj[u]) if (v > u && inBest[v]) edgeCount++;
    }
    int cliqueCount = 0;
    for (auto &C : fullC) {
        bool ok = true;
        for (int v : C) if (!inBest[v]) { ok = false; break; }
        if (ok) cliqueCount++;
    }
    for (int v : bestSet) cout << v << " ";
    cout << "\n" << fixed << setprecision(6) << bestD << " " << edgeCount << " " << cliqueCount << "\n";
    return 0;
}
