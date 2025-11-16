#pragma once
#include "Step.h"
#include "StepTree.h"
#include "../expression/ASTNode.h"

namespace driftcalc::steps {

class StepTreeBuilder
{
public:
    StepTree build(const driftcalc::expr::ExprNode& rootExpr);
};

} // namespace driftcalc::steps