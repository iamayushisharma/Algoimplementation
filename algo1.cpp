#include <bits/stdc++.h>
using namespace std;

const double INF = 1e18;
int n, m, h;
vector<vector<int>> adj;
vector<vector<int>> cliques;
int curComb[20];

int S, T, N;
int head[200000], to_[5000000], nxt_[5000000], ecnt;
double cap_[5000000];
int level_[200000], ptr_[200000];
bool inS_[200000], bestIn_[200000];
double degC_[200000];

void addEdge(int u, int v, double c) {
    to_[ecnt] = v; cap_[ecnt] = c; nxt_[ecnt] = head[u]; head[u] = ecnt++;
    to_[ecnt] = u; cap_[ecnt] = 0; nxt_[ecnt] = head[v]; head[v] = ecnt++;
}

bool bfs() {
    queue<int> q;
    fill(level_, level_ + N, -1);
    level_[S] = 0; q.push(S);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int e = head[u]; e != -1; e = nxt_[e]) {
            if (level_[to_[e]] < 0 && cap_[e] > 1e-12) {
                level_[to_[e]] = level_[u] + 1;
                q.push(to_[e]);
            }
        }
    }
    return level_[T] >= 0;
}

double dfs(int u, double f) {
    if (u == T || f < 1e-15) return f;
    for (int &e = ptr_[u]; e != -1; e = nxt_[e]) {
        if (level_[to_[e]] == level_[u] + 1 && cap_[e] > 1e-15) {
            double p = dfs(to_[e], min(f, cap_[e]));
            if (p > 1e-15) {
                cap_[e] -= p; cap_[e^1] += p;
                return p;
            }
        }
    }
    return 0;
}

double maxflow() {
    double flow = 0;
    while (bfs()) {
        for (int i = 0; i < N; i++) ptr_[i] = head[i];
        double p;
        while ((p = dfs(S, INF)) > 1e-15) flow += p;
    }
    return flow;
}

void gen(int start, int depth) {
    if (depth == h) {
        cliques.emplace_back(curComb, curComb + h);
        return;
    }
    for (int i = start; i < n; i++) {
        bool valid = true;
        for (int j = 0; j < depth; j++) {
            if (!binary_search(adj[curComb[j]].begin(), adj[curComb[j]].end(), i)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            curComb[depth] = i;
            gen(i + 1, depth + 1);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    while (cin.peek() == '#') {
        string dummy;
        getline(cin, dummy);
    }

    vector<pair<int, int>> edges;
    int u, v;
    int maxNode = 0;

    while (true) {
        string line;
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        stringstream ss(line);
        if (!(ss >> u)) break;
        if (!(ss >> v)) {
            h = u;
            break;
        }
        edges.emplace_back(u, v);
        maxNode = max({maxNode, u, v});
    }

    n = maxNode + 1;
    m = edges.size();

    cin >> h;

    adj.assign(n, {});
    for (auto &[u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < n; i++) sort(adj[i].begin(), adj[i].end());

    gen(0, 0);

    fill(degC_, degC_ + n, 0.0);
    for (auto &c : cliques) {
        for (int u : c) {
            degC_[u] += 1.0;
        }
    }

    S = 0;
    T = 1 + n + cliques.size();
    N = T + 1;
    double lo = 0, hi = *max_element(degC_, degC_ + n);
    double eps = 1.0 / (n * (n - 1));

    while (hi - lo > eps) {
        double a = (lo + hi) / 2;
        fill(head, head + N, -1);
        ecnt = 0;

        for (int v = 0; v < n; v++) {
            addEdge(S, 1 + v, degC_[v]);
            addEdge(1 + v, T, a * h);
        }

        for (int i = 0; i < cliques.size(); i++) {
            int cid = 1 + n + i;
            for (int u : cliques[i]) {
                addEdge(cid, 1 + u, INF);
            }
            for (int v = 0; v < n; v++) {
                bool ok = true;
                for (int u : cliques[i]) {
                    if (!binary_search(adj[u].begin(), adj[u].end(), v)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) addEdge(1 + v, cid, 1.0);
            }
        }

        double flow = maxflow();
        fill(inS_, inS_ + N, false);
        queue<int> q;
        q.push(S);
        inS_[S] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int e = head[u]; e != -1; e = nxt_[e]) {
                if (cap_[e] > 1e-12 && !inS_[to_[e]]) {
                    inS_[to_[e]] = true;
                    q.push(to_[e]);
                }
            }
        }

        bool onlyS = true;
        for (int v = 0; v < n; v++) {
            if (inS_[1 + v]) {
                onlyS = false;
                break;
            }
        }
        if (onlyS) hi = a;
        else {
            lo = a;
            for (int v = 0; v < n; v++) bestIn_[v] = inS_[1 + v];
        }
    }

    vector<int> selected;
    for (int v = 0; v < n; v++) if (bestIn_[v]) selected.push_back(v);

    for (int v : selected) cout << v << " ";
    cout << "\n";

    if (!selected.empty()) {
        double totalCliques = 0;
        for (auto &c : cliques) {
            bool inside = true;
            for (int u : c) {
                if (!bestIn_[u]) {
                    inside = false;
                    break;
                }
            }
            if (inside) totalCliques += 1.0;
        }
        double density = totalCliques / selected.size();
        cout << fixed << setprecision(10);
        cout << "Density: " << density << "\n";

        int totalEdges = 0;
        for (int i = 0; i < selected.size(); i++) {
            for (int j = i + 1; j < selected.size(); j++) {
                if (binary_search(adj[selected[i]].begin(), adj[selected[i]].end(), selected[j])) {
                    totalEdges++;
                }
            }
        }
        cout << "Total Edges in Dense Subgraph: " << totalEdges << "\n";
        cout << "Number of " << h << "-cliques in Dense Subgraph: " << fixed << setprecision(0) << totalCliques << "\n";
        cout << "Need Density (Edges/Nodes): " << fixed << setprecision(10) << (double)totalEdges / selected.size() << "\n";
    }

    return 0;
}

