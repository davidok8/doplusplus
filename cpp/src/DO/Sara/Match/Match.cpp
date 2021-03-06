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

#include <fstream>
#include <string>
#include <sstream>

#include <DO/Sara/Match.hpp>
#include <DO/Sara/Graphics.hpp>


using namespace std;


namespace DO { namespace Sara {

  ostream & operator<<(ostream & os, const Match& m)
  {
    os << "source=" << m.x_pos().transpose()
       << " target=" << m.y_pos().transpose() << endl;
    os << "score=" << m.score() << " " << "rank=" << m.rank();
    return os;
  }

  bool write_matches(const vector<Match>& matches, const string& filepath)
  {
    ofstream file(filepath.c_str());
    if (!file.is_open()) {
      cerr << "Cant open file" << filepath << endl;
      return false;
    }

    file << matches.size() << endl;
    for(auto m = matches.begin(); m != matches.end(); ++m)
      file << m->x_index() << ' ' << m->y_index() << ' '
           << m->rank() << ' ' << m->score() << endl;

    return true;
  }

  bool read_matches(vector<Match>& matches,
                    const string& filepath,
                    float score_thres)
  {
    if (!matches.empty())
      matches.clear();

    ifstream file(filepath.c_str());
    if (!file.is_open())
    {
      cerr << "Cant open file: " << filepath << endl;
      return false;
    }

    size_t match_count;
    file >> match_count;

    matches.reserve(match_count);
    for (size_t i = 0; i < match_count; ++i)
    {
      Match m;

      file >> m.x_index() >> m.y_index() >> m.rank() >> m.score();
      if(m.score() > score_thres)
        break;

      matches.push_back(m);
    }

    return true;
  }

  bool read_matches(vector<Match>& matches,
                    const vector<OERegion>& source_keys,
                    const vector<OERegion>& target_keys,
                    const string& fileName,
                    float score_thres)
  {
    if (!matches.empty())
      matches.clear();

    std::ifstream file(fileName.c_str());
    if (!file.is_open()) {
      std::cerr << "Cant open file: " << fileName << std::endl;
      return false;
    }

    std::size_t match_count;
    file >> match_count;

    matches.reserve(match_count);
    for (size_t i = 0; i < match_count; ++i)
    {
      Match m;

      file >> m.x_index() >> m.y_index() >> m.rank() >> m.score();
      m.x_pointer() = &source_keys[m.x_index()];
      m.y_pointer() = &target_keys[m.y_index()];

      if(m.score() > score_thres)
        break;

      matches.push_back(m);
    }

    return true;
  }

  void draw_image_pair(const Image<Rgb8>& I1, const Image<Rgb8>& I2,
                       const Point2f& off2, float scale)
  {
    display(I1, Point2i::Zero(), scale);
    display(I2, (off2*scale).cast<int>(), scale);
  }

  void draw_match(const Match& m, const Color3ub& c, const Point2f& t,
                  float z)
  {
    m.x().draw(c, z);
    m.y().draw(c, z, t);
    Point2f p1 = m.x_pos()*z;
    Point2f p2 = (m.y_pos() + t)*z;
    draw_line(p1, p2, c);
  }

  void draw_matches(const vector<Match>& matches, const Point2f& off2, float z)
  {
    for (auto m = matches.begin(); m != matches.end(); ++m)
      draw_match(*m, Color3ub(rand()%256, rand()%256, rand()%256), off2, z);
  }

  void check_matches(const Image<Rgb8>& I1, const Image<Rgb8>& I2,
                     const vector<Match>& matches,
                     bool redraw_everytime, float z)
  {
    Point2f off{ float(I1.width()), 0.f };
    draw_image_pair(I1, I2);
    for (auto m = matches.begin(); m != matches.end(); ++m)
    {
      if (redraw_everytime)
        draw_image_pair(I1, I2, z);
      draw_match(*m, Color3ub(rand()%256, rand()%256, rand()%256), off, z);
      cout << *m << endl;
      get_key();
    }
  }

} /* namespace Sara */
} /* namespace DO */
