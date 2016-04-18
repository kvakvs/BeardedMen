// -*- C++ -*-
//
// Copyright Sylvain Bougerel 2009 - 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK
#define SPATIAL_ENABLE_ASSERT // detect interal issues that should not occur

#include <boost/test/unit_test.hpp>
#include "spatial_test_fixtures.hpp"

BOOST_AUTO_TEST_CASE( test_empty_kdtree_basic )
{
  idle_pointset_fix<int2> fix;
  BOOST_CHECK(fix.container.end().node->left == fix.container.end().node);
  BOOST_CHECK(fix.container.end().node->right == fix.container.end().node);
  BOOST_CHECK(fix.container.end().node->parent == fix.container.end().node);
  BOOST_CHECK(fix.container.begin().node == fix.container.end().node); // leftmost
  BOOST_CHECK(fix.container.cbegin().node == fix.container.end().node);
  BOOST_CHECK(fix.container.empty());
  BOOST_CHECK(fix.container.begin() == fix.container.end());
  BOOST_CHECK(fix.container.cbegin() == fix.container.end());
  BOOST_CHECK(fix.container.cbegin() == fix.container.end());
  BOOST_CHECK(fix.container.cend() == fix.container.end());
  BOOST_CHECK(fix.container.rbegin() == fix.container.rend());
  BOOST_CHECK(fix.container.crbegin() == fix.container.rend());
  BOOST_CHECK(fix.container.crend() == fix.container.rend());
}

BOOST_AUTO_TEST_CASE( test_empty_kdtree_basic_clear )
{
  idle_pointset_fix<int2> fix;
  BOOST_REQUIRE_NO_THROW(fix.container.clear());
  BOOST_CHECK(fix.container.cend().node->left == fix.container.end().node);
  BOOST_CHECK(fix.container.end().node->right == fix.container.end().node);
  BOOST_CHECK(fix.container.end().node->parent == fix.container.end().node);
  BOOST_CHECK(fix.container.begin().node->right == fix.container.end().node); // leftmost
  BOOST_CHECK(fix.container.empty());
}

BOOST_AUTO_TEST_CASE( test_empty_kdtree_basic_swap )
{
  runtime_idle_pointset_fix<int2> fix1;
  runtime_idle_pointset_fix<int2> fix2;
  runtime_idle_pointset_fix<int2>::container_type::iterator iter1 = fix1.container.end();
  runtime_idle_pointset_fix<int2>::container_type::iterator iter2 = fix2.container.end();
  swap(fix1.container, fix2.container);
  BOOST_CHECK(fix1.container.end() == iter1);
  BOOST_CHECK(fix2.container.end() == iter2);
  BOOST_CHECK(fix1.container.empty());
  BOOST_CHECK(fix2.container.empty());
}

BOOST_AUTO_TEST_CASE( test_empty_kdtree_basic_max_size )
{
  idle_pointset_fix<double6> fix;
  std::allocator<details::Kdtree_link<double6, double6> > node_alloc;
  BOOST_CHECK(fix.container.max_size() == node_alloc.max_size());
}

BOOST_AUTO_TEST_CASE( test_empty_kdtree_basic_accessor )
{
  idle_pointset_fix<double6> fix;
  idle_pointset_fix<double6>::container_type::allocator_type alloc;
  idle_pointset_fix<double6>::container_type::rank_type rank;
  idle_pointset_fix<double6>::container_type::key_compare compare;
  BOOST_CHECK_EQUAL(fix.container.dimension(), 6);
  BOOST_CHECK_NO_THROW(rank = fix.container.rank());
  BOOST_CHECK_NO_THROW(compare = fix.container.key_comp());
  BOOST_CHECK_NO_THROW(alloc = fix.container.get_allocator());
}

BOOST_AUTO_TEST_CASE( test_empty_kdtree )
{
  idle_point_multimap_fix<int2, std::string> fix;
  BOOST_CHECK(fix.container.empty());
  BOOST_CHECK(fix.container.begin() == fix.container.end());
  // The rest was tested above...
}

BOOST_AUTO_TEST_CASE( test_kdtree_insert_iterate_forward )
{
  idle_pointset_fix<int2> fix;
  spatial::import::array<int2, 100> points;
  for(spatial::import::array<int2, 100>::iterator i
        = points.begin(); i != points.end(); ++i)
    {
      (*i)[0] = std::rand() % 20; // to increase chances of collisions
      (*i)[1] = std::rand() % 20;
      BOOST_REQUIRE_NO_THROW(fix.container.insert(*i));
      BOOST_TEST_CHECKPOINT("Adding element " << (i - points.begin()));
    }
  BOOST_CHECK(fix.container.empty() == false);
  BOOST_REQUIRE_EQUAL(fix.container.size(), 100);
  BOOST_CHECK_EQUAL(fix.container.count(), 100);
  int count = 0;
  for(idle_pointset_fix<int2>::container_type::iterator i
        = fix.container.begin(); i != fix.container.end(); ++i)
    {
      spatial::import::array<int2, 100>::iterator match;
      BOOST_REQUIRE((match = std::find(points.begin(), points.end(), *i))
                    != points.end());
      (*match)[0] = -1; // Prevent the same point from being found twice.
      (*match)[1] = -1;
      BOOST_REQUIRE_LE(++count, 100);
    }
  BOOST_CHECK_EQUAL(count, 100);
}

BOOST_AUTO_TEST_CASE( test_Kdtree_insert_100_iterate_backward )
{
  idle_pointset_fix<int2> fix;
  spatial::import::array<int2, 100> points;
  for(spatial::import::array<int2, 100>::iterator i
        = points.begin(); i != points.end(); ++i)
    {
      (*i)[0] = std::rand() % 20; // to increase chances of collisions
      (*i)[1] = std::rand() % 20;
      BOOST_REQUIRE_NO_THROW(fix.container.insert(*i));
      BOOST_TEST_CHECKPOINT("Adding element " << (i - points.begin()));
    }
  BOOST_CHECK(fix.container.empty() == false);
  BOOST_REQUIRE_EQUAL(fix.container.size(), 100);
  BOOST_CHECK_EQUAL(fix.container.count(), 100);
  int count = 0;
  for(idle_pointset_fix<int2>::container_type::reverse_iterator i
        = fix.container.rbegin(); i != fix.container.rend(); ++i)
    {
      spatial::import::array<int2, 100>::iterator match;
      BOOST_REQUIRE((match = std::find(points.begin(), points.end(), *i))
                  != points.end());
      (*match)[0] = -1; // Prevent the same point from being found twice.
      (*match)[1] = -1;
      BOOST_REQUIRE_LE(++count, 100);
    }
  BOOST_CHECK_EQUAL(count, 100);
}

BOOST_AUTO_TEST_CASE( test_kdtree_erase_iter )
{
  // check that erase at edge preserve basic iterators
  {
    idle_pointset_fix<quad> fix(30, randomize(-10, 10));
    typedef idle_pointset_fix<quad>::container_type
      ::iterator iter_type;
    std::size_t track_size = fix.container.size();
    while (fix.container.size() != 0)
      {
        iter_type iter = fix.container.begin();
        BOOST_REQUIRE_NO_THROW(fix.container.erase(iter));
        BOOST_CHECK_EQUAL(fix.container.size(), --track_size);
        BOOST_CHECK(iter != fix.container.begin());
        int count = 0;
        for(iter_type i = fix.container.begin();
            i != fix.container.end(); ++i, ++count);
        BOOST_CHECK_EQUAL(count, track_size);
      }
  }
  {
    idle_pointset_fix<quad> fix(30, randomize(-10, 10));
    typedef idle_pointset_fix<quad>::container_type
      ::iterator iter_type;
    typedef idle_pointset_fix<quad>::container_type
      ::reverse_iterator riter_type;
    std::size_t track_size = fix.container.size();
    while (fix.container.size() != 0)
      {
        iter_type iter = --fix.container.end();
        BOOST_REQUIRE_NO_THROW(fix.container.erase(iter));
        BOOST_CHECK_EQUAL(fix.container.size(), --track_size);
        BOOST_CHECK(iter != (--fix.container.end()));
        int count = 0;
        for(riter_type i = fix.container.rbegin();
            i != fix.container.rend(); ++i, ++count);
        BOOST_CHECK_EQUAL(count, track_size);
      }
  }
  // erase all and check that total ordering is preserved.
  {
    typedef idle_pointset_fix<int2>::container_type
      ::iterator iterator;
    typedef ordered_iterator
      <const idle_pointset_fix<int2>::container_type> ordered_iterator;
    idle_pointset_fix<int2> fix(100, randomize(-10, 10));
    std::size_t track_size = fix.container.size();
    while (fix.container.size() != 0)
      {
        iterator eraser = fix.container.begin();
        std::advance(eraser, static_cast<ptrdiff_t>
                     (static_cast<size_t>(rand()) % fix.container.size()));
        ordered_iterator begin = ordered_begin(fix.container);
        ordered_iterator end = ordered_end(fix.container);
        std::size_t count = 0;
        for(ordered_iterator i = begin; i != end; ++i, ++count);
        BOOST_REQUIRE_EQUAL(count, track_size);
        count = 0;
        for(ordered_iterator i = end; i != begin; --i, ++count);
        BOOST_REQUIRE_EQUAL(count, track_size);
        BOOST_REQUIRE_NO_THROW(fix.container.erase(eraser));
        BOOST_CHECK_EQUAL(fix.container.size(), --track_size);
      }
  }
}

BOOST_AUTO_TEST_CASE( test_kdtree_bulk_insert )
{
  // reuse test_kdtree_insert_100_iterate_forward
  runtime_idle_pointset_fix<int2> fix;
  spatial::import::array<int2, 100> points;
  for(spatial::import::array<int2, 100>::iterator i
        = points.begin(); i != points.end(); ++i)
    {
      (*i)[0] = std::rand() % 20;
      (*i)[1] = std::rand() % 20;
    }
  BOOST_REQUIRE_NO_THROW(fix.container.insert(points.begin(), points.end()));
  BOOST_CHECK(fix.container.empty() == false);
  BOOST_REQUIRE_EQUAL(fix.container.size(), 100);
  BOOST_CHECK_EQUAL(fix.container.count(), 100);
  int count = 0;
  for(runtime_idle_pointset_fix<int2>::container_type::iterator i
        = fix.container.begin(); i != fix.container.end(); ++i)
    {
      spatial::import::array<int2, 100>::iterator match;
      BOOST_REQUIRE((match = std::find(points.begin(), points.end(), *i))
                  != points.end());
      (*match)[0] = -1; // Prevent the same point from being found twice.
      (*match)[1] = -1;
      BOOST_REQUIRE_LE(++count, 100);
    }
  BOOST_CHECK_EQUAL(count, 100);
}

BOOST_AUTO_TEST_CASE( test_kdtree_copy_empty )
{
  idle_point_multimap_fix<quad, std::string> fix;
  idle_point_multimap_fix<quad, std::string>::container_type copy(fix.container);
  BOOST_CHECK_EQUAL(fix.container.size(), copy.size());
  BOOST_CHECK_EQUAL(fix.container.dimension(), copy.dimension());
  BOOST_CHECK(copy.begin() == copy.end());
}

BOOST_AUTO_TEST_CASE( test_kdtree_copy )
{
  // Simple copy (no rebalancing) should result in identical tree structure, so
  // iterators should output the same sequence.
  idle_pointset_fix<int2> fix(100, randomize(-100, 100));
  idle_pointset_fix<int2>::container_type copy(fix.container);
  BOOST_CHECK_EQUAL(fix.container.size(), copy.size());
  BOOST_CHECK_EQUAL(fix.container.dimension(), copy.dimension());
  BOOST_CHECK(fix.container == copy);
}

BOOST_AUTO_TEST_CASE( test_kdtree_assign_empty_trees )
{
  idle_point_multimap_fix<quad, std::string> empty;
  idle_point_multimap_fix<quad, std::string> full(100, increase());
  full.container = empty.container;
  BOOST_CHECK_EQUAL(empty.container.size(), full.container.size());
  BOOST_CHECK_EQUAL(empty.container.dimension(), full.container.dimension());
  BOOST_CHECK(full.container.begin() == full.container.end());
}

BOOST_AUTO_TEST_CASE( test_kdtree_assignment )
{
  idle_pointset_fix<int2> fix1(100, randomize(-20, 20));
  idle_pointset_fix<int2> fix2(20, randomize(-20, 20));
  fix2.container = fix1.container;
  BOOST_CHECK_EQUAL(fix1.container.size(), fix2.container.size());
  BOOST_CHECK_EQUAL(fix1.container.dimension(), fix2.container.dimension());
  BOOST_CHECK(fix1.container == fix2.container);
}

BOOST_AUTO_TEST_CASE( test_kdtree_swap )
{
  idle_pointset_fix<int2> fix1(100, randomize(-20, 20));
  idle_pointset_fix<int2> fix2(100, randomize(-20, 20));
  std::vector<int2> point_fix1; point_fix1.reserve(100);
  std::vector<int2> point_fix2; point_fix2.reserve(100);
  std::copy(fix1.container.cbegin(), fix1.container.cend(),
            std::back_inserter(point_fix1));
  std::copy(fix2.container.cbegin(), fix2.container.cend(),
            std::back_inserter(point_fix2));
  using std::swap; // it should still resolve into spatial::swap
  // swap once
  swap(fix1.container, fix2.container);
  typedef idle_pointset_fix<int2>::container_type::iterator iterator;
  std::size_t count = 0;
  for(iterator i = fix1.container.begin();
      i != fix1.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix2[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix2[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
  count = 0;
  for(iterator i = fix2.container.begin();
      i != fix2.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix1[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix1[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
  // swap twice, back to normal
  swap(fix1.container, fix2.container);
  typedef idle_pointset_fix<int2>::container_type::iterator iterator;
  count = 0;
  for(iterator i = fix1.container.begin();
      i != fix1.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix1[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix1[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
  count = 0;
  for(iterator i = fix2.container.begin();
      i != fix2.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix2[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix2[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
}

BOOST_AUTO_TEST_CASE( test_kdtree_swap_empty )
{
  idle_pointset_fix<int2> fix1(100, randomize(-10, 20));
  idle_pointset_fix<int2> fix2;
  std::vector<int2> point_fix1; point_fix1.reserve(100);
  std::copy(fix1.container.cbegin(), fix1.container.cend(),
            std::back_inserter(point_fix1));
  using std::swap; // it should still resolve into spatial::swap
  // swap once
  swap(fix1.container, fix2.container);
  typedef idle_pointset_fix<int2>::container_type::iterator iterator;
  std::size_t count = 0;
  for(iterator i = fix2.container.begin();
      i != fix2.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix1[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix1[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
  BOOST_CHECK(fix1.container.begin() == fix1.container.end());
  BOOST_CHECK_EQUAL(fix1.container.size(), 0);
  // swap twice, back to normal
  swap(fix1.container, fix2.container);
  count = 0;
  for(iterator i = fix1.container.begin();
      i != fix1.container.end(); ++i, ++count)
    {
      BOOST_CHECK_EQUAL((*i)[0], point_fix1[count][0]);
      BOOST_CHECK_EQUAL((*i)[1], point_fix1[count][1]);
    }
  BOOST_CHECK_EQUAL(count, 100);
  BOOST_CHECK(fix2.container.begin() == fix2.container.end());
  BOOST_CHECK_EQUAL(fix2.container.size(), 0);
}

BOOST_AUTO_TEST_CASE( test_kdtree_rebalance_empty )
{
  idle_pointset_fix<int2> fix;
  BOOST_REQUIRE_NO_THROW(fix.container.rebalance());
  BOOST_CHECK(fix.container.empty());
}

BOOST_AUTO_TEST_CASE( test_kdtree_rebalance_narrow )
{
  idle_pointset_fix<int2> fix(100, randomize(-5, 5));
  idle_pointset_fix<int2>::container_type copy(fix.container);
  BOOST_REQUIRE_NO_THROW(copy.rebalance());
  BOOST_CHECK_EQUAL(fix.container.size(), copy.size());
  BOOST_CHECK(copy == fix.container);
}


BOOST_AUTO_TEST_CASE( test_kdtree_copy_rebalance_uniform )
{
  // Simple copy (rebalancing) should result in a tree that has the same nodes
  idle_pointset_fix<int2> fix(100, decrease());
  idle_pointset_fix<int2>::container_type copy(fix.container, true);
  BOOST_CHECK_EQUAL(fix.container.size(), copy.size());
  BOOST_CHECK_EQUAL(fix.container.dimension(), copy.dimension());
  BOOST_CHECK(fix.container == copy);
}

BOOST_AUTO_TEST_CASE( test_kdtree_copy_rebalance_wide )
{
  // The performance gain of rebalance cannot be mesured now (see performance
  // executable) but we can make sure that total ordering has remained.
  idle_pointset_fix<double6> fix(100, randomize(-10, 10));
  idle_pointset_fix<double6>::container_type copy(fix.container, true);
  BOOST_CHECK_EQUAL(fix.container.size(), copy.size());
  BOOST_CHECK(copy == fix.container);
}

BOOST_AUTO_TEST_CASE( test_kdtree_find )
{
  {
    // Find on empty tree should return end.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    idle_pointset_fix<int2>::container_type::iterator it
      = fix.container.find(some_value);
    BOOST_CHECK(it == fix.container.end());
  }
  {
    // Find on empty const tree should return const end
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    idle_pointset_fix<int2>::container_type::const_iterator it
      = const_cast<const idle_pointset_fix<int2>*>
      (&fix)->container.find(some_value);
    BOOST_CHECK(it == fix.container.cend());
  }
  {
    // Find the one value of a tree should return begining.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    fix.container.insert(some_value);
    idle_pointset_fix<int2>::container_type::iterator it
      = fix.container.find(some_value);
    BOOST_CHECK(it == fix.container.begin());
  }
  {
    // Find on a const tree with one value should return const beginning.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    fix.container.insert(some_value);
    idle_pointset_fix<int2>::container_type::const_iterator it
      = const_cast<const idle_pointset_fix<int2>::container_type*>
      (&fix.container)->find(some_value);
    BOOST_CHECK(it == fix.container.cbegin());
  }
  {
    // Find a value not in the tree should return end.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    int2 other_value(1, 1);
    fix.container.insert(some_value);
    idle_pointset_fix<int2>::container_type::iterator it
      = fix.container.find(other_value);
    BOOST_CHECK(it == fix.container.end());
  }
  {
    // Find a value not in the const tree should return const end.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    int2 other_value(1, 1);
    fix.container.insert(some_value);
    idle_pointset_fix<int2>::container_type::const_iterator it
      = const_cast<const idle_pointset_fix<int2>::container_type*>
      (&fix.container)->find(other_value);
    BOOST_CHECK(it == fix.container.end());
  }
  {
    // Find should find all points in the tree, even when const
    idle_pointset_fix<int2> fix(100, randomize(-20, 20));
    {
      idle_pointset_fix<int2>::container_type::iterator iter
        = fix.container.begin();
      idle_pointset_fix<int2>::container_type::iterator end
        = fix.container.end();
      for(; iter != end; ++iter)
        {
          idle_pointset_fix<int2>::container_type::iterator it
            = fix.container.find(*iter);
          BOOST_REQUIRE(it != end);
          BOOST_CHECK(*it == *iter);
        }
    }
    {
      idle_pointset_fix<int2>::container_type::const_iterator iter
        = fix.container.cbegin();
      idle_pointset_fix<int2>::container_type::const_iterator end
        = fix.container.cend();
      for(; iter != end; ++iter)
        {
          idle_pointset_fix<int2>::container_type::const_iterator it
            = const_cast<const idle_pointset_fix<int2>::container_type*>
            (&fix.container)->find(*iter);
          BOOST_REQUIRE(it != end);
          BOOST_CHECK(*it == *iter);
        }
    }
    // But it should not find a point that is not in the tree!
    {
      int2 some_value(30, 30);
      idle_pointset_fix<int2>::container_type::iterator iter
        = fix.container.find(some_value);
      BOOST_CHECK(iter == fix.container.end());
    }
    {
      int2 some_value(30, 30);
      idle_pointset_fix<int2>::container_type::const_iterator iter
        = const_cast<idle_pointset_fix<int2>::container_type*>
        (&fix.container)->find(some_value);
      BOOST_CHECK(iter == fix.container.end());
    }
    // Verify that the equal operator is indeed used in place of the rest
    {
      typedef details::Kdtree<details::Dynamic_rank, int2, int2,
                              bracket_less<int2>, std::allocator<int2> >
        kdtree_type;
      // 1D tree where we store 2d objects...
      kdtree_type tree(details::Dynamic_rank(1));
      int2 one(0, 1);
      int2 two(0, 2);
      int2 three(0, 3);
      tree.insert(one);
      tree.insert(two);
      tree.insert(three);
      kdtree_type::iterator iter;
      kdtree_type::iterator end = tree.end();
      iter = tree.find(one);
      BOOST_REQUIRE(iter != end);
      BOOST_CHECK(one == *iter || two == *iter || three == *iter);
    }
  }
}

BOOST_AUTO_TEST_CASE ( test_kdtree_erase_key )
{
  {
    // Erase on empty tree should return 0.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    size_type count = fix.container.erase(some_value);
    BOOST_CHECK_EQUAL(count, 0);
    BOOST_CHECK(fix.container.end() == fix.container.begin());
    BOOST_CHECK(fix.container.empty());
  }
  {
    // Erase one value of a tree should return 1 and leave an empty tree.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    fix.container.insert(some_value);
    BOOST_CHECK(!fix.container.empty());
    size_type count = fix.container.erase(some_value);
    BOOST_CHECK_EQUAL(count, 1);
    BOOST_CHECK(fix.container.end() == fix.container.begin());
    BOOST_CHECK(fix.container.empty());
  }
  {
    // Erase a value not in the tree should leave the tree untouched.
    idle_pointset_fix<int2> fix;
    int2 some_value(0, 0);
    int2 other_value(1, 1);
    fix.container.insert(some_value);
    idle_pointset_fix<int2>::container_type::iterator
      begin_before = fix.container.begin();
    idle_pointset_fix<int2>::container_type::iterator
      end_before = fix.container.end();
    size_type count  = fix.container.erase(other_value);
    BOOST_CHECK_EQUAL(count, 0);
    BOOST_CHECK(!fix.container.empty());
    BOOST_CHECK(fix.container.end() != fix.container.begin());
    BOOST_CHECK(fix.container.end() == end_before);
    BOOST_CHECK(fix.container.begin() == begin_before);
  }
  {
    // Should be able to erase multiple values
    typedef details::Kdtree<details::Dynamic_rank, int2, int2,
                            bracket_less<int2>, std::allocator<int2> >
      kdtree_type;
    // 1D tree where we store 2d objects...
    kdtree_type tree(details::Dynamic_rank(1));
    int2 one  ( 1, 0 );
    int2 two  ( 2, 0 );
    int2 four ( 4, 0 );
    tree.insert(one);
    tree.insert(two);
    tree.insert(two);
    tree.insert(four);
    // ... And equal should take the second dimension into account.
    BOOST_REQUIRE_EQUAL(tree.count(), 4);
    size_type count = tree.erase(two);
    BOOST_CHECK_EQUAL(count, 2);
    BOOST_REQUIRE_EQUAL(tree.count(), 2);
    BOOST_CHECK(tree.find(one) != tree.end());
    BOOST_CHECK(tree.find(four) != tree.end());
    BOOST_CHECK(tree.find(two) == tree.end());
    kdtree_type::iterator iter = tree.begin();
    BOOST_REQUIRE(iter != tree.end());
    BOOST_REQUIRE_NO_THROW(++iter);
    BOOST_REQUIRE(iter != tree.end());
    BOOST_REQUIRE_NO_THROW(++iter);
    BOOST_REQUIRE(iter == tree.end());
  }
}

BOOST_AUTO_TEST_CASE (test_kdtree_equal)
{
  {
    // These 2 trees should be equal (contents are equivalent)
    idle_boxmap_fix<quad, std::string> one;
    one.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    idle_boxmap_fix<quad, std::string> two;
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    BOOST_CHECK(one.container == two.container);
  }
  {
    // These 2 trees should be different.
    idle_boxmap_fix<quad, std::string> one;
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    idle_boxmap_fix<quad, std::string> two;
    two.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    BOOST_CHECK(one.container != two.container);
  }
}

BOOST_AUTO_TEST_CASE (test_kdtree_lexicographic)
{
  {
    // These 2 trees should be equal (contents are equivalent)
    idle_boxmap_fix<quad, std::string> one;
    one.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    idle_boxmap_fix<quad, std::string> two;
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    BOOST_CHECK(!(one.container < two.container
                  || one.container > two.container));
  }
  {
    // These one is a "prefix" of two lexicographically
    idle_boxmap_fix<quad, std::string> one;
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    one.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    idle_boxmap_fix<quad, std::string> two;
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(1, 2, 1, 1), ""));
    two.container.insert(std::make_pair(quad(2, 2, 1, 1), ""));
    BOOST_CHECK(one.container < two.container);
  }
}
