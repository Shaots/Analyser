#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace analyser::metric;
class average : public ::testing::Test {
protected:
    AverageAccumulator av;
};

TEST_F(average, 1) {
    av.Reset();
    av.Finalize();
    EXPECT_DOUBLE_EQ(av.Get(), 0.0);
}

TEST_F(average, 2) {
    av.Reset();
    av.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    av.Finalize();
    EXPECT_DOUBLE_EQ(av.Get(), 1);
}

TEST_F(average, 3) {
    av.Reset();
    av.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    av.Accumulate(MetricResult{.metric_name = "Cyclomatic_complexity", .value = 4});
    av.Accumulate(MetricResult{.metric_name = "Parameters count", .value = 2});
    av.Finalize();
    EXPECT_DOUBLE_EQ(av.Get(), 7.0 / 3);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
