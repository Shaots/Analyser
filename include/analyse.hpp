#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    auto resRanges = files | std::views::transform([](auto &&str) {
                         file::File f(str);
                         return function::FunctionExtractor{}.Get(f);
                     }) |
                     std::views::join | std::views::transform([&metric_extractor](auto &&func) {
                         auto res = metric_extractor.Get(func);
                         return std::make_pair(func, res);
                     });
    std::vector<std::pair<function::Function, metric::MetricResults>> res;
    res.reserve(std::ranges::distance(res));
    for (auto ele : resRanges) {
        res.emplace_back(std::move(ele));
    }
    return res;
}

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
