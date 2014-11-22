#ifndef _THERMODYNAMICS_
#define _THERMODYNAMICS_

#include <string.h>

#include "JetSinglePhaseLiquid.h"
#include "JetSinglePhaseVapor.h"

#include "spline1d.h"

#include "VLE_Flash_TPCW.h"

#ifndef SPLINE_OK
#define SPLINE_OK    0
#endif

#ifndef SPLINE_ERROR
#define SPLINE_ERROR 1
#endif

class Thermodynamics {
    private:
    protected:
        JetSinglePhaseLiquid *liquid;
        JetSinglePhaseVapor  *vapor;

        // Splines
        spline1dinterpolant hsigmaC_;
        int info_hsigmaC;
    
        static double Tref_rock, Tref_water/*, P*/;
        static double Rock_Cr;  // [J/(m*m*m*K)] Care should be taken: Cr and Cw display different units.
        static double Cw_; // == Water_Cw_specific [J/(kg*K)]

        // Typical values for the adimensionalization
        static double T_typical_;
        static double Rho_typical_; // For the time being, this will be RhoWconst = 998 [kg/m^3]. In the future, this value should be the density of pure water at the temperature T_typical.
        static double U_typical_;
        static double h_typical_;
        static double rhoW_const;

        // Some constants for mug
        static double a0, a1, a2, a3;

        // some constants for muw
        static double b0, b1, b2, b3, b4, b5;
        
        // Store the compositions for fixed temperature
        static double xc_T, yw_T;
        static double dxc_dTheta, d2xc_dTheta2, dyw_dTheta, d2yw_dTheta2;
        static double used_Theta;

        static VLE_Flash_TPCW *Flash;
        
        // Create a spline
        int create_spline(const char*, const char*, double, spline1dinterpolant&);

        int hW_jet(const double Theta, int degree, JetMatrix &hWj);

//        int hsigmaC_jet_splines(const double Theta, int degree, JetMatrix &hsigmaCj);
        int hsigmaC_jet(const double Theta, int degree, JetMatrix &hsigmaCj);

//        // For the fitting of hsigmaC:
//        //
//        RealVector A, B, C;
//        RealVector A_times_B;
//        RealVector minus_A_times_B_times_B;

        Parameter *P_parameter;
        double mc, mw;

    public:
        Thermodynamics(const char *hsigmaC_name, Parameter *P);
        virtual ~Thermodynamics();
    
        int RockEnthalpyVol_jet(const double Theta, int degree, JetMatrix &revj) const;

        int SuperCriticEnthalpyVol_jet(const double yw, const double Theta, int degree, JetMatrix &Hsij) const;
        int SuperCriticEnthalpyVol_jet(const double Theta, int degree, JetMatrix &Hsij) const;

        static int static_SuperCriticEnthalpyVol_jet(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            return ((Thermodynamics*)obj)->SuperCriticEnthalpyVol_jet(state(0) /*yw*/, state(1) /*Theta*/, degree, jm);
        }


        int AqueousEnthalpyVol_jet(const double xc, const double Theta, int degree, JetMatrix &Haj) const;
        int AqueousEnthalpyVol_jet(const double Theta, int degree, JetMatrix &Haj) const;

        int Rhosic_jet(const double yw, const double Theta, int degree, JetMatrix &rhosicj) const;

        static int Rhosic_jet2_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;

            double yw    = state(0);
            double Theta = state(1);

            jm.resize(2, 1);

            return thermo->Rhosic_jet(yw, Theta, degree, jm);
        }

        int Rhosic_jet(const double Theta, int degree, JetMatrix &rhosicj) const;

        static int Rhosic_jet_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;
            double Theta = state(0);

            return thermo->Rhosic_jet(Theta, degree, jm);
        }

        int Rhosiw_jet(const double yw, const double Theta, int degree, JetMatrix &rhosiwj) const;
        int Rhosiw_jet(const double Theta, int degree, JetMatrix &rhosiwj) const;

        static int Rhosiw_jet_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;
            double Theta = state(0);

            return thermo->Rhosiw_jet(Theta, degree, jm);
        }

        static int Rhosiw_jet2_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;

            double yw    = state(0);
            double Theta = state(1);

            jm.resize(2, 1);

            return thermo->Rhosiw_jet(yw, Theta, degree, jm);
        }

        int Rhoac_jet(const double xc, const double Theta, int degree, JetMatrix &rhoacj) const;
        int Rhoac_jet(const double Theta, int degree, JetMatrix &rhoacj) const;

        static int Rhoac_jet_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;
            double Theta = state(0);

            return thermo->Rhoac_jet(Theta, degree, jm);
        }

        static int Rhoac_jet2_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;

            double yw    = state(0);
            double Theta = state(1);

            jm.resize(2, 1);

            return thermo->Rhoac_jet(yw, Theta, degree, jm);
        }

        int Rhoaw_jet(const double xc, const double Theta, int degree, JetMatrix &rhoawj) const;
        int Rhoaw_jet(const double Theta, int degree, JetMatrix &rhoawj) const;

        static int Rhoaw_jet_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;
            double Theta = state(0);

            return thermo->Rhoaw_jet(Theta, degree, jm);
        }

        static int Rhoaw_jet2_test(void *obj, const RealVector &state, int degree, JetMatrix &jm){
            Thermodynamics *thermo = (Thermodynamics*)obj;

            double yw    = state(0);
            double Theta = state(1);

            jm.resize(2, 1);

            return thermo->Rhoaw_jet(yw, Theta, degree, jm);
        }

        // TODO: Vamos a ver si es mejor un JET, pues por ahora parece que son llamadas siempre las funciones y sus derivadas.
        // Water viscosity
        void muw(double T, double &muw, double &dmuw_dT, double &d2muw_dT2) const;
        void inv_muw(double T, double &nuw, double &dnuw_dT, double &d2nuw_dT2) const;

        // Gas viscosity
        void mug(double T, double &mug, double &dmug_dT, double &d2mug_dT2) const;
        void inv_mug(double T, double &nug, double &dnug_dT, double &d2nug_dT2) const;

        void setTtypical(double);
        void setRhoTypical(double);
        void UTypical (double);
        
        // Public static
        //
        
        // This method MUST BE CALLED before any Thermodynamics can be used!
        // Otherwise, it will segfault.
        //
        static void set_flash(VLE_Flash_TPCW *f){Flash = f; return;}
        
        static void SetCompositions(double Theta);
        
        static double Cr(void){return Rock_Cr;}

        static double Cw(void){return Cw_;}

        static double T_typical(void){return T_typical_;}

        static double U_typical(void){return U_typical_;}

        static double Rho_typical(void){return Rho_typical_;}

        // Convert from Theta to T (dimensionless -> dimensioned)
        static double Theta2T(double Theta){return Theta*T_typical_ + Tref_water;}

        // Convert from T to Theta (dimensioned -> dimensionless)
        static double T2Theta(double T){return (T - Tref_water)/T_typical_;}

        // Convert from U to u (dimensionless -> dimensioned)
        static double U2u(double U){return U*U_typical_;}

        // Convert from u to U (dimensioned -> dimensionless)
        static double u2U(double u){return u/U_typical_;}

//        void test_jets(){
//            JetTester(&static_SuperCriticEnthalpyVol_jet, this);
//            JetTester(&static_AqueousEnthalpyVol_jet, this);

//            return;
//        }
};

#endif //  _THERMODYNAMICS_

