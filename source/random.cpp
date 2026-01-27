#include <arclib/random.h>
#include <algorithm>

bool arcl::chance(double c, random_engine& e) {
    c = std::clamp(c, 0.0, 1.0);
    return e.randf() < c;
}
