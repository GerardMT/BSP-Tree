
#ifndef BSP_TREE_BSP_TREE_H
#define BSP_TREE_BSP_TREE_H

#include <vector>

#define GLM_SWIZZLE
#include "glm/glm.hpp"

using namespace std;

class bsp_tree {
public:
    struct point {
        float x;
        float y;
        float z;
    };

    struct polygon {
        glm::vec3 p[3];
    };

    ~bsp_tree();

    /**
     * Construct the BSP tree from the list of polygons provided.
     * @param polygons list of polygons.
     */
    void construct(const vector<polygon> &polygons);

    /**
     * Return the number of nodes which compose the BSP tree.
     * @return number of nodes.
     */
    unsigned int nodes();

    /**
     * Return the number of fragments (split planes) which have been generated while the construction of the BSP tree.
     * @return number of fragments.
     */
    unsigned int fragments();

private:
    typedef glm::vec4 plane;

    struct node {
        node *l;
        node *r;
        vector<polygon> pols;
    };

    node *root_;

    unsigned int nodes_;
    unsigned int fragments_;

    /**
     * Given a list of polygons, choose the polygon which defines the splitting plane.
     * @param polygons list of polygons.
     * @return index of the chosen polygon.
     */
    unsigned int polygon_index(const vector<polygon> &polygons) const;

    /**
     * Recursive function. Construct the BSP tree from the list of polygons provided. The right subtree contains those polygons which are in front of the node plane,
     * while the left subtree contains those which are behind the node plane.
     * @param polygons list of polygons.
     * @param n node.
     */
    void construct_rec(const vector<polygon> &polygons, node *n);

    /**
     * Compute the plane defined by a polygon.
     * @param pol polygon.
     * @param pl plane. Output parameter.
     */
    void to_plane(const polygon &pol, plane &pl) const;

    enum dist_res {
        ON = 0,
        FRONT = 1,
        BACK = 2,
        HALF = 3
    };

    /**
     * Signed distance between a plane and a polygon. Given the polygon points, return:<br>
     * <pre>
     *     <b>ON</b> if they are in the plane,<br>
     *     <b>FRONT</b> if are in front of the plane,<br>
     *     <b>FRONT</b> if are in behind the plane,<br>
     *     <b>HALF</b> some are in front and some ar e behind the plane.<br>
     * </pre>
     * @param pl plane.
     * @param pol polygon.
     * @return the position of the polygon with respect of the plane.
     */
    dist_res distance(const plane &pl, const polygon &pol) const;

    /**
     * Return the intersection point between a plane and a segment.
     * @param pl plane.
     * @param a segment start point.
     * @param b segment end point.
     * @param i intersection point. Output parameter.
     */
    void plane_segment_intersection(const bsp_tree::plane &pl, const glm::vec3 &a, const glm::vec3 &b, glm::vec3 &i) const;

    /**
     * Auxiliary function. Abstraction of the front and back sides. Given a plane and 3 points which represent a polygon, the point <b>a</b> is always the
     * only point in the <b>polygons_a</b> side. Points <b>b1</b> and <b>b2</b> are in the <b>polygons_b</b> side.
     * @param pl plane.
     * @param a point in the side <b>polygons_a</b>.
     * @param b1 point in the side <b>polygons_b</b>.
     * @param b2 point in the side <b>polygons_b</b>.
     * @param polygons_a list of polygons in front of the plane to add the new polygons. One new polygon added.
     * @param polygons_b list of polygons in front of the plane to add the new polygons. Two new polygons added.
     */
    void polygon_split_aux(const bsp_tree::plane &pl, const glm::vec3 &a, const glm::vec3 &b1, const glm::vec3 &b2, vector<bsp_tree::polygon> &polygons_a, vector<bsp_tree::polygon> &polygons_b) const;

    /**
     * Given a plane and a polygon which intersects the plane, split the polygon by the plane.
     * @param pl plane.
     * @param pol polygon.
     * @param polygons_front list of polygons in front of the plane to add the new polygons.
     * @param polygons_back list of polygons behind of the plane to add the new polygons.
     */
    void polygon_split(const bsp_tree::plane &pl, const polygon &pol, vector<bsp_tree::polygon> &polygons_front, vector<bsp_tree::polygon> &polygons_back) const;

    /**
     * Delete recursively all tree nodes. Including <b>n</b>.
     * @param n tree root node.
     */
    void erase_rec(node *n);
};


#endif //BSP_TREE_BSP_TREE_H
