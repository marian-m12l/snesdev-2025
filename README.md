# SNES dynamic split-screen demo

A basic demo showing a working (although sub-optimal) dynamic split-screen (a.k.a. Voronoi split-screen). When the players are close enough, they share a single screen. Once they get far apart, a variable-slope split separates players depending on their relative positions.


Unfortunately, this is not a game as there's nothing to do except roam the map...


Developed in C (and a bit of assembly) with PVSnesLib using the assets from graphics examples (https://github.com/alekmaul/pvsneslib/tree/master/snes-examples/graphics​) during SNESDEV 2025 (https://itch.io/jam/snesdev-2025)


This ROM runs on original hardware (tested on [my homemade flashcart](https://github.com/marian-m12l/pico-snes-cartridge) :-)).
