#ifndef _COLORCURVE_
#define _COLORCURVE_

#include <vector>

#include "FluxFunction.h"
#include "AccumulationFunction.h"
#include "eigen.h"

#define INTERPOLATION_ERROR -1
#define INTERPOLATION_OK     0

//#define _SHOCK_SIMPLE_ACCUMULATION_  10  // Traditional rarefaction, using dgeev.
//#define _SHOCK_GENERAL_ACCUMULATION_ 11  // Rarefaction with generalized eigenpairs, using dggev.

#ifndef UNCLASSIFIABLE_POINT
#define UNCLASSIFIABLE_POINT (-1)
#endif

extern "C" void dgesv_(int*,int*,double *,int*,int *,double *,int* ,int*);


struct HugoniotPolyLine {
public:
    std::vector<RealVector> point;  	// Each element has size = dimension (dim).
    std::vector<double>     speed;      // Speed at each point.
    std::vector<RealVector> eigenvalue; // Each element has size = number of valid eigenvalues (fam).
    int type;
    
    HugoniotPolyLine() {
      
        type = -1;
    };

    ~HugoniotPolyLine() {
      
    };
};

// Color table. Could be anywhere.
//struct ColorTable{
//    public:
//        static int color[16][3];
//};

class ColorCurve {
    private:
    protected:
        int solve(const double *A,  double *b, int dim, double *x);

        void Left_Newton_improvement(const RealVector &input, const int type, RealVector &out);
        void Right_Newton_improvement(const RealVector &input, const int type, RealVector &out);

        int interpolate(const RealVector &p, double &s_p,
                        const std::vector<double> &eigenvalue_p, const int type_p,
                        const RealVector &q, double &s_q,
                        const std::vector<double> &eigenvalue_q, const int type_q,
                        vector<RealVector> &r, vector<int> &rtype);

        int complete_point(RealVector &p, double &s, std::vector<double> &eigenvalue);

        int classify_point(RealVector &p, double &s, std::vector<double> &eigenvalue);

        void classify_segment(RealVector &p,  RealVector &q, 
                              std::vector<HugoniotPolyLine> &classified_curve,
                              std::vector<RealVector> &transition_list);

        FluxFunction * fluxFunction_;
        AccumulationFunction * accFunction_;

        RealVector ref_point;
        std::vector<double> ref_eigenvalue;
        RealVector F_ref, G_ref;
    public:

    ColorCurve(const FluxFunction &, const AccumulationFunction &);
    virtual ~ColorCurve();

// So por chamada deixo este metodo...
    void classify_curve(vector<vector<RealVector> > &, const RealVector &, int, int, vector<HugoniotPolyLine> &output);
// ... de ser tudo certo, isto va embora.

    void classify_segmented_curve(std::vector<RealVector>  &original, const RealVector &ref,
                                  std::vector<HugoniotPolyLine> &classified_curve,
                                  std::vector<RealVector> &transition_list);

    void classify_continuous_curve(std::vector<RealVector>  &original, const RealVector &ref,
                                   std::vector<HugoniotPolyLine> &classified_curve,
                                   std::vector<RealVector> &transition_list);
};

#endif // _COLORCURVE_
