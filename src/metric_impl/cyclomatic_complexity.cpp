#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    auto res = f.ast | std::views::split('\n') |
               std::views::transform([](auto &&str) { return std::string_view{str}; }) |
               std::views::filter([](auto &&str) {
                   return !str.contains("return") && !str.contains("case_clause") && (str.contains("statement") || str.contains("clause") || str.contains("case_pattern") || str.contains("conditional_expression"));
               });
    return (int)std::ranges::distance(res);
}

std::string CyclomaticComplexityMetric::Name() const { return "Cyclomatic_complexity"; }

}  // namespace analyser::metric::metric_impl
