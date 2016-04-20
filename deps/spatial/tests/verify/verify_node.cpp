// -*- C++ -*-
//
// Copyright Sylvain Bougerel 2009 - 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK
#define SPATIAL_ENABLE_ASSERT  // detect interal issues that should not occur

#include <boost/test/unit_test.hpp>
#include "../../src/bits/spatial_node.hpp"
#include "spatial_test_types.hpp"

using namespace spatial;

struct header_fixture {
    details::Node<details::Kdtree_link<int, int> > header;
    header_fixture() {
        header.parent = &header;
        header.left   = &header;
        header.right  = &header;
    }
};

struct int_node_fixture {
    /*          H
                |
               Root
               /   \
               L     R
              /  \
            LL    LR       */
    typedef details::Node<details::Kdtree_link<int, int> > node_type;
    node_type header;
    node_type node_root;
    node_type node_left;
    node_type node_left_left;
    node_type node_left_right;
    node_type node_right;

    int_node_fixture() {
        header.parent          = &node_root;
        header.left            = &header;
        header.right           = &node_right;
        node_root.parent       = &header;
        node_root.left         = &node_left;
        node_root.right        = &node_right;
        node_left.parent       = &node_root;
        node_left.left         = &node_left_left;
        node_left.right        = &node_left_right;
        node_right.parent      = &node_root;
        node_right.left        = 0;
        node_right.right       = 0;
        node_left_right.parent = &node_left;
        node_left_right.left   = 0;
        node_left_right.right  = 0;
        node_left_left.parent  = &node_left;
        node_left_left.left    = 0;
        node_left_left.right   = 0;
    }
};

struct int2_node_fixture {
    /*           H
                 |
               (2,2)
               /   \
           (1,1)   (3,3)
           /  \
       (0,0)  (1,1)   */
    typedef details::Node<details::Kdtree_link<int2, int2> > node_type;
    node_type  header;
    node_type* leftmost;
    details::Kdtree_link<int2, int2> node_root;
    details::Kdtree_link<int2, int2> node_left;
    details::Kdtree_link<int2, int2> node_left_left;
    details::Kdtree_link<int2, int2> node_left_right;
    details::Kdtree_link<int2, int2> node_right;

    int2_node_fixture() {
        using namespace ::spatial::details;
        header.parent           = &node_root;
        header.left             = &header;
        header.right            = &node_right;
        leftmost                = &node_left_left;
        node_root.parent        = &header;
        node_root.left          = &node_left;
        node_root.right         = &node_right;
        value(&node_root)       = twos;
        node_left.parent        = &node_root;
        node_left.left          = &node_left_left;
        node_left.right         = &node_left_right;
        value(&node_left)       = ones;
        node_right.parent       = &node_root;
        node_right.left         = 0;
        node_right.right        = 0;
        value(&node_right)      = threes;
        node_left_right.parent  = &node_left;
        node_left_right.left    = 0;
        node_left_right.right   = 0;
        value(&node_left_right) = ones;
        node_left_left.parent   = &node_left;
        node_left_left.left     = 0;
        node_left_left.right    = 0;
        value(&node_left_left)  = zeros;
    }
};

struct int2_relaxed_node_fixture {
    /*         H
               |
             (2,2)5
             /   \
        (1,1)3  (3,3)1
         /  \
    (0,0)1  (1,1)1       */
    typedef details::Node<details::Relaxed_kdtree_link<int2, int2> > node_type;
    node_type  header;
    node_type* leftmost;
    details::Relaxed_kdtree_link<int2, int2> node_root;
    details::Relaxed_kdtree_link<int2, int2> node_left;
    details::Relaxed_kdtree_link<int2, int2> node_left_left;
    details::Relaxed_kdtree_link<int2, int2> node_left_right;
    details::Relaxed_kdtree_link<int2, int2> node_right;

    int2_relaxed_node_fixture() {
        using namespace ::spatial::details;
        header.parent                  = &node_root;
        header.left                    = &header;
        header.right                   = &node_right;
        leftmost                       = &node_left_left;
        node_root.parent               = &header;
        node_root.left                 = &node_left;
        node_root.right                = &node_right;
        link(&node_root)->weight       = 5;
        value(&node_root)              = twos;
        node_left.parent               = &node_root;
        node_left.left                 = &node_left_left;
        node_left.right                = &node_left_right;
        link(&node_left)->weight       = 3;
        value(&node_left)              = ones;
        node_right.parent              = &node_root;
        node_right.left                = 0;
        node_right.right               = 0;
        link(&node_right)->weight      = 1;
        value(&node_right)             = threes;
        node_left_right.parent         = &node_left;
        node_left_right.left           = 0;
        node_left_right.right          = 0;
        link(&node_left_right)->weight = 1;
        value(&node_left_right)        = ones;
        node_left_left.parent          = &node_left;
        node_left_left.left            = 0;
        node_left_left.right           = 0;
        link(&node_left_left)->weight  = 1;
        value(&node_left_left)         = zeros;
    }
};

struct large_int2_relaxed_node_fixture {
    /*              H
                    |
                 (2,2)7
              /          \
        (1,1)3           (3,3)3
         /  \             /
    (0,0)1  (1,1)1     (3,3)2
                         \
                       (3,3)1     */
    typedef details::Node<details::Relaxed_kdtree_link<int2, int2> > node_type;
    node_type  header;
    node_type* leftmost;
    node_type  node_root;
    node_type  node_left;
    node_type  node_left_left;
    node_type  node_left_right;
    node_type  node_right;
    node_type  node_right_left;
    node_type  node_right_left_right;

    large_int2_relaxed_node_fixture() {
        using namespace ::spatial::details;
        header.parent                        = &node_root;
        header.left                          = &header;
        header.right                         = &node_right;
        leftmost                             = &node_left_left;
        node_root.parent                     = &header;
        node_root.left                       = &node_left;
        node_root.right                      = &node_right;
        link(&node_root)->weight             = 7;
        value(&node_root)                    = twos;
        node_left.parent                     = &node_root;
        node_left.left                       = &node_left_left;
        node_left.right                      = &node_left_right;
        link(&node_left)->weight             = 3;
        value(&node_left)                    = ones;
        node_right.parent                    = &node_root;
        node_right.left                      = &node_right_left;
        node_right.right                     = 0;
        link(&node_right)->weight            = 3;
        value(&node_right)                   = threes;
        node_right_left.parent               = &node_right;
        node_right_left.left                 = 0;
        node_right_left.right                = &node_right_left_right;
        link(&node_right_left)->weight       = 2;
        value(&node_right_left)              = threes;
        node_right_left_right.parent         = &node_right_left;
        node_right_left_right.left           = 0;
        node_right_left_right.right          = 0;
        link(&node_right_left_right)->weight = 1;
        value(&node_right_left_right)        = threes;
        node_left_right.parent               = &node_left;
        node_left_right.left                 = 0;
        node_left_right.right                = 0;
        link(&node_left_right)->weight       = 1;
        value(&node_left_right)              = ones;
        node_left_left.parent                = &node_left;
        node_left_left.left                  = 0;
        node_left_left.right                 = 0;
        link(&node_left_left)->weight        = 1;
        value(&node_left_left)               = zeros;
    }
};

BOOST_AUTO_TEST_CASE(test_header) {
    using namespace ::spatial::details;
    header_fixture fix1;
    BOOST_CHECK(header(&fix1.header));
    int_node_fixture fix2;
    BOOST_CHECK(header(&fix2.header));
    BOOST_CHECK(!header(&fix2.node_root));
    BOOST_CHECK(!header(&fix2.node_left));
    BOOST_CHECK(!header(&fix2.node_left_left));
    BOOST_CHECK(!header(&fix2.node_left_right));
    BOOST_CHECK(!header(&fix2.node_right));
    BOOST_CHECK(!header(&fix2.node_right));
}

BOOST_AUTO_TEST_CASE(test_5_node) {
    using namespace ::spatial::details;
    int_node_fixture fix;
    BOOST_CHECK(header(&fix.header));
    BOOST_CHECK(!header(&fix.node_root));
    BOOST_CHECK(!header(&fix.node_left));
    BOOST_CHECK(!header(&fix.node_right));
    BOOST_CHECK(!header(&fix.node_left_right));
    BOOST_CHECK(!header(&fix.node_left_left));
    BOOST_CHECK(minimum(&fix.node_root) == &fix.node_left_left);
    BOOST_CHECK(maximum(&fix.node_root) == &fix.node_right);
    Node<Kdtree_link<int, int> >* node = &fix.node_left_left;
    BOOST_REQUIRE((node = increment(node)) == &fix.node_left);
    BOOST_REQUIRE((node = increment(node)) == &fix.node_left_right);
    BOOST_REQUIRE((node = increment(node)) == &fix.node_root);
    BOOST_REQUIRE((node = increment(node)) == &fix.node_right);
    BOOST_REQUIRE((node = increment(node)) == &fix.header);
    BOOST_REQUIRE((node = decrement(node)) == &fix.node_right);
    BOOST_REQUIRE((node = decrement(node)) == &fix.node_root);
    BOOST_REQUIRE((node = decrement(node)) == &fix.node_left_right);
    BOOST_REQUIRE((node = decrement(node)) == &fix.node_left);
    BOOST_REQUIRE((node = decrement(node)) == &fix.node_left_left);
    const Node<Kdtree_link<int, int> >* cnode = &fix.node_root;
    BOOST_REQUIRE((cnode = preorder_increment(cnode)) == &fix.node_left);
    BOOST_REQUIRE((cnode = preorder_increment(cnode)) == &fix.node_left_left);
    BOOST_REQUIRE((cnode = preorder_increment(cnode)) == &fix.node_left_right);
    BOOST_REQUIRE((cnode = preorder_increment(cnode)) == &fix.node_right);
    BOOST_REQUIRE((cnode = preorder_increment(cnode)) == &fix.header);
}

/**
 *  In this test, we attempt to test all swapping combination possible in
 *  the 5-node tree defined above. Everytime, after swapping all nodes shall
 *  be reacheable, and the expected layout should be tested.
 */
BOOST_AUTO_TEST_CASE(test_swap_node_aux) {
    using namespace ::spatial::details;
    typedef int_node_fixture::node_type node_type;
    {  // Swap left_left with right
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       right     = &fix.node_right;
        swap_node_aux(left_left, right);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(right == &fix.node_right);
        BOOST_CHECK(fix.node_left_left.left == 0);
        BOOST_CHECK(fix.node_left_left.right == 0);
        BOOST_CHECK(fix.node_left_left.parent == &fix.node_root);
        BOOST_CHECK(fix.node_root.right == &fix.node_left_left);
        BOOST_CHECK(fix.node_root.left == &fix.node_left);
        BOOST_CHECK(fix.node_root.parent == &fix.header);
        BOOST_CHECK(fix.node_right.left == 0);
        BOOST_CHECK(fix.node_right.right == 0);
        BOOST_CHECK(fix.node_right.parent == &fix.node_left);
        BOOST_CHECK(fix.node_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left.left == &fix.node_right);
        BOOST_CHECK(fix.node_left.parent == &fix.node_root);
    }
    {  // Swap right with left_left (mirror procedure)
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       right     = &fix.node_right;
        swap_node_aux(right, left_left);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(right == &fix.node_right);
        BOOST_CHECK(fix.node_left_left.left == 0);
        BOOST_CHECK(fix.node_left_left.right == 0);
        BOOST_CHECK(fix.node_left_left.parent == &fix.node_root);
        BOOST_CHECK(fix.node_root.right == &fix.node_left_left);
        BOOST_CHECK(fix.node_root.left == &fix.node_left);
        BOOST_CHECK(fix.node_root.parent == &fix.header);
        BOOST_CHECK(fix.node_right.left == 0);
        BOOST_CHECK(fix.node_right.right == 0);
        BOOST_CHECK(fix.node_right.parent == &fix.node_left);
        BOOST_CHECK(fix.node_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left.left == &fix.node_right);
        BOOST_CHECK(fix.node_left.parent == &fix.node_root);
    }
    {  // Swap left_left with root
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       root      = &fix.node_root;
        swap_node_aux(left_left, root);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(fix.node_left_left.left == &fix.node_left);
        BOOST_CHECK(fix.node_left_left.right == &fix.node_right);
        BOOST_CHECK(fix.node_left_left.parent == &fix.header);
        BOOST_CHECK(fix.node_root.right == 0);
        BOOST_CHECK(fix.node_root.left == 0);
        BOOST_CHECK(fix.node_root.parent == &fix.node_left);
        BOOST_CHECK(fix.node_right.left == 0);
        BOOST_CHECK(fix.node_right.right == 0);
        BOOST_CHECK(fix.node_right.parent == &fix.node_left_left);
        BOOST_CHECK(fix.node_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left.left == &fix.node_root);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_left);
        BOOST_CHECK(fix.header.parent == &fix.node_left_left);
        BOOST_CHECK(fix.header.left == &fix.header);
        BOOST_CHECK(fix.header.right == &fix.node_right);
    }
    {  // Swap root with left_left (mirror procedure)
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       root      = &fix.node_root;
        swap_node_aux(root, left_left);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(fix.node_left_left.left == &fix.node_left);
        BOOST_CHECK(fix.node_left_left.right == &fix.node_right);
        BOOST_CHECK(fix.node_left_left.parent == &fix.header);
        BOOST_CHECK(fix.node_root.right == 0);
        BOOST_CHECK(fix.node_root.left == 0);
        BOOST_CHECK(fix.node_root.parent == &fix.node_left);
        BOOST_CHECK(fix.node_right.left == 0);
        BOOST_CHECK(fix.node_right.right == 0);
        BOOST_CHECK(fix.node_right.parent == &fix.node_left_left);
        BOOST_CHECK(fix.node_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left.left == &fix.node_root);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_left);
        BOOST_CHECK(fix.header.parent == &fix.node_left_left);
        BOOST_CHECK(fix.header.left == &fix.header);
        BOOST_CHECK(fix.header.right == &fix.node_right);
    }
    {  // Swap left_left with left
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       left      = &fix.node_left;
        swap_node_aux(left_left, left);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == 0);
        BOOST_CHECK(fix.node_left.right == 0);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_left);
        BOOST_CHECK(fix.node_left_left.left == &fix.node_left);
        BOOST_CHECK(fix.node_left_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left_left.parent == &fix.node_root);
        BOOST_CHECK(&fix.node_left_left == fix.node_root.left);
        BOOST_CHECK(&fix.node_left_left == fix.node_left_right.parent);
    }
    {  // Swap left with left_left (invertion)
        int_node_fixture fix;
        node_type*       left_left = &fix.node_left_left;
        node_type*       left      = &fix.node_left;
        swap_node_aux(left, left_left);
        BOOST_CHECK(left_left == &fix.node_left_left);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == 0);
        BOOST_CHECK(fix.node_left.right == 0);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_left);
        BOOST_CHECK(fix.node_left_left.left == &fix.node_left);
        BOOST_CHECK(fix.node_left_left.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_left_left.parent == &fix.node_root);
        BOOST_CHECK(&fix.node_left_left == fix.node_root.left);
        BOOST_CHECK(&fix.node_left_left == fix.node_left_right.parent);
    }
    {                          // Swap left_right with left
        int_node_fixture fix;  // swap with right child
        node_type*       left_right = &fix.node_left_right;
        node_type*       left       = &fix.node_left;
        swap_node_aux(left_right, left);
        BOOST_CHECK(left_right == &fix.node_left_right);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == 0);
        BOOST_CHECK(fix.node_left.right == 0);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_right);
        BOOST_CHECK(fix.node_left_right.left == &fix.node_left_left);
        BOOST_CHECK(fix.node_left_right.right == &fix.node_left);
        BOOST_CHECK(fix.node_left_right.parent == &fix.node_root);
        BOOST_CHECK(&fix.node_left_right == fix.node_root.left);
        BOOST_CHECK(&fix.node_left_right == fix.node_left_left.parent);
    }
    {  // Swap left with left_right (invertion)
        int_node_fixture fix;
        node_type*       left_right = &fix.node_left_right;
        node_type*       left       = &fix.node_left;
        swap_node_aux(left, left_right);
        BOOST_CHECK(left_right == &fix.node_left_right);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == 0);
        BOOST_CHECK(fix.node_left.right == 0);
        BOOST_CHECK(fix.node_left.parent == &fix.node_left_right);
        BOOST_CHECK(fix.node_left_right.left == &fix.node_left_left);
        BOOST_CHECK(fix.node_left_right.right == &fix.node_left);
        BOOST_CHECK(fix.node_left_right.parent == &fix.node_root);
        BOOST_CHECK(&fix.node_left_right == fix.node_root.left);
        BOOST_CHECK(&fix.node_left_right == fix.node_left_left.parent);
    }
    {  // Swap left and root
        int_node_fixture fix;
        node_type*       root = &fix.node_root;
        node_type*       left = &fix.node_left;
        swap_node_aux(left, root);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == &fix.node_root);
        BOOST_CHECK(fix.node_left.right == &fix.node_right);
        BOOST_CHECK(fix.node_left.parent == &fix.header);
        BOOST_CHECK(fix.node_root.left == &fix.node_left_left);
        BOOST_CHECK(fix.node_root.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_root.parent == &fix.node_left);
        BOOST_CHECK(fix.header.parent == &fix.node_left);
        BOOST_CHECK(&fix.node_root == fix.node_left_left.parent);
        BOOST_CHECK(&fix.node_root == fix.node_left_right.parent);
    }
    {  // Swap root and left (invertion)
        int_node_fixture fix;
        node_type*       root = &fix.node_root;
        node_type*       left = &fix.node_left;
        swap_node_aux(root, left);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(left == &fix.node_left);
        BOOST_CHECK(fix.node_left.left == &fix.node_root);
        BOOST_CHECK(fix.node_left.right == &fix.node_right);
        BOOST_CHECK(fix.node_left.parent == &fix.header);
        BOOST_CHECK(fix.node_root.left == &fix.node_left_left);
        BOOST_CHECK(fix.node_root.right == &fix.node_left_right);
        BOOST_CHECK(fix.node_root.parent == &fix.node_left);
        BOOST_CHECK(fix.header.parent == &fix.node_left);
        BOOST_CHECK(&fix.node_root == fix.node_left_left.parent);
        BOOST_CHECK(&fix.node_root == fix.node_left_right.parent);
    }
    {  // Swap right and root
        int_node_fixture fix;
        node_type*       root  = &fix.node_root;
        node_type*       right = &fix.node_right;
        swap_node_aux(right, root);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(right == &fix.node_right);
        BOOST_CHECK(fix.node_right.left == &fix.node_left);
        BOOST_CHECK(fix.node_right.right == &fix.node_root);
        BOOST_CHECK(fix.node_right.parent == &fix.header);
        BOOST_CHECK(fix.node_root.left == 0);
        BOOST_CHECK(fix.node_root.right == 0);
        BOOST_CHECK(fix.node_root.parent == &fix.node_right);
        BOOST_CHECK(fix.header.parent == &fix.node_right);
        BOOST_CHECK(&fix.node_right == fix.node_left.parent);
    }
    {  // Swap root and right (invertion)
        int_node_fixture fix;
        node_type*       root  = &fix.node_root;
        node_type*       right = &fix.node_right;
        swap_node_aux(root, right);
        BOOST_CHECK(root == &fix.node_root);
        BOOST_CHECK(right == &fix.node_right);
        BOOST_CHECK(fix.node_right.left == &fix.node_left);
        BOOST_CHECK(fix.node_right.right == &fix.node_root);
        BOOST_CHECK(fix.node_right.parent == &fix.header);
        BOOST_CHECK(fix.node_root.left == 0);
        BOOST_CHECK(fix.node_root.right == 0);
        BOOST_CHECK(fix.node_root.parent == &fix.node_right);
        BOOST_CHECK(fix.header.parent == &fix.node_right);
        BOOST_CHECK(&fix.node_right == fix.node_left.parent);
    }
}

BOOST_AUTO_TEST_CASE(test_swap_kdtree_node) {  // swap only position (tested
                                               // before). Do not swap values!
    using namespace ::spatial::details;
    int2_node_fixture             fix;
    int2_node_fixture::node_type* left_left = &fix.node_left_left;
    int2_node_fixture::node_type* right     = &fix.node_right;
    swap_node(left_left, right);
    BOOST_CHECK(fix.node_left_left.value == zeros);
    BOOST_CHECK(fix.node_right.value == threes);
    BOOST_CHECK(right == &fix.node_left_left);
    BOOST_CHECK(left_left == &fix.node_right);
}

BOOST_AUTO_TEST_CASE(test_swap_relaxed_kdtree_node) {  // swap position and
                                                       // weight, but not
                                                       // values!
    using namespace ::spatial::details;
    int2_relaxed_node_fixture             fix;
    int2_relaxed_node_fixture::node_type* left  = &fix.node_left;
    int2_relaxed_node_fixture::node_type* right = &fix.node_right;
    swap_node(left, right);
    BOOST_CHECK_EQUAL(fix.node_left.weight, 1u);
    BOOST_CHECK_EQUAL(fix.node_right.weight, 3u);
    BOOST_CHECK(fix.node_left.value == ones);
    BOOST_CHECK(fix.node_right.value == threes);
    BOOST_CHECK(right == &fix.node_left);
    BOOST_CHECK(left == &fix.node_right);
}

BOOST_AUTO_TEST_CASE(test_Node_iterator) {
    using namespace ::spatial::details;
    // increment and decrement tested earlier, check pre/post correctness and
    // derefencing
    {
        quad test_object(1, 2, 3, 4);
        Kdtree_link<quad, quad> test_node;
        test_node.parent = &test_node;
        test_node.left   = 0;
        test_node.right  = 0;
        test_node.value  = test_object;
        Node_iterator<Kdtree_link<quad, quad> > iter(&test_node);
        BOOST_CHECK(*iter == test_object);
        BOOST_CHECK_EQUAL(iter->x, 1);
        BOOST_CHECK_EQUAL(iter->y, 2);
        BOOST_CHECK_EQUAL(iter->z, 3);
        BOOST_CHECK_EQUAL(iter->w, 4);
        // should compile: non-const iterator!
        BOOST_CHECK((*iter = test_object) == test_object);
        BOOST_CHECK_EQUAL((iter->x = 3), 3);
    }
    {
        quad test_object(1, 2, 3, 4);
        Relaxed_kdtree_link<quad, quad> test_node;
        test_node.parent = &test_node;
        test_node.left   = 0;
        test_node.right  = 0;
        test_node.value  = test_object;
        Node_iterator<Relaxed_kdtree_link<quad, quad> > iter(&test_node);
        BOOST_CHECK(*iter == test_object);
        BOOST_CHECK_EQUAL(iter->x, 1);
        BOOST_CHECK_EQUAL(iter->y, 2);
        BOOST_CHECK_EQUAL(iter->z, 3);
        BOOST_CHECK_EQUAL(iter->w, 4);
        // should compile: non-const iterator!
        BOOST_CHECK((*iter = test_object) == test_object);
        BOOST_CHECK_EQUAL((iter->x = 3), 3);
    }
    {
        // Make sure that constant iterator does not invalidate anything
        int2_node_fixture fix;
        Node_iterator<Kdtree_link<int2, int2> > iter(&fix.node_root);
        BOOST_CHECK(*iter == twos);
    }
    {
        int2_node_fixture fix;
        Node_iterator<Kdtree_link<int2, int2> > iter(&fix.node_root);
        BOOST_REQUIRE((++iter).node == &fix.node_right);
        BOOST_REQUIRE((--iter).node == &fix.node_root);
        BOOST_REQUIRE((iter++).node == &fix.node_root);
        BOOST_REQUIRE(iter.node == &fix.node_right);
        BOOST_REQUIRE((iter--).node == &fix.node_right);
        BOOST_REQUIRE(iter.node == &fix.node_root);
    }
}

BOOST_AUTO_TEST_CASE(test_Const_Node_iterator) {
    using namespace ::spatial::details;
    // increment and decrement tested earlier, check pre/post correctness and
    // derefencing
    {
        quad test_object(1, 2, 3, 4);
        Kdtree_link<quad, quad> test_node;
        test_node.parent = &test_node;
        test_node.left   = 0;
        test_node.right  = 0;
        test_node.value  = test_object;
        Const_node_iterator<Kdtree_link<quad, quad> > iter(&test_node);
        BOOST_CHECK(*iter == test_object);
        BOOST_CHECK_EQUAL(iter->x, 1);
        BOOST_CHECK_EQUAL(iter->y, 2);
        BOOST_CHECK_EQUAL(iter->z, 3);
        BOOST_CHECK_EQUAL(iter->w, 4);
    }
    {
        quad test_object(1, 2, 3, 4);
        Relaxed_kdtree_link<quad, quad> test_node;
        test_node.parent = &test_node;
        test_node.left   = 0;
        test_node.right  = 0;
        test_node.value  = test_object;
        Const_node_iterator<Relaxed_kdtree_link<quad, quad> > iter(&test_node);
        BOOST_CHECK(*iter == test_object);
        BOOST_CHECK_EQUAL(iter->x, 1);
        BOOST_CHECK_EQUAL(iter->y, 2);
        BOOST_CHECK_EQUAL(iter->z, 3);
        BOOST_CHECK_EQUAL(iter->w, 4);
    }
    {
        // Make sure that constant iterator does not invalidate anything
        int2_node_fixture fix;
        Const_node_iterator<Kdtree_link<int2, int2> > iter(&fix.node_root);
        BOOST_CHECK(*iter == twos);
    }
    {
        int2_node_fixture fix;
        Const_node_iterator<Kdtree_link<int2, int2> > iter(&fix.node_root);
        BOOST_REQUIRE((++iter).node == &fix.node_right);
        BOOST_REQUIRE((--iter).node == &fix.node_root);
        BOOST_REQUIRE((iter++).node == &fix.node_root);
        BOOST_REQUIRE(iter.node == &fix.node_right);
        BOOST_REQUIRE((iter--).node == &fix.node_right);
        BOOST_REQUIRE(iter.node == &fix.node_root);
    }
}

BOOST_AUTO_TEST_CASE(test_Preorder_node_iterator) {
    using namespace ::spatial::details;
    // increment and decrement tested earlier, check pre/post correctness and
    // derefencing
    {
        quad test_object(1, 2, 3, 4);
        Kdtree_link<quad, quad> test_node;
        test_node.parent = &test_node;
        test_node.left   = 0;
        test_node.right  = 0;
        test_node.value  = test_object;
        Preorder_node_iterator<Kdtree_link<quad, quad> > iter(&test_node);
        BOOST_CHECK(*iter == test_object);
        BOOST_CHECK_EQUAL(iter->x, 1);
        BOOST_CHECK_EQUAL(iter->y, 2);
        BOOST_CHECK_EQUAL(iter->z, 3);
        BOOST_CHECK_EQUAL(iter->w, 4);
    }
    {
        int2_node_fixture fix;
        Preorder_node_iterator<Kdtree_link<int2, int2> > iter(&fix.node_root);
        BOOST_REQUIRE((++iter).node == &fix.node_left);
        BOOST_REQUIRE((iter++).node == &fix.node_left);
        BOOST_REQUIRE(iter.node == &fix.node_left_left);
    }
    {
        int2_node_fixture fix;
        Preorder_node_iterator<Kdtree_link<int2, int2> > a(&fix.node_root),
            b(&fix.node_root), c(&fix.node_left);
        BOOST_CHECK(a == b);
        BOOST_CHECK(a != c);
    }
    {
        int2_node_fixture fix;
        Preorder_node_iterator<Kdtree_link<int2, int2> > a(&fix.node_root),
            b(a);
        BOOST_CHECK(a == b);
    }
}
