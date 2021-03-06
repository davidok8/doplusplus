// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2014-2016 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#define BOOST_TEST_MODULE "Core/Pixel/Pixel Traits"

#include <cstdint>

#include <boost/test/unit_test.hpp>

#include <DO/Sara/Core/Pixel/PixelTraits.hpp>


using namespace std;
using namespace DO::Sara;


BOOST_AUTO_TEST_SUITE(TestPixelTraits)

BOOST_AUTO_TEST_CASE(test_pixel_traits_min_max_value)
{
  BOOST_CHECK_EQUAL(numeric_limits<int>::min(), PixelTraits<int>::min());
  BOOST_CHECK_EQUAL(0, PixelTraits<float>::min());

  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), PixelTraits<int>::max());
  BOOST_CHECK_EQUAL(1, PixelTraits<float>::max());

  auto expected_black = Matrix<uint8_t, 3, 1>::Zero();
  auto actual_black = PixelTraits<Matrix<uint8_t, 3, 1> >::min();
  BOOST_CHECK_EQUAL(expected_black, actual_black);

  auto expected_zeros = Vector3f::Zero();
  auto actual_zeros = PixelTraits<Vector3f>::min();
  BOOST_CHECK_EQUAL(expected_zeros, actual_zeros);

  auto expected_black_3 = Matrix<uint8_t, 3, 1>::Zero();
  auto actual_black_3 = PixelTraits<Matrix<uint8_t, 3, 1> >::min();
  BOOST_CHECK_EQUAL(expected_black_3, actual_black_3);

  auto expected_zeros_3 = Vector3f::Zero();
  auto actual_zeros_3 = PixelTraits<Vector3f>::min();
  BOOST_CHECK_EQUAL(expected_zeros_3, actual_zeros_3);

  auto expected_ones_3 = Vector3f::Ones();
  auto actual_ones_3 = PixelTraits<Vector3f>::max();
  BOOST_CHECK_EQUAL(expected_ones_3, actual_ones_3);
}

BOOST_AUTO_TEST_SUITE_END()
