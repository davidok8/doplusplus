// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2013-2016 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#pragma once

#include <DO/Sara/Defines.hpp>

#include <QtOpenGL>
#ifdef __APPLE__
#  include <OpenGL/GLU.h>
#else
#  include <GL/glu.h>
#endif


namespace DO::Sara::GL {

  constexpr GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
  constexpr GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
  constexpr GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
  constexpr GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
  constexpr GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
  constexpr GLfloat defaultAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};

  constexpr double defaultAxisLength = 10.0;
  constexpr double defaultAxisRadius = 0.125;

  //! @brief Coordinate system frame class.
  class DO_SARA_EXPORT Frame
  {
  public:
    //! @brief Default constructor.
    Frame();

    //! @brief Destructor.
    ~Frame();

    //! @brief Draw frame in the window.
    void draw(double axisLength, double axisRadius = defaultAxisRadius);

  private:
    GLUquadricObj* pQuadObj;
  };


}  // namespace DO::Sara::GL
