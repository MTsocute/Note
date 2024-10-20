#include <iostream>
#include "include/stdc++.h"
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {
    }

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
    }

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {
    }
};

class Solution {
public:
    TreeNode *balanceBST(TreeNode *root) {
        // BST -> vector
        inOrderT(root);

        return sortedArr2AVL(v, 0, v.size() - 1);
    }

    TreeNode *sortedArr2AVL(const vector<int> &arr, int left, int right) {
        if (left > right) return nullptr;

        int mid = left + (right - left) / 2;
        auto *root = new TreeNode(arr[0]);
        root->left = sortedArr2AVL(arr, left, mid - 1);
        root->right = sortedArr2AVL(arr, mid + 1, right);

        return root;
    }

    void inOrderT(TreeNode *root) {
        if (root == nullptr) return;
        inOrderT(root->left);
        v.emplace_back(root->val);
        inOrderT(root->right);
    }

private:
    vector<int> v;
};

int main() {

    Solution s;


    return 0;
}

