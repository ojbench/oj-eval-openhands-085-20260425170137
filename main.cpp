#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node *left, *right, *parent;
    Node(int v) : val(v), left(nullptr), right(nullptr), parent(nullptr) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> tokens;
    long long x;
    while (cin >> x) {
        if (x == -2) break;
        tokens.push_back(x);
    }

    Node* root = nullptr;
    size_t idx = 0;
    if (!tokens.empty()) {
        long long t = tokens[idx++];
        if (t != -1) {
            root = new Node((int)t);
            struct Frame { Node* node; int stage; }; // 0: fill left, 1: fill right, 2: done
            vector<Frame> st;
            st.push_back({root, 0});
            while (!st.empty()) {
                Frame &f = st.back();
                if (f.stage == 0) {
                    if (idx >= tokens.size()) break;
                    long long v = tokens[idx++];
                    if (v == -1) {
                        f.node->left = nullptr;
                        f.stage = 1;
                    } else {
                        Node* child = new Node((int)v);
                        child->parent = f.node;
                        f.node->left = child;
                        f.stage = 1;
                        st.push_back({child, 0});
                    }
                } else if (f.stage == 1) {
                    if (idx >= tokens.size()) break;
                    long long v = tokens[idx++];
                    if (v == -1) {
                        f.node->right = nullptr;
                        f.stage = 2;
                    } else {
                        Node* child = new Node((int)v);
                        child->parent = f.node;
                        f.node->right = child;
                        f.stage = 2;
                        st.push_back({child, 0});
                    }
                } else {
                    st.pop_back();
                }
            }
        }
    }

    // Prune: remove subtrees that do not contain a 1 (i.e., nodes that become leaf 0)
    if (root) {
        vector<pair<Node*, bool>> st;
        st.reserve( (tokens.size() + 1) );
        st.push_back({root, false});
        while (!st.empty()) {
            auto cur = st.back();
            st.pop_back();
            Node* node = cur.first;
            bool visited = cur.second;
            if (!node) continue;
            if (!visited) {
                st.push_back({node, true});
                if (node->right) st.push_back({node->right, false});
                if (node->left) st.push_back({node->left, false});
            } else {
                if (node->left == nullptr && node->right == nullptr && node->val == 0) {
                    Node* p = node->parent;
                    if (p == nullptr) {
                        delete node;
                        root = nullptr;
                    } else {
                        if (p->left == node) p->left = nullptr; else if (p->right == node) p->right = nullptr;
                        delete node;
                    }
                }
            }
        }
    }

    // Output extended preorder of pruned tree
    vector<int> out;
    out.reserve(tokens.size() + 2);
    if (!root) {
        out.push_back(-1);
    } else {
        struct F { Node* node; int state; }; // 0: print val and go left, 1: go right
        vector<F> st;
        st.push_back({root, 0});
        while (!st.empty()) {
            F f = st.back();
            st.pop_back();
            if (f.state == 0) {
                out.push_back(f.node->val);
                st.push_back({f.node, 1});
                if (f.node->left) st.push_back({f.node->left, 0}); else out.push_back(-1);
            } else {
                if (f.node->right) st.push_back({f.node->right, 0}); else out.push_back(-1);
            }
        }
    }

    for (size_t i = 0; i < out.size(); ++i) {
        if (i) cout << ' ';
        cout << out[i];
    }
    cout << '\n';

    // Free memory
    if (root) {
        vector<Node*> st;
        st.push_back(root);
        vector<Node*> post;
        while (!st.empty()) {
            Node* n = st.back(); st.pop_back();
            post.push_back(n);
            if (n->left) st.push_back(n->left);
            if (n->right) st.push_back(n->right);
        }
        for (auto it = post.rbegin(); it != post.rend(); ++it) delete *it;
    }
    return 0;
}
