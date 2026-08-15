#include <bits/stdc++.h>
using namespace std;

// To hash a multiset: Sum of elements of: (B^num) * freq[num]
const long long MOD = 1000000009LL; // prime modulo (alternative: 10000000000000061 or 10000000000061)
const long long BASE = 131LL; // any number > alphabet size and coprime to MOD.

string s, t;
vector<long long> pws, h;
long long hashT = 0, ans = 0;

long long convertChar(char c) {
    return static_cast<long long>(c - 'a' + 1);
}

bool sameHash(int l1, int len1, int l2, int len2) {
    int r1 = l1 + len1;
    int r2 = l2 + len2;
    long long h1 = (h[r1] - h[l1] * pws[len1] % MOD + MOD) % MOD;
    long long h2 = (h[r2] - h[l2] * pws[len2] % MOD + MOD) % MOD;
    return h1 == h2;
}

void precalc() {
    int lenS = static_cast<int>(s.size());
    int lenT = static_cast<int>(t.size());

    pws.assign(lenS + 1, 1);
    h.assign(lenS + 1, 0);

    for (int i = 1; i <= lenS; ++i) {
        pws[i] = (pws[i - 1] * BASE) % MOD;
    }

    for (int i = 0; i < lenS; ++i) {
        h[i + 1] = (h[i] * BASE + convertChar(s[i])) % MOD;
    }

    for (int i = 0; i < lenT; ++i) {
        hashT = (hashT * BASE + convertChar(t[i])) % MOD;
    }
}

void doit() {
    int lenS = static_cast<int>(s.size());
    int lenT = static_cast<int>(t.size());

    precalc();

    for (int i = lenT; i <= lenS; ++i) {
        long long curHash = (h[i] - h[i - lenT] * pws[lenT] % MOD + MOD) % MOD;
        if (curHash == hashT) {
            ++ans;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> s >> t;
    doit();
    cout << ans << '\n';
    return 0;
}
