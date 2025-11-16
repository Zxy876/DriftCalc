#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

#include "exprtk.hpp"
#include "expression/ExpressionEngine.h"
#include "steps/StepTreeBuilder.h"
#include "render/DraftRenderer.h"

int main() 
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "DriftCalc");

    using namespace driftcalc::expr;
    using namespace driftcalc::steps;

    // ===== 1. 测试表达式 =====
    const std::string expr_str = "(3+5)/2^2 - 1";

    ExpressionEngine engine;
    ExprNodePtr ast = engine.parse(expr_str);
    double ourResult = ast->evaluate();
    std::cout << "[ExpressionEngine] " << expr_str << " = " << ourResult << std::endl;

    // ===== 2. exprtk 对照 =====
    using expression_t = exprtk::expression<double>;
    using parser_t     = exprtk::parser<double>;

    expression_t expression;
    parser_t parser;
    parser.compile(expr_str, expression);
    std::cout << "[exprtk]          " << expr_str << " = " << expression.value() << std::endl;

    // ===== 3. 生成 StepTree =====
    StepTreeBuilder builder;
    StepTree stepTree = builder.build(*ast);
    StepPtr rootStep  = stepTree.root;

    std::string stepText = 
        "Step: " + rootStep->description + 
        " | Result: " + rootStep->exprAfter;

    // ===== 4. 初始化草稿渲染器 =====
    driftcalc::DraftRenderer draft;
    draft.setWindow(&window);   // ★ 绑定 SFML 窗口，一次就够

    // ========== 主循环 ==========
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        // 现在 begin/end 只是占位，后面可以塞动画逻辑
        draft.beginFrame(window.getSize().x, window.getSize().y);

        draft.drawHandRect(40.f, 80.f, 600.f, 80.f);
        draft.drawHandText(60.f, 120.f, stepText);

        draft.endFrame();

        window.display();
    }

    return 0;
}