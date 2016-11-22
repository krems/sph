#include <SFML/Graphics.hpp>
#include "graphics.h"


#define DRAW_VELOCITIES true

Graphics::Graphics(double xb, double yb, double xs, double ys) : BOUND_X(xb), BOUND_Y(yb), X_SIZE(xs), Y_SIZE(ys),
                                                                 window(sf::VideoMode((unsigned int) (X_SIZE),
                                                                                      (unsigned int) (Y_SIZE)),
                                                                        "Water under gravity") {}


void Graphics::buildShapeFromParticle(sf::CircleShape &shape, const Particle &p) const {
    shape.setRadius(4);
    shape.setOutlineThickness(1);
    shape.setPosition((float) (X_SIZE - p.coord.x), (float) (Y_SIZE - p.coord.y));
    shape.setOutlineColor(getColor(p));
    shape.setFillColor(sf::Color(*((unsigned int*) &p.rho)));
}

const sf::Color &Graphics::getColor(const Particle &p) const {
    if (p.is_movable()) {
        return sf::Color::Blue;
    } else {
        return sf::Color::Magenta;
    }
}

void Graphics::update_data(const vector<Particle *> *particles) {
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
        if (DRAW_VELOCITIES) {
            draw_velocities(**it);
        }
    }
    window.display();
    return window.isOpen();
}

void Graphics::draw_velocities(const Particle &p) {
    if (!p.is_movable()) {
        return;
    }
    const b2Vec2 &endPoint = p.veloc + p.coord;
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(X_SIZE - p.coord.x, Y_SIZE - p.coord.y)),
                    sf::Vertex(sf::Vector2f(X_SIZE - endPoint.x, Y_SIZE - endPoint.y))
            };

    window.draw(line, 2, sf::Lines);
}
