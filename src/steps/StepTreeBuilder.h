#pragma once

#include "../expression/ASTNode.h"
#include "StepTree.h"

namespace driftcalc::steps {

class StepTreeBuilder {
public:
    static StepTree build(const driftcalc::expr::ExprNode& expr);
};

} // namespace driftcalc::steps