#include "metric.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) { metrics.emplace_back(std::move(metric)); }

// В 24.04 не поддерживается std::ranges::to<>()
MetricResults MetricExtractor::Get(const function::Function &func) const {
    auto res = metrics | std::views::transform([&func](auto &&metric) { return metric->Calculate(func); });
    MetricResults resVec;
    resVec.reserve(std::ranges::distance(res));
    for (auto ele : res) {
        resVec.emplace_back(std::move(ele));
    }
    return resVec;
}

}  // namespace analyser::metric
