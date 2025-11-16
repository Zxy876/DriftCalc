#include "StepTreeBuilder.h"
#include <sstream>

using namespace driftcalc::expr;
using namespace driftcalc::steps;

namespace {

std::string toString(double v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

// 递归生成步骤树
StepPtr generateSteps(const ExprNode& node) {
    // ---- NumberNode ----
    if (auto num = dynamic_cast<const NumberNode*>(&node)) {
        StepPtr st = std::make_shared<Step>(StepType::EvaluateNumber);
        double val = num->evaluate();

        st->exprAfter = toString(val);
        st->description = "识别到数字：" + toString(val);

        return st;
    }

    // ---- UnaryMinusNode ----
    if (auto un = dynamic_cast<const UnaryMinusNode*>(&node)) {
        StepPtr step = std::make_shared<Step>(StepType::EvaluateUnary);

        // 子节点步骤
        StepPtr childStep = generateSteps(un->childNode());
        step->children.push_back(childStep);

        double val = node.evaluate();
        step->exprAfter = "-" + childStep->exprAfter;
        step->description = "对数字取反（前缀负号）";

        return step;
    }

    // ---- BinaryNode ----
    if (auto bin = dynamic_cast<const BinaryNode*>(&node)) {
        StepPtr step = std::make_shared<Step>(StepType::EvaluateBinary);

        // 左右步骤
        StepPtr L = generateSteps(bin->leftNode());
        StepPtr R = generateSteps(bin->rightNode());

        step->children.push_back(L);
        step->children.push_back(R);

        double val = node.evaluate();
        step->exprAfter = toString(val);

        // 根据类型生成更友善的描述
        if (dynamic_cast<const AddNode*>(&node)) {
            step->description = "执行加法操作：左(" + L->exprAfter + ") + 右(" + R->exprAfter + ")";
        } 
        else if (dynamic_cast<const SubNode*>(&node)) {
            step->description = "执行减法操作：左(" + L->exprAfter + ") - 右(" + R->exprAfter + ")";
        } 
        else if (dynamic_cast<const MulNode*>(&node)) {
            step->description = "执行乘法操作：左(" + L->exprAfter + ") × 右(" + R->exprAfter + ")";
        } 
        else if (dynamic_cast<const DivNode*>(&node)) {
            step->description = "执行除法操作：左(" + L->exprAfter + ") ÷ 右(" + R->exprAfter + ")";
        } 
        else if (dynamic_cast<const PowNode*>(&node)) {
            step->description = "执行幂运算：左(" + L->exprAfter + ") ^ 右(" + R->exprAfter + ")";
        }
        else {
            step->description = "执行二元运算";
        }

        return step;
    }

    throw std::runtime_error("未知类型节点（StepTreeBuilder 无法识别）");
}

} // anonymous namespace


// ------------------------------------------------------------
// 公开 API：构建步骤树
// ------------------------------------------------------------
StepTree StepTreeBuilder::build(const ExprNode& exprNode) {
    StepPtr rootStep = generateSteps(exprNode);

    // final wrapper step
    StepPtr finalStep = std::make_shared<Step>(StepType::FinalResult);
    finalStep->children.push_back(rootStep);
    finalStep->exprAfter = rootStep->exprAfter;
    finalStep->description = "最终结果：" + rootStep->exprAfter;

    return StepTree(finalStep);
}