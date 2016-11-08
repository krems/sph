#include <SFML/Graphics.hpp>
#include "graphics.h"


Graphics::Graphics(double xb, double yb, double xs, double ys) : BOUND_X(xb), BOUND_Y(yb), X_SIZE(xs), Y_SIZE(ys),
                                                                 window(sf::VideoMode((unsigned int) (X_SIZE),
                                                                                      (unsigned int) (Y_SIZE)),
                                                                        "Water under gravity") {}

void Graphics::draw_borders(sf::RenderWindow &window) const {
    sf::RectangleShape rectangle(sf::Vector2f((float) (BOUND_X - 2.), (float) (Y_SIZE)));
    rectangle.setPosition(0, 0);
    rectangle.setFillColor(sf::Color::Cyan);
    window.draw(rectangle);

    sf::RectangleShape rectangle1(sf::Vector2f((float) (X_SIZE), (float) (BOUND_Y - 2)));
    rectangle1.setPosition(0, 0);
    rectangle1.setFillColor(sf::Color::Cyan);
    window.draw(rectangle1);

    sf::RectangleShape rectangle2(sf::Vector2f((float) (X_SIZE), (float) BOUND_Y));
    rectangle2.setPosition(0, (float) (Y_SIZE - BOUND_Y + 2));
    rectangle2.setFillColor(sf::Color::Cyan);
    window.draw(rectangle2);

    sf::RectangleShape rectangle3(sf::Vector2f((float) (X_SIZE), (float) (Y_SIZE)));
    rectangle3.setPosition((float) (X_SIZE - BOUND_X + 2), (float) (BOUND_Y - 2));
    rectangle3.setFillColor(sf::Color::Cyan);
    window.draw(rectangle3);
}


void Graphics::buildShapeFromParticle(sf::CircleShape &shape, const Particle &p) const {
    shape.setRadius(5);
    shape.setOutlineThickness(0);
    shape.setPosition((float) (X_SIZE - p.coord.x), (float) (Y_SIZE - p.coord.y));
}

void Graphics::update_data(const std::vector<Particle *> *particles) {
    this->particles = particles;
}

bool Graphics::gui_main_loop() {
    sf::Event event;
    if (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            // ignore
        }
    }
    window.clear();

    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        sf::CircleShape shape;
        buildShapeFromParticle(shape, **it);
        window.draw(shape);
//                    if (DRAW_VELOCITIES) {
//                        draw_velocities(window, p);
//                    }
//                    if (DRAW_PRESSURE) {
//                        draw_pressure(window, p);
//                    }
    }
    draw_borders(window);
    window.display();
    return window.isOpen();
}
