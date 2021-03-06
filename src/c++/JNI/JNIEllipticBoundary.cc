/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JNIHugoniotCurveCalc.cc
 **/


//! Definition of JNIHugoniotCurveCalc
/*!
	
TODO:
	
NOTE : 

@ingroup JNI
 */


#include "rpnumerics_EllipticBoundaryCalc.h"
#include "Elliptic_Boundary.h"
#include "JNIDefs.h"
#include "RpNumerics.h"
#include <vector>
#include <iostream>



using std::vector;
using namespace std;

JNIEXPORT jobject JNICALL Java_rpnumerics_EllipticBoundaryCalc_nativeCalc
(JNIEnv * env, jobject obj) {

    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);

    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);

    jclass arrayListClass = env->FindClass("java/util/ArrayList");

    jclass ellipticBoundaryClass = env->FindClass(ELLIPTICBOUNDARY_LOCATION);

   
    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");
    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");

    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID ellipticBoundaryConstructor = env->GetMethodID(ellipticBoundaryClass, "<init>", "(Ljava/util/List;)V");


    
    
     int dimension = RpNumerics::physicsVector_->at(0)->boundary()->minimums().size();

    const FluxFunction * flux = RpNumerics::physicsVector_->at(0)->flux();
    const AccumulationFunction * accum = RpNumerics::physicsVector_->at(0)->accumulation();


    GridValues * gv = RpNumerics::physicsVector_->at(0)->gridvalues();

    
    


    jobject leftSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);

    // Storage space for the segments:
    std::vector<RealVector> left_vrs;



    Elliptic_Boundary ellipticBoundary;

    ellipticBoundary.curve(flux, accum,
            *gv, left_vrs);


//    if ( Debug::get_debug_level() == 5 ) {
        ////cout << "left_vrs.size()  = " << left_vrs.size() << endl;
//    }

    if (left_vrs.size() == 0)return NULL;

    for (unsigned int i = 0; i < left_vrs.size() / 2; i++) {


        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);

        double * leftCoords = (double *) left_vrs.at(2 * i);
        double * rightCoords = (double *) left_vrs.at(2 * i + 1);

        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);

        //Construindo left e right points
        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);

        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);


        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
        env->CallObjectMethod(leftSegmentsArray, arrayListAddMethod, realSegment);

    }


    jobject result = env->NewObject(ellipticBoundaryClass, ellipticBoundaryConstructor, leftSegmentsArray);


    return result;

}
















