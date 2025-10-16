#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

class NamingStyle : public testing::TestWithParam<std::pair<std::string, std::string>> {};
TEST_P(NamingStyle, 3) {
    std::pair<std::string, std::string> input = GetParam();
    analyser::file::File f(input.first);
    analyser::function::FunctionExtractor extractor;
    analyser::function::Function funcs = extractor.Get(f).at(0);
    NamingStyleMetric naming;
    EXPECT_EQ(std::get<std::string>(naming.CalculateImpl(funcs)), input.second);
}

INSTANTIATE_TEST_SUITE_P(
    A, NamingStyle,
    testing::Values(std::make_pair("comments.py", "snake case"),
                    std::make_pair("exceptions.py", "snake case"),
                    std::make_pair("if.py", "camel case"),
                    std::make_pair("loops.py", "pascal case"),
                    std::make_pair("many_lines.py", "lower case"),
                    std::make_pair("many_parameters.py", "snake case"),
                    std::make_pair("match_case.py", "snake case"),
                    std::make_pair("nested_if.py", "pascal case"),
                    std::make_pair("simple.py", "snake case"),
                    std::make_pair("ternary.py", "snake case")));

}  // namespace analyser::metric::metric_impl
