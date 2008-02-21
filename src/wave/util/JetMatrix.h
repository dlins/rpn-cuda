/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JetMatrix.h
 */

#ifndef _JetMatrix_H
#define _JetMatrix_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "Vector.h"
#include "except.h"
#include "JacobianMatrix.h"
#include "HessianMatrix.h"
#include "RealVector.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */



/*!@brief Utility class to store a function value and its derivatives
 * 
 * 
 *
 * @ingroup wave
 */


class JetMatrix {
private:
    int n_comps_, size_;
    Vector v_;
    bool c0_, c1_, c2_;
    
    class RangeViolation : public exception { };
    
public:
    
    JetMatrix(void);
   
    /*! Constructs a matrix with a arbitrary dimension
     *
     *  Dimension is the number of components whose function are stored into the JetMatrix
     *
     *@param n_comps Dimension of the function stored into the JetMatrix
     */
    
    JetMatrix(const int n_comps);
    
    /*! Copy constructor
     */
    
    JetMatrix(const JetMatrix & jetMatrix);
    virtual ~JetMatrix();
    
    /*! Fills a RealVector with the function components values
     *@param [out] values The RealVector to be filled
     */
    void f(RealVector &);
    
    /*!Fills a JacobianMatrix with the first derivative components values stored into the JetMatrix
     * Function components values accessor
     *@param [out] jMatrix The JacobianMatrix to be filled
     */
    
    void jacobian(JacobianMatrix & jMatrix);
    
    /*!Fills a HessianMatrix with the second derivative components values stored into the JetMatrix
     * Second derivative components values accessor
     *
     *@param[out] hMatrix The HessianMatrix to be filled
     */
    
    void hessian(HessianMatrix & hMatrix);
    
    /*! Changes the function components values stored into the JetMatrix
     * Function components values mutator
     *@param [in] fValues New function components values
     */
    
    void setF(const RealVector & fValues);
    
    /*! Changes the first derivative values stored into the JetMatrix
     *First derivative components mutator
     *@param [in] jMatrix New first derivative components values
     */
    
    void setJacobian(const JacobianMatrix & jMatrix);
    
    /*! Changes the second derivative values stored into the JetMatrix
     *Second derivative components mutator
     *@param [in] hMatrix New second derivative components values
     */
    
    void setHessian(const HessianMatrix & hMatrix);
    
    
    /*! Returns the JetMatrix dimension
     */
    int n_comps(void) const;
    
    /*! Changes the JetMatrix dimension
     */
    
    void resize(int n_comps);
    
    void range_check(int comp) const;
    
    /*! Sets all components to zero
     */
    
    JetMatrix &zero(void);
    
    double * operator()(void);
    
    /*! Returns a function component value
     *@param i The function component index
     */
    double operator()(int i);
    
    /*! Returns a first derivative component value
     *@param i The first derivative component row
     * *@param j The first derivative component  column
     */
    
    double operator()(int i, int j);
    
    /*! Returns a second derivative component value
     *
     *@param i The second derivative component row
     *@param j The second derivative component  column
     *@param k The second derivative component  deep
     */
    
    double operator()(int i, int j, int k);
    
    /*!Changes a function component value
     *@param i The function component index
     *@param value The new component value
     */
    
    void operator()(int i, double value);

    /*! Changes a first derivative component value
     *@param i The first derivative component row
     *@param j The first derivative component column
     *@param value The new first derivative component value
     */
    
    void operator()(int i, int j, double value);

    /*! Changes a second derivative component value
     *@param i The second derivative component row
     *@param j The second derivative component column
     *@param k The second derivative component deep
     *@param value The new second derivative component value
     */
        
    void operator()(int i, int j, int k, double value);
    
};

inline void JetMatrix::range_check(int comp) const {
    if (comp < 0 || comp >= n_comps())
        THROW(JetMatrix::RangeViolation());
}


inline   JetMatrix & JetMatrix::zero(void) {
    v_.zero();
    return *this;
}

inline  double * JetMatrix::operator()(void) {
    return v_.components();
}

inline  double JetMatrix::operator()(int i) {
    range_check(i);
    if (!c0_)
        THROW(JetMatrix::RangeViolation());
    return v_.component(i);
}

inline  double JetMatrix::operator()(int i, int j) {
    range_check(i);
    range_check(j);
    if (!c1_)
        THROW(JetMatrix::RangeViolation());
    return v_.component((n_comps_) + (i*n_comps_ + j));
}


inline double JetMatrix::operator()(int i, int j, int k) {
    range_check(i);
    range_check(j);
    range_check(k);
    if (!c2_)
        THROW(JetMatrix::RangeViolation());
    return v_.component((n_comps_ * (1 + n_comps_)) + (i*n_comps_*n_comps_ + j*n_comps_ + k));
}

inline  void JetMatrix::operator()(int i, double value) {
    range_check(i);
    c0_=true;
    double * value_ = & v_.component(i);
    *value_ = value;
}

inline  void JetMatrix::operator()(int i, int j, double value) {
    
    range_check(i);
    range_check(j);
    c1_=true;
    double * value_ = & v_.component((n_comps_) + (i*n_comps_ + j));
    *value_ = value;
}

inline  void JetMatrix::operator()(int i, int j, int k, double value) {
    range_check(i);
    range_check(j);
    range_check(k);
    c2_=true;
    double * value_ = & v_.component((n_comps_ * (1 + n_comps_)) + (i*n_comps_*n_comps_ + j*n_comps_ + k));
    *value_ = value;
}



inline void JetMatrix::setHessian(const HessianMatrix & input){
    
    int i, j, k;
    for (i=0;i < n_comps() ; i++){
        for (j=0; j < n_comps() ; j++){
            for (k=0 ;k < n_comps(); k++){
                
                operator()(i, j, k, input(i, j, k));
                
            }
        }
    }
}


inline void JetMatrix::hessian(HessianMatrix & hMatrix){
    int i, j, k;
    for (i=0;i < n_comps() ; i++){
        for (j=0; j < n_comps() ; j++){
            for (k=0 ;k < n_comps(); k++){
                double value = operator()(i, j, k);
                hMatrix(i, j, k, value);
            }
        }
    }
}

inline void JetMatrix::setF(const RealVector &input){
    int i;
    for (i=0;i< n_comps();i++){
        operator()(i, input.component(i));
    }
    
}

inline void JetMatrix::f(RealVector & vector){
    
    int i;
    for (i=0; i < n_comps();i++){
        
        vector.component(i)=operator()(i);
    }
}


inline void JetMatrix::setJacobian(const JacobianMatrix & input){
    
    int i, j;
    for(i=0;i< n_comps();i++){
        for (j=0; j < n_comps();j++){
            operator()(i, j, input(i, j));
        }
    }
    
    
}

inline void JetMatrix::jacobian(JacobianMatrix &jMatrix){
    
    int i, j;
    for (i=0;i < n_comps(); i++){
        for (j=0; j < n_comps();j++ ){
            double value = operator()(i, j);
            jMatrix(i, j, value);
        }
    }
}






#endif //! _JetMatrix_H
