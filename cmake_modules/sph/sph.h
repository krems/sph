#include <vector>
#include "../math/b2Math.h"
#include "Particle.h"
#ifndef SPH_SPH_H
#define SPH_SPH_H

using std::vector;


static const double pi = 3.1415;
static const double dt = 0.1;
static const double dt_sq = dt * dt;
static const double h = 100.;
static const double h_sq = h * h;
static const double rest_dist = 1.5;
static const double rest_press = 10.;
static const double gas_const = 8.344 * 300 * 10e8;
static const double mu_visc = .05;
static const double gravity = 10.;
static const double rest_rho = 1.;

static const double max_rho = 10e-5;

class Kernel {
public:
    static double w_rho(const Particle &subj, const Particle &neighbour) {
        double dist_sq = subj.distance_squared(neighbour);
        if (dist_sq >= h_sq) {
            return 0.;
        }
        return 315. / (64. * pi * pow(h, 9)) * pow((h_sq - dist_sq), 3);
    }

    static double w_pressure(const Particle &subj, const Particle &neighbour) {
        double dist_sq = subj.distance_squared(neighbour);
        if (dist_sq >= h_sq || dist_sq == 0) {
            return 0., 0.;
        }
        double mul = 15. / (pi * pow(h, 6)) * pow((h - subj.distance(neighbour)), 3);
        return mul;
    }

    static b2Vec2 dw_pressure(const Particle &subj, const Particle &neighbour) {
        double dist_sq = subj.distance_squared(neighbour);
        if (dist_sq >= h_sq || dist_sq == 0) {
            return b2Vec2(0.f, 0.f);
        }
        double dist = subj.distance(neighbour);
        double mul = -15. * 3. / (pi * pow(h, 6)) * pow((h - dist), 2) / dist;
        return (subj.coord - neighbour.coord) * mul;
    }

    static double ddw_visc(const Particle &subj, const Particle &neighbour) {
        double dist_sq = subj.distance_squared(neighbour);
        if (dist_sq >= h_sq) {
            return 0.;
        }
        return 45. / (pi * pow(h, 6)) * (h - subj.distance(neighbour));
    }
};


class SPH {
    const double BOUND_X;
    const double BOUND_Y;
    const double X_SIZE;
    const double Y_SIZE;
public:
    SPH(double xb, double yb, double xs, double ys);

    void compute_next_state(const vector<Particle*> *particles, vector<Particle*> *result) const;

    vector<Particle*> *generate_particles(int particle_count) const;

private:
    double xwall_pressure(const Particle &subj) const {
        return 0.;
    }

    double ywall_pressure(const Particle &subj) const {
        return 0.;
    }

    void build_movable_particles(int particle_count, vector<Particle *> *_particles) const;

    void build_wall_particles(vector<Particle *> *pVector) const;
};

#endif //SPH_SPH_H