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

struct Step;
using StepPtr = std::shared_ptr<Step>;

struct Step {
    StepType type;

    std::string exprBefore;   // 操作前表达式
    std::string exprAfter;    // 操作后表达式
    std::string description;  // 给渲染器用的文本说明

    std::vector<StepPtr> children; // 子步骤

    StepPtr parent = nullptr;      // ★关键：用于导航（上下左右移动）

    Step(StepType t) : type(t) {}
};

} // namespace driftcalc::steps