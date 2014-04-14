#include "Boundary.h"

Boundary::~Boundary() {
};
//double Boundary::epsilon = 1e-10;

int Boundary::intersection(const RealVector &p, const RealVector &q, RealVector &r, int &w) const {
    w = -1;

//    std::cout << "Boundary. p = " << p << ", q = " << q << std::endl;
//    std::cout << "inside(p) = " << inside(p) << ", inside(q) = " << inside(q) << std::endl;


    if      ( inside(p) &&  inside(q)) return BOUNDARY_INTERSECTION_BOTH_INSIDE;
    else if (!inside(p) && !inside(q)) return BOUNDARY_INTERSECTION_BOTH_OUTSIDE;
    else {
        int n = p.size();

        // Initialize the temporary points
        RealVector pp(p);
        RealVector qq(q);

        // Switch the temporary points if need be, such that pp is inside and qq is outside
        if (!inside(pp)) {
            RealVector temp(pp);
            pp = qq;
            qq = temp;
        }

        // Minimum distance.
        double d = epsilon*norm(pp - qq);

        // Iterate while the distance between the points is greater than d.
        //
        int count = 0;

        while (norm(pp - qq) > d && count < 100) {
            count++;
            r = .5*(pp + qq);

            if (inside(r)) pp = r;
            else           qq = r;
        }

        return BOUNDARY_INTERSECTION_FOUND;

    }
}

