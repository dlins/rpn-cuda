#ifndef _IMPLICITFUNCTION_
#define _IMPLICITFUNCTION_

#include "GridValues.h"

#define IMPROVABLE_OK    0
#define IMPROVABLE_ERROR 1

class ImplicitFunction {
    private:
    protected:
        GridValues *gv;

    public:
        ImplicitFunction(){gv = 0;}
        virtual ~ImplicitFunction(){}

        virtual int function_on_square(double *foncub, int i, int j) = 0;

        // If available, the linear interpolation provided by hcube will be
        // completed here.
        // 
        virtual bool improvable(void){return false;}
        virtual int complete(const RealVector &p0, const RealVector &p1, const RealVector &p_init, RealVector &p_completed){return IMPROVABLE_ERROR;}

        virtual void map(const RealVector &p, double &f, RealVector &map_Jacobian) {
            /**
             * f = 0.0;
             * map_Jacobian.component(0) = 0.0;
             * map_Jacobian.component(1) = 0.0;
            **/

            printf("ATTENTION, it is using NULL ImplicitFunction::map()\n");

            return;
        }

        GridValues * grid_value(void){return gv;}
};

#endif // _IMPLICITFUNCTION_

