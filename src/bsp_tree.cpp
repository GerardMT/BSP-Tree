
#include "bsp_tree.h"

#include <cstdlib>

unsigned int bsp_tree::polygon_index(const vector<polygon> &polygons) const {
    return rand() % polygons.size();
}

void bsp_tree::construct(const vector<polygon> &polygons) {
    if (polygons.empty()) {
        return;
    }

    fragments_ = 0;

    root_ = new node;
    nodes_ = 1;
    construct_rec(polygons, root_);
}

void bsp_tree::construct_rec(const vector<polygon> &polygons, node *n) {
    unsigned int pol_i = polygon_index(polygons);
    n->pols.push_back(polygons[pol_i]);

    plane pl;
    to_plane(polygons[pol_i], pl);

    vector<polygon> polygons_front;
    vector<polygon> polygons_back;

    for (unsigned int i = 0; i < polygons.size(); ++i) {
        if (i != pol_i) {
            switch (distance(pl, polygons[i])) {
                case ON:
                    n->pols.push_back(polygons[i]);
                    break;
                case FRONT:
                    polygons_front.push_back(polygons[i]);
                    break;

                case BACK:
                    polygons_back.push_back(polygons[i]);
                    break;

                case HALF:
                    polygon_split(pl, polygons[i], polygons_front, polygons_back);
                    break;
            }
        }
    }

    fragments_ += n->pols.size();

    if (!polygons_front.empty()) {
        n->r = new node;
        ++nodes_;
        construct_rec(polygons_front, n->r);
    } else {
        n->r = nullptr;
    }

    if (!polygons_back.empty()) {
        n->l = new node;
        ++nodes_;
        construct_rec(polygons_back, n->l);
    } else {
        n->l = nullptr;
    }
}

void bsp_tree::to_plane(const bsp_tree::polygon &pol, bsp_tree::plane &pl) const {
    glm::vec3 u = pol.p[1] - pol.p[0];
    glm::vec3 v = pol.p[2] - pol.p[0];

    glm::vec3 r = glm::cross(u, v);
    pl.x = r.x;
    pl.y = r.y;
    pl.z = r.z;
    pl.w = -glm::dot(pl.xyz(), pol.p[0]);
}

bsp_tree::dist_res bsp_tree::distance(const bsp_tree::plane &pl, const bsp_tree::polygon &pol) const {
    float d1 = glm::dot(pl, glm::vec4(pol.p[0], 1));
    float d2 = glm::dot(pl, glm::vec4(pol.p[1], 1));

    if (d1 < 0 && d2 > 0) {
        return bsp_tree::HALF;
    } else {
        float d3 = glm::dot(pl, glm::vec4(pol.p[2], 1));

        if (d3 < 0) {
            return bsp_tree::BACK;
        } else if (d3 > 0) {
            return bsp_tree::FRONT;
        } else if (d1 == 0 && d2 == 0 && d3 == 0) {
            return bsp_tree::ON;
        }
    }
}

void bsp_tree::plane_segment_intersection(const bsp_tree::plane &pl, const glm::vec3 &a, const glm::vec3 &b, glm::vec3 &i) const {
    float t = glm::dot(pl, glm::vec4(a, 1)) / glm::dot(pl.xyz(), b - a);
    i = a + t * (a - b);
}

void bsp_tree::polygon_split_aux(const bsp_tree::plane &pl, const glm::vec3 &a, const glm::vec3 &b1, const glm::vec3 &b2, vector<bsp_tree::polygon> &polygons_a, vector<bsp_tree::polygon> &polygons_b) const {
    glm::vec3 i_ab1;
    plane_segment_intersection(pl, a, b1, i_ab1);

    glm::vec3 i_ab2;
    plane_segment_intersection(pl, a, b2, i_ab2);

    polygon p_a;
    p_a.p[0] = a;
    p_a.p[1] = i_ab1;
    p_a.p[2] = i_ab2;
    polygons_a.push_back(p_a);

    polygon p_b1;
    p_b1.p[0] = b1;
    p_b1.p[1] = i_ab2;
    p_b1.p[2] = i_ab1;
    polygons_b.push_back(p_b1);

    polygon p_b2;
    p_b2.p[0] = b1;
    p_b2.p[1] = b2;
    p_b2.p[2] = i_ab2;
    polygons_b.push_back(p_b2);
}

void bsp_tree::polygon_split(const bsp_tree::plane &pl, const polygon &pol, vector<bsp_tree::polygon> &polygons_front, vector<bsp_tree::polygon> &polygons_back) const {
    float d1 = glm::dot(pl, glm::vec4(pol.p[0], 1));
    float d2 = glm::dot(pl, glm::vec4(pol.p[1], 1));
    float d3 = glm::dot(pl, glm::vec4(pol.p[2], 1));

    if (d1 < 0 && d2 > 0 && d3 > 0) {
        polygon_split_aux(pl, pol.p[0], pol.p[1], pol.p[2], polygons_front, polygons_back);
    } else if (d2 < 0 && d1 > 0 && d3 > 0) {
        polygon_split_aux(pl, pol.p[1], pol.p[0], pol.p[2], polygons_front, polygons_back);
    } else if (d3 < 0 && d1 > 0 && d2 > 0) {
        polygon_split_aux(pl, pol.p[2], pol.p[0], pol.p[1], polygons_front, polygons_back);
    } else if (d1 > 0 && d2 < 0 && d3 < 0) {
        polygon_split_aux(pl, pol.p[0], pol.p[1], pol.p[2], polygons_back, polygons_front);
    } else if (d2 > 0 && d1 < 0 && d3 < 0) {
        polygon_split_aux(pl, pol.p[1], pol.p[0], pol.p[2], polygons_back, polygons_front);
    } else if (d3 > 0 && d1 < 0 && d2 < 0) {
        polygon_split_aux(pl, pol.p[2], pol.p[1], pol.p[0], polygons_back, polygons_front);
    }
}

bsp_tree::~bsp_tree() {
    erase_rec(root_);
}

void bsp_tree::erase_rec(node *n) {
    if (n->l != nullptr) {
        erase_rec(n->l);
    }

    if (n->r != nullptr) {
        erase_rec(n->r);
    }

    delete n;
}

unsigned int bsp_tree::nodes() {
    return nodes_;
}

unsigned int bsp_tree::fragments() {
    return fragments_;
}
