

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace analyser::metric;

class catgory : public ::testing::Test {
protected:
    CategoricalAccumulator cat;
};

TEST_F(catgory, 1) {
    cat.Reset();
    cat.Finalize();
    EXPECT_TRUE(cat.Get().empty());
}

TEST_F(catgory, 2) {
    cat.Reset();
    cat.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    EXPECT_TRUE(cat.Get().empty());
}

TEST_F(catgory, 3) {
    cat.Reset();
    cat.Accumulate(MetricResult{.metric_name = "Lines count", .value = 1});
    cat.Accumulate(MetricResult{.metric_name = "Cyclomatic_complexity", .value = 4});
    cat.Accumulate(MetricResult{.metric_name = "Parameters count", .value = 2});
    cat.Accumulate(MetricResult{.metric_name = "Naming Style", .value = "camel case"});
    cat.Accumulate(MetricResult{.metric_name = "Parameters count", .value = "camel case"});

    auto freq = cat.Get();
    EXPECT_FALSE(freq.empty());
    EXPECT_EQ(freq["camel case"], 2);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
