#ifndef _PARAMETRICPLOT_
#define _PARAMETRICPLOT_

#include <math.h>

#include "Boundary.h"
#include "Curve.h"

#ifndef INITIAL_POINT_FOUND
#define INITIAL_POINT_FOUND 0
#endif

#ifndef INITIAL_POINT_NOT_FOUND
#define INITIAL_POINT_NOT_FOUND 1
#endif

#define TEST_PARAMETRICPLOT
//
//#ifdef TEST_PARAMETRICPLOT
//#include "canvas.h"
//#include "canvasmenuscroll.h"
//#endif 

class ParametricPlot {
    private:
    protected:
        static int find_initial_point_within_domain(RealVector (*f)(void*, double), 
                                                    void *obj, 
                                                    double &phi, double phi_final, double delta_phi, 
                                                    const Boundary *b,
                                                    Curve &curve);

        static void find_curve(RealVector (*f)(void*, double), void *obj, double &phi, double phi_final, double max_distance, double delta_phi, const Boundary *b, Curve &curve);

        static void intersection(RealVector (*f)(void*, double), void *obj, const Boundary *b, double max_distance,
                                  double init_phi_p, double init_phi_q, 
                                  double &phi);

    public:
        static void plot(RealVector (*f)(void*, double), 
                         void *obj, double phi_init, double phi_final, int n, const Boundary *b, std::vector<Curve> &curve);

        static void plot(RealVector (*f)(void*, double), 
                         void *obj, const Boundary *b, std::vector<Curve> &curve){

            double phi_init  = 0.0;
            double phi_final = 2.0*M_PI;

            int n = 1e3;

            plot(f, obj, phi_init, phi_final, n, b, curve);

            return;
        }

//        #ifdef TEST_PARAMETRICPLOT
//        static Canvas *canvas;
//        static CanvasMenuScroll *scroll;
//        #endif
};

#endif // _PARAMETRICPLOT_

