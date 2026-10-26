//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//


void SubtypeTesterPrintCoordinatesAlias(char EquationType, float xRangeValue, float yRangeValue, int addOrDeleteAlias, int termPosition, float termExponent, float termCoefficient, int termIdentifier);

void AddGrapherInstanceAlias(char EquationType, float xRangeValue, float yRangeValue, float resolutionValue);

void CoordinatesUpdaterForSwiftAlias(const char* inputEquation, float* SwiftBufferPointer, float xRange, float yRange, float resol);



//RELATED TO OBJECT MASTER
void primitiveAdder(const char* primitiveEqn, int numberOfPrimitives, int objLvl, int objPos);

void mainEqnObjectModifiersAlias(int objectLevel, int objectPosition, const float* DerSliderValues, int SliderCnt, const int* levelPositionCoordsOfEqnModifierObjects,int ObjCnt);

void setObjectRotationDependencyAlias(const char* rotationEquation, int objectLevel, int objectPosition, const int* levelPositionCoordsOfEqnModifierObjects, int ObjCnt, const float* DerSliderValues, int SldrCnt, int rotEqnMode, int IndiceThatsSelected);

void setObjectTranslationDependencyAlias(const char* translationEquation, int objectLevel, int objectPosition, const int* levelPositionCoordsOfEqnModifierObjects, int ObjCnt, const float* DerSliderValues, int SldrCnt, int translEqnMode, int IndiceThatsSelected);
void setInitobjectMstrValuesAlias();
void resetObjAlias(int objLvl, int objPos);
void resetObjMstrAlias();

void calculateObjMstrAlias();
void incrementAnimValAlias(float multiplier);

void setInitValuesForObjectMasterAlias(float xRange, float yRange, float resol, float animVal, float* MetalBufferPtr, float* PtrToColorBuffer, float* PtrToLightBuffer, float* PtrToHeightBuffer, float* PtrToOffsetBuffer);

//LIGHT, COLOR, AND CROSS-SEC OBJECT FUNCS!!!
void setCrossSectionObjectAlias(const char* PointerToEqn, float yRangeVal, float resolutionVal);
        void setCrossSectionObjDependencyAlias(const float* SliderValsPtr, int numberOfSliderVals, const int* ModObjsCoordsPtr, int ModObjCnt);

void addLightObjectAlias(const char* PointerToXYeqn, const char* PointerToZDirEqn, const char* PointerToIntensityEqn);
            void addXYeqnDependencyAlias(int lightObjPos, const float* SliderValsPtr, int numSldrs, const int* ModObjCoordsPtr, int ObjCnt);
            void addZDirDependencyAlias(int zDirObjPos, const float* SliderValsPtr, int numberOfSldrs, const int* ModObjCoordsPtr, int ObjCnt);
            void addIntensityDependencyAlias(int intensityObjPos, const float* SldrValsPtr, int numberOfSldrs, const int* ModObjCoordsPtr, int ObjCnt);
void addClrObjectAlias(const char* RedEqnPtr, const char* GreenEqnPtr, const char* BlueEqnPtr, const char* ReflectivityEqnPtr);
            void colorEqnModifiersAlias(const float* SldrValsRedPtr, int numSldrsRed, const float* SldrValsGreenPtr, int numSldrsGreen, const float* SldrValsBluePtr, int numSldrsBlue, const float* SldrValsReflPtr, int numSldrsRefl, const int* ModObjsRedPtr, int numObjsRed, const int* ModObjsGreenPtr, int numObjsGreen, const int* ModObjsBluePtr, int numObjsBlue, const int* ModObjsReflPtr, int numObjsRefl, int clrObjPos);
//LIGHT, COLOR, AND CROSS-SEC OBJECT FUNCS!!!


int animateObjMstr(float animMultiplier, float duration, int descale,float descaleFactor);    //only called when only the selected objects are to be animated
    int animateSelectedObjects(float animMultiplier, float duration, const int* PtrToPosCoordsOfObjs, const int ObjCnt, int descale, float descaleFactor);

void animateStaticObjMstr(float descaleFactor);
void animateSelectedStaticObjects(const int* PtrToPosCoordsOfObjs, const int ObjCnt);

void clearMetalAlias();

unsigned int returnPointsTerDrawAlias();
//RELATED TO OBJECT MASTER

void logChronoStart();
void logChronoStopAndPrint();
