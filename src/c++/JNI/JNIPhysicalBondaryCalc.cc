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


#include "rpnumerics_PhysicalBoundaryCalc.h"
#include "JNIDefs.h"
#include "RpNumerics.h"
#include <vector>
#include <iostream>



using std::vector;
using namespace std;

JNIEXPORT jobject JNICALL Java_rpnumerics_PhysicalBoundaryCalc_calcNative
(JNIEnv * env, jobject obj) {

    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);

    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);

    jclass arrayListClass = env->FindClass("java/util/ArrayList");

    jclass physicalBoundaryClass = env->FindClass(PHYSICALBOUNDARY_LOCATION);



    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");

    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");

    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jmethodID physicalBoundaryConstructor = env->GetMethodID(physicalBoundaryClass, "<init>", "(Ljava/util/List;)V");

    jobject segmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);


    int dimension = RpNumerics::physicsVector_->at(0)->boundary()->minimums().size();

    Boundary * physicsBoundary = (Boundary *) RpNumerics::physicsVector_->at(0)->boundary();

    std::vector<vector<RealVector> > left_vrs;


    physicsBoundary->physical_boundary(left_vrs);

   
   
    if (left_vrs.size() == 0)
        return NULL;

    for (int i = 0; i < left_vrs.size(); i++) {

        for (int j = 0; j < left_vrs[i].size()/2; j++) {
            
            
             jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
             jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);
//
            
            double * leftCoords = (double *) left_vrs[i].at(2 * j);
            double * rightCoords = (double *) left_vrs[i].at(2 * j + 1);


            env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
            env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);


            //Construindo left e right points
            jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);
            jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);


            jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
            env->CallObjectMethod(segmentsArray, arrayListAddMethod, realSegment);
        }
    }


    jobject result = env->NewObject(physicalBoundaryClass, physicalBoundaryConstructor, segmentsArray);

    // Limpando

    env->DeleteLocalRef(realSegmentClass);
    env->DeleteLocalRef(realVectorClass);
    env->DeleteLocalRef(arrayListClass);


    return result;


}
















