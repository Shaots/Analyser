#pragma once
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

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CodeLinesCountMetric final: IMetric {
    MetricResult::ValueType CalculateImpl(const function::Function& f) const;
    std::string Name() const;
};

} // namespace analyser::metric::metric_impl
