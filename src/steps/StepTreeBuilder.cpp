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

StepPtr buildStepNode(const ExprNode& node, StepPtr parent)
{
    // ---------- Number ----------
    if (auto num = dynamic_cast<const NumberNode*>(&node)) {
        StepPtr st = std::make_shared<Step>(StepType::EvaluateNumber);

        st->exprBefore = toString(num->evaluate());
        st->exprAfter  = toString(num->evaluate());
        st->description = "数字节点：" + st->exprAfter;
        st->parent = parent;

        return st;
    }

    // ---------- Unary ----------
    if (auto un = dynamic_cast<const UnaryMinusNode*>(&node)) {
        StepPtr st = std::make_shared<Step>(StepType::EvaluateUnary);
        st->parent = parent;

        StepPtr child = buildStepNode(un->getChild(), st);
        st->children.push_back(child);

        st->exprBefore = "-" + child->exprAfter;
        st->exprAfter  = "-" + child->exprAfter;
        st->description = "取反运算";

        return st;
    }

    // ---------- Binary ----------
    if (auto bin = dynamic_cast<const BinaryNode*>(&node)) {
        StepPtr st = std::make_shared<Step>(StepType::EvaluateBinary);
        st->parent = parent;

        StepPtr L = buildStepNode(bin->getLeft(), st);
        StepPtr R = buildStepNode(bin->getRight(), st);

        st->children.push_back(L);
        st->children.push_back(R);

        st->exprBefore = "(" + L->exprAfter + " ? " + R->exprAfter + ")";
        st->exprAfter  = toString(node.evaluate());
        st->description = "二元运算：" + st->exprAfter;

        return st;
    }

    throw std::runtime_error("未知 AST 节点类型");
}

} // namespace



StepTree StepTreeBuilder::build(const ExprNode& exprNode)
{
    StepPtr root = buildStepNode(exprNode, nullptr);

    StepPtr finalStep = std::make_shared<Step>(StepType::FinalResult);
    finalStep->children.push_back(root);
    root->parent = finalStep;

    finalStep->exprBefore = root->exprBefore;
    finalStep->exprAfter  = root->exprAfter;
    finalStep->description = "最终结果：" + root->exprAfter;

    return StepTree(finalStep);
}