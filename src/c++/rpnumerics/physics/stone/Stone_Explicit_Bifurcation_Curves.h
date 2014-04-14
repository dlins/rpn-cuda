#ifndef STONE_EXPLICIT_BIFURCATION_CURVES
#define STONE_EXPLICIT_BIFURCATION_CURVES

#include "Explicit_Bifurcation_Curves.h"
#include "StoneFluxFunction.h"
#include "IsoTriang2DBoundary.h"

// The domain is divided in regions according
// to the value of the inequalities that define the explicit bifurcations curves.
//
// Let the triangle below be divided in the following regions:
//
//     The line between the "pure W" vertex and the "No W" side divides the triangle in two regions:
//     below (W-) and above (W+) it. 
//
//     The line between the "pure O" vertex and the "No O" side divides the triangle in two regions:
//     to the left (O-) and to the right (O+) of it.
//
//     The line between the "pure G" vertex and the "No G" side divides the triangle in two regions:
//     below (G-) and above (G+) it. 
//
// Any point in the domain can be classified according to where in all of the three abovementioned regions is.
// The suffixes "-" and "+" above are replaced by "M" and "P" respectively.
//
//
//             pure O
//               /|\
//              / | \
//             /  |  \
//            /   |   \
//     No W  /    |    \ No G
//          /     |     \
//         /  -   |  +   \
//        /       |       \
//       /        |        \
// pure G-------------------pure W
//              No O
// The 

#define REGION_WM_OM_GM 0
#define REGION_WM_OP_GM 1
#define REGION_WP_OP_GM 2
#define REGION_WP_OP_GP 3
#define REGION_WP_OM_GP 4
#define REGION_WM_OM_GP 5

class Stone_Explicit_Bifurcation_Curves : public Explicit_Bifurcation_Curves {
private:
protected:
    StoneFluxFunction *f;

    void line(const RealVector &p, const RealVector &q, int nos, std::vector<RealVector> &v);
    RealVector sec_bif_correspondence(int side_opposite_vertex, const RealVector &point, const RealVector &mu);

  

    void points_to_segments(std::vector<RealVector> &v);




public:
    Stone_Explicit_Bifurcation_Curves(StoneFluxFunction *ff);
    virtual ~Stone_Explicit_Bifurcation_Curves();

    void expl_sec_bif_crv(int side_opposite_vertex, int nos,
            std::vector<RealVector> &vertex_to_umbilic,
            std::vector<RealVector> &umbilic_to_side);

    RealVector equations(const RealVector &point);
    int region(const RealVector &equations);

    int cross_sec_bif(const RealVector &previous_point, const RealVector &point, RealVector &crossing_point, int &region);
    
    void vertex_and_side(int side_opposite_vertex, const RealVector &mu, RealVector &vertex, RealVector &point_on_side);

    void sec_bif_correspondence(int side_opposite_vertex, int nos,
            std::vector<RealVector> &point,
            std::vector<RealVector> &correspondent_point);
};

#endif // STONE_EXPLICIT_BIFURCATION_CURVES

