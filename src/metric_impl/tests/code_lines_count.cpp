#include "metric_impl/code_lines_count.hpp"
#include "function.hpp"
#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
using namespace std::string_literals;

class CodeLines : public testing::TestWithParam<std::pair<std::string, int>> {};
TEST_P(CodeLines, 1) {
    std::pair<std::string, int> input = GetParam();
    analyser::file::File f(input.first);
    analyser::function::FunctionExtractor extractor;
    analyser::function::Function funcs = extractor.Get(f).at(0);
    CodeLinesCountMetric codeLinesCount;
    EXPECT_EQ(std::get<int>(codeLinesCount.CalculateImpl(funcs)), input.second);
}

INSTANTIATE_TEST_SUITE_P(
    A, CodeLines,
    testing::Values(std::make_pair("comments.py"s, 4),
                    std::make_pair("exceptions.py"s, 8),
                    std::make_pair("if.py"s, 4),
                    std::make_pair("loops.py"s, 7),
                    std::make_pair("many_lines.py"s, 15),
                    std::make_pair("many_parameters.py"s, 2),
                    std::make_pair("match_case.py"s, 8),
                    std::make_pair("nested_if.py"s, 9),
                    std::make_pair("simple.py"s, 7),
                    std::make_pair("ternary.py"s, 2)));
}  // namespace analyser::metric::metric_impl
