// -*- C++ -*-
//
// Copyright Sylvain Bougerel 2009 - 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK
#define SPATIAL_ENABLE_ASSERT // detect interal issues that should not occur

#include <boost/test/unit_test.hpp>
#include "../../src/bits/spatial_math.hpp"
#include "../../src/exception.hpp"
#include "spatial_test_fixtures.hpp"

BOOST_AUTO_TEST_CASE( text_except_check_dimension )
{
  pointset_fix<int2> fix(0);
  BOOST_CHECK_NO_THROW
    (except::check_dimension(fix.container.dimension(), 0));
  BOOST_CHECK_THROW
    (except::check_dimension(fix.container.dimension(), 2),
     invalid_dimension);
}

BOOST_AUTO_TEST_CASE( text_except_check_node )
{
  {
    details::Node<details::Kdtree_link<int, int> >* ptr = 0;
    BOOST_CHECK_THROW(except::check_node(ptr),
                      invalid_node);
  }
  {
    pointset_fix<quad> fix(1);
    BOOST_CHECK_NO_THROW(except::check_node(fix.container.begin().node));
    BOOST_CHECK_THROW(except::check_node(fix.container.end().node),
                      invalid_node);
  }
}

BOOST_AUTO_TEST_CASE( text_except_check_iterator )
{
  {
    details::Node_iterator<details::Kdtree_link<int2, int2> > i(0);
    BOOST_CHECK_THROW(except::check_node_iterator(i.node),
                      invalid_iterator);
  }
  {
    idle_pointset_fix<int2> fix(1);
    details::Node_iterator<details::Kdtree_link<const int2, const int2> >
      i(fix.container.begin().node);
    details::Node_iterator<details::Kdtree_link<const int2, const int2> >
      j(fix.container.end().node);
    BOOST_CHECK_NO_THROW(except::check_node_iterator(i.node));
    BOOST_CHECK_THROW(except::check_node_iterator(j.node),
                      invalid_iterator);
    BOOST_CHECK_NO_THROW(except::check_iterator(i, i));
    BOOST_CHECK_THROW(except::check_iterator(i, j),
                      invalid_iterator);
  }
}

BOOST_AUTO_TEST_CASE( text_except_check_empty_tree )
{
  pointset_fix<int2> fix(0);
  BOOST_CHECK_THROW(except::check_empty_container(fix.container),
                    invalid_empty_container);
}

BOOST_AUTO_TEST_CASE( text_except_check_range )
{
  pointset_fix<int2> fix(0);
  BOOST_CHECK_NO_THROW(except::check_open_bounds
                       (fix.container, zeros, ones));
  BOOST_CHECK_NO_THROW(except::check_bounds
                       (fix.container, zeros, ones));
  BOOST_CHECK_NO_THROW(except::check_closed_bounds
                       (fix.container, zeros, zeros));
  BOOST_CHECK_THROW(except::check_open_bounds
                    (fix.container, zeros, zeros), invalid_bounds);
  BOOST_CHECK_THROW(except::check_bounds
                    (fix.container, zeros, zeros), invalid_bounds);
  BOOST_CHECK_THROW(except::check_closed_bounds
                    (fix.container, ones, zeros), invalid_bounds);
  // For boxes, point2d are used as 1D boxes, or intervals.
  int2 lh_box(0, 1);
  int2 hl_box(1, 0);
  BOOST_CHECK_NO_THROW
    (except::check_box(fix.container, lh_box, llhh_layout_tag()));
  BOOST_CHECK_THROW
    (except::check_box(fix.container, hl_box, llhh_layout_tag()),
     invalid_box);
  BOOST_CHECK_NO_THROW
    (except::check_box(fix.container, lh_box, lhlh_layout_tag()));
  BOOST_CHECK_THROW
    (except::check_box(fix.container, hl_box, lhlh_layout_tag()),
     invalid_box);
  BOOST_CHECK_NO_THROW
    (except::check_box(fix.container, hl_box, hhll_layout_tag()));
  BOOST_CHECK_THROW
    (except::check_box(fix.container, lh_box, hhll_layout_tag()),
     invalid_box);
  BOOST_CHECK_NO_THROW
    (except::check_box(fix.container, hl_box, hlhl_layout_tag()));
  BOOST_CHECK_THROW
    (except::check_box(fix.container, lh_box, hlhl_layout_tag()),
     invalid_box);
}

BOOST_AUTO_TEST_CASE( text_except_check_math )
{
  BOOST_CHECK_NO_THROW(except::check_positive_distance(0));
  BOOST_CHECK_NO_THROW(except::check_positive_distance(1));
  BOOST_CHECK_THROW(except::check_positive_distance(-1), invalid_distance);
  BOOST_CHECK_NO_THROW(except::check_abs(0));
  BOOST_CHECK_EQUAL(except::check_abs(0), 0);
  BOOST_CHECK_NO_THROW(except::check_abs(1));
  BOOST_CHECK_EQUAL(except::check_abs(1), 1);
  BOOST_CHECK_NO_THROW(except::check_abs(-1));
  BOOST_CHECK_EQUAL(except::check_abs(-1), 1);
  BOOST_CHECK_THROW(except::check_abs((std::numeric_limits<int>::min)()),
                    arithmetic_error);
  BOOST_CHECK_NO_THROW(except::check_positive_add(0, 0));
  BOOST_CHECK_EQUAL(except::check_positive_add(0, 0), 0);
  BOOST_CHECK_NO_THROW(except::check_positive_add(1, 1));
  BOOST_CHECK_EQUAL(except::check_positive_add(1, 1), 2);
  BOOST_CHECK_THROW(except::check_positive_add
                    (std::numeric_limits<int>::max(), 1),
                    arithmetic_error);
  BOOST_CHECK_NO_THROW(except::check_square(0));
  BOOST_CHECK_EQUAL(except::check_square(0), 0);
  BOOST_CHECK_NO_THROW(except::check_square(-1));
  BOOST_CHECK_EQUAL(except::check_square(-1), 1);
  BOOST_CHECK_THROW(except::check_square(std::numeric_limits<int>::max()),
                    arithmetic_error);
  BOOST_CHECK_NO_THROW(except::check_positive_mul(0, 0));
  BOOST_CHECK_EQUAL(except::check_positive_mul(0, 0), 0);
  BOOST_CHECK_NO_THROW(except::check_positive_mul(1, 1));
  BOOST_CHECK_EQUAL(except::check_positive_mul(1, 1), 1);
  BOOST_CHECK_THROW(except::check_positive_mul
                    (std::numeric_limits<int>::max(), 2),
                    arithmetic_error);
}
