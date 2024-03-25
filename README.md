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
- B: [Brian's Brain](https://en.wikipedia.org/wiki/Brians_Brain) Automaton;
- S: [Seeds](https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)) Automaton;
- L: [Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) Automaton;
- P: Toggles plague mode (Only available when Life is active);
- D: Disease Automaton (a variation on Life);
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
