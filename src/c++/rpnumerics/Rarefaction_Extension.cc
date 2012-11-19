#include "Rarefaction_Extension.h"

void Rarefaction_Extension::extension_curve(GridValues & gridValues, const FluxFunction *curve_flux, const AccumulationFunction *curve_accum,
        RealVector &initial_point,
        double deltaxi,
        int curve_family, int extension_family,
        int increase,
        const Boundary *boundary,
        //        const RealVector &pmin, const RealVector &pmax, int *number_of_grid_points, // For the domain.
        //        int domain_family,
        //        const FluxFunction *domain_flux, const AccumulationFunction *domain_accum,
       int characteristic_where,//, int singular,
        std::vector<RealVector> &curve_segments,
        std::vector<RealVector> &domain_segments) {
    curve_segments.clear();
    domain_segments.clear();

    std::vector<RealVector> rarefaction_curve;
    std::vector<RealVector> inflection_points;


    int info = Rarefaction::curve(initial_point,
            RAREFACTION_INITIALIZE_YES,
            (const RealVector *) 0,
            curve_family,
            increase,
            RAREFACTION_FOR_ITSELF,
            deltaxi,
            curve_flux, curve_accum,
            RAREFACTION_GENERAL_ACCUMULATION,
            (Boundary*) boundary,
            rarefaction_curve, inflection_points);


    if (rarefaction_curve.size() < 2) return;

    int n = initial_point.size();
    
    cout<<"Tamanho do n: "<<n<<endl;

    // Turn the curve of points into a curve of segments.
    vector <RealVector> rarefaction_segments;
    rarefaction_segments.resize(2 * rarefaction_curve.size() - 2);
    for (int i = 0; i < rarefaction_curve.size() - 1; i++) {
        rarefaction_segments[2 * i].resize(n);
        rarefaction_segments[2 * i + 1].resize(n);
        for (int j = 0; j < 2; j++) {
            rarefaction_segments[2 * i].component(j) = rarefaction_curve[i].component(j);
            rarefaction_segments[2 * i + 1].component(j) = rarefaction_curve[i + 1].component(j);
        }
        if (n == 3){
            rarefaction_segments[2 * i + 1].component(2) = 1.0;
            rarefaction_segments[2 * i ].component(2) = 1.0;
        }
            
    }

    Extension_Curve extension_curve; 
    extension_curve.curve(curve_flux, curve_accum, gridValues, characteristic_where,
            true, extension_family, rarefaction_segments,
            curve_segments,
            domain_segments);

    return;
}

