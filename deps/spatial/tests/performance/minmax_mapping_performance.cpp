#include <iostream>
#include <vector>
#include <sstream>

#include "../../src/point_multiset.hpp"
#include "../../src/idle_point_multiset.hpp"
#include "../../src/mapping_iterator.hpp"

#include "chrono.hpp"
#include "random.hpp"
#include "point_type.hpp"

template <spatial::dimension_type N, typename Point, typename Distribution>
void compare_libraries
(std::size_t data_size, const Distribution& distribution)
{
  std::cout << "\t" << N << " dimensions, " << data_size << " objects:" << std::endl;
  std::vector<Point> data;
  data.reserve(data_size);
  for (std::size_t i = 0; i < data_size; ++i)
    data.push_back(Point(distribution));
  {
    // Mapping begin into an idle_point_multiset
    spatial::idle_point_multiset<N, Point> cobaye;
    cobaye.insert_rebalance(data.begin(), data.end());
    std::cout << "\t\tidle_point_multiset (maximum):\t" << std::flush;
    utils::time_point start = utils::process_timer_now();
    for (int j = 0; j != N; ++j)
      {
        spatial::mapping_iterator<spatial::idle_point_multiset<N, Point> >
          i = mapping_end(cobaye, 0); --i;
      }
    utils::time_point stop = utils::process_timer_now();
    std::cout << (stop - start) << "sec" << std::endl;
    std::cout << "\t\tidle_point_multiset:\t" << std::flush;
    start = utils::process_timer_now();
    for (int j = 0; j != N; ++j)
      {
        spatial::mapping_iterator<spatial::idle_point_multiset<N, Point> >
           i = mapping_begin(cobaye, 0); ++i;
      }
    stop = utils::process_timer_now();
    std::cout << (stop - start) << "sec" << std::endl;
  }
  {
    // Mapping begin into a point_multiset
    spatial::point_multiset<N, Point> cobaye;
    cobaye.insert(data.begin(), data.end());
    std::cout << "\t\tpoint_multiset:\t" << std::flush;
    utils::time_point start = utils::process_timer_now();
    for (int j = 0; j != N; ++j)
      {
        spatial::mapping_iterator<spatial::point_multiset<N, Point> >
           i = mapping_begin(cobaye, 0); ++i;
      }
    utils::time_point stop = utils::process_timer_now();
    std::cout << (stop - start) << "sec" << std::endl;
    std::cout << "\t\tpoint_multiset (reverse):\t" << std::flush;
    start = utils::process_timer_now();
    for (int j = 0; j != N; ++j)
      {
        spatial::mapping_iterator<spatial::point_multiset<N, Point> >
          i = mapping_end(cobaye, 0); --i;
      }
    stop = utils::process_timer_now();
    std::cout << (stop - start) << "sec" << std::endl;
  }
}

int main (int argc, char **argv)
{
  if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <sample size: integer>"
                << std::endl;
      return 1;
    }

  // Build initialization memory
  std::istringstream argbuf(argv[1]);
  std::size_t data_size;
  argbuf >> data_size;
  utils::random_engine engine(243287873);

  std::cout << "Uniform distribution:" << std::endl;
  utils::uniform_double_distribution uniform(engine, -1.0, 1.0);
  compare_libraries<3, point3_type, utils::uniform_double_distribution>
    (data_size, uniform);
  compare_libraries<9, point9_type, utils::uniform_double_distribution>
    (data_size, uniform);

  std::cout << "Normal distribution:" << std::endl;
  utils::normal_double_distribution normal(engine, -1.0, 1.0);
  compare_libraries<3, point3_type, utils::normal_double_distribution>
    (data_size, normal);
  compare_libraries<9, point9_type, utils::normal_double_distribution>
    (data_size, normal);

  std::cout << "Narrow normal distribution:" << std::endl;
  utils::narrow_double_distribution narrow(engine, -1.0, 1.0);
  compare_libraries<3, point3_type, utils::narrow_double_distribution>
    (data_size, narrow);
  compare_libraries<9, point9_type, utils::narrow_double_distribution>
    (data_size, narrow);
}
