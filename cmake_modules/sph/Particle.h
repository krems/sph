#ifndef SPH_PTC_H
#define SPH_PTC_H

#include "../math/b2Math.h"

struct Particle {
    enum Type {
        Movable,
        Wall,
    };

    explicit Particle(const b2Vec2 &coord, const b2Vec2 &veloc, double rho) :
            coord(coord), veloc(veloc), rho(rho) {
        this->m = 1.;
        this->type = Movable;
    }

    explicit Particle(double x, double y, double vx, double vy, double rho) : coord(x, y), veloc(vx, vy), rho(rho) {
        this->m = 1.;
        this->type = Movable;
    }

    explicit Particle(double x, double y) : coord(x, y), veloc(0, 0), rho(1.) {
        this->m = 1.;
        this->type = Wall;
    }

    explicit Particle(const b2Vec2 &coord) : coord(coord), veloc(0, 0), rho(1.) {
        this->m = 1.;
        this->type = Wall;
    }

    b2Vec2 coord;
    b2Vec2 veloc;
    double rho;
    double m;
    Type type;

    double distance_squared(const Particle &other) const {
        return b2DistanceSquared(this->coord, other.coord);
    }

    double distance(const Particle &other) const {
        return b2Distance(this->coord, other.coord);
    }

    bool is_movable() {
        return type == Movable;
    }
};
#endif //SPH_PTC_H