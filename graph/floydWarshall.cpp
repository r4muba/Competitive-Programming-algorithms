#include <bits/stdc++.h>

const int nax {500};
std::array< std::array<int, nax>, nax> dis{}, p{};

const int INF {1'000'000'000};
for (auto& row : dis) row.fill(INF);
for (auto& row : p) row.fill(-1);

for (int b{}; b < n; ++b) {
    for (int i{}; i < n; ++i) {
        for (int j{}; j < n; ++j) {
            if (dis[i][b] == INF || dis[b][j] == INF) continue;
            if (dis[i][j] > dis[i][b] + dis[b][j]) {
                dis[i][j] = dis[i][b] + dis[b][j];
                p[i][j] = b;
            }
        }
    }
}

int cycleSize{INF};
for (int i{}; i < n; ++i) {
    cycleSize = std::min(cycleSize, dis[i][i]);
}

std::vector<int> ans{};
for (int I{}; I < n; ++I) {
    if (dis[I][I] == cycleSize){ 
        auto reconstruct = [&](auto& self, int i, int j) -> void {
            if (p[i][j] == -1) {
                ans.push_back(i);
                return;
            }
            auto k {p[i][j]};
            self(self, i, k);
            self(self, k, j);
        };
        reconstruct(reconstruct, I, I);
    }
}
