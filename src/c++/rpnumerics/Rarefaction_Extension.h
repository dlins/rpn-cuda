#ifndef _RAREFACTION_EXTENSIONTPCW_
#define _RAREFACTION_EXTENSIONTPCW_

#include "Boundary.h"
#include "Rarefaction.h"
#include "Extension_Curve.h"
#include "GridValues.h"

class Rarefaction_Extension {
private:
protected:
public:


    static void extension_curve(GridValues  &, const FluxFunction *curve_flux, const AccumulationFunction *curve_accum,
            RealVector &initial_point,
            double deltaxi,
            int curve_family, int extension_family,
            int increase,
            const Boundary *boundary,
            //                                    const RealVector &pmin, const RealVector &pmax, int *number_of_grid_points,         // For the domain.
            //                                    int domain_family,
            //                                    const  FluxFunction *domain_ff,  const AccumulationFunction *domain_aa,
                                    int characteristic_where, //int singular,
            std::vector<RealVector> &curve_segments,
            std::vector<RealVector> &domain_segments);
};

#endif // _RAREFACTION_EXTENSION_

