#ifndef SPH_GRAPHICS_H
#define SPH_GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "../sph/sph.h"
#include <vector>

using std::vector;

class Graphics {
    const double BOUND_X;
    const double BOUND_Y;
    const double X_SIZE;
    const double Y_SIZE;
    sf::RenderWindow window;

    const std::vector<Particle*> *particles;
public:
    Graphics(double xb, double yb, double xs, double ys);

    void buildShapeFromParticle(sf::CircleShape &shape, const Particle &p) const;

    void update_data(const std::vector<Particle*> *particles);

    bool gui_main_loop();

    const sf::Color &getColor(const Particle &p) const;
};

#endif //SPH_GRAPHICS_H