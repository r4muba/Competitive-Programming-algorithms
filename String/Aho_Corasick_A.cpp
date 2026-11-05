class aho {
private:
    struct aho_node {
        aho_node* childrens[26];
        aho_node* fail;
        aho_node* out;
        bool is_word;
        long long count;
        std::vector<long long> idx_words;

        aho_node() {
            for (long long i = 0; i < 26; ++i) childrens[i] = nullptr;
            fail = nullptr;
            out = nullptr;
            is_word = false;
            count = 0;
        }
    };

    aho_node* root;
    std::vector<aho_node*> order_bfs;

    aho_node* find(const std::string& word) {
        aho_node* current = root;
        for (auto l : word) {
            if (!current->childrens[l - 'a']) return nullptr;
            current = current->childrens[l - 'a'];
        }
        return current;
    }

public:
    aho() {
        root = new aho_node;
    }

    ~aho() {
        for (int i = (int)order_bfs.size() - 1; i >= 0; --i) {
            delete order_bfs[i];
        }
        delete root;
    }

    bool prefix(const std::string& prefix) {
        return find(prefix) != nullptr;
    }

    void insert(const std::string& word, long long id) {
        aho_node* current = root;
        for (auto l : word) {
            if (!current->childrens[l - 'a']) {
                current->childrens[l - 'a'] = new aho_node;
            }
            current = current->childrens[l - 'a'];
        }
        current->is_word = true;
        current->idx_words.push_back(id);
    }

    void build() {
        std::queue<aho_node*> q;
        root->fail = root;
        root->out = nullptr;

        for (long long i{}; i < 26; ++i) {
            if (root->childrens[i]) {
                root->childrens[i]->fail = root;
                root->childrens[i]->out = nullptr;
                q.push(root->childrens[i]);
            }
        }

        while (!q.empty()) {
            aho_node* current = q.front();
            q.pop();

            order_bfs.push_back(current);

            for (long long i{}; i < 26; i++) {
                aho_node* child = current->childrens[i];

                if (child) {
                    aho_node* f = current->fail;

                    while (f != root && !f->childrens[i]) {
                        f = f->fail;
                    }

                    if (f->childrens[i] && f->childrens[i] != child) {
                        child->fail = f->childrens[i];
                    } else {
                        child->fail = root;
                    }

                    if (child->fail->is_word) {
                        child->out = child->fail;
                    } else {
                        child->out = child->fail->out;
                    }

                    q.push(child);
                }
            }
        }
    }

    void text_search(const std::string& text) {
        aho_node* current = root;

        for (long long i{}; i < text.size(); ++i) {
            long long l = text[i] - 'a';

            while (current != root && !current->childrens[l]) {
                current = current->fail;
            }

            if (current->childrens[l]) {
                current = current->childrens[l];
            }

            // Simplemente sumamos 1, sin recorrer los enlaces out
            current->count++;
        }
    }

    void propagate_counts() {
        // Recorremos desde las hojas hacia la raíz usando el Reverse BFS
        for (int i = (int)order_bfs.size() - 1; i >= 0; --i) {
            aho_node* node = order_bfs[i];
            if (node->fail) {
                node->fail->count += node->count;
            }
        }
    }

    void get_results(std::vector<long long>& found) {
        // Asignamos los conteos finales a los índices correspondientes
        for (aho_node* node : order_bfs) {
            if (node->is_word) {
                for (long long id : node->idx_words) {
                    found[id] += node->count;
                }
            }
        }
    }

    bool search(const std::string& word) {
        aho_node* end = find(word);
        return (end && end->is_word);
    }
};

int main() {
    aho a;
    // 1. Insertamos todos los patrones en el Trie
    for (long long i = 0; i < n; ++i) {
        std::cin >> vec[i];
        a.insert(vec[i], i);
    }
    // 2. Construimos los enlaces fail y out (BFS)
    a.build();
    // 3. Pasamos el texto general por el autómata (se cuenta en las hojas/nodos)
    a.text_search(text);
    // 4. Propagamos los conteos hacia arriba usando los enlaces fail
    a.propagate_counts();
    // 5. Extraemos los resultados
    std::vector<long long> found(n, 0); 
    a.get_results(found);
}