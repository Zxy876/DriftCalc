#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

#include "exprtk.hpp"
#include "expression/ExpressionEngine.h"
#include "steps/StepTreeBuilder.h"
#include "steps/StepNavigator.h"
#include "render/DraftRenderer.h"

int main() 
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "DriftCalc");

    using namespace driftcalc::expr;
    using namespace driftcalc::steps;

    // ===== 测试表达式 =====
    const std::string expr_str = "(3+5)/2^2 - 1";

    ExpressionEngine engine;
    ExprNodePtr ast = engine.parse(expr_str);

    // ===== 构建 StepTree =====
    StepTreeBuilder builder;
    StepTree stepTree = builder.build(*ast);
    StepPtr rootStep  = stepTree.root;

    // ===== 创建 StepNavigator（导航系统）=====
    StepNavigator navigator(rootStep);

    // ===== 草稿渲染器 =====
    driftcalc::DraftRenderer draft;
    draft.setWindow(&window);

    // ========== 主循环 ==========
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();

            // ———— 方向键导航 ————
            if (const auto* key = ev->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Left)  navigator.moveLeft();
                if (key->code == sf::Keyboard::Key::Right) navigator.moveRight();
                if (key->code == sf::Keyboard::Key::Up)    navigator.moveUp();
                if (key->code == sf::Keyboard::Key::Down)  navigator.moveDown();
            }
        }

        window.clear(sf::Color::White);

        draft.beginFrame(window.getSize().x, window.getSize().y);

        // ———— 动态根据当前步骤渲染 ————
        StepPtr cur = navigator.current();
        std::string stepText =
            "Step: " + cur->description + 
            " | Result: " + cur->exprAfter;

        draft.drawHandRect(40.f, 80.f, 600.f, 80.f);
        draft.drawHandText(60.f, 120.f, stepText);

        draft.endFrame();
        window.display();
    }

    return 0;
}