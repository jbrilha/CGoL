#include "simulation.hpp"

int main(int argc, char *argv[]) {
    Simulation simulation;

    simulation.init_life();

    while(simulation.run());

    return EXIT_SUCCESS;
}
