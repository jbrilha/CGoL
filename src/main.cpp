#include "simulation.hpp"

int main(int argc, char *argv[]) {
    Simulation simulation;

    simulation.init();

    while(simulation.run());

    return EXIT_SUCCESS;
}
