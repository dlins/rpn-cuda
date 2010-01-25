/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) ContinuationRarefactionFlow.h
 */

#ifndef _ContinuationRarefactionFlow_H
#define _ContinuationRarefactionFlow_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include <math.h>
#include <stdlib.h>
#include "RealVector.h"
#include "JacobianMatrix.h"
#include "HessianMatrix.h"
#include "RarefactionFlow.h"
#include "eigen.h"


/*
 * ---------------------------------------------------------------
 * Definitions:
 */


// Maximum number of rows and columns of the matrix being considered, 
// and consequently of the number of its eigenvalues
////#define EIG_MAX 2      

// Epsilon value, minimum threshold. 
// Any number x such that abs(x) < EPS will be considered 
// equal to zero in certain circumstances
//#define EPS 1e-30

//#define EIG_MAX 3 //used to be 3

// Maximum number of points in the orbit
//#define PNT_MAX 10000

// Functions that successfully carry on their task will return SUCCESSFUL_PROCEDURE,
// since most of the results are not floats or ints, but vectors and as such must
// be returned by means of pointers (which are passed in the parameters).
//
// Functions that encounter some kind of problem will return ABORTED_PROCEDURE.
// In this case the results of the functions should be disregarded, i.e., 
// a function invoking another function MUST check wether the invoked function
// could or could not successfully perform its task.
//

extern"C" {

    double ddot_(int *, double *, int *, double *, int *);
}

class ContinuationRarefactionFlow : public RarefactionFlow {
private:

    void fill_with_jet(const FluxFunction & flux_object, int n, double *in, int degree, double *F, double *J, double *H);
    void applyH(int n, double *xi, double *H, double *eta, double *out)const;
    void matrixmult(int m, int p, int n, double *A, double *B, double *C)const;
  

    // Last viable eigenvalue
    double lasteigenvalue;

    // Last viable shock speed
    //double shock_speed_var;

    // Reference speed (See the identity in Proposition 10.11 of
    // "An Introduction to Conservation Laws:
    // Theory and Applications to Multi-Phase Flow" (monotonicity
    // of the rarefaction).
    double ref_speed;

    // Reference vector whose inner product with the eigenvector
    // must be positive.
//    double re[EIG_MAX];

    int rarefaction(int *neq, double *xi, double *in, double *out, int *nparam, double *param);

public:
//    ContinuationRarefactionFlow(const int, const int, const FluxFunction &);
    ContinuationRarefactionFlow(const RealVector & , const int, const int, const FluxFunction &);
    ContinuationRarefactionFlow(const ContinuationRarefactionFlow &);

    int flux(int n, int family, double *in, double *lambda, double *out);
    double prodint(int n, double *a, double *b)const;

    int flux(const RealVector &, RealVector &);
    int fluxDeriv(const RealVector &, JacobianMatrix &);
    int fluxDeriv2(const RealVector &, HessianMatrix &);

    WaveFlow * clone()const;

    virtual ~ContinuationRarefactionFlow();

};

#endif //! _ContinuationRarefactionFlow_H
