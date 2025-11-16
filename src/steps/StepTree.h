#pragma once
#include "Step.h"

namespace driftcalc::steps {

struct StepTree {
    StepPtr root;

    explicit StepTree(StepPtr r) : root(std::move(r)) {}
};

} // namespace driftcalc::steps