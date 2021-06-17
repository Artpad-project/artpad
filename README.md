# Artpad

Artpad is a small image editing and video manipulation software.

## Getting Started

### Building the project
```
git clone git@github.com:Artpad-project/artpad.git
cd artpad
make build
```

All binary files are compiled into `bin/` at the root of the project. 

### Running the project

```
make artpad
./artpad
```

Run the program with the command `./artpad`

### Prerequisites

- gcc
- CMake
- gtk 3.0
- ffmpeg (video only)

## File Architecture

```
.
├── lib (required libraries)
├── include (header files)
├── test (test files and samples)
└── src
    ├── main.c
    ├── Makefile
    └── C source files
```

## Project Avancement Recap

### GUI

- [X] Image GUI
- [X] Layers
- [X] Undo / Redo
- [X] Load / Save


### Image processing

- [X] Saturation
- [X] Rotation
- [X] Magic Wand
- [X] Drawing 

### Video processing

- [X] Video loading/saving
- [X] Rotoscoping
- [X] Feature detection

### TODO

- [X] Video GUI
- [X] Video stabilization

## Authors

* [**AUBERT Tom**](https://github.com/TomAubertEpita)
* [**DESMARAIS Lowen**](https://github.com/Epita-work-Lowen)
* [**DUBOIN Léo**](https://github.com/d4ilyrun)
* [**LECOMTE Paul**](https://github.com/PaulLecomte)

## Website

- https://artpad-project.github.io/artpad-web
