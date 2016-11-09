#include <vector>
#include "sph.h"

using std::vector;

SPH::SPH(double xb, double yb, double xs, double ys) : BOUND_X(xb), BOUND_Y(yb), X_SIZE(xs), Y_SIZE(ys) {}

void SPH::compute_next_state(const vector<Particle*> *particles, vector<Particle*> *result) const {
    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        const Particle &subj = **it;
        if (subj.type == Particle::Wall) {
            Particle* new_particle = new Particle(subj);
            result->push_back(new_particle);
            continue;
        }
        double rho = 1.;
        b2Vec2 dp(0.f, 0.f);
        b2Vec2 visc(0.f, 0.f);
        for (auto inrit = particles->cbegin(); inrit != particles->cend(); ++inrit) {
            const Particle& n = **inrit;
            if (&n != &subj && subj.distance_squared(n) < h) {
                rho += n.m * Kernel::w_rho(subj, n);

                double subj_press = rest_press + gas_const * (subj.rho - rest_rho);
                double n_press = rest_press + gas_const * (n.rho - rest_rho);
                double common_pressure = n.m * (subj_press + n_press) / (2. * subj.rho);
                b2Vec2 dpress_ker = Kernel::dw_pressure(subj, n);
                dp += dpress_ker * common_pressure;

                double dd_visc_ker = Kernel::ddw_visc(subj, n);
                visc += (n.veloc - subj.veloc) * n.m * dd_visc_ker / n.rho;
            }
        }
        visc *= mu_visc;

        b2Vec2 ext_forces(xwall_pressure(subj), -gravity + ywall_pressure(subj));
        b2Vec2 acceleration((visc - dp + (ext_forces)) / rho);
        b2Vec2 nveloc(subj.veloc + (acceleration) * dt);
        b2Vec2 ncoord(subj.coord + dt * (nveloc + acceleration * dt / 2.));

        Particle* new_particle = new Particle(ncoord, nveloc, dp, rho);
        result->push_back(new_particle);
    }
}

vector<Particle*> *SPH::generate_particles(int particle_count) const {
    vector<Particle*> *_particles = new vector<Particle*>();
    build_wall_particles(_particles);
    build_movable_particles(particle_count, _particles);
    return _particles;
}

void SPH::build_movable_particles(int particle_count, vector<Particle *> *_particles) const {
    double X1_WALL = BOUND_X + 200;
    double x = BOUND_X + 2;
    double y = BOUND_Y + 2;
    for (int i = 0; i < particle_count; ++i) {
        if (x >= X1_WALL - 2) {
            x = BOUND_X + 2;
            y += 3;
        }
        Particle* particle = new Particle(x, y, 0., 0., 1.);
        _particles->push_back(particle);
        x += 5;
    }
}

void SPH::build_wall_particles(vector<Particle *> *_particles) const {
    double x = BOUND_X;
    double y = BOUND_Y;
    while (x <= X_SIZE) {
        Particle* particle = new Particle(x, y);
        _particles->push_back(particle);
        ++x;
    }
    x = BOUND_X;
    y = Y_SIZE;
    while (x <= X_SIZE) {
        Particle* particle = new Particle(x, y);
        _particles->push_back(particle);
        ++x;
    }
    x = BOUND_X;
    y = BOUND_Y + 1;
    while (y < Y_SIZE) {
        Particle* particle = new Particle(x, y);
        _particles->push_back(particle);
        ++y;
    }
    x = X_SIZE;
    y = BOUND_Y + 1;
    while (y < Y_SIZE) {
        Particle* particle = new Particle(x, y);
        _particles->push_back(particle);
        ++y;
    }
}
