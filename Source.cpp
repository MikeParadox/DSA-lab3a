#include "BStree.h"
#include <algorithm>
#include <iterator>
#include <list>
#include <print>
#include <random>
#include <set>
#include <vector>



using std::print;
using std::size_t;

template<typename InputIter> void print(InputIter first, InputIter last)
{

    if (std::is_same<
            typename std::iterator_traits<InputIter>::iterator_category,
            std::random_access_iterator_tag>::value)
    {
        print("Random access iterator range : ");
        while (first != last) print("{} ", *first++);
    }
    else
    {
        print("Any input iterator range : ");
        while (first != last) print("{} ", *first++);
    }
}

int main()
{

    const size_t sz = 15;
    std::vector<int> v;
    v.reserve(sz);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, +1000);
    generate_n(back_inserter(v), sz, [&]() { return (dis(gen) % 5000); });
    sort(v.begin(), v.end());
    print("\n -------------------------------- \n");
    copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    print("\n -------------------------------- \n");
    std::set<int> bb(v.begin(), v.end());
    print(bb.begin(), bb.end());
    // BinarySearchTree<int> bst;
    // std::println("{}", bst.size());
    // std::print(" -------------------------------- \n");

#ifdef _WIN32
    system("pause");
#endif //_WIN32
    return 0;
}