# Seagull!
![seagull](https://images.pexels.com/photos/56618/seagull-sky-holiday-bird-56618.jpeg)
A "playground" for cellular automata. Initially just Conway's Game of Life (hence the name, C GoL...) \
In C++ with raw OpenGL :D

## Currently available controls:
- ESC: Closes the application;
- ENTER: Start/stop simulation;
- M1 / LEFT MOUSE: Make cell "alive" at cursor location;
- M2 / RIGHT MOUSE: "Kill" cell at cursor location;
- R: Randomly fills the grid with "live" cells;
- C: Clears the grid;
- 1: [Brian's Brain](https://en.wikipedia.org/wiki/Brian's_Brain) Automaton;
- 2: [Day & Night](https://en.wikipedia.org/wiki/Day_and_Night_(cellular_automaton)) Automaton;
- 3: Disease Automaton (a variation on Life);
- 4: [Live Free or Die](https://conwaylife.com/wiki/OCA:Live_Free_or_Die) Automaton;
- 5: [Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) Automaton;
    - P: Toggles plague mode (Only available when Life is active);
- 6: [Rule 90](https://en.wikipedia.org/wiki/Rule_90) Automaton;
- 7: [Seeds](https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)) Automaton;
- +: Step once through simulation*;

\* GLFW reads this as GLFW_KEY_EQUAL so it might change depending on your layout

## Build instructions -- REQUIRES CMAKE AND g++ COMPILER
Copy into your terminal while on the parent directory:

```sh
cmake -B build --fresh && cmake --build build
```

If everything goes as it's supposed to, you should find the built executable in the bin directory.
To run it:
```sh
./bin/cgol
```
