#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class Cyclomatic : public testing::TestWithParam<std::pair<std::string, int>> {};
TEST_P(Cyclomatic, 2) {
    std::pair<std::string, int> input = GetParam();
    analyser::file::File f(input.first);
    analyser::function::FunctionExtractor extractor;
    analyser::function::Function funcs = extractor.Get(f).at(0);
    CyclomaticComplexityMetric cyclomatic;
    EXPECT_EQ(std::get<int>(cyclomatic.CalculateImpl(funcs)), input.second);
}

INSTANTIATE_TEST_SUITE_P(
    A, Cyclomatic,
    testing::Values(std::make_pair("comments.py"s, 0),
                    std::make_pair("exceptions.py"s, 4),
                    std::make_pair("if.py"s, 1),
                    std::make_pair("loops.py"s, 3),
                    std::make_pair("many_lines.py"s, 1),
                    std::make_pair("many_parameters.py"s, 1),
                    std::make_pair("match_case.py"s, 4),
                    std::make_pair("nested_if.py"s, 5),
                    std::make_pair("simple.py"s, 1),
                    std::make_pair("ternary.py"s, 2)));

}  // namespace analyser::metric::metric_impl
