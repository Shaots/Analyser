#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace analyser::metric;
class sum_average : public ::testing::Test {
protected:
    SumAverageAccumulator sumAv;
};

TEST_F(sum_average, 1) {
    sumAv.Reset();
    sumAv.Finalize();
    const auto res = sumAv.Get();
    const auto answer = SumAverageAccumulator::SumAverage{.sum = 0, .average = 0.0};
    EXPECT_EQ(res, answer);
}

TEST_F(sum_average, 2) {
    sumAv.Reset();
    sumAv.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    sumAv.Finalize();
    const auto res = sumAv.Get();
    const auto answer = SumAverageAccumulator::SumAverage{.sum = 1, .average = 1.0};
    EXPECT_EQ(res, answer);
}

TEST_F(sum_average, 3) {
    sumAv.Reset();
    sumAv.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    sumAv.Accumulate(MetricResult{.metric_name = "Cyclomatic_complexity", .value = 4});
    sumAv.Accumulate(MetricResult{.metric_name = "Parameters count", .value = 2});
    sumAv.Finalize();
    const auto res = sumAv.Get();
    const auto answer = SumAverageAccumulator::SumAverage{.sum = 7, .average = 7.0/3};
    EXPECT_EQ(res, answer);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
