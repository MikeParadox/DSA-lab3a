#pragma once

//  Фрагменты для реализации сбалансированных деревьев поиска - заготовка, не
//  рабочая, доделать

#include <cassert>
#include <exception>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <queue>
#include <string>
#include <vector>


template<typename T, class Compare = std::less<T>,
         class Allocator = std::allocator<T>>
class BinarySearchTree
{
    struct Node;

public:
    using key_type = T;
    using key_compare = Compare;
    using value_type = T;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    using node_allocator_type =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    class iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = BinarySearchTree::value_type;
        using difference_type = BinarySearchTree::difference_type;
        using pointer = BinarySearchTree::const_pointer;
        using reference = BinarySearchTree::const_reference;

        iterator(Node* cur)
            : _current{cur} {} // not explicit to allow
                               // implicit conversion from Node*

        reference operator*() const noexcept { return _current->data; }
        pointer operator->() const noexcept { return _current->data; }
        iterator& operator++() noexcept // TODO to test
        {
            if (_current->is_nill) return _current;

            if (!_current->right->is_nill)
            {
                _current = _current->right;
                while (!_current->left->is_nill) _current = _current->left;
                return _current;
            }
            else
            {
                if (_current->parent->is_nill) _current = _current->parent;

                else
                {
                    auto temp = _current->parent;
                    if (temp->left == _current)
                    {
                        _current = temp;
                        return temp;
                    }

                    while (temp->data < _current->data)
                    {
                        temp = temp->parent;
                        if (temp->is_nill)
                        {
                            _current = temp;
                            return temp;
                        }
                    }
                    _current = temp;
                }
            }
            return _current;
        }
        iterator& operator--() noexcept // TODO to test
        {
            if (_current->is_nill) return _current;

            else if (!_current->left->is_nill)
            {
                _current = _current->left;
                while (!_current->right->is_nill) _current = _current->right;
                return _current;
            }
            else
            {
                if (_current->parent->is_nill) _current = _current->parent;

                else
                {
                    auto temp = _current->parent;
                    if (temp->right == _current) _current = temp;
                    else
                    {
                        while (temp->data > _current->data)
                        {
                            temp = temp->parent;
                            if (temp->is_nill) break;
                        }
                        _current = temp;
                    }
                }
            }
            return _current;
        }
        iterator& operator++(int) noexcept
        {
            iterator temp{_current};
            auto old = temp;
            ++temp;
            _current = temp._current;
            return old._current;
        }
        iterator& operator--(int) noexcept
        {
            iterator temp{_current};
            auto old = temp;
            --temp;
            _current = temp._current;
            return old._current;
        }
        auto operator<=>(const iterator& rhs) const = default;

        bool is_parent() const;
        bool is_left() const;
        bool is_right() const;

    private:
        Node* _current;
    };

    using const_iterator = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    template<class InputIterator>
    BinarySearchTree(InputIterator first, InputIterator last,
                     Compare comparator = Compare(),
                     allocator_type alloc = allocator_type());
    BinarySearchTree(Compare comparator = Compare(),
                     allocator_type alloc = allocator_type());
    BinarySearchTree(std::initializer_list<T> lst);
    BinarySearchTree(const BinarySearchTree& rhs);

    const BinarySearchTree& operator=(const BinarySearchTree& rhs);
    const BinarySearchTree& operator=(BinarySearchTree&& rhs);

    iterator begin() const noexcept { return _end->left; }
    iterator end() const noexcept { return _end; }

    reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }
    reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(end());
    }


    allocator_type get_allocator() const noexcept { return _alc; }
    key_compare key_comp() const noexcept { return _cmp; }
    value_compare value_comp() const noexcept { return _cmp; }
    inline bool empty() const noexcept { return !_size; }

    size_type size() const { return _size; }
    void swap(BinarySearchTree& other) noexcept;

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last);
    std::pair<iterator, bool> insert(const T& value);
    iterator insert(const_iterator position, const value_type& x);

    iterator find(const value_type& value) const;
    iterator lower_bound(const value_type& key);
    const_iterator lower_bound(const value_type& key) const;
    iterator upper_bound(const value_type& key);
    const_iterator upper_bound(const value_type& key) const;
    size_type count(const value_type& key) const;
    std::pair<const_iterator, const_iterator>
    equal_range(const value_type& key) const;
    iterator erase(iterator elem);
    size_type erase(const value_type& elem);
    iterator erase(const_iterator first, const_iterator last);

    auto operator<=>(const BinarySearchTree& rhs) const = default;
    // bool operator==(const BinarySearchTree<T>& rhs);
    // bool operator!=(const BinarySearchTree<T>& rhs);
    // bool operator>=(const BinarySearchTree<T>& rhs);
    // bool operator<=(const BinarySearchTree<T>& rhs);
    // bool operator>(const BinarySearchTree<T>& rhs);
    // bool operator<(const BinarySearchTree<T>& rhs);

    void clear();
    ~BinarySearchTree();

private:
    using AllocType =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    AllocType _alc;
    Compare _cmp{Compare()};
    std::size_t _size{0};
    Node* _end;


    iterator root() { return _end->right; }

    struct Node
    {
        Node(value_type d, Node* p, Node* l, Node* r)
            : data{d}, parent(p), left{l}, right{r}
        {
        }

        value_type data;
        Node* parent;
        Node* left;
        Node* right;
        bool is_nill{false};
    };

    Node* make_end()
    {
        _end = _alc.allocate(1);

        std::allocator_traits<AllocType>::construct(_alc, &(_end->parent));
        _end->parent = _end;

        std::allocator_traits<AllocType>::construct(_alc, &(_end->left));
        _end->left = _end;

        std::allocator_traits<AllocType>::construct(_alc, &(_end->right));
        _end->right = _end;

        return _end;
    }

    Node* make_node(T&& elem, Node* parent, Node* left, Node* right)
    {
        Node* new_node{_alc.allocate(1)};

        std::allocator_traits<AllocType>::construct(_alc, &(new_node->parent));
        new_node->parent = parent;

        std::allocator_traits<AllocType>::construct(_alc, &(new_node->left));
        new_node->left = left;

        std::allocator_traits<AllocType>::construct(_alc, &(new_node->right));
        new_node->right = right;

        std::allocator_traits<AllocType>::construct(_alc, &(new_node->data),
                                                    std::forward<T>(elem));

        return new_node;
    }

    void delete_end(Node* node)
    {
        std::allocator_traits<AllocType>::destroy(_alc, &(node->parent));
        std::allocator_traits<AllocType>::destroy(_alc, &(node->left));
        std::allocator_traits<AllocType>::destroy(_alc, &(node->right));
        std::allocator_traits<AllocType>::deallocate(_alc, node, 1);
    }

    inline void delete_node(Node* node)
    {
        std::allocator_traits<AllocType>::destroy(_alc, &(node->data));
        delete_end(node);
    }

    template<class RandomIterator>
    void ordered_insert(RandomIterator first, RandomIterator last,
                        iterator position)
    {
        if (first >= last) return;
        RandomIterator center = first + (last - first) / 2;
        iterator new_pos =
            insert(position, *center); //  итератор на вставленный элемент
        ordered_insert(first, center, position);
        ordered_insert(center + 1, last, ++position);
    }
};


template<class T, class Compare, class Allocator>
template<class InputIterator>
BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(InputIterator first,
                                                          InputIterator last,
                                                          Compare comparator,
                                                          allocator_type alloc)
    : _alc{alloc}, _cmp{comparator}, _end{make_end()}
{
    {
        if (std::is_same<
                typename std::iterator_traits<InputIterator>::iterator_category,
                typename std::random_access_iterator_tag>::value)
            ordered_insert(first, last, end());
        else std::for_each(first, last, [this](T x) { insert(x); });
    }
}

template<class T, class Compare, class Allocator>
BinarySearchTree<T, Compare, Allocator>::BinarySearchTree(Compare comparator,
                                                          allocator_type alloc)
    : _alc{alloc}, _cmp{comparator}, _end{make_end()}
{
}

// BinarySearchTree(std::initializer_list<T> lst);

// BinarySearchTree(const BinarySearchTree& rhs);

// pointer operator->() const noexcept;
// iterator& operator++() noexcept;
// iterator& operator--() noexcept;
// iterator& operator++(int) noexcept;
// iterator& operator--(int) noexcept;
// bool operator!=(const iterator& other) const;
// bool operator==(const iterator& other) const;








