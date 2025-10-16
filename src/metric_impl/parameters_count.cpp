#include "metric_impl/parameters_count.hpp"

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

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    auto res = f.ast | std::views::split('\n') |
               std::views::transform([](auto &&str) { return std::string_view{str}; }) |
               std::views::take_while([](auto &&str) { return !str.contains("body"); }) |
               std::views::filter([](auto &&str) { return str.contains("identifier"); });
    return (int)(std::ranges::distance(res) - 1);
}

std::string CountParametersMetric::Name() const { return "Parameters count"; }

}  // namespace analyser::metric::metric_impl
