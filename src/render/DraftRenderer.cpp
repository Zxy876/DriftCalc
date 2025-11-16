#include "DraftRenderer.h"
#include <iostream>

namespace driftcalc
{

DraftRenderer::DraftRenderer()
{
}

void DraftRenderer::setWindow(sf::RenderWindow* win)
{
    window = win;
}

void DraftRenderer::beginFrame(int w, int h)
{
    // future NanoVG integration
    (void)w;
    (void)h;
}

void DraftRenderer::endFrame()
{
    // future NanoVG integration
}

void DraftRenderer::drawHandRect(float x, float y, float w, float h)
{
    if (!window) return;

    sf::RectangleShape rect({w, h});
    rect.setFillColor(sf::Color(200, 220, 255));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);
    rect.setPosition({x, y});

    window->draw(rect);
}

void DraftRenderer::drawHandText(float x, float y, const std::string& text)
{
    if (!window) return;

    static sf::Font font;
    static bool loaded = false;

    if (!loaded)
    {
        loaded = font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
        if (!loaded)
        {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }
    }

    // SFML3 constructor: Text(font, string, size)
    sf::Text t(font, text, 20);
    t.setFillColor(sf::Color::Black);
    t.setPosition({x, y});

    window->draw(t);
}

} // namespace driftcalc