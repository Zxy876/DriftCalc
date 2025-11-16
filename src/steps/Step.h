#pragma once
#include <string>
#include <vector>
#include <memory>

namespace driftcalc::steps {

enum class StepType {
    EvaluateNumber,
    EvaluateUnary,
    EvaluateBinary,
    Combine,
    FinalResult
};

struct Step {
    StepType type;
    std::string exprBefore;   // 步骤发生前的表达式（字符串）
    std::string exprAfter;    // 步骤发生后的表达式（字符串）
    std::string description;  // 文本解释（给草稿风渲染器）
    std::vector<std::shared_ptr<Step>> children;

    Step(StepType t) : type(t) {}
};

using StepPtr = std::shared_ptr<Step>;

} // namespace driftcalc::steps