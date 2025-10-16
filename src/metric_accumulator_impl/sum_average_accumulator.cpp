#include "metric_accumulator_impl/sum_average_accumulator.hpp"

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

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += std::get<int>(metric_result.value);
    ++count;
}

void SumAverageAccumulator::Finalize() {
    average = (count != 0) ? ((double)sum / count) : 0.0;
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    sum = 0;
    count = 0;
    average = 0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    return SumAverage{.sum=this->sum, .average=this->average};
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
