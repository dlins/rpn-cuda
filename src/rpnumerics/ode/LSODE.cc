/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) LSODE.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "LSODE.h"
#include "FluxParams.h"

using std::vector;

/*
 * ---------------------------------------------------------------
 * Definitions:
 */

 double LSODE::tout_=0.05;

const RpFunction * LSODE::rpFunction_=NULL;

LSODE::LSODE(const LSODEProfile & profile):profile_(new LSODEProfile(profile)){
    rpFunction_=profile_->getFunction();
}


LSODE::LSODE(const LSODE & copy):profile_(new LSODEProfile((LSODEProfile &)copy.getProfile())){
    rpFunction_=profile_->getFunction();
}

LSODE::~LSODE() {
    delete profile_;
    
}


RealVector & LSODE::solve(const RealVector & input) const{
    
    
    int i;
    
    double xi=0; //HARDCODED Useless variable !!
    xi=1;       // Only to remove warnings
    
    double t=0;

    double * param;
    double  * atol;
    
    int neq = profile_->numberOfEquations();
    
    int itol = profile_->absoluteToleranceType();
    
    int mf = profile_->methodFlag();
    
    int itask = profile_->task();
    
    int iopt= profile_->opt();
    
    int lrw = profile_->lengthRWork();
    int liw = profile_->lengthIWork();
    
    int istate=1;//Initial value of istate !
    
    switch (itol){
        
        case 1:
            
            break;
        case 2:
            
            atol = new double[profile_->numberOfEquations()];
            for (i = 0; i < profile_->numberOfEquations(); i++)
                atol[i] = profile_->absoluteToleranceComponent(i);
            
            break;
        default:
            
            cout << "Error in tolerance choose" <<endl;
    }
    
    
    double *rwork = new double [profile_->lengthRWork()];
    
    int * iwork = new int[profile_->lengthIWork()];
    
    double rtol = profile_->relativeTolerance();
    
    double * U= new double [input.size()];
    
    int nparam=profile_->paramLength();
    
    param = new double[nparam];
    
    for (i=0; i < nparam;i++){
        
        param[i]=profile_->paramComponent(i);
    }
    
    
    for (i=0;i < input.size();i++){
        
        U[i]=input.component(i);
        
    }
    
    int info;
    
    info=solver(&LSODE::function, &neq, &U[0], &t, &LSODE::tout_, &itol, &rtol, &atol[0], &itask, &istate, &iopt, &rwork[0], &lrw, &iwork[0], &liw, &LSODE::jacrarefaction, &mf, &nparam, &param[0]);
    
    tout_=t+profile_->deltaTime();
    
    RealVector * result= new RealVector(neq);
    
    for (i=0;i< neq;i++){
        
        result->component(i)=U[i];
        
    }

    delete rwork;
    delete iwork;
    delete U;
    delete param;
    
    return *result;
}




int LSODE::function(int * neq , double * xi , double* U , double * out){
    
    int i, status ;
    
    WaveState  wState(*neq) ;
    
    for (i=0;i < *neq;i++){
        wState(i)=U[i];
    }
    
    JetMatrix jMatrix(*neq);
    
    status=rpFunction_->jet(wState, jMatrix, 0); //This function must return the same codes of LSODE s functions
    
    for(i=0;i< *neq;i++){
        
        out[i]=jMatrix(i);
        
    }
    
    return status;
    
    
    
}

ODESolution & LSODE::solve(const RealVector & input, int ) const{
    
    LSODEStopGenerator * stopGenerator = (LSODEStopGenerator *) profile_->getStopGenerator();
    
    vector <RealVector> coords;
    
    vector <double> times;
    
    int i;
    
    double xi=0; //HARDCODED Useless variable !!
    xi=1;       // Only to remove warnings
    
    double t=0;
    
    double tout=0.05;
    double * param;
    double  * atol;
    
    int neq = profile_->numberOfEquations();
    
    int itol = profile_->absoluteToleranceType();
    
    int mf = profile_->methodFlag();
    
    int itask = profile_->task();
    
    int iopt= profile_->opt();
    
    int lrw = profile_->lengthRWork();
    int liw = profile_->lengthIWork();
    
    int istate=1;//Initial value of istate !
    
    switch (itol){
        
        case 1:
            
            break;
        case 2:
            
            atol = new double[profile_->numberOfEquations()];
            for (i = 0; i < profile_->numberOfEquations(); i++)
                atol[i] = profile_->absoluteToleranceComponent(i);
            
            break;
        default:
            
            cout << "Error in tolerance choose" <<endl;
    }
    
    
    double *rwork = new double [profile_->lengthRWork()];
    
    int * iwork = new int[profile_->lengthIWork()];
    
    double rtol = profile_->relativeTolerance();
    
    double * U= new double [input.size()];
    
    int nparam=profile_->paramLength();
    
    param = new double[nparam];
    
    for (i=0; i < nparam;i++){
        
        param[i]=profile_->paramComponent(i);
    }
    
    
    for (i=0;i < input.size();i++){
        
        U[i]=input.component(i);
        
    }
    
    while (stopGenerator->getStatus()){
        
        int info;
        
        info=solver(&LSODE::function, &neq, &U[0], &t, &tout, &itol, &rtol, &atol[0], &itask, &istate, &iopt, &rwork[0], &lrw, &iwork[0], &liw, &LSODE::jacrarefaction, &mf, &nparam, &param[0]);
        
        stopGenerator->setFunctionStatus(info);
        
        if (info ==SUCCESSFUL_PROCEDURE){
            RealVector realVector(profile_->numberOfEquations());
            
            for (i=0;i < realVector.size();i++)
                realVector.component(i)=U[i];
            
            coords.push_back(realVector);
            
            tout=t+profile_->deltaTime();
            
            times.push_back(tout);
            
            stopGenerator->increaseTotalPoints();
        }
    }
    
    
    delete stopGenerator;
    delete rwork;
    delete iwork;
    delete U;
    delete param;
    
    ODESolution * solution = new ODESolution(coords, times, 1); //TODO 1 is a dummy value
    
    return *solution;
    
}

const ODESolverProfile & LSODE::getProfile()const { return *profile_;}


void LSODE::setProfile(const LSODEProfile & profile) {
    
    delete profile_;
    profile_=new LSODEProfile(profile);
}



int LSODE::jacrarefaction(int *neq, double *t, double *y, int *ml, int *mu, double *pd, int *nrpd){
    return SUCCESSFUL_PROCEDURE;
}

int LSODE::solver(int (*f)(int *, double *, double *, double *), int *neq, double *y, double *t, double *tout,
        int *itol, double *rtol, double *atol, int *itask, int *istate, int *iopt, double *rwork, int *lrw,
        int *iwork, int *liw, int(*j)(int *, double *, double *, int *, int *, double *, int *),
        int *mf, int *nparam, double *param) const{
    
    lsode_(f, neq, y, t, tout, itol, rtol, atol, itask, istate,
            iopt, rwork, lrw, iwork, liw, j, mf, nparam, param);
    
    
    if (*istate == 2){
        return SUCCESSFUL_PROCEDURE;
    }
    else {
        return *istate;
    }
    
}


//! Code comes here! daniel@impa.br

