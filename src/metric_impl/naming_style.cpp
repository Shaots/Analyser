#include "metric_impl/naming_style.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function &f) const {
    std::cout << f.name << std::endl;
    if (f.name.contains('_'))
        return std::string("snake case");
    if (std::ranges::all_of(f.name, [](unsigned char c) { return std::islower(c); }))
        return std::string("lower case");
    if (std::isupper(f.name[0]))
        return std::string("pascal case");
    return std::string("camel case");
}

std::string NamingStyleMetric::Name() const { return "Naming Style"; };

}  // namespace analyser::metric::metric_impl
