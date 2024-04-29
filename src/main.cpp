#include "simulation.hpp"

int main(int argc, char *argv[]) {
    Simulation simulation(argv[0]);

    simulation.init();

    simulation.run();

    return EXIT_SUCCESS;
}
