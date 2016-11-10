#include <vector>
#include "sph.h"

using std::vector;

SPH::SPH(double xb, double yb, double xs, double ys) : BOUND_X(xb), BOUND_Y(yb), X_SIZE(xs), Y_SIZE(ys) {}

void SPH::compute_next_state(const vector<Particle *> *particles, vector<Particle *> *result) const {
    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        Particle *new_particle = calcNewParticleState(particles, **it);
        result->push_back(new_particle);
    }
}

Particle *SPH::calcNewParticleState(const vector<Particle *> *particles, const Particle &subj) const {
    if (subj.type == Particle::Wall) {
        return new Particle(subj);
    }
    double drho = 1.;
    b2Vec2 f_pressure(0.f, 0.f);
    b2Vec2 visc(0.f, 0.f);
    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        const Particle &n = **it;
        if (&n == &subj || subj.distance_squared(n) >= h) {
            continue;
        }

        drho += n.m * Kernel::w_rho(subj, n);

        double subj_press = rest_press + gas_const * (subj.rho - rest_rho);
        double n_press = rest_press + gas_const * (n.rho - rest_rho);
        f_pressure -= Kernel::dw_pressure(subj, n) * n.m * (subj_press + n_press) / (2. * n.rho);

        double dd_visc_ker = Kernel::ddw_visc(subj, n);
        visc += (n.veloc - subj.veloc) * n.m * dd_visc_ker / n.rho;
    }
    visc *= mu_visc;

//    double rho = subj.rho + dt * drho;
    b2Vec2 ext_forces(0, -gravity);
    b2Vec2 acceleration = visc + f_pressure + ext_forces;
    b2Vec2 nveloc(subj.veloc + acceleration * dt);
    b2Vec2 ncoord(subj.coord + dt * (nveloc + acceleration * dt / 2.));

    Particle *new_particle = new Particle(ncoord, nveloc, drho);
    return new_particle;
}

vector<Particle *> *SPH::generate_particles(int particle_count) const {
    vector<Particle *> *_particles = new vector<Particle *>();
    build_wall_particles(_particles);
    build_movable_particles(particle_count, _particles);
    return _particles;
}

void SPH::build_movable_particles(int particle_count, vector<Particle *> *_particles) const {
    double xstep = 5.9;
    double ystep = 5.7;
    double X1_WALL = BOUND_X + 200;
    double x = BOUND_X + 2;
    double y = BOUND_Y + 2;
    for (int i = 0; i < particle_count; ++i) {
        if (x >= X1_WALL - 2) {
            x = BOUND_X + 2;
            y += ystep;
        }
        Particle *particle = new Particle(x, y, 0., 0., 1.);
        _particles->push_back(particle);
        x += xstep;
    }
}

void SPH::build_wall_particles(vector<Particle *> *_particles) const {
    int layers = 5;
    double step = 0.5;
    double x = BOUND_X;
    double y = BOUND_Y;
    for (int i = 0; i < layers; ++i) {
        while (x <= X_SIZE) {
            Particle *particle = new Particle(x, y);
            _particles->push_back(particle);
            x += step;
        }
        y += step;
    }
    x = BOUND_X;
    y = Y_SIZE;
    for (int i = 0; i < layers; ++i) {
        while (x <= X_SIZE) {
            Particle *particle = new Particle(x, y);
            _particles->push_back(particle);
            x += step;
        }
        y += step;
    }
    x = BOUND_X;
    y = BOUND_Y + 1;
    for (int i = 0; i < layers; ++i) {
        while (y < Y_SIZE) {
            Particle *particle = new Particle(x, y);
            _particles->push_back(particle);
            y += step;
        }
        x += step;
    }
    x = X_SIZE;
    y = BOUND_Y + 1;
    for (int i = 0; i < layers; ++i) {
        while (y < Y_SIZE) {
            Particle *particle = new Particle(x, y);
            _particles->push_back(particle);
            y += step;
        }
        x += step;
    }
}
