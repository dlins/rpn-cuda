/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) Stone.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "Stone.h"
#include "Hugoniot_Curve.h"
#include "Double_Contact.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */


Boundary * Stone::defaultBoundary() const {


    return new Three_Phase_Boundary();
    //    RealVector pmin(2);
    //
    //    pmin.component(0) = 0;
    //    pmin.component(1) = 0;
    //
    //    RealVector pmax(2);
    //
    //    pmax.component(0) = 1.0;
    //    pmax.component(1) = 1.0;

    //Saturacoes negativas

    //     A.component(0) = -1;
    //    A.component(1) = 2;
    //
    //    RealVector B(2);
    //
    //    B.component(0) = -1;
    //    B.component(1) = 2;
    //
    //    RealVector C(2);
    //
    //    C.component(0) = 2;
    //    C.component(1) = -1;



    //
    //      A.component(0) = -1;
    //    A.component(1) = -1;
    //
    //    RealVector B(2);
    //
    //    B.component(0) = -1;
    //    B.component(1) = 2;
    //
    //    RealVector C(2);
    //
    //    C.component(0) = 2;
    //    C.component(1) = -1;






}

void Stone::setParams(vector<string> params) {


    RealVector fluxParamVector(7);

    //Flux params
    for (int i = 0; i < fluxParamVector.size(); i++) {

        double paramValue = atof(params[i].c_str());

        fluxParamVector.component(i) = paramValue;

    }

    StoneFluxFunction & stoneFlux = (StoneFluxFunction&) fluxFunction();

    stoneFlux.fluxParams(StoneParams(fluxParamVector));

    //Perm params
    RealVector permVector(20);

    for (int i = 7; i < params.size(); i++) {

        double paramValue = atof(params[i].c_str());

        permVector.component(i - 7) = paramValue;


    }
    StonePermParams permParams(permVector);

    stoneFlux.setPermParams(permParams);

}

Stone::Stone() : SubPhysics(StoneFluxFunction(StoneParams(), StonePermParams()), StoneAccumulation(), *defaultBoundary(), Multid::PLANE, "Stone", _SIMPLE_ACCUMULATION_) {
    setHugoniotFunction(new Hugoniot_Curve());
    setDoubleContactFunction(new Double_Contact());
}

Stone::Stone(const Stone & copy) : SubPhysics(copy.fluxFunction(), copy.accumulation(), copy.boundary(), Multid::PLANE, "Stone", _SIMPLE_ACCUMULATION_) {
    setHugoniotFunction(new Hugoniot_Curve());
    setDoubleContactFunction(new Double_Contact());

}

SubPhysics * Stone::clone()const {
    return new Stone(*this);
}

Stone::~Stone() {
}

