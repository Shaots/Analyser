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
    auto resRanges = analysis | std::views::filter([](const auto& p){
        return p.first.class_name.has_value();
    }) | std::views::chunk_by([](const auto& left, const auto& right){
        return left.first.class_name.value() == right.first.class_name.value();
    });
    // | rs::to<std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>>;
    
    std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>> res;

    res.resize(std::ranges::distance(resRanges));
    for (auto [index, vec] : std::views::zip(std::views::iota(0), resRanges)) {
        res[index].reserve(std::ranges::distance(vec));
        for (auto ele :vec) {
            res[index].emplace_back(std::move(ele));
        }
    }
    return res;
}

auto SplitByFiles(const auto &analysis) {
    auto resRanges = analysis | std::views::chunk_by([](const auto& left, const auto& right){
        return left.first.filename == right.first.filename;
    });
    // | rs::to<std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>>;
    std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>> res;

    res.resize(std::ranges::distance(resRanges));
    for (auto [index, vec] : std::views::zip(std::views::iota(0), resRanges)) {
        res[index].reserve(std::ranges::distance(vec));
        for (auto ele :vec) {
            res[index].emplace_back(std::move(ele));
        }
    }
    return res;

}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    rs::for_each(analysis, [&accumulator](const auto &metric) {
        accumulator.AccumulateNextFunctionResults(metric);
    }, &std::pair<function::Function, metric::MetricResults>::second);
}

}  // namespace analyser
