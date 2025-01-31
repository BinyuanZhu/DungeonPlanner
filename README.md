# Dungeon Planner

A minimal D&D dungeon mapper in C++.

## Dependencies
- OpenGL 3.3
- GLFW3
- GLAD

## Build

Change the main shader paths in main.cpp and use absolute pathing. To build yourself, run:

```bash
cmake .
make
./DungeonPlanner
```

Note:
- Read CMakeLists.txt for more details on build.
- Assumes brew install of GLFW3 and GLM.

If you want, you can delete dependencies and move the include and lib contents to usr/local/include and usr/local/lib respectively. Just adjust the CMakeLists.txt file accordingly.
