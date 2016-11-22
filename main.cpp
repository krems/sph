#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "cmake_modules/sph/sph.h"
#include "cmake_modules/graph/graphics.h"

#ifndef BORDER_X
#define BORDER_X 130.
#endif

#ifndef BORDER_Y
#define BORDER_Y 20.
#endif

#ifndef Y_SIZE
#define Y_SIZE 480. - BORDER_Y
#endif

#ifndef X_SIZE
#define X_SIZE 640. - BORDER_X
#endif

using std::vector;

int main(int argc, char *argv[]) {
    SPH sph(BORDER_X, BORDER_Y, X_SIZE, Y_SIZE);
    Graphics grp(BORDER_X, BORDER_Y, X_SIZE, Y_SIZE);
    vector<Particle*> *particles = sph.generate_particles(8);
    vector<Particle*> *particles2 = new vector<Particle*>();
    grp.update_data(particles);
    while (true) {
        sph.compute_next_state(particles, particles2);
        grp.update_data(particles2);
        bool continuee = grp.gui_main_loop();
        for (Particle* p : *particles) {
            delete p;
        }
        particles->clear();
        std::swap(particles, particles2);
        if (!continuee) {
            break;
        }
        usleep(16);
    }
    delete particles;
    for (Particle* p : *particles2) {
        delete p;
    }
    delete particles2;
    return 0;
}
