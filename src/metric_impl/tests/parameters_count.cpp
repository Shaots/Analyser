#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using namespace std::string_literals;

class ParamCount : public testing::TestWithParam<std::pair<std::string, int>> {};
TEST_P(ParamCount, 2) {
    std::pair<std::string, int> input = GetParam();
    analyser::file::File f(input.first);
    analyser::function::FunctionExtractor extractor;
    analyser::function::Function funcs = extractor.Get(f).at(0);
    CountParametersMetric paramCount;
    EXPECT_EQ(std::get<int>(paramCount.CalculateImpl(funcs)), input.second);
}

INSTANTIATE_TEST_SUITE_P(
    A, ParamCount,
    testing::Values(std::make_pair("comments.py"s, 3),
                    std::make_pair("exceptions.py"s, 0), 
                    std::make_pair("if.py"s, 1),
                    std::make_pair("loops.py"s, 1), 
                    std::make_pair("many_lines.py"s, 0),
                    std::make_pair("many_parameters.py"s, 5),
                    std::make_pair("match_case.py"s, 1),
                    std::make_pair("nested_if.py"s, 2), 
                    std::make_pair("simple.py"s, 0),
                    std::make_pair("ternary.py"s, 1)));

}  // namespace analyser::metric::metric_impl
