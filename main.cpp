#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
// #include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (!options.Parse(argc, argv)) {
        return 1;
    }

    try {
        analyser::metric::MetricExtractor metric_extractor;
        using namespace analyser::metric::metric_impl;
        metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<NamingStyleMetric>());
        metric_extractor.RegisterMetric(std::make_unique<CountParametersMetric>());

        auto analysis = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);
        std::cout << "Analyse function" << std::endl;
        std::ranges::for_each(analysis, [](const auto &p) {
                                  std::string cls_name = p.first.class_name.has_value() ? ("::" + p.first.class_name.value()) : "";
                                  std::cout << p.first.filename << cls_name << "::" << p.first.name << ": " << std::endl;

                                  std::ranges::for_each(p.second, [](const auto &metricResult) {
                                    std::cout << "\t" << metricResult.metric_name << ": ";
                                    std::visit([&metricResult](const auto& val){std::cout << val << ", " << std::endl;}, metricResult.value); });
                                });
        std::cout << std::endl;

        analyser::metric_accumulator::MetricsAccumulator accumulator;
        using namespace analyser::metric_accumulator::metric_accumulator_impl;
        accumulator.RegisterAccumulator("Lines count", std::make_unique<AverageAccumulator>());
        accumulator.RegisterAccumulator("Cyclomatic_complexity", std::make_unique<AverageAccumulator>());
        accumulator.RegisterAccumulator("Parameters count", std::make_unique<SumAverageAccumulator>());
        accumulator.RegisterAccumulator("Naming Style", std::make_unique<CategoricalAccumulator>());

        auto splitedByFiles = analyser::SplitByFiles(analysis);
        std::cout << "Split by files:" << std::endl;
        std::ranges::for_each(splitedByFiles, [&accumulator](const auto &vec) {
            std::cout << vec.front().first.filename << std::endl;
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(vec, accumulator);
            auto res1 = accumulator.GetFinalizedAccumulator<AverageAccumulator>("Lines count").Get();
            std::cout << "\tLines count: Average = " << res1 << std::endl;
        
            auto res2 = accumulator.GetFinalizedAccumulator<AverageAccumulator>("Cyclomatic_complexity").Get();
            std::cout << "\tCyclomatic_complexity: Average = " << res2 << std::endl;

            auto res3 = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("Parameters count").Get();
            std::cout << "\tParameters count: Sum = " << res3.sum << "\tAverage = " << res3.average << std::endl;

            auto res4 = accumulator.GetFinalizedAccumulator<CategoricalAccumulator>("Naming Style").Get();
            std::cout << "\tNaming Style:";
            std::ranges::for_each(res4, [](const auto& p) {
                std::cout << "\t" << p.first << ":" << p.second << ", ";
            });
            std::cout << std::endl;
        });
        std::cout << std::endl;

        auto splitedByClass = analyser::SplitByClasses(analysis);
        std::cout << "Split by class:" << std::endl;
        std::ranges::for_each(splitedByClass, [&accumulator](const auto &vec) {
            std::cout << vec.front().first.filename << std::endl;
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(vec, accumulator);
            auto res1 = accumulator.GetFinalizedAccumulator<AverageAccumulator>("Lines count").Get();
            std::cout << "\tLines count: Average = " << res1 << std::endl;
        
            auto res2 = accumulator.GetFinalizedAccumulator<AverageAccumulator>("Cyclomatic_complexity").Get();
            std::cout << "\tCyclomatic_complexity: Average = " << res2 << std::endl;

            auto res3 = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("Parameters count").Get();
            std::cout << "\tParameters count: Sum = " << res3.sum << "\tAverage = " << res3.average << std::endl;

            auto res4 = accumulator.GetFinalizedAccumulator<CategoricalAccumulator>("Naming Style").Get();
            std::cout << "\tNaming Style:";
            std::ranges::for_each(res4, [](const auto& p) {
                std::cout << "\t" << p.first << ":" << p.second << ", ";
            });
            std::cout << std::endl;
        });
        std::cout << std::endl;

        analyser::AccumulateFunctionAnalysis(analysis, accumulator);
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            return 1;
        }
    return 0;
}
