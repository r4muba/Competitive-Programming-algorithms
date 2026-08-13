struct Trie {
    struct Node {
        int nxt[26];
        bool end;

        Node() {
            fill(nxt, nxt + 26, -1);
            end = false;
        }
    };

    vector<Node> tr;

    Trie() {
        tr.emplace_back();
    }

    void insert(const string& s) {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1) {
                tr[u].nxt[x] = (int)tr.size();
                tr.emplace_back();
            }

            u = tr[u].nxt[x];
        }

        tr[u].end = true;
    }

    bool find(const string& s) {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1)
                return false;

            u = tr[u].nxt[x];
        }

        return tr[u].end;
    }

    bool prefix(const string& s) {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1)
                return false;

            u = tr[u].nxt[x];
        }

        return true;
    }
};
