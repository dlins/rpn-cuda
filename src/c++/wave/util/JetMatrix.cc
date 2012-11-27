/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JetMatrix.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include <iosfwd>


#include "JetMatrix.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */


JetMatrix::JetMatrix(void) : n_comps_(1), size_(3), v_(size_), c0_(false), c1_(false), c2_(false) {
}

JetMatrix::JetMatrix(const int n_comps) : n_comps_(n_comps), size_(n_comps * (1 + n_comps * (1 + n_comps))),
v_(size_),
c0_(false),
c1_(false),
c2_(false) {

}

JetMatrix::JetMatrix(const JetMatrix & jetMatrix) :
n_comps_(jetMatrix.n_comps_), size_(n_comps_ * (1 + n_comps_ * (1 + n_comps_))),
v_(RealVector(jetMatrix.v_)), c0_(false), c1_(false), c2_(false) {
}

JetMatrix::JetMatrix(int degree, int n_comps, double * values) :
n_comps_(n_comps), size_(n_comps_ * (1 + n_comps_ * (1 + n_comps_))),
v_(RealVector(size_, values)), c0_(true), c1_(false), c2_(false) {

    if (degree >= 1) c1_ = true;
    if (degree >= 2) c2_ = true;
}

JetMatrix::~JetMatrix() {
};

int JetMatrix::n_comps(void) const {
    return n_comps_;
}

int JetMatrix::size(void) const {
    return size_;
}

void JetMatrix::resize(int n_comps) {
    size_ = (n_comps * (1 + n_comps * (1 + n_comps)));
    v_.resize(size_);
    n_comps_ = n_comps;
}

std::ostream & operator<<(std::ostream &out, const JetMatrix &r) {

    //Function value
    out << "Function value: \n";

    for (int i = 0; i < r.n_comps_; i++) {
        out << r(i);
       out << " ";

    }
    out << "\n";


    out << "First derivative: \n";
  
    for (int i = 0; i < r.n_comps_; i++) {
        for (int j = 0; j < r.n_comps_; j++) {
            out << r(i, j);
            out << " ";
        }
        out << "\n";
    }

    out << "Second derivative: \n";

    for (int i = 0; i < r.n_comps_; i++) {
        for (int j = 0; j < r.n_comps_; j++) {
            for (int k = 0; k < r.n_comps_; k++) {
                out << r(i, j, k);
                out << " ";
            }
            
        }
        out << "\n";
    }

 
 



}






