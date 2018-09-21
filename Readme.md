# Roguelike game for stream

[![Build Status](https://travis-ci.org/YakPie/roguelike-for-stream.svg?branch=master)](https://travis-ci.org/YakPie/roguelike-for-stream)

This is a roguelike game being developed [live at Twitch](http://twitch.com/yakpie)!
Check out the development schedule at [our webpage](http://yakpie.com).

## Description

A traditional ASCII based Roguelike that you can play in the terminal. The catch is that the Roguelike game connects to the twitch chat and let's the viewers interact with the game. The viewers can spawn monsters or loot to either help the player or make it harder for them!

## Build & Run

The game is currently dependent on `ncurses`.

To build:

```
mkdir build
cd build
cmake ..
make
```

To run:

```
./main ../level1
```
