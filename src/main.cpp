#include <SFML/Graphics.hpp>
#include "exprtk.hpp"
#include "expression/ExpressionEngine.h"

#include <iostream>
#include <optional>

int main() {

    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "DriftCalc Starter");

    using driftcalc::expr::ExpressionEngine;

    // ===== 1. 用我们自己的 ExpressionEngine 解析 & 计算 =====
    const std::string expr_str = "(3+5)/2^2 - 1";

    ExpressionEngine engine;
    auto ast = engine.parse(expr_str);
    double ourResult = ast->evaluate();

    std::cout << "[ExpressionEngine] " << expr_str << " = " << ourResult << std::endl;

    // ===== 2. 用 exprtk 计算对比一下 =====
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double>     parser_t;

    expression_t expression;
    parser_t     parser;

    if (!parser.compile(expr_str, expression)) {
        std::cerr << "exprtk parse error!" << std::endl;
        return -1;
    }

    std::cout << "[exprtk]          " << expr_str << " = " << expression.value() << std::endl;

    // ===== SFML 画个矩形，确认渲染没问题 =====
    sf::RectangleShape rect({200.f, 120.f});
    rect.setPosition({100.f, 100.f});
    rect.setFillColor(sf::Color(180, 180, 180));

    while (window.isOpen()) {

        // 新版 SFML3 事件循环
        while (const std::optional<sf::Event> ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(rect);
        window.display();
    }

    return 0;
}