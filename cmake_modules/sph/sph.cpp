#include <vector>
#include <future>
#include <algorithm>
#include "sph.h"

using std::vector;

SPH::SPH(double xb, double yb, double xs, double ys) : BOUND_X(xb), BOUND_Y(yb), X_SIZE(xs), Y_SIZE(ys) {}

bool compare_x(Particle* lhs, Particle* rhs) {
    return lhs->coord.x < rhs->coord.x;
}

void SPH::compute_next_state(vector<Particle *> *particles, vector<Particle *> *result) const {
    std::sort(particles->begin(), particles->end(), compare_x);
//    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
//        const Particle &n = **it;
//        printf("%f ", n.coord.x);
//    }
    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        const Particle &p = **it;
        Particle *newParticle = calcNewParticleState(particles, p);
        result->push_back(newParticle);
    }
//    vector<std::future<Particle*>> futures;
//    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
//        const Particle &p = **it;
//        futures.push_back(std::async(std::launch::async, [&particles, &p, this] (){ return calcNewParticleState(particles, p);}));
//    }
//    for (auto &f : futures) {
//        f.wait();
//        result->push_back(f.get());
//    }
}

Particle *SPH::calcNewParticleState(const vector<Particle *> *particles, const Particle &subj) const {
    if (!subj.is_movable()) {
        return new Particle(subj);
    }
    b2Vec2 ext_forces(0, -gravity);
    double drho = 0.;
    b2Vec2 f_pressure(0.f, 0.f);
    b2Vec2 visc(0.f, 0.f);

    double subj_press = rest_press + gas_const * (subj.rho - rest_rho);
    for (auto it = particles->cbegin(); it != particles->cend(); ++it) {
        const Particle &n = **it;
        if (&n == &subj) {
            continue;
        }
        if (subj.coord.x + 2 * h < n.coord.x) {
            break;
        }
        if (subj.distance_squared(n) >= 4 * h_sq) {
            continue;
        }

        drho -= n.m * b2Dot((subj.veloc - n.veloc), Kernel::gradw(subj, n));

        double n_press = rest_press + gas_const * (n.rho - rest_rho);
        f_pressure -= Kernel::gradw(subj, n) * n.m * (subj_press / (subj.rho * subj.rho) + n_press / (n.rho * n.rho));

        double dd_visc_ker = Kernel::dw(subj, n);
        visc += (n.veloc - subj.veloc) * n.m * dd_visc_ker / n.rho;
    }
    visc *= mu_visc;

    double rho = subj.rho + dt * drho;
    b2Vec2 acceleration = visc + f_pressure + ext_forces;
    b2Vec2 nveloc = subj.veloc + acceleration * dt;
    b2Vec2 ncoord = subj.coord + dt * (nveloc + acceleration * dt / 2.);

    Particle *new_particle = new Particle(ncoord, nveloc, rho);
    return new_particle;
}

vector<Particle *> *SPH::generate_particles(int particle_count) const {
    vector<Particle *> *_particles = new vector<Particle *>();
    build_wall_particles(_particles);
    build_movable_particles(particle_count, _particles);
    return _particles;
}

void SPH::build_movable_particles(int particle_count, vector<Particle *> *_particles) const {
    double xstep = 20.9;
    double ystep = 20.7;
    double X1_WALL = BOUND_X + 200;
    double x = BOUND_X + 20;
    double y = BOUND_Y + 20;
    for (int i = 0; i < particle_count; ++i) {
        if (x >= X1_WALL - 20) {
            x = BOUND_X + 20;
            y += ystep;
        }
        Particle *particle = new Particle(x, y, 0., 0., 1.);
        _particles->push_back(particle);
        x += xstep;
    }
}

void SPH::build_wall_particles(vector<Particle *> *_particles) const {
    int layers = 10;
    double step = 10.5;
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
