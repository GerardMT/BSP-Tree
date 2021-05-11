
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "bsp_tree.h"

using namespace std;

const unsigned int RANDOM_POLYGONS_SIZE = 1000;
const unsigned int EXPERIMENT_ITERATIONS = 10;

struct ply_format_exception : public std::exception {
    const char *what() const throw() {
        return "Incorrect ply format";
    }
};

/**
 * Load the polygons of a model. Only supports ASCII PLY file format.
 * @param filename location of the file to load.
 * @param polygons loaded polygons. Output parameter.
 */
void load_ply(const string &filename, vector<bsp_tree::polygon> &polygons) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw ply_format_exception();
    }

    string line;
    if (!getline(file, line)) {
        throw ply_format_exception();
    }

    if (line != "ply") {
        throw ply_format_exception();
    }

    unsigned int vertices_size;
    unsigned int faces_size;

    bool header = true;
    while (header && getline(file, line)) {
        string s;
        istringstream iss(line);

        if (!(iss >> s)) {
            throw ply_format_exception();
        }

        if (s == "format") {
            if (!(iss >> s)) {
                throw ply_format_exception();
            }

            if (s != "ascii") {
                throw ply_format_exception();
            }
        } else if (s == "element") {
            unsigned int n;
            if (!(iss >> s >> n)) {
                throw ply_format_exception();
            }

            if (s == "vertex") {
                vertices_size = n;
            } else if (s == "face") {
                faces_size = n;
            }
        } else if (s == "end_header") {
            header = false;
        }
    }

    vector<glm::vec3> vertices;
    vertices.resize(vertices_size);

    for (unsigned int i = 0; i < vertices_size; ++i) {
        if (!(file >> vertices[i].x >> vertices[i].y >> vertices[i].z)) {
            throw ply_format_exception();
        }
    }

    struct ply_polygon {
        unsigned int a, b, c;
    };

    vector<ply_polygon> faces;
    faces.resize(faces_size);

    for (unsigned int i = 0; i < faces_size; ++i) {
        unsigned int n;

        if (!(file >> n >> faces[i].a >> faces[i].b >> faces[i].c)) {
            throw ply_format_exception();
        }

        if (n != 3) {
            throw ply_format_exception();
        }
    }

    polygons.resize(faces_size);

    for (unsigned int i = 0; i < faces_size; ++i) {
        polygons[i].p[0] = vertices[faces[i].a];
        polygons[i].p[1] = vertices[faces[i].b];
        polygons[i].p[2] = vertices[faces[i].c];
    }
}

/**
 * Generate a BSP tree and check that the number of fragments (plane fragments) is smaller than the upper bound (number of polygons)^2.
 * @param polygons polygons to generate de BSP tree.
 */
void experiment(const vector<bsp_tree::polygon> &polygons) {
    bsp_tree tree;
    tree.construct(polygons);

    unsigned int fragments = tree.fragments();
    unsigned int polygons_size_2 = polygons.size() * polygons.size();
    float percentage = static_cast<float>(fragments) / static_cast<float>(polygons_size_2) * 100.0f;

    cout << "Polygons: " << polygons.size() << "\n";
    cout << "Nodes: " << tree.nodes() << "\n";
    cout << "Fragments: " << fragments << "\n";
    cout << "Fragments (" << fragments << ") smaller than polygons^2 (" << polygons_size_2 << "): " << (fragments < polygons_size_2 ? "true" : "false") << " (" << setprecision(3) << fixed <<  percentage << "%)" << "\n";
}

/**
 * Generate a random coordinate between 0 and 100.
 * @return coordinate.
 */
float rand_coord() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
}

int main() {
    cout << "POLYGONS FROM MODEL" << "\n";
    cout << "===================" << "\n";
    vector<bsp_tree::polygon> polygons;

    cout << "Monkey" << "\n";
    load_ply("../res/models/monkey.ply", polygons);
    experiment(polygons);

    polygons.clear();

    cout << "\n";
    cout << "Sphere" << "\n";
    load_ply("../res/models/dragon.ply", polygons);
    experiment(polygons);

    polygons.clear();

    cout << "\n";
    cout << "Teapod" << "\n";
    load_ply("../res/models/teapot.ply", polygons);
    experiment(polygons);

    cout << "\n";
    cout << "RANDOM POLYGONS" << "\n";
    cout << "===============" << "\n";

    for (unsigned int k = 0; k < EXPERIMENT_ITERATIONS; ++k) {
        polygons.resize(RANDOM_POLYGONS_SIZE);

        // Generate RANDOM_POLYGONS_SIZE polygons
        for (unsigned int i = 0; i < RANDOM_POLYGONS_SIZE; ++i) {
            for (char j = 0; j < 3; ++j) {
                polygons[i].p[j].x = rand_coord();
                polygons[i].p[j].y = rand_coord();
                polygons[i].p[j].z = rand_coord();
            }
        }

        cout << "\n";
        cout << "Experiment " << k << "\n";
        experiment(polygons);
    }

    cout << flush;

    return 0;
}