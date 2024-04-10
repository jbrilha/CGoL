#include "simulation.hpp"

int main(int argc, char *argv[]) {
    Simulation simulation(argv[0]);

    simulation.init();

    while(simulation.run());

    return EXIT_SUCCESS;
}
