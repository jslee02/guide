# GUIDE: Graphical User Interface for DART Engine [![Build Status](https://travis-ci.org/jslee02/guide.png?branch=master)](https://travis-ci.org/jslee02/guide)

> **Warning**: Guide is under heavy development.

Guide is my spare-time project that is a multibody dynamics simulator powered by [DART](http://dartsim.github.io/).

## Dependencies

* DART 5.1 for robot dynamics
* Qt 5 for GUI
* OpenSceneGraph for 3d rendering

## Installation from Source

Once the dependencies are installed, you can build Guide using CMake:

```
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
$ sudo make install
```

## License

[MIT License](https://opensource.org/licenses/MIT)

