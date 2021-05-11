# BSP Tree
A simple non-generic implementation of a BSP-Tree for polygonal models.

A more complete documentation can be found [here](docs/report/report.pdf).

## Requirements
The software requires the *glm* library to be installed. In Fedora:

	sudo dnf install glm-devel

## Provided example
### Bulid and run
Clone the repository:

	git clone https://github.com/GerardMT/BSP-Tree
	cd BSP-Tree

Compile the code:

	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ../
	make

Simply run:

	./bsp-tree

### Execution output
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

...
```