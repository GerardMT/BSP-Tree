# BSP Tree

A non-generic implementation of a BSP-Tree.

A more complete documentation can be found [here](docs/report/report.pdf).
The report can be found in:
./doc/main.pdf

## Features
-

## Requirements
The software requires the *glm* library to be installed. In Fedora:

	sudo dnf install glm-devel

## Build
Clone the repository:

	git clone https://github.com/GerardMT/BSP-Tree
	cd Cloth

Compile the code:

	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ../
	make

## Run
Simply run:

	./bsp-tree

## Exectuion example
```
POLYGONS FROM MODEL
===================
Monkey
Polygons: 62976
Nodes: 134333
Fragments: 165553
Fragments (165553) smaller than polygons^2 (3965976576): true (0.004%)

Sphere
Polygons: 201031
Nodes: 360565
Fragments: 485805
Fragments (485805) smaller than polygons^2 (1758757297): true (0.028%)

Teapod
Polygons: 13536
Nodes: 22262
Fragments: 30469
Fragments (30469) smaller than polygons^2 (183223296): true (0.017%)

RANDOM POLYGONS
===============

Experiment 0
Polygons: 1000
Nodes: 17486
Fragments: 20872
Fragments (20872) smaller than polygons^2 (1000000): true (2.087%)

Experiment 1
Polygons: 1000
Nodes: 12409
Fragments: 14724
Fragments (14724) smaller than polygons^2 (1000000): true (1.472%)

Experiment 2
Polygons: 1000
Nodes: 14879
Fragments: 17648
Fragments (17648) smaller than polygons^2 (1000000): true (1.765%)

Experiment 3
Polygons: 1000
Nodes: 13115
Fragments: 15602
Fragments (15602) smaller than polygons^2 (1000000): true (1.560%)

Experiment 4
Polygons: 1000
Nodes: 13851
Fragments: 16517
Fragments (16517) smaller than polygons^2 (1000000): true (1.652%)

Experiment 5
Polygons: 1000
Nodes: 13045
Fragments: 15455
Fragments (15455) smaller than polygons^2 (1000000): true (1.546%)

Experiment 6
Polygons: 1000
Nodes: 13393
Fragments: 15859
Fragments (15859) smaller than polygons^2 (1000000): true (1.586%)

Experiment 7
Polygons: 1000
Nodes: 13299
Fragments: 15774
Fragments (15774) smaller than polygons^2 (1000000): true (1.577%)

Experiment 8
Polygons: 1000
Nodes: 14359
Fragments: 17007
Fragments (17007) smaller than polygons^2 (1000000): true (1.701%)

Experiment 9
Polygons: 1000
Nodes: 11809
Fragments: 13880
Fragments (13880) smaller than polygons^2 (1000000): true (1.388%)
```