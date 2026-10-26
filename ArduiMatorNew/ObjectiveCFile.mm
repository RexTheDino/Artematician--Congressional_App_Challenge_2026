//
//  ObjectiveCFile.mm
//  ArduiMatorNew
//
//  Created by AnshuDiya on 8/20/26.
//

#import <Foundation/Foundation.h>
#include "CplusplusLogic.hpp"
#include <vector>
#include <cmath>


//static Tester Testing;

GraphingApplication GraphingApplicationInstance(300,1000,1.0);

extern "C" {
void AddGrapherInstanceAlias(char EquationType, float xRangeValue, float yRangeValue, float resolutionValue){
    GraphingApplicationInstance.AddGrapherInstance(EquationType, xRangeValue, yRangeValue, resolutionValue);
    
}
}

extern "C" {

auto beginTime = std::chrono::high_resolution_clock::now();
auto endTime = std::chrono::high_resolution_clock::now();

void logChronoStart(){//start stopwatch
    static bool hasBeenCalled = false;
    if (!hasBeenCalled){
        beginTime = std::chrono::high_resolution_clock::now();
        hasBeenCalled = true;
    }
}
void logChronoStopAndPrint(){//stop stopwatch and print total time
    static bool hasBeenCalled = false;
    if(!hasBeenCalled){
        endTime = std::chrono::high_resolution_clock::now();
        auto totalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime-beginTime);
        std::cout << "Total Time: "; std::cout << totalTime.count();
        hasBeenCalled = true;
    }
}

void SubtypeTesterPrintCoordinatesAlias(char EquationType, float xRangeValue, float yRangeValue, int addOrDeleteAlias, int termPosition, float termExponent, float termCoefficient, int termIdentifier) {
    bool addOrDelete;
    switch (addOrDeleteAlias) {
        case 0:
            addOrDelete = false;
            break;
            
        default:
            addOrDelete = true;
            break;
    }
    //call the function
    GraphingApplicationInstance.CurrentGraphingInstance->SubtypeTesterPrintCoordinates(EquationType, addOrDelete, termPosition, termExponent, termCoefficient, termIdentifier);
}

void CoordinatesUpdaterForSwiftAlias(const char* inputEquation, float* SwiftBufferPointer, float xRange, float yRange, float resol){
    std::string inputExpression(inputEquation); //converts swift-passed null-terminated string to string c++ can read
    GraphingApplicationInstance.CurrentGraphingInstance->setInitGrapherValues(xRange, yRange, resol);
    //GraphingApplicationInstance.CurrentGraphingInstance->CoordinatesUpdaterForSwift(inputExpression, SwiftBufferPointer);
}

void primitiveAdder(const char* primitiveEqn, int32_t numberOfPrimitives, int32_t objLvl, int32_t objPos){
    //SINCE POINTER IS PROVIDED TO AN ARRAY OF MULTIPLE NULL TERMINATED STRINGS WE NEED TO SORT EACH ONE OUT!!
    int sizeOfPreviousChars = 0;
    for(int g = 0; g < numberOfPrimitives; g++){
        std::string mainEqn((primitiveEqn+sizeOfPreviousChars));//increment sizeOfPreviousChars so pointer always ends up on a new string
        sizeOfPreviousChars += mainEqn.size()+1;//+1 to accomodate for the \0
        GraphingApplicationInstance.ObjectMasterInstance->addObjectPrimitive(mainEqn, objLvl, objPos);
    }
}

void mainEqnObjectModifiersAlias(int32_t objectLevel, int32_t objectPosition, const float* DerSliderValues, int32_t SliderCnt,const int32_t* levelPositionCoordsOfEqnModifierObjects,int32_t ObjCnt){
    std::vector<float> SliderValues; SliderValues.reserve(100);
    std::vector<int> ObjCoords; ObjCoords.reserve(200);
    for(int k = 0; k < SliderCnt; k++){
        SliderValues.push_back(*(DerSliderValues+k));//appends slider values to the vector to be passed to c++
    }
    for(int f = 0; f < (ObjCnt*2); f++){ //objectCount*2 since each object has 2 values assigned, level and position
        ObjCoords.push_back(*(levelPositionCoordsOfEqnModifierObjects+f));//increments pointer and pushes back level,pos coords onto vector
    }
    GraphingApplicationInstance.ObjectMasterInstance->mainEqnObjectModifiers(objectLevel, objectPosition, SliderValues, ObjCoords);
}

void setObjectRotationDependencyAlias(const char* rotationEquation, int32_t objectLevel, int32_t objectPosition, const int32_t* levelPositionCoordsOfEqnModifierObjects, int32_t ObjCnt, const float* DerSliderValues, int32_t SldrCnt, int32_t rotEqnMode, int32_t IndiceThatsSelected){
    
    std::string rotEqn(rotationEquation);
    std::vector<float> SliderValues; SliderValues.reserve(100);
    std::vector<int> ObjCoords; ObjCoords.reserve(200);
    for(int h = 0; h < SldrCnt; h++){
        SliderValues.push_back(*(DerSliderValues+h));//pointer arithmetic
    }
    for(int w = 0; w < (ObjCnt*2); w++){
        ObjCoords.push_back(*(levelPositionCoordsOfEqnModifierObjects+w));
    }
    GraphingApplicationInstance.ObjectMasterInstance->setObjectRotationDependency(rotEqn, objectLevel, objectPosition, ObjCoords, SliderValues, rotEqnMode, IndiceThatsSelected);
}

void setObjectTranslationDependencyAlias(const char* translationEquation, int32_t objectLevel, int32_t objectPosition, const int32_t* levelPositionCoordsOfEqnModifierObjects, int32_t ObjCnt, const float* DerSliderValues, int32_t SldrCnt, int32_t translEqnMode, int32_t IndiceThatsSelected){
    std::string translEqn(translationEquation);
    std::vector<float> SliderValues; SliderValues.reserve(100);
    std::vector<int> ObjCoords; ObjCoords.reserve(200);
    for(int y = 0; y<(ObjCnt*2); y++){
        ObjCoords.push_back(*(levelPositionCoordsOfEqnModifierObjects+y));
    }
    for(int u = 0; u < SldrCnt; u++){
        SliderValues.push_back(*(DerSliderValues+u));
    }
    GraphingApplicationInstance.ObjectMasterInstance->setObjectTranslationDependency(translEqn, objectLevel, objectPosition, ObjCoords, SliderValues, translEqnMode, IndiceThatsSelected);
}

//initialize cross section object
void setCrossSectionObjectAlias(const char* PointerToEqn, float yRangeVal, float resolutionVal){
    std::string crossSecPrimEqn(PointerToEqn);
    GraphingApplicationInstance.ObjectMasterInstance->setCrossSectionEqn(crossSecPrimEqn);
    GraphingApplicationInstance.ObjectMasterInstance->setCrossSectionYrange(yRangeVal);
    GraphingApplicationInstance.ObjectMasterInstance->setCrossSectionResol(resolutionVal);
}

void setCrossSectionObjDependencyAlias(const float* SliderValsPtr, int numberOfSliderVals, const int* ModObjsCoordsPtr, int ModObjCnt){
    std::vector<float> DerSliderVals;
    for(int u = 0; u < numberOfSliderVals; u++){
        DerSliderVals.push_back(SliderValsPtr[u]);
    }
    std::vector<int> ModObjCoords;
    for(int n = 0; n < ModObjCnt*2; n++){
        ModObjCoords.push_back(ModObjsCoordsPtr[n]);
    }
    GraphingApplicationInstance.ObjectMasterInstance->crossSectionEqnModifiers(DerSliderVals, ModObjCoords);
}

void addLightObjectAlias(const char* PointerToXYeqn, const char* PointerToZDirEqn, const char* PointerToIntensityEqn){
    std::string XYeqn(PointerToXYeqn); std::string ZDirEqn(PointerToZDirEqn); std::string IntensityEqn(PointerToIntensityEqn);
    GraphingApplicationInstance.ObjectMasterInstance->addLightEqns(XYeqn, IntensityEqn, ZDirEqn);
}
    void addXYeqnDependencyAlias(int lightObjPos, const float* SliderValsPtr, int numSldrs, const int* ModObjCoordsPtr, int ObjCnt){
        std::vector<int> ModObjs;
        for(int v = 0; v < ObjCnt*2; v++){
            ModObjs.push_back(ModObjCoordsPtr[v]);
        }
        std::vector<float> SldrVals;
        for(int b = 0; b < numSldrs; b++){
            SldrVals.push_back(SliderValsPtr[b]);
        }
        GraphingApplicationInstance.ObjectMasterInstance->addXYeqnModifiers(lightObjPos, SldrVals, ModObjs);
    }
        void addZDirDependencyAlias(int zDirObjPos, const float* SliderValsPtr, int numberOfSldrs, const int* ModObjCoordsPtr, int ObjCnt){
            std::vector<int> ModObjs;
            for(int v = 0; v < ObjCnt*2; v++){
                ModObjs.push_back(ModObjCoordsPtr[v]);
            }
            std::vector<float> SldrVals;
            for(int b = 0; b < numberOfSldrs; b++){
                SldrVals.push_back(SliderValsPtr[b]);
            }
            GraphingApplicationInstance.ObjectMasterInstance->ZEqnModifiers(zDirObjPos, SldrVals, ModObjs);
        }
        void addIntensityDependencyAlias(int intensityObjPos, const float* SldrValsPtr, int numberOfSldrs, const int* ModObjCoordsPtr, int ObjCnt){
            std::vector<int> ModObjs;
            for(int v = 0; v < ObjCnt*2; v++){
                ModObjs.push_back(ModObjCoordsPtr[v]);
            }
            std::vector<float> SldrVals;
            for(int b = 0; b < numberOfSldrs; b++){
                SldrVals.push_back(SldrValsPtr[b]);
            }
            GraphingApplicationInstance.ObjectMasterInstance->intensityEqnModifiers(intensityObjPos, SldrVals, ModObjs);
        }

void addClrObjectAlias(const char* RedEqnPtr, const char* GreenEqnPtr, const char* BlueEqnPtr, const char* ReflectivityEqnPtr){//add an object of color/refl type
    std::string redEqn(RedEqnPtr); std::string blueEqn(BlueEqnPtr); std::string greenEqn(GreenEqnPtr); std::string reflEqn(ReflectivityEqnPtr);
    GraphingApplicationInstance.ObjectMasterInstance->addClrEqn(redEqn, greenEqn, blueEqn, reflEqn);
}
        void colorEqnModifiersAlias(const float* SldrValsRedPtr, int numSldrsRed, const float* SldrValsGreenPtr, int numSldrsGreen, const float* SldrValsBluePtr, int numSldrsBlue, const float* SldrValsReflPtr, int numSldrsRefl, const int* ModObjsRedPtr, int numObjsRed, const int* ModObjsGreenPtr, int numObjsGreen, const int* ModObjsBluePtr, int numObjsBlue, const int* ModObjsReflPtr, int numObjsRefl, int clrObjPos){//wtf
            
            std::vector<float> RedSldrs, GreenSldrs, BlueSldrs, ReflSldrs; std::vector<int> RedObjs, GreenObjs, BlueObjs, ReflObjs;
                for(int k = 0; k < numSldrsRed; k++){
                    RedSldrs.push_back(SldrValsRedPtr[k]);
                }
                for(int k = 0; k < numSldrsGreen; k++){
                    GreenSldrs.push_back(SldrValsGreenPtr[k]);
                }
                for(int k = 0; k < numSldrsBlue; k++){
                    BlueSldrs.push_back(SldrValsBluePtr[k]);
                }
                for(int k = 0; k < numSldrsRefl; k++){
                    ReflSldrs.push_back(SldrValsReflPtr[k]);
                }
            for(int k = 0; k < numObjsRed*2; k++){
                RedObjs.push_back(ModObjsRedPtr[k]);
            }
            for(int k = 0; k < numObjsGreen*2; k++){
                GreenObjs.push_back(ModObjsGreenPtr[k]);
            }
            for(int k = 0; k < numObjsBlue*2; k++){
                BlueObjs.push_back(ModObjsBluePtr[k]);
            }
            for(int k = 0; k < numObjsRefl*2; k++){
                ReflObjs.push_back(ModObjsReflPtr[k]);
            }
            GraphingApplicationInstance.ObjectMasterInstance->colorEqnModifiers(RedSldrs, GreenSldrs, BlueSldrs, ReflSldrs, RedObjs, GreenObjs, BlueObjs, ReflObjs, clrObjPos);
        }


void setInitValuesForObjectMasterAlias(float xRange, float yRange, float resol, float animVal, float* MetalBufferPtr, float* PtrToColorBuffer, float* PtrToLightBuffer, float* PtrToHeightBuffer, float* PtrToOffsetBuffer){
    GraphingApplicationInstance.ObjectMasterInstance->setInitValuesForObjMstr(xRange, yRange, resol, animVal, MetalBufferPtr, PtrToColorBuffer, PtrToLightBuffer, PtrToHeightBuffer, PtrToOffsetBuffer);
}

void setInitobjectMstrValuesAlias(){
    GraphingApplicationInstance.ObjectMasterInstance->setInitObjectMasterValues();
}
void resetObjAlias(int32_t objLvl, int32_t objPos){
    GraphingApplicationInstance.ObjectMasterInstance->resetObject(objLvl, objPos);//clear individual object
}
void resetObjMstrAlias(){
    GraphingApplicationInstance.ObjectMasterInstance->resetObjectMaster();
}
void resetAnimValAlias(){
    GraphingApplicationInstance.ObjectMasterInstance->resetAnimationValue();
}
    void calculateObjMstrAlias(bool descale, float descaleFactor = 1.0){
        if (!descale){
            GraphingApplicationInstance.ObjectMasterInstance->calculateObjectValues();
        } else {
            GraphingApplicationInstance.ObjectMasterInstance->calculateObjectValues(descale, descaleFactor);
        }
    }
void calculateDescaledObjMstrAlias(float currentDeScaleFactor){ //to be used to scale the frame from the bottom right to the middle with the animation. convert progress counter to the current deScale factor.
    GraphingApplicationInstance.ObjectMasterInstance->calculateObjectValues(true, currentDeScaleFactor);
}
    void calculateSelectedObjectsAlias(std::vector<int> PosCoordsOfObjToDraw, bool descale, float descaleFactor){
        GraphingApplicationInstance.ObjectMasterInstance->calculateOnlyTheseObjects(PosCoordsOfObjToDraw, descale, descaleFactor);
    }
    void calculateOnlyTheseLightObjectsAlias(std::vector<int> PosCoordsOfLightObjToDraw, bool descale, float descaleFactor){
        GraphingApplicationInstance.ObjectMasterInstance->calculateOnlyTheseLightObjects(PosCoordsOfLightObjToDraw, descale, descaleFactor);
    }
    void incrementAnimValAlias(float multiplier){
        GraphingApplicationInstance.ObjectMasterInstance->incrementAnimationValue(multiplier);
    }

//returns '0'/false until it reaches the end of duration at which point it returns '1'/true
//for 'reset' parameter, provide value greater than 0 to reset!
int animateObjMstr(float animMultiplier, float duration, int descale = 0,float descaleFactor = 1.0){ //duration provided in seconds.
    bool descaleBool; if(descale == 0){descaleBool = false;}else{descaleBool = true;}
    static int numberOfTimesCalled = 0;
    int maxNumTimesCalled = duration*60;//number of frames it needs to be called for at 60 fps. will stop drawing and return false when it reaches that point
    if(numberOfTimesCalled < maxNumTimesCalled){
        calculateObjMstrAlias(descaleBool, descaleFactor);//increments animVal and writes to metal buffer with each anim increment
        incrementAnimValAlias(animMultiplier);
        numberOfTimesCalled++;//increments number of times called
        return 0;
    } else {
        numberOfTimesCalled = 0; //resets number of times called to 0. so if this function is called again it will reanimate. therefore in ssift have a seperate variable toggled to true when this returns true and that variable controls wether this runs and is reset when the user clicks the calculate button.
        resetAnimValAlias();
        return 1;//returns false when reached end of duration
    }

}
//only called when only the selected objects are to be animated
int animateSelectedObjects(float animMultiplier, float duration, const int* PtrToPosCoordsOfObjs, const int ObjCnt, int descale = 0, float descaleFactor = 1.0){
    std::vector<int> PosCoordsOfObjs;//to be filled with the coords of the selected objects to draw.
    for(int p = 0; p < ObjCnt; p++){
        int ObjLvl = *(PtrToPosCoordsOfObjs+(p*2));//grab the object level of the specific objects to draw
        int ObjPos = *(PtrToPosCoordsOfObjs+1+(p*2));//grab the position coords of the specified objects to draw.
        PosCoordsOfObjs.push_back(ObjLvl);
        PosCoordsOfObjs.push_back(ObjPos);
    }
    bool descaleBool; if(descale == 0){descaleBool = false;}else{descaleBool = true;}
    static int numberOfTimesCalled = 0;
    int maxNumTimesCalled = duration*60;//number of frames it needs to be called for at 60 fps. will stop drawing and return false when it reaches that point
    if(numberOfTimesCalled < maxNumTimesCalled){
        calculateSelectedObjectsAlias(PosCoordsOfObjs,descaleBool,descaleFactor);//increments animVal and writes to metal buffer with each anim increment
        incrementAnimValAlias(animMultiplier);
        numberOfTimesCalled++;//increments number of times called
        return 0;
    } else {
        numberOfTimesCalled = 0; //resets number of times called to 0. so if this function is called again it will reanimate. therefore in ssift have a seperate variable toggled to true when this returns true and that variable controls wether this runs and is reset when the user clicks the calculate button.
        resetAnimValAlias();
        return 1;//returns false when reached end of duration
    }
}
//only called when only the selected objects are to be animated
int animateSelectedLightObjects(float animMultiplier, float duration, const int* PtrToPosCoordsOfLightObjs, const int ObjCnt, int descale = 0, float descaleFactor = 1.0){
    std::vector<int> PosCoordsOfLightObjs;//to be filled with the coords of the selected objects to draw.
    for(int p = 0; p < ObjCnt; p++){
        int LightObjPos = *(PtrToPosCoordsOfLightObjs+p);//grab the position coords of the specified objects to draw.
        PosCoordsOfLightObjs.push_back(LightObjPos);
    }
    bool descaleBool; if(descale == 0){descaleBool = false;}else{descaleBool = true;}
    static int numberOfTimesCalled = 0;
    int maxNumTimesCalled = duration*60;//number of frames it needs to be called for at 60 fps. will stop drawing and return false when it reaches that point
    if(numberOfTimesCalled < maxNumTimesCalled){
        calculateOnlyTheseLightObjectsAlias(PosCoordsOfLightObjs, descale, descaleFactor);//increments animVal and writes to metal buffer with each anim increment
        incrementAnimValAlias(animMultiplier);
        numberOfTimesCalled++;//increments number of times called
        return 0;
    } else {
        numberOfTimesCalled = 0; //resets number of times called to 0. so if this function is called again it will reanimate. therefore in ssift have a seperate variable toggled to true when this returns true and that variable controls wether this runs and is reset when the user clicks the calculate button.
        resetAnimValAlias();
        return 1;//returns false when reached end of duration
    }
}

void animateStaticObjMstr(float descaleFactor){
    //simply calls function
    calculateDescaledObjMstrAlias(descaleFactor);
}
//doesnt work
void animateSelectedStaticObjects(const int* PtrToPosCoordsOfObjs, const int ObjCnt){
    std::vector<int> PosCoordsOfObjs;//to be filled with the coords of the selected objects to draw.
    for(int p = 0; p < ObjCnt; p++){
        int ObjLvl = *(PtrToPosCoordsOfObjs+(p*2));//grab the object level of the specific objects to draw
        int ObjPos = *(PtrToPosCoordsOfObjs+1+(p*2));//grab the position coords of the specified objects to draw.
        PosCoordsOfObjs.push_back(ObjLvl);
        PosCoordsOfObjs.push_back(ObjPos);
    }
    //calculateSelectedObjectsAlias(PosCoordsOfObjs);//increments animVal and writes to metal buffer with each anim increment
}

void clearMetalAlias(){
    GraphingApplicationInstance.ObjectMasterInstance->clearMetalBuffer();
}


unsigned int returnPointsTerDrawAlias(){
    return (unsigned int)(GraphingApplicationInstance.ObjectMasterInstance->returnPointsTerDraw()); //returns current number of points to draw
}
}

