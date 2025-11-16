#include "StepNavigator.h"

using namespace driftcalc::steps;

StepNavigator::StepNavigator(StepPtr root)
    : rootStep(root), currentStep(root)
{
    indexStack.push_back(0);
}

void StepNavigator::moveLeft() {
    if (!currentStep->parent) return;

    auto& siblings = currentStep->parent->children;
    int idx = indexStack.back();

    if (idx > 0) {
        idx--;
        indexStack.back() = idx;
        currentStep = siblings[idx];
    }
}

void StepNavigator::moveRight() {
    if (!currentStep->parent) return;

    auto& siblings = currentStep->parent->children;
    int idx = indexStack.back();

    if (idx < (int)siblings.size() - 1) {
        idx++;
        indexStack.back() = idx;
        currentStep = siblings[idx];
    }
}

void StepNavigator::moveUp() {
    if (currentStep->children.empty()) return;

    currentStep = currentStep->children[0];
    indexStack.push_back(0);
}

void StepNavigator::moveDown() {
    if (indexStack.size() <= 1) return; // 根节点无法下移

    indexStack.pop_back();
    int parentIdx = indexStack.back();

    StepPtr parent = currentStep->parent;
    currentStep = parent->children[parentIdx];
}