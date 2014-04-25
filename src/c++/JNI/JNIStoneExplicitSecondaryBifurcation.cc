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


#include "rpnumerics_StoneExplicitSecondaryBifurcationCurveCalc.h"
#include "Stone_Explicit_Bifurcation_Curves.h"
#include "JNIDefs.h"
#include "RpNumerics.h"
#include "Debug.h"
#include <vector>
#include <iostream>

using std::vector;
using namespace std;

/*
 * Class:     rpnumerics_StoneExplicitSecondaryBifurcationCurveCalc
 * Method:    nativeCalc
 * Signature: (II)Lrpnumerics/RpSolution;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_StoneExplicitSecondaryBifurcationCurveCalc_nativeCalc__II
(JNIEnv * env, jobject obj, jint opositeSide, jint numberOfSteps) {

    jclass classPhasePoint = (env)->FindClass(PHASEPOINT_LOCATION);

    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);

    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);

    jclass arrayListClass = env->FindClass("java/util/ArrayList");

    jclass hysteresisCurveClass = env->FindClass(SECONDARY_BIFURCATION_LOCATION);

    jmethodID toDoubleMethodID = (env)->GetMethodID(classPhasePoint, "toDouble", "()[D");
    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");
    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");

    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID hysteresisCurveConstructor = env->GetMethodID(hysteresisCurveClass, "<init>", "(Ljava/util/List;Ljava/util/List;Lwave/util/RealVector;)V");

    int dimension = RpNumerics::getPhysics().domain().dim();
    //int dimension = 2;

    jobject leftSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);
    jobject rightSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);

    // Storage space for the segments:
    std::vector<RealVector> left_vrs;
    std::vector<RealVector> right_vrs;

    const FluxFunction * leftFlux = &RpNumerics::getPhysics().fluxFunction();

    // muw, muo, mug
    RealVector flux_params = leftFlux->fluxParams().params();

    double muw = flux_params(3);
    double muo = flux_params(4);
    double mug = flux_params(5);

    double sum_mu = muw + muo + mug;


    RealVector umbp(2);
    umbp(0) = muw / sum_mu;
    umbp(1) = muo / sum_mu;


    jdoubleArray umbilicArray = env->NewDoubleArray(dimension);

    double * umbilicCoords = (double *) umbp;

    env->SetDoubleArrayRegion(umbilicArray, 0, dimension, umbilicCoords);

    jobject umbilicPointVector = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, umbilicArray);









    Stone_Explicit_Bifurcation_Curves sebc((StoneFluxFunction*) leftFlux);

    sebc.sec_bif_correspondence(opositeSide, numberOfSteps, left_vrs, right_vrs);


    if (Debug::get_debug_level() == 5) {
        cout << "left_vrs.size()  = " << left_vrs.size() << endl;
        cout << "right_vrs.size()  = " << right_vrs.size() << endl;
    }


    if (left_vrs.size() == 0 || right_vrs.size() == 0)return NULL;


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

    for (unsigned int i = 0; i < right_vrs.size() / 2; i++) {



        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);

        double * leftCoords = (double *) right_vrs.at(2 * i);
        double * rightCoords = (double *) right_vrs.at(2 * i + 1);


        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);

        //Construindo left e right points
        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);

        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);
        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);


        env->CallObjectMethod(rightSegmentsArray, arrayListAddMethod, realSegment);

    }


    jobject result = env->NewObject(hysteresisCurveClass, hysteresisCurveConstructor, leftSegmentsArray, rightSegmentsArray,umbilicPointVector);


    return result;

}

/*
 * Class:     rpnumerics_StoneExplicitSecondaryBifurcationCurveCalc
 * Method:    nativeCalc
 * Signature: (Ljava/util/List;)Lrpnumerics/RpSolution;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_StoneExplicitSecondaryBifurcationCurveCalc_nativeCalc__Ljava_util_List_2
(JNIEnv * env, jobject obj, jobject areasList) {

    //    jclass classPhasePoint = (env)->FindClass(PHASEPOINT_LOCATION);
    //
    //    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);
    //
    //    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);
    //
    //    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    //    
    //    jclass classList = env->FindClass("java/util/List");
    //
    //    jclass hysteresisCurveClass = env->FindClass(SECONDARY_BIFURCATION_LOCATION);
    //    
    //    
    //    jclass areaClass = env->FindClass(AREA_LOCATION);
    //    
    //    jmethodID listGetMethodID = env->GetMethodID(classList, "get", "(I)Ljava/lang/Object;");
    //    
    //    jmethodID areaGetResolutionMethodID = env->GetMethodID(areaClass, "getResolution", "()Lwave/util/RealVector;");
    //    
    //    
    //    jmethodID downLeftMethodID = env->GetMethodID(areaClass, "getDownLeft", "()Lwave/util/RealVector;");
    //    jmethodID topRightMethodID = env->GetMethodID(areaClass, "getTopRight", "()Lwave/util/RealVector;");
    //
    //    
    //    jmethodID toDoubleMethodID = (env)->GetMethodID(classPhasePoint, "toDouble", "()[D");
    //    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");
    //    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");
    //
    //    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    //    jmethodID hysteresisCurveConstructor = env->GetMethodID(hysteresisCurveClass, "<init>", "(Ljava/util/List;Ljava/util/List;)V");
    //
    //    int dimension = RpNumerics::getPhysics().domain().dim();
    //    //int dimension = 2;
    //
    //    jobject leftSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);
    //    jobject rightSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);
    //    
    //    
    //    
    //      //LEFT GRID VALUES
    //
    //
    //    jobject leftArea = env->CallObjectMethod(areasList, listGetMethodID, 0);
    //
    //    jobject leftResolutionRealVector = env->CallObjectMethod(leftArea, areaGetResolutionMethodID);
    //
    //    jdoubleArray leftResolutionArray = (jdoubleArray) env->CallObjectMethod(leftResolutionRealVector, toDoubleMethodID);
    //
    //    double resolutionBuffer [2];
    //
    //    env->GetDoubleArrayRegion(leftResolutionArray, 0, 2, resolutionBuffer);
    //
    //    vector<int> leftResolution;
    //
    //
    //    for (int i = 0; i < 2; i++) {
    //        leftResolution.push_back((int) resolutionBuffer[i]);
    //    }
    //
    //    jobject downLeftRealVector = env->CallObjectMethod(leftArea, downLeftMethodID);
    //
    //    jdoubleArray downLeftArray = (jdoubleArray) env->CallObjectMethod(downLeftRealVector, toDoubleMethodID);
    //
    //    double downLeftBuffer [2];
    //
    //    env->GetDoubleArrayRegion(downLeftArray, 0, 2, downLeftBuffer);
    //
    //    jobject topRight = env->CallObjectMethod(leftArea, topRightMethodID);
    //
    //    jdoubleArray topRightArray = (jdoubleArray) env->CallObjectMethod(topRight, toDoubleMethodID);
    //
    //    double topRightBuffer [2];
    //
    //    env->GetDoubleArrayRegion(topRightArray, 0, 2, topRightBuffer);
    //
    //    RealVector leftPmin(2, downLeftBuffer);
    //    RealVector rightPmax(2, topRightBuffer);
    //    
    //    
    //    
    //    if ( Debug::get_debug_level() == 5 ) {
    //        cout<<"No JNI LEFT: "<<leftPmin<<" "<<rightPmax<<endl;
    //    }
    //
    //    const Boundary * boundary = RpNumerics::getPhysics().getSubPhysics(0).getPreProcessedBoundary();
    //
    //
    //    GridValues leftGridValues(boundary, leftPmin, rightPmax, leftResolution);
    //
    //
    //
    //    //RIGHT GRID VALUES
    //
    //
    //    jobject rightArea = env->CallObjectMethod(areasList, listGetMethodID, 1);
    //
    //    jobject rightResolutionRealVector = env->CallObjectMethod(rightArea, areaGetResolutionMethodID);
    //
    //    jdoubleArray rightResolutionArray = (jdoubleArray) env->CallObjectMethod(rightResolutionRealVector, toDoubleMethodID);
    //
    //    env->GetDoubleArrayRegion(rightResolutionArray, 0, 2, resolutionBuffer);
    //
    //    vector<int> rightResolution;
    //
    //
    //    for (int i = 0; i < 2; i++) {
    //        rightResolution.push_back((int) resolutionBuffer[i]);
    //    }
    //
    //    jobject downRightRealVector = env->CallObjectMethod(rightArea, downLeftMethodID);
    //
    //    jdoubleArray downRightArray = (jdoubleArray) env->CallObjectMethod(downRightRealVector, toDoubleMethodID);
    //
    //
    //
    //    env->GetDoubleArrayRegion(downRightArray, 0, 2, downLeftBuffer);
    //
    //    jobject topRightR = env->CallObjectMethod(rightArea, topRightMethodID);
    //
    //    jdoubleArray topRightArrayR = (jdoubleArray) env->CallObjectMethod(topRightR, toDoubleMethodID);
    //
    //
    //
    //    env->GetDoubleArrayRegion(topRightArrayR, 0, 2, topRightBuffer);
    //
    //    RealVector leftPminR(2, downLeftBuffer);
    //    RealVector rightPmaxR(2, topRightBuffer);
    //
    //
    //    
    //    if ( Debug::get_debug_level() == 5 ) {
    //        cout<<"No JNI RIGHT: "<<leftPminR<<" "<<rightPmaxR<<endl;
    //    }
    //    
    //
    //    GridValues rightGridValues(boundary, leftPminR, rightPmaxR, rightResolution);
    //
    //
    //    // Storage space for the segments:
    //    std::vector<RealVector> left_vrs;
    //    std::vector<RealVector> right_vrs;
    //
    //    const FluxFunction * leftFlux = &RpNumerics::getPhysics().fluxFunction();
    //    const AccumulationFunction * leftAccum = &RpNumerics::getPhysics().accumulation();
    //
    //    const FluxFunction * rightFlux = leftFlux;
    //    const AccumulationFunction * rightAccum = leftAccum;
    //
    //
    //    Secondary_Bifurcation sb;
    //
    //    sb.curve(leftFlux, leftAccum, leftGridValues,
    //            rightFlux, rightAccum, rightGridValues,
    //            left_vrs, right_vrs);
    //
    //    if ( Debug::get_debug_level() == 5 ) {
    //        cout << "left_vrs.size()  = " << left_vrs.size() << endl;
    //        cout << "right_vrs.size()  = " << right_vrs.size() << endl;
    //    }
    //
    //
    //    if (left_vrs.size() == 0 || right_vrs.size() == 0)return NULL;
    //
    //
    //    for (unsigned int i = 0; i < left_vrs.size() / 2; i++) {
    //
    //
    //        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
    //        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);
    //
    //        double * leftCoords = (double *) left_vrs.at(2 * i);
    //        double * rightCoords = (double *) left_vrs.at(2 * i + 1);
    //
    //
    //        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
    //        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);
    //
    //        //Construindo left e right points
    //        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);
    //
    //        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);
    //
    //
    //        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
    //        env->CallObjectMethod(leftSegmentsArray, arrayListAddMethod, realSegment);
    //
    //    }
    //
    //    for (unsigned int i = 0; i < right_vrs.size() / 2; i++) {
    //
    //
    //
    //        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
    //        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);
    //
    //        double * leftCoords = (double *) right_vrs.at(2 * i);
    //        double * rightCoords = (double *) right_vrs.at(2 * i + 1);
    //
    //
    //        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
    //        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);
    //
    //        //Construindo left e right points
    //        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);
    //
    //        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);
    //        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
    //
    //
    //        env->CallObjectMethod(rightSegmentsArray, arrayListAddMethod, realSegment);
    //
    //    }
    //
    //
    //    jobject result = env->NewObject(hysteresisCurveClass, hysteresisCurveConstructor, leftSegmentsArray, rightSegmentsArray);
    //
    //
    //    return result;
    //    


}











