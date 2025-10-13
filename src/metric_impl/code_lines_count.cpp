#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <charconv>
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

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    auto res =
        f.ast | std::views::split('\n') | std::views::transform([](auto &&str) { return std::string_view{str}; }) |
        std::views::filter([](auto &&str) { return str.contains("(function_definition") || str.contains("comment"); }) |
        std::views::transform([](auto &&str) {
            const std::string marker = "(function_definition";
            size_t id_pos = str.find(marker, 0);
            if (id_pos == std::string::npos) {
                return 0;
            }

            int start;
            size_t start_coord = str.find('[', id_pos);
            size_t start_comma = str.find(',', start_coord);
            auto [e, errc] = std::from_chars(str.begin() + start_coord + 1, str.begin() + start_comma, start);
            if (errc != std::errc{}) {
                throw std::invalid_argument("Cannot convert '" + std::string(str) + "' to integral");
            }

            int end;
            size_t end_coord = str.find('[', start_comma);
            size_t end_comma = str.find(',', end_coord);
            auto [e_, errc_] = std::from_chars(str.begin() + end_coord + 1, str.begin() + end_comma, end);
            if (errc_ != std::errc{}) {
                throw std::invalid_argument("Cannot convert '" + std::string(str) + "' to integral");
            }
            return end - start + 1;
        });
    return int(*(res.begin()) - std::ranges::distance(res) + 1);
}

std::string CodeLinesCountMetric::Name() const { return "Lines count"; }

}  // namespace analyser::metric::metric_impl
