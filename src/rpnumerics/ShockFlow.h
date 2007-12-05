/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) ShockFlow.h
 **/

#ifndef _ShockFlow_H
#define _ShockFlow_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "WaveFlow.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */

class ShockFlow : public WaveFlow {
    
public: 
    
    int jet(const int degree, const RealVector &u, JetMatrix &m);

};




#endif //! _ShockFlow_H
