// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2021-present David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#define BOOST_TEST_MODULE "ImageProcessing/Level Sets/Fast Marching Method"

#include <DO/Sara/ImageProcessing/LevelSets/FastMarching.hpp>

#include "../AssertHelpers.hpp"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_fast_marching)
{
  namespace sara = DO::Sara;

  using T = float;

  auto gradient = sara::Image<float>(20, 10);

  // auto fm = sara::FastMarching<T, 2>{gradient};
}
