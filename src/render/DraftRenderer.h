#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace driftcalc
{

class DraftRenderer
{
public:
    DraftRenderer();

    // Begin/End frame system
    void beginFrame(int w, int h);
    void endFrame();

    // Tell renderer which window to draw on
    void setWindow(sf::RenderWindow* win);

    // Drawing API
    void drawHandRect(float x, float y, float w, float h);
    void drawHandText(float x, float y, const std::string& text);

private:
    sf::RenderWindow* window = nullptr;
};

} // namespace driftcalc