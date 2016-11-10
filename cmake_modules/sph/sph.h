#include <vector>
#include "../math/b2Math.h"
#include "Particle.h"
#ifndef SPH_SPH_H
#define SPH_SPH_H

using std::vector;


static const double pi = 3.1415;
static const double dt = 0.08;
static const double dt_sq = dt * dt;
static const double h = 100.;
static const double h_sq = h * h;
static const double rest_dist = 1.5;
static const double rest_press = 0.;
//static const double rest_press = 10.;
static const double gas_const = 8.344 * 300 * 10e8;
static const double mu_visc = .0;
//static const double mu_visc = .05;
static const double gravity = 10.;
static const double rest_rho = 1.;
//static const double rest_rho = 1.;

static const double max_rho = 10e-5;

class Kernel {
public:
    static double w(const Particle &subj, const Particle &neighbour) {
        double dist = subj.distance(neighbour);
        double x = dist / h;
        if (x >= 2) {
            return 0.;
        }
        if (x >= 1) {
            return 1. / 4. * (2 - x) * (2 - x) * (2 - x) / (pi * h * h * h);
        }
        return 1. - 3. / 2. * x * x + 3. / 4. * x * x * x / (pi * h * h * h);
    }

    static double dw(const Particle &subj, const Particle &neighbour) {
        double dist = subj.distance(neighbour);
        double x = dist / h;
        if (x >= 2) {
            return 0.;
        }
        if (x >= 1) {
            return - 3. / 4. * (2 - x) * (2 - x) / (pi * h * h * h * h);
        }
        return 9. / 4. * x * x - 3. * x / (pi * h * h * h * h);
    }

    static b2Vec2 gradw(const Particle &subj, const Particle &neighbour) {
        double dist = subj.distance(neighbour);
        double x = dist / h;
        if (x >= 2) {
            return b2Vec2(0., 0.);
        }
        if (x >= 1) {
            return b2Vec2(-3. * neighbour.coord.x * (2 - dist) * (2 - dist) / (4 * dist) / (pi * h * h * h),
                          -3. * neighbour.coord.y * (2 - dist) * (2 - dist) / (4 * dist)) / (pi * h * h * h);
        }
        return b2Vec2(-3. * neighbour.coord.x + 9. / 4. * dist / (pi * h * h * h),
                      -3. * neighbour.coord.y + 9. / 4. * dist / (pi * h * h * h));
    }

//    static double w_pressure(const Particle &subj, const Particle &neighbour) {
//        double dist_sq = subj.distance_squared(neighbour);
//        if (dist_sq >= h_sq || dist_sq == 0) {
//            return 0., 0.;
//        }
//        double mul = 15. / (pi * pow(h, 6)) * pow((h - subj.distance(neighbour)), 3);
//        return mul;
//    }
//
//    static b2Vec2 dw_pressure(const Particle &subj, const Particle &neighbour) {
//        double dist_sq = subj.distance_squared(neighbour);
//        if (dist_sq >= h_sq || dist_sq == 0) {
//            return b2Vec2(0.f, 0.f);
//        }
//        double dist = subj.distance(neighbour);
//        double mul = -15. * 3. / (pi * pow(h, 6)) * pow((h - dist), 2) / dist;
//        return (subj.coord - neighbour.coord) * mul;
//    }
//
//    static double ddw_visc(const Particle &subj, const Particle &neighbour) {
//        double dist_sq = subj.distance_squared(neighbour);
//        if (dist_sq >= h_sq) {
//            return 0.;
//        }
//        return 45. / (pi * pow(h, 6)) * (h - subj.distance(neighbour));
//    }
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
    void build_movable_particles(int particle_count, vector<Particle *> *_particles) const;

    void build_wall_particles(vector<Particle *> *pVector) const;

    Particle *calcNewParticleState(const vector<Particle *> *particles, const Particle &subj) const;
};

#endif //SPH_SPH_H