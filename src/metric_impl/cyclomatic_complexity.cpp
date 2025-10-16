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
#include <unordered_set>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    static const std::unordered_set<std::string> forbiddenKeys = {"return", "case_clause"};
    static const std::unordered_set<std::string> necessaryKeys = {"statement", "clause", "case_pattern", "conditional_expression"};
    auto res = f.ast | std::views::split('\n') |
               std::views::transform([](auto &&str) { return std::string_view{str}; }) |
               std::views::filter([](auto &&str) {
                   return std::ranges::none_of(forbiddenKeys, [&str](const auto &key){ return str.contains(key);}) &&
                          std::ranges::any_of(necessaryKeys, [&str](const auto &key){ return str.contains(key);});
               });
    return (int)std::ranges::distance(res);
}

std::string CyclomaticComplexityMetric::Name() const { return "Cyclomatic_complexity"; }

}  // namespace analyser::metric::metric_impl
