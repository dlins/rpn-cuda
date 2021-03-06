/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JNIWaveCurveSpeedDiagram.cc
 **/

//! Definition of JNIWaveCurveSpeedDiagram
/*!
 *
 * TODO:
 *
 * NOTE :
 *
 * @ingroup JNI
 */

#include "rpnumerics_WaveCurve.h"

#include "RpNumerics.h"
#include "JNIDefs.h"
#include "WaveCurve.h"
#include <iostream>

JNIEXPORT jobject JNICALL Java_rpnumerics_WaveCurve_nativeDiagramCalc
(JNIEnv *env, jobject obj, jint family, jint curveID) {


    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);
    jclass diagramClass = env->FindClass(DIAGRAM_LOCATION);
    jclass diagramLineClass = env->FindClass(DIAGRAMLINE_LOCATION);


    jmethodID diagramConstructor = env->GetMethodID(diagramClass, "<init>", "(Ljava/util/List;)V");
    jmethodID diagramLineConstructor = env->GetMethodID(diagramLineClass, "<init>", "(Ljava/util/List;)V");

    jmethodID diagramLineDefaultConstructor = env->GetMethodID(diagramLineClass, "<init>", "()V");

    jmethodID addPartMethodID = env->GetMethodID(diagramLineClass, "addPart", "(Ljava/util/List;)V");
    
    
    jmethodID setLineNameMethodID = env->GetMethodID(diagramLineClass, "setName", "(Ljava/lang/String;)V");


    jmethodID setTypeMethodID = env->GetMethodID(diagramLineClass, "setType", "(II)V");



    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");

    int dimension = 2;


    std::vector<Curve> arclength_speed, arclength_eigenvalues;
    std::vector<Curve> arclength_reference_eigenvalues;


    const WaveCurve * waveCurve = RpNumerics::getWaveCurve(curveID);


    waveCurve->speed_map(arclength_speed, arclength_eigenvalues, arclength_reference_eigenvalues);


    jobject diagramLinesList = env->NewObject(arrayListClass, arrayListConstructor, NULL);


    jobject speedLine = env->NewObject(diagramLineClass, diagramLineDefaultConstructor, NULL);
    

    
    jstring speedLineName = env->NewStringUTF("Speed");
    
     
    env->CallVoidMethod(speedLine,setLineNameMethodID,speedLineName);

    jobject speedLineList = env->NewObject(arrayListClass, arrayListConstructor, NULL); //ADDING SPEED 

    for (int i = 0; i < arclength_speed.size(); i++) {

        jobject speedLinePartList = env->NewObject(arrayListClass, arrayListConstructor, NULL);

        int partType = arclength_speed[i].type;
        for (int j = 0; j < arclength_speed[i].curve.size(); j++) {

            jdoubleArray speedArray = env->NewDoubleArray(dimension);
            env->SetDoubleArrayRegion(speedArray, 0, dimension, (double *) arclength_speed[i].curve.at(j));
            jobject realVector = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, speedArray);
            env->CallObjectMethod(speedLinePartList, arrayListAddMethod, realVector);


        }
        env->CallObjectMethod(speedLine, addPartMethodID, speedLinePartList);
        env->CallObjectMethod(speedLine, setTypeMethodID, i, partType);


    }

        env->CallObjectMethod(diagramLinesList, arrayListAddMethod, speedLine);



    //REF POINT SPEED 

    for (int i = 0; i < arclength_reference_eigenvalues.size(); i++) {

        jobject eigenLine = env->NewObject(diagramLineClass, diagramLineDefaultConstructor, NULL);
        
        stringstream lineName ;
        lineName <<"Reference point eigen value "<<i;
        
        jstring speedLineName = env->NewStringUTF(lineName.str().c_str());
        env->CallVoidMethod(eigenLine,setLineNameMethodID,speedLineName);

        jobject speedLinePartList = env->NewObject(arrayListClass, arrayListConstructor, NULL);

        for (int j = 0; j < arclength_reference_eigenvalues[i].curve.size(); j++) {

            jdoubleArray speedArray = env->NewDoubleArray(dimension);

            env->SetDoubleArrayRegion(speedArray, 0, dimension, (double *) arclength_reference_eigenvalues[i].curve.at(j));

            jobject realVector = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, speedArray);

            //            //cout << arclength_reference_eigenvalues[i].curve.at(j) << endl;

            env->CallObjectMethod(speedLinePartList, arrayListAddMethod, realVector);


        }

        env->CallObjectMethod(eigenLine, setTypeMethodID, 0, 0);// Pq nao passa duas vezes ??
        env->CallObjectMethod(eigenLine, setTypeMethodID, 1, 1);

        //        env->CallObjectMethod(linePartsList, arrayListAddMethod, speedLinePartList);
        env->CallObjectMethod(eigenLine, addPartMethodID, speedLinePartList);



        env->CallObjectMethod(diagramLinesList, arrayListAddMethod, eigenLine);
    }









    for (int eigenValueIndex = 1; eigenValueIndex < dimension + 1; eigenValueIndex++) {// Numero de auto valores


        jobject eigenLine = env->NewObject(diagramLineClass, diagramLineDefaultConstructor, NULL);
        
        
         stringstream lineName ;
        lineName <<"Eigen value "<<eigenValueIndex-1;
        
        jstring speedLineName = env->NewStringUTF(lineName.str().c_str());
        env->CallVoidMethod(eigenLine,setLineNameMethodID,speedLineName);
        
        
        
//        jobject eigenLineList = env->NewObject(arrayListClass, arrayListConstructor, NULL); //EIGEN VALUES 


        for (int i = 0; i < arclength_eigenvalues.size(); i++) { //Quantidade de subcurvas da curva de onda


            jobject speedLinePartList = env->NewObject(arrayListClass, arrayListConstructor, NULL);


            for (int j = 0; j < arclength_eigenvalues[i].curve.size(); j++) { //Pontos dentro de cada subcurva


                jdoubleArray speedArray = env->NewDoubleArray(dimension);

                double temp[dimension];

                temp[0] = arclength_eigenvalues[i].curve[j](0);
                temp[1] = arclength_eigenvalues[i].curve[j](eigenValueIndex);


                env->SetDoubleArrayRegion(speedArray, 0, dimension, temp);

                jobject realVector = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, speedArray);

                //            //cout << arclength_speed[i].curve.at(j) << endl;

                env->CallObjectMethod(speedLinePartList, arrayListAddMethod, realVector);


            }
            
            env->CallObjectMethod(eigenLine, setTypeMethodID, i, arclength_eigenvalues[i].type);// Pq nao passa duas vezes ??
            env->CallObjectMethod(eigenLine, addPartMethodID, speedLinePartList);

//            env->CallObjectMethod(eigenLineList, arrayListAddMethod, speedLinePartList);

        }

//        jobject eigenLine = env->NewObject(diagramLineClass, diagramLineConstructor, eigenLineList);

        env->CallObjectMethod(diagramLinesList, arrayListAddMethod, eigenLine);

    }







    //






    jobject diagram = (env)->NewObject(diagramClass, diagramConstructor, diagramLinesList);

    return diagram;




}




