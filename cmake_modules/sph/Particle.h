#ifndef SPH_PTC_H
#define SPH_PTC_H

#include "../math/b2Math.h"

struct Particle {
    explicit Particle(const b2Vec2 &coord, const b2Vec2 &veloc, const b2Vec2 &imp, double rho) :
            coord(coord), veloc(veloc), imp(imp), rho(rho) {
        this->m = 1.;
    }

    explicit Particle(double x, double y, double vx, double vy, double rho) : coord(x, y), veloc(vx, vy), rho(rho) {
        this->m = 1.;
    }

    b2Vec2 coord;
    b2Vec2 veloc;
    b2Vec2 imp;
    double rho;
    double m;

    double distance_squared(const Particle &other) const {
        return b2DistanceSquared(this->coord, other.coord);
    }

    double distance(const Particle &other) const {
        return b2Distance(this->coord, other.coord);
    }
};
#endif //SPH_PTC_H