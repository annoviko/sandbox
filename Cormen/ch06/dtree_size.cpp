/* 6.2 - Calculate d-tree hight with n nodes. Here d - maximum amount of children, n - amount elements in the tree. */

#include <cmath>
#include <iostream>


std::size_t tree_size(const std::size_t n, const std::size_t d) {
  const double approx_height = std::log( (1 - (1 - (double) d) * (double) n) ) / std::log((double) d);
  return (std::size_t) std::ceil(approx_height);
}


void assert(const std::size_t expected, const std::size_t actual, const std::size_t n, const std::size_t d) {
  if (expected != actual) {
    std::cout << "[FAIL] n = '" << n << "', d = '" << d << "'" << std::endl;
    std::cout << "       Expected: " << expected << ", Actual: " << actual << std::endl;
  }
  else {
    std::cout << "[PASS] n = '" << n << "', d = '" << d << "'" << std::endl;
  }
}


void test_height_tree(void) {
  assert(1, tree_size(1, 2), 1, 2);
  assert(1, tree_size(1, 3), 1, 3);

  assert(2, tree_size(2, 2), 2, 2);
  assert(2, tree_size(3, 2), 2, 3);
  assert(2, tree_size(2, 3), 2, 3);
  assert(2, tree_size(3, 3), 3, 3);
  assert(2, tree_size(4, 3), 4, 3);
  assert(2, tree_size(2, 4), 2, 4);
  assert(2, tree_size(3, 4), 3, 4);
  assert(2, tree_size(4, 4), 4, 4);
  assert(2, tree_size(5, 4), 5, 4);

  assert(3, tree_size(5, 3), 5, 3);
  assert(3, tree_size(6, 3), 6, 3);
  assert(3, tree_size(12, 3), 12, 3);
  assert(3, tree_size(13, 3), 13, 3);

  assert(4, tree_size(14, 3), 14, 3);
  assert(4, tree_size(18, 3), 18, 3);
  assert(4, tree_size(32, 3), 32, 3);
  assert(4, tree_size(40, 3), 40, 3);

  assert(5, tree_size(41, 3), 41, 3);
  assert(5, tree_size(43, 3), 43, 3);
}


int main(int argc, char *argv[]) {
  if (argc == 3) {
    std::size_t n = atoi(argv[1]);
    std::size_t d = atoi(argv[2]);

    std::cout << tree_size(n, d) << std::endl;
  }
  else {
    test_height_tree();
  }

  return 0;
}