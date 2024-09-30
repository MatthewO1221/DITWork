#ifndef TREE_SEARCH_1_H
#define TREE_SEARCH_1_H
#include <iostream>
#include <vector>
#include <deque>

template <typename T>
struct TNode
{
    T data = T();
    std::vector<TNode<T> *> children = {};
};

// recursive implementation of DFS
template <typename T>
TNode<T> const * // returns first found node that is equal to val
tree_depth_first_search_rec(TNode<T> const *pRoot, T const &val)
{
    if (!pRoot)
    {
        return nullptr;
    }

    if (pRoot->data == val)
    {
        return pRoot;
    }

    for (auto child : pRoot->children)
    {
        auto result = tree_depth_first_search_rec(child, val);

        if (result)
        {
            return result;
        }
    }

    return nullptr;
}

// iterative implementation of DFS
// use stack (push,top,pop) or vector(push_back,back,pop_back)
// when inserting children, make sure they will be dicovered in the same
// order as in TNode's array children
template <typename T>
TNode<T> const * // returns first found node that is equal to val
tree_depth_first_search_iter(TNode<T> const *pRoot, T const &val)
{
    std::vector<const TNode<T> *> openlist;
    openlist.push_back(pRoot);

    while (openlist.size() > 0)
    {
        const TNode<T> *cur = openlist.back();
        if (cur->data == val)
        {
            return cur;
        }

        openlist.pop_back();

        // insert children in the order ( 0,1,2,3,... )
        // reverse to make it "left-to-right"
        openlist.insert(openlist.end(), cur->children.rbegin(), cur->children.rend());
    }

    return nullptr; // failed
}

// iterative implementation of BFS
// simple change from tree_depth_first_search_iter (previous)
// change container type to deque
template <typename T>
TNode<T> const * // returns first found node that is equal to val
tree_breadth_first_search_iter(TNode<T> const *pRoot, T const &val)
{
    std::deque<const TNode<T> *> openlist;

    openlist.push_back(pRoot);

    while (openlist.size() > 0)
    {
        const TNode<T> *cur = openlist.back();

        if (cur->data == val)
        {
            return cur;
        }

        openlist.pop_back();

        for (auto child : cur->children)
        {
            openlist.push_front(child);
        }
    }

    return nullptr;
}

#endif
