#pragma once
#include "StepTree.h"
#include <vector>

namespace driftcalc::steps {

class StepNavigator {
public:
    StepNavigator(StepPtr root);

    StepPtr current() const { return currentStep; }

    // 左右：同级切换
    void moveLeft();
    void moveRight();

    // 上：进入 children（逻辑深入）
    void moveUp();

    // 下：回到 parent（拉远视角）
    void moveDown();

private:
    StepPtr rootStep;
    StepPtr currentStep;

    // 记录当前位置的路径
    std::vector<int> indexStack;  // 每层在 children 的 index
};

} // namespace driftcalc::steps