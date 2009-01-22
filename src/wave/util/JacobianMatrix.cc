/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JacobianMatrix.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "JacobianMatrix.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */

JacobianMatrix::JacobianMatrix(const int n_comps) : RealMatrix2(n_comps),
n_comps_(n_comps)

 {
}

JacobianMatrix::JacobianMatrix(const JacobianMatrix & jacobianMatrix) : RealMatrix2(jacobianMatrix.n_comps()),
n_comps_(jacobianMatrix.n_comps_)

 {

}



