//
//  CplusplusLogic.cpp
//  ArduiMatorNew
//
//  Created by AnshuDiya on 8/20/26.
//

#include "CplusplusLogic.hpp"
#include <vector>
#include <cmath>
#include <memory>


void GraphingApplication::AddGrapherInstance(char EquationType, float xRangeValue, float yRangeValue, float resolutionValue){
        std::unique_ptr<Grapher> TempUnique = std::make_unique<Grapher>(EquationType, xRangeValue, yRangeValue, resolutionValue);
        
        CurrentGraphingInstance = TempUnique.get();
        
        //GrapherPointerArray.push_back(std::move(TempUnique));
    
    
}

std::vector<float> GraphingApplication::returnCurrentInstanceCalculatedCoordinates(){
    return CurrentGraphingInstance->CurrentGraphingSubtypePointer->returnCoordinatesArray();
}

void GraphingApplication::PrintCurrentCoordinates(){
    std::vector<float> TempArray = CurrentGraphingInstance->printOrReturnCurrentCoords(true);
}

//assumes points are provided to it in an (x,y) format with x-values at even slots and y-values at odd slots
void GrapherStaticModifiers::rotate(std::vector<float>* PointerToArrayToBeModified, float theta, float rotateAboutX, float rotateAboutY){
    auto sizeOfArray = PointerToArrayToBeModified->size();

    float currentX = 0.0f;
    float currentY = 0.0f;
    
    float calculatedX = 0.0f;
    float calculatedY = 0.0f;
    for (int d = 0; d < sizeOfArray/2; d++) {

        currentX = (*PointerToArrayToBeModified)[d*2];
        currentY = (*PointerToArrayToBeModified)[1+(d*2)];
            calculatedX = rotateAboutX+((currentX-rotateAboutX)*std::cos(theta))-((currentY-rotateAboutY)*std::sin(theta));
            calculatedY = rotateAboutY+((currentX -rotateAboutX)*std::sinf(theta))+((currentY-rotateAboutY)*std::cosf(theta));
        (*PointerToArrayToBeModified)[d*2] = calculatedX;
        (*PointerToArrayToBeModified)[1+(d*2)] = calculatedY;
    }
}

void GrapherStaticModifiers::rotate(std::vector<float> *PointerToArrayToBeModified, float x, float y){
        //calculates angle to givwn point
        float theta = atan2f(y, x);
    int sizeOfArray = (int)(*PointerToArrayToBeModified).size();
    
    float currentX=0.0f;
    float currentY=0.0f;
    
    for (int s = 0; s < sizeOfArray/2; s++) {
        //currentX = (*PointerToArrayToModifyBy)[j*2];
        //currentY = (*PointerToArrayToModifyBy)[1+(j*2)];
        //theta = std::atan2f(currentY, currentX); //angle to coordinate pair in array to modify by
            //(*PointerToArrayToBeModified)[j*2] = (currentX*std::cosf(theta))-(currentY*std::sinf(theta)); //sets xValue fo array to modify
            //(*PointerToArrayToBeModified)[(j*2)+1] = (currentX*(std::sinf(theta)))+(currentY*std::cosf(theta));
        currentX = (*PointerToArrayToBeModified)[s*2];
        currentY = (*PointerToArrayToBeModified)[1+(s*2)];
            (*PointerToArrayToBeModified)[s*2] = (currentX*std::cosf(theta))-(currentY*std::sinf(theta)); //sets xValue fo array to modify
            (*PointerToArrayToBeModified)[(s*2)+1] = (currentX*(std::sinf(theta)))+(currentY*std::cosf(theta));//sets y-val of array to modify
    }
}

void GrapherStaticModifiers::rotate(std::vector<float> *PointerToArrayToBeModified, std::vector<float> *PointerToArrayToModifyBy){
    auto sizeOfArray = (*PointerToArrayToBeModified).size();
    if(sizeOfArray != (*PointerToArrayToModifyBy).size()){
        std::cout<<"bad!!! arrays dont match size will go out of bounds";
    }
    float theta;
    float currentX; //current x to take angle from
    float currentY; //current Y to take angle from
    
    
    for (int j = 0; j < sizeOfArray/2; j++) {
        currentX = (*PointerToArrayToModifyBy)[j*2];
        currentY = (*PointerToArrayToModifyBy)[1+(j*2)];
        theta = std::atan2f(currentY, currentX); //angle to coordinate pair in array to modify by
            currentX = (*PointerToArrayToBeModified)[j*2];
            currentY = (*PointerToArrayToBeModified)[1+(j*2)];
        
        
            (*PointerToArrayToBeModified)[j*2] = (currentX*std::cosf(theta))-(currentY*std::sinf(theta)); //sets xValue fo array to modify
            (*PointerToArrayToBeModified)[(j*2)+1] = (currentX*(std::sinf(theta)))+(currentY*std::cosf(theta));
    }
}

void GrapherStaticModifiers::rotateByArrayValue(std::vector<float> *PointerToArrayToBeModified, std::vector<float> *PointerToArrayToModifyBy){
    auto sizeOfArray = (*PointerToArrayToBeModified).size();
    if(sizeOfArray != (*PointerToArrayToModifyBy).size()){
        std::cout<<"bad!!! arrays dont match size will go out of bounds";
    }
    float yValToRotateBy; //to be set to y-vals of array to rotate by
    float currentX;
    float currentY;//to be set to x and y coords of array to rotate by
    for (int h = 0; h < sizeOfArray/2; h++) {
        currentX = (*PointerToArrayToBeModified)[h*2]; currentY = (*PointerToArrayToBeModified)[1+(h*2)];
            yValToRotateBy = (*PointerToArrayToModifyBy)[1+(h*2)];//theta set to y-val of array to rotate by
        
        (*PointerToArrayToBeModified)[h*2] = (currentX*std::cosf(yValToRotateBy))-(currentY*std::sinf(yValToRotateBy));//rotates by theta=yVal of array to rotate by
        (*PointerToArrayToBeModified)[(h*2)+1] = (currentX*(std::sinf(yValToRotateBy)))+(currentY*std::cosf(yValToRotateBy));
    }
}

void GrapherStaticModifiers::translate(std::vector<float> *PointerToArrayToBeModified, float xTranslation, float yTranslation){
    auto sizeOfArray = (*PointerToArrayToBeModified).size();
    for (int g = 0; g < sizeOfArray; g++) {
        switch (g%2) {
            case 0:
                (*PointerToArrayToBeModified)[g] += xTranslation;
                break;
            case 1:
                (*PointerToArrayToBeModified)[g] += yTranslation;
        }
    }
}


void GrapherStaticModifiers::translate(std::vector<float>* PointerToArrayToBeModified, std::vector<float>* PointerToArrayToModifyBy, int translMode){
    auto sizeOfArray = (*PointerToArrayToBeModified).size();
    if(sizeOfArray!=(*PointerToArrayToModifyBy).size()){
        std::cout<<"bad!! array sizes dont match";
    }
    if(translMode == 0){//translate in both x and y directions
        for (int k = 0; k < sizeOfArray; k++) {
            (*PointerToArrayToBeModified)[k] += (*PointerToArrayToModifyBy)[k]; //add y1 of second to y1 of first, x1 of second ot x1 of first, etc. dont need a switch case statement since the x's and y's match indices
        }
    } else if(translMode == 1){
        auto sizeOfOnlyXpoints = sizeOfArray/2;
        for(int j = 0; j < sizeOfOnlyXpoints; j++){
            (*PointerToArrayToBeModified)[j*2] += (*PointerToArrayToModifyBy)[j*2];//only translate horizontally
        }
    } else if(translMode == 2){
        auto sizeOfOnlyYpoints = sizeOfArray/2;
        for (int b = 0; b < sizeOfOnlyYpoints; b++) {
            (*PointerToArrayToBeModified)[1+(b*2)] += (*PointerToArrayToModifyBy)[1+(b*2)];//only translate vertically
        }
    }
}


GraphingApplication::ObjectMaster::ObjectMaster(){
    IntermediateObjectlevelPoints.reserve(10000000); //reserves 20 million points to store the coordinates of all object sub-levels except the very top. DO NOT create multiple object masters just lke you shouldnt create multiple Graphing Applications. a single objectMaster can handle multiple objects at the top level so no need to create multiple object masters.
    RawObjectPoints.reserve(10000000);
    LightInfArray.reserve(100000);
    HeightLookupArray.reserve(10000);
    ClrAndReflctArray.reserve(100000);
        CrossSecObj.GrapherPointerArray.resize(1);//resizes by one to hold singular pprimitive
}

GraphingApplication::ObjectMaster::Object::Object(){
    
    //resizes vectors to prevent lag further down
    ConstantModifierObjectsMainEqn.reserve(100);
    ConstantModifierSliderValuesMainEqn.reserve(100);
        ConstantModifierObjectsRotationEqn.reserve(100);
        ConstantModifierSliderValuesRotationEqn.reserve(100);
            ConstantModifierObjectsTranslationEqn.reserve(100);
            ConstantModifierSliderValuesTranslationEqn.reserve(100);
}

float& GraphingApplication::ObjectMaster::returnAnimationValue(){
    float& returnReference = animationValue;
    return returnReference;
}
void GraphingApplication::ObjectMaster::resetAnimationValue(){
    animationValue = 0.0; //resetted
}
void GraphingApplication::ObjectMaster::resetObjects(){
    ObjectArray.clear();
    lightInfObjArray.clear();
    ClrAndRflctivityObjArray.clear();
    ObjectArray.reserve(100);//reserving space for efficiency
    lightInfObjArray.reserve(100);
    ClrAndRflctivityObjArray.reserve(100);
}
void GraphingApplication::ObjectMaster::resetObject(int objectLvl, int objectPosition){
    if((objectLvl >= ObjectArray.size())||(objectPosition >= ObjectArray[objectLvl].size())){
        std::cout << "bad! out of bounds";
    }
    ObjectArray[objectLvl].erase(ObjectArray[objectLvl].begin()+objectPosition-1);//we are doing -1 since position is supposed to start at 0. so if position is 1 then index 0 erased
}

void GraphingApplication::ObjectMaster::resetObjectMaster(){
    resetAnimationValue();
    resetObjects();
} //resets the whole thing

void GraphingApplication::ObjectMaster::Object::setInitObjectValues(float xRange, float yRange, float resol){
    xRangeOfObject = xRange;
    yRangeOfObject = yRange;
    resolOfObject = resol;
    for(int m = 0; m < GrapherPointerArray.size(); m++){
        GrapherPointerArray[m].GrapherPointer->setInitGrapherValues(xRange, yRange, resol);//resets init values for each grapher instance in grapher array
    }
}

//creates new instane of grapher and stores equation in the same struct and adds that struct to the vector
void GraphingApplication::ObjectMaster::Object::AddGraph(std::string eqnForGrapherToAdd){
    
    GrapherAndEquation NewGrapherAndEquation;
        NewGrapherAndEquation.EquationForGrapher = eqnForGrapherToAdd;
        NewGrapherAndEquation.GrapherPointer = std::make_unique<Grapher>('P', xRangeOfObject, yRangeOfObject, resolOfObject);
    GrapherPointerArray.push_back(std::move(NewGrapherAndEquation));
}

void GraphingApplication::ObjectMaster::Object::ChangeGrapherEquation(std::string ChangedEquation, int GrapherPosition){
    GrapherAndEquation NewGrapherAndEquation;
        NewGrapherAndEquation.EquationForGrapher = ChangedEquation;
        NewGrapherAndEquation.GrapherPointer = std::make_unique<Grapher>('P', xRangeOfObject, yRangeOfObject, resolOfObject);
    GrapherPointerArray[GrapherPosition-1] = std::move(NewGrapherAndEquation);//sets new equation in the grapherPointerArray for the specified position
}

void GraphingApplication::ObjectMaster::Object::heightTableFuncs::calcCrossSecAndWriteToMaster(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    std::vector<float> dummyVector = {};
    int HashtagCounter = 0;
        auto& currentGrapher = CrossSecEqnAndGraph;
        auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
        //no modifiers for height Lookup primitive
        std::vector<float> SliderValues = {};
        
        float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
        PointerToObjectMasterInstance->HeightLookupArray.resize(300.0f*resolOfHeightTable); //resizes to previous size plsu size of primitive points to be output
    float* PtrToHeightLookupArray = &PointerToObjectMasterInstance->HeightLookupArray[0];
        std::vector<float> TempRawArray(300.0f*resolOfHeightTable);
        float* PointerToTempRawArrayElement = &TempRawArray[0];//&PointerToObjectMasterInstance->HeightLookupArray[sizeOfRawArray];
        
        //xRange will always be sqrt(8) since thats the max dist the fragment shader will ever be called for
        currentGrapher.GrapherPointer->setInitGrapherValues(std::sqrt(8.0f), yRangeOfHeightTable, resolOfHeightTable);//sets init values for the grapher belonging to lookup
        //supposed to fill lookup array with coords output
        currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, true, dummyVector, dummyVector, animationValue, PointerToTempRawArrayElement);//outputs as only Y-coords in non-NDC fashion. we now need to convert these y-coords to z-NDC which ranges from 0.0 to 1.0. this is done by taking the abs of all coordinates we calculate and scaling them down by the heightLookup yRange.
        for (int m = 0; m < (int)(300.0f*resolOfHeightTable); m++) {
            TempRawArray[m] /= yRangeOfHeightTable;//scaling down to 0.0-1.0
            TempRawArray[m] = std::abs(TempRawArray[m]);
                PtrToHeightLookupArray[m] = TempRawArray[m];//fills ObjMstr height lookup table with calculated values
        }
}

void GraphingApplication::ObjectMaster::Object::lightInfoObjFuncs::calculateNwriteToObjMstrLightInfArray(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    calculateMainEqns(PointerToObjectMasterInstance, ThisObjPtr);
    calculateZdirEqn(PointerToObjectMasterInstance, ThisObjPtr);
    calculateIntensityEqn(PointerToObjectMasterInstance, ThisObjPtr);
    if (ZDirCoords.size() != IntensityVals.size()){
        std::cout << "BAD! INTENSITY COORDS AND Z-DIR COORDS DONT MATCH WILL GO OUT OF BOUNDS";
    }
    std::vector<float>& ObjMstrLightInfoBufferPtr = PointerToObjectMasterInstance->LightInfArray;//make sure in the Mstr constructor that this is resized to such a size itll never run out
    float* ObjRawCoordsPtr = &ThisObjPtr->ObjectRawCoords[0];
    ObjMstrLightInfoBufferPtr.reserve(ZDirCoords.size());
    //we need a packedfloat_4 format of x,y,z,Intensity. we will loop through w for the length of the ZDir array in the format x,y,z,i = [(w*4)],[(w*4)+1],[(w*4)+2],[(w*4)+3]
    for (int w = 0; w < ZDirCoords.size(); w++) {
        ObjMstrLightInfoBufferPtr.push_back(ObjRawCoordsPtr[(w*2)]);
        ObjMstrLightInfoBufferPtr.push_back(ObjRawCoordsPtr[1+(w*2)]);
        ObjMstrLightInfoBufferPtr.push_back(ZDirCoords[w]);
        ObjMstrLightInfoBufferPtr.push_back(IntensityVals[w]);
    }
}
        void GraphingApplication::ObjectMaster::Object::lightInfoObjFuncs::calculateMainEqns(ObjectMaster *PointerToObjectMasterInstance, Object *ThisObjPtr){
            auto numberOfPrimitives = ThisObjPtr->GrapherPointerArray.size();
        int HashtagCounter = 0;
        for (int j = 0; j < 1; j++) { //loops through primitives adding their coords to the raw points array
            auto& currentGrapher = ThisObjPtr->GrapherPointerArray[0];
            auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
            std::vector<float> CoefficientArray = {}; CoefficientArray.reserve((int)(2*(300.0f/ThisObjPtr->resolOfObject)));
            for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
                if(eqnString[d] == '#'){
                    int ModifierobjectLevel = ThisObjPtr->ConstantModifierObjectsMainEqn[0+(HashtagCounter*2)]; //level of object that correspodns to this modifier hashtag
                    int ModifierObjectPosition = ThisObjPtr->ConstantModifierObjectsMainEqn[1+(HashtagCounter*2)]; //pos coords of object corresponding to Hashtag
                    
                    //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
                    std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
                    //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
                    CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
                    
                    HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
                }
            }//eqn modifier loop
            
            std::vector<float> SliderValues = {};
        if((!ThisObjPtr->ConstantModifierSliderValuesMainEqn.empty())||(numberOfPrimitives > 1)){
                std::cout << "Warning! Having main eqn slider values when there are more than one object(s)!";
            } else {
                SliderValues = ThisObjPtr->ConstantModifierSliderValuesMainEqn; //slider values to be used in main eqn
            }
            
            float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
            ThisObjPtr->ObjectRawCoords.resize((300.0f*ThisObjPtr->resolOfObject)*2); //resizes to previous size plsu size of primitive points to be output
            //takes the pointer ot the start of the element of the resized and empty portion of raw array
            float* PointerToRawArrayElement = &(ThisObjPtr->ObjectRawCoords[0]);
            
            //supposed to fill raw array with coords output by primitive
            currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, false, SliderValues, CoefficientArray, animationValue, PointerToRawArrayElement);
        }
        
        int sizeOfPrimitive = 2*(300.0f*ThisObjPtr->resolOfObject);
        ThisObjPtr->ObjectCenterCoords.resize(sizeOfPrimitive);
        
        //to be filled with rotation and translation eqn as well as calculating object center coords.
        
        ThisObjPtr->calculateRotation(PointerToObjectMasterInstance);
        ThisObjPtr->calculateTranslation(PointerToObjectMasterInstance);
        
        for (int s = 0; s < sizeOfPrimitive; s++) {
            ThisObjPtr->ObjectCenterCoords[s] = ThisObjPtr->ObjectRawCoords[s]; //fills cetner coords with raw coords.no averaging since a lightObject is only supposed to have one primitive
        }
    }
void GraphingApplication::ObjectMaster::Object::lightInfoObjFuncs::calculateZdirEqn(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    int HashtagCounter = 0;
        auto& currentGrapher = ZDirGraphNEqn;
        auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
        std::vector<float> CoefficientArray = {}; CoefficientArray.reserve((int)(300.0f/ThisObjPtr->resolOfObject));
        for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
            if(eqnString[d] == '#'){
                int ModifierobjectLevel = ConstModObjsZDirEqn[0+(HashtagCounter*2)]; //level of object that correspodns to this modifier hashtag
                int ModifierObjectPosition = ConstModObjsZDirEqn[1+(HashtagCounter*2)]; //pos coords of object corresponding to Hashtag
                
                //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
                std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
                //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
                CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
                
                HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
            }
        }//eqn modifier loop
        
        std::vector<float> SliderValues = {};
        if(!ConstModSldrValsZDirEqn.empty()){
            std::cout << "Warning! Having main eqn slider values when there are more than one object(s)!";
        } else {
            SliderValues = ConstModSldrValsZDirEqn; //slider values to be used in main eqn
        }
        
        float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
    ZDirCoords.resize(300*ThisObjPtr->resolOfObject);
    float* PtrToZDir = &ZDirCoords[0];
    //fills ZDir coords with calculated z-values (pure y-values)
    currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, true, SliderValues, CoefficientArray, animationValue, PtrToZDir);
}
//writes calculated intensity vals to this object's coords vector
void GraphingApplication::ObjectMaster::Object::lightInfoObjFuncs::calculateIntensityEqn(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    
    int HashtagCounter = 0;
        auto& currentGrapher = IntensityGraphNEqn;
        auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
        std::vector<float> CoefficientArray = {}; CoefficientArray.reserve((int)(300.0f/ThisObjPtr->resolOfObject));
        for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
            if(eqnString[d] == '#'){
                int ModifierobjectLevel = ConstModObjsIntensityEqn[0+(HashtagCounter*2)]; //level of object that correspodns to this modifier hashtag
                int ModifierObjectPosition = ConstModObjsIntensityEqn[1+(HashtagCounter*2)]; //pos coords of object corresponding to Hashtag
                
                //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
                std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
                //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
                CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
                
                HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
            }
        }//eqn modifier loop
        
        std::vector<float> SliderValues = {};
        if(!ConstModSldrValsIntensityEqn.empty()){
            std::cout << "Warning! Having main eqn slider values when there are more than one object(s)!";
        } else {
            SliderValues = ConstModSldrValsIntensityEqn; //slider values to be used in main eqn
        }
        
        float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
    IntensityVals.resize(300*ThisObjPtr->resolOfObject);
    float* PtrToIntensityVals = &IntensityVals[0];
    //pure y-values
    currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, true, SliderValues, CoefficientArray, animationValue, PtrToIntensityVals);
    
}

//packed_float4 where reflectivity goes first followed by rgb
void GraphingApplication::ObjectMaster::Object::clrAndRflctFuncs::calculateNwriteToObjMstrClrAndRflctArray(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    calculateClrsAndWrite(PointerToObjectMasterInstance, ThisObjPtr);
    reflectivityCalcAndWrite(PointerToObjectMasterInstance, ThisObjPtr);
    PointerToObjectMasterInstance->ClrAndReflctArray.reserve(BlueClrCoords.size()*4);
    std::vector<float>& PointerToClrReflArray = PointerToObjectMasterInstance->ClrAndReflctArray;
    for (int h = 0; h < RedClrCoords.size(); h++) {//fills with pakced_float4 format
        PointerToClrReflArray.push_back(ReflectivityCoords[h]);
        PointerToClrReflArray.push_back(RedClrCoords[h]);
        PointerToClrReflArray.push_back(GreenClrCoords[h]);
        PointerToClrReflArray.push_back(BlueClrCoords[h]);
    }
}

void GraphingApplication::ObjectMaster::Object::clrAndRflctFuncs::calculateClrsAndWrite(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    for (int u = 0; u < 3; u++) {
        int HashtagCounter = 0;
            auto& currentGrapher = rgbGrapherNEqn[u];
            auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
            std::vector<float> CoefficientArray = {}; CoefficientArray.reserve((int)(300.0f/ThisObjPtr->resolOfObject));
            for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
                if(eqnString[d] == '#'){
                    int ModifierobjectLevel = -1;
                    int ModifierObjectPosition = -1;//initialized to -1 to easily recognize memory violations
                    switch (u) {
                        case 0:
                            ModifierobjectLevel = ModObjsRed[0+(HashtagCounter*2)];
                            ModifierObjectPosition = ModObjsRed[1+(HashtagCounter*2)];
                            break;
                        case 1:
                            ModifierobjectLevel = ModObjsGreen[0+(HashtagCounter*2)];
                            ModifierObjectPosition = ModObjsGreen[1+(HashtagCounter*2)];
                            break;
                        case 2:
                            ModifierobjectLevel = ModObjsBlue[0+(HashtagCounter*2)];
                            ModifierObjectPosition = ModObjsBlue[1+(HashtagCounter*2)];
                            break;
                    }
                    
                    //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
                    std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
                    //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
                    CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
                    
                    HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
                }
            }//eqn modifier loop
            
            float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
        std::vector<float> SliderValues = {};
        float* PtrToClrCoords = nullptr;
        switch (u) {
            case 0:
                SliderValues = ModSlidersRed;
                RedClrCoords.resize(300.0f*ThisObjPtr->resolOfObject);
                PtrToClrCoords = &RedClrCoords[0];
                break;
            case 1:
                SliderValues = ModSlidersGreen;
                GreenClrCoords.resize(300.0f*ThisObjPtr->resolOfObject);
                PtrToClrCoords = &GreenClrCoords[0];
                break;
            case 2:
                SliderValues = ModSlidersBlue;
                BlueClrCoords.resize(300.0f*ThisObjPtr->resolOfObject);
                PtrToClrCoords = &BlueClrCoords[0];
                break;
        }
        //pure y-values
        currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, true, SliderValues, CoefficientArray, animationValue, PtrToClrCoords);
    }
}
void GraphingApplication::ObjectMaster::Object::clrAndRflctFuncs::reflectivityCalcAndWrite(ObjectMaster *PointerToObjectMasterInstance, Object* ThisObjPtr){
    int HashtagCounter = 0;
    auto& currentGrapher = ReflectivityGrapherNEqn;
    auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
    std::vector<float> CoefficientArray = {}; CoefficientArray.reserve((int)(300.0f/ThisObjPtr->resolOfObject));
    for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
        if(eqnString[d] == '#'){
            int ModifierobjectLevel = ModObjsReflectivity[0+(HashtagCounter*2)];
            int ModifierObjectPosition = ModObjsReflectivity[1+(HashtagCounter*2)];
            //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
            std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
            //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
            CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
            HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
        }
    }//eqn modifier loop
    
    float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
    std::vector<float> SliderValues = {};
    if (!ModSlidersReflectivity.empty()){SliderValues = ModSlidersReflectivity;}
    ReflectivityCoords.resize(300.0f*ThisObjPtr->resolOfObject);
    float* PtrToReflCoords = &ReflectivityCoords[0];
    //pure y-values
    currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, true, SliderValues, CoefficientArray, animationValue, PtrToReflCoords);
}


void GraphingApplication::ObjectMaster::Object::calculateValuesAndWriteToObjectMasterArray(ObjectMaster* PointerToObjectMasterInstance){
    if(objectMode == 0){
        
        auto numberOfPrimitives = GrapherPointerArray.size();
        
        int HashtagCounter = 0;
        for (int j = 0; j < numberOfPrimitives; j++) { //loops through primitives adding their coords to the raw points array
            auto& currentGrapher = GrapherPointerArray[j];
            auto eqnString = currentGrapher.EquationForGrapher; auto sizeOfEqnString = eqnString.size();
            std::vector<float> CoefficientArray = {}; CoefficientArray.reserve(numberOfPrimitives*(int(2*(300.0f/resolOfObject))));
            for (int d = 0; d < sizeOfEqnString; d++) { //EqnModifierLogicLoop
                if(eqnString[d] == '#'){
                    if(numberOfPrimitives > 1){
                        std::cout << "bad";//bad!! cant have more than one primitive in an object whose main equation has eqn modifiers
                    } //output bad
                    int ModifierobjectLevel = ConstantModifierObjectsMainEqn[0+(HashtagCounter*2)]; //level of object that correspodns to this modifier hashtag
                    int ModifierObjectPosition = ConstantModifierObjectsMainEqn[1+(HashtagCounter*2)]; //pos coords of object corresponding to Hashtag
                    
                    //The function returns a reference to a float vector that is then passed to the CoordinatesUpdater for the respective grapher as argument
                    std::vector<float>& ModifierVector = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(ModifierobjectLevel, ModifierObjectPosition);
                    //inserts the corresponding values of the object into the coefficient array to eventually be passed down to the grapher primitive
                    CoefficientArray.insert(CoefficientArray.end(), ModifierVector.begin(),ModifierVector.end());
                    
                    HashtagCounter++; //increments hashtag counter so the next hashtag goes to position 2 and etc.
                }
            }//eqn modifier loop
            
            std::vector<float> SliderValues = {};
            if((!ConstantModifierSliderValuesMainEqn.empty())&&(numberOfPrimitives > 1)){
                std::cout << "Warning! Having main eqn slider values when there are more than one object(s)!";
            } else {
                SliderValues = ConstantModifierSliderValuesMainEqn; //slider values to be used in main eqn
            }
            
            float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
            auto sizeOfRawArray = ObjectRawCoords.size();
            ObjectRawCoords.resize((sizeOfRawArray+((300.0f*resolOfObject)*2))); //resizes to previous size plsu size of primitive points to be output
            //takes the pointer ot the start of the element of the resized and empty portion of raw array
            float* PointerToRawArrayElement = &(ObjectRawCoords[sizeOfRawArray]);
            
            //supposed to fill raw array with coords output by primitive
            currentGrapher.GrapherPointer->CoordinatesUpdaterForSwift(eqnString, false, SliderValues, CoefficientArray, animationValue, PointerToRawArrayElement);
        }
        
        int sizeOfPrimitive = 2*(300.0f*resolOfObject);
        
        ObjectCenterCoords.resize(sizeOfPrimitive);
        
        
        //to be filled with rotation and translation eqn as well as calculating object center coords.
        calculateTranslation(PointerToObjectMasterInstance);
        calculateRotation(PointerToObjectMasterInstance);
        
        for (int s = 0; s < sizeOfPrimitive; s++) {
            float averageValueOfPrimitives = 0.0;
            for (int z = 0; z < numberOfPrimitives; z++) {
                averageValueOfPrimitives += ObjectRawCoords[(sizeOfPrimitive*z)+s]; //adds the value of the 'z' index of each primitive to avg value.
            }
            averageValueOfPrimitives /= numberOfPrimitives; //averages totalled values
            
            ObjectCenterCoords[s] = averageValueOfPrimitives; //fills wth average value
        }
        std::vector<float>* ObjMstrRawCoordsVectorPtr = &PointerToObjectMasterInstance->RawObjectPoints;
        std::vector<float>* ObjMstrAvgCoordsPtr = &PointerToObjectMasterInstance->IntermediateObjectlevelPoints;
        for (int e = 0; e < ObjectRawCoords.size(); e++) {
            (*ObjMstrRawCoordsVectorPtr).push_back(ObjectRawCoords[e]); //fils the end of object master raw coords with object raw coords
        }
        for (int n = 0; n < ObjectCenterCoords.size(); n++) {
            (*ObjMstrAvgCoordsPtr).push_back(ObjectCenterCoords[n]); //fils in end of object master object array with object center coords
        }
        // std::cout << "Done";
    }
}

void GraphingApplication::ObjectMaster::Object::calculateRotation(ObjectMaster *PointerToObjectMasterInstance){
    auto sizeOfRotEqn = RotationEquation.size();
    int hashtagCounter = 0;
    std::vector<float> CoefficientArray = {};
    std::vector<float>& CoeffArrayReference = CoefficientArray;
    for (int g = 0; g < sizeOfRotEqn; g++) {
        if(RotationEquation[g] == '#'){
            int objectLevelOfModifier = ConstantModifierObjectsRotationEqn[(0+(hashtagCounter*2))];
            int objectPositionOfModifier = ConstantModifierObjectsRotationEqn[1+(hashtagCounter*2)];
            //vector of object corresponding to hashtag modifier
                std::vector<float>& newModifierValues = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(objectLevelOfModifier, objectPositionOfModifier);
            //inserts corresponding hashtag values into coefficient array
            CoefficientArray.insert(CoefficientArray.end(), newModifierValues.begin(),newModifierValues.end());
            
            hashtagCounter++;
        }
    }
    Grapher GrapherTempInstance('P', xRangeOfObject, yRangeOfObject, resolOfObject); //temp grapher instance to calculate rotation
    std::vector<float> VectorToRotateBy((2*(300.0f*resolOfObject)),0.0); //creates vector with size points to be output by Grapher
    float* PointerToVectorToRotateBy = &VectorToRotateBy[0];
    
    float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
    
    //fills in a vector with the values to rotate by using a temporary instance of grapher
    //if rotation equation isnt set dont do anything
    if(RotationEquation != ""){
        GrapherTempInstance.CoordinatesUpdaterForSwift(RotationEquation, false, ConstantModifierSliderValuesRotationEqn, CoeffArrayReference, animationValue, PointerToVectorToRotateBy);
        
        //std::vector<float>* PointerToObjectRawCoords = &ObjectRawCoords; //pointer to the center obejct coords calculated by main eqn after averaging primitives
        std::vector<float>* PointerToRotateByVector = &VectorToRotateBy;
        auto numberOfPrimitives = (ObjectRawCoords.size()/(2*(resolOfObject*300.0f)));
        auto sizeOfPrimitive = 300*resolOfObject*2;
        for(int y = 0; y < numberOfPrimitives; y++){
            auto subsetStart = ObjectRawCoords.begin() + (y*sizeOfPrimitive);
            auto subsetEnd = subsetStart+sizeOfPrimitive;
            std::vector<float> RawCoordsSubet(subsetStart,subsetEnd);//initializes subset of raw coords with the current primitive's points so we can loop through them and with multiple calls to .rotate() instead of calculating RotateByVector again for every vector
            std::vector<float>* SubsetPointer = &RawCoordsSubet;
            
            GrapherStaticModifiers ModifierInstance;
            
            switch (RotEqnMode) {
                case 1://rotate by atan2 of provided array
                    ModifierInstance.rotate(SubsetPointer, PointerToRotateByVector);
                    break;
                case 2:
                    ModifierInstance.rotateByArrayValue(SubsetPointer, PointerToRotateByVector);
                    break;
                case 3:{//rotates by atan2 of the coordinate pair pointed by the indice set
                    float xValOfIndice = (*PointerToRotateByVector)[selectedIndiceRot]; float yValOfIndice = (*PointerToRotateByVector)[selectedIndiceRot+1];
                    ModifierInstance.rotate(SubsetPointer, xValOfIndice, yValOfIndice);}
                    break;
                case 4:{//rotates by y-val of the coordinate pointed by the indice
                    float yValOfIndice = (*PointerToRotateByVector)[selectedIndiceRot+1];//need to do +1 since indice points to an x-value and we need its corresponding y-val
                    ModifierInstance.rotate(SubsetPointer, yValOfIndice, 0.0, 0.0);}
                    break;
                default:
                    std::cout << "bad! calculateRotation could not find a matching test case";
                    break;
            }
            
            //commented out for now to test new function that is supposed ot rotate by y-val instead of atan coords
            //ModifierInstance.rotate(SubsetPointer, PointerToRotateByVector); //rotates by vector returned by tempGrapherInstance
            //ModifierInstance.rotateByArrayValue(SubsetPointer, PointerToRotateByVector);
                std::copy(RawCoordsSubet.begin(),RawCoordsSubet.end(),subsetStart); //copies modified points in RawCoordsSubset for the specified primtiive into ObjectRawCoords
        }
    }
}

void GraphingApplication::ObjectMaster::Object::calculateTranslation(ObjectMaster *PointerToObjectMasterInstance){
    auto sizeOfTranslEqn = TranslationEquation.size();
    int hashtagCounter = 0;
    std::vector<float> CoefficientArray = {};
    std::vector<float>& CoeffArrayReference = CoefficientArray;
    for (int g = 0; g < sizeOfTranslEqn; g++) {
        if(TranslationEquation[g] == '#'){
            int objectLevelOfModifier = ConstantModifierObjectsTranslationEqn[(0+(hashtagCounter*2))];
            int objectPositionOfModifier = ConstantModifierObjectsTranslationEqn[1+(hashtagCounter*2)];
            //vector of object corresponding to hashtag modifier
                std::vector<float>& newModifierValues = PointerToObjectMasterInstance->ReturnReferenceToRequestedObjectVector(objectLevelOfModifier, objectPositionOfModifier);
            //inserts corresponding hashtag values into coefficient array
            CoefficientArray.insert(CoefficientArray.end(), newModifierValues.begin(),newModifierValues.end());
            
            hashtagCounter++;
        }
    }
    Grapher GrapherTempInstance('P', xRangeOfObject, yRangeOfObject, resolOfObject); //temp grapher instance to calculate translation
    std::vector<float> VectorToTranslateBy((2*(300.0f*resolOfObject)),0.0); //creates vector with size points to be output by Grapher
    float* PointerToVectorToTranslateBy = &VectorToTranslateBy[0];
    
    float& animationValue = PointerToObjectMasterInstance->returnAnimationValue();
    
    //fills in a vector with the values to rotate by using a temporary instance of grapher
    //only execute when translation equation is set
    if(TranslationEquation != ""){
        GrapherTempInstance.CoordinatesUpdaterForSwift(TranslationEquation, false, ConstantModifierSliderValuesTranslationEqn, CoeffArrayReference, animationValue, PointerToVectorToTranslateBy);
        
        std::vector<float>* PointerToTranslateByVector = &VectorToTranslateBy;
        auto numberOfPrimitives = (ObjectRawCoords.size()/(2*(resolOfObject*300.0f)));
        auto sizeOfPrimitive = 300.0f*resolOfObject*2;
        for(int y = 0; y < numberOfPrimitives; y++){
            auto subsetStart = ObjectRawCoords.begin() + (y*sizeOfPrimitive);
            auto subsetEnd = subsetStart+sizeOfPrimitive;
            std::vector<float> RawCoordsSubset(subsetStart,subsetEnd);//initializes subset of raw coords with the current primitive's points so we can loop through them and with multiple calls to .rotate() instead of calculating RotateByVector again for every vector
            std::vector<float>* SubsetPointer = &RawCoordsSubset;//pointer to vector containing this primitive'spoints that are to be translated
            
            GrapherStaticModifiers ModifierInstance;
            switch (TranslEqnMode) {
                case 1:
                    ModifierInstance.translate(SubsetPointer, PointerToTranslateByVector, 1);//translate only horizontally by array
                    break;
                case 2:
                    ModifierInstance.translate(SubsetPointer, PointerToTranslateByVector, 2);//translate only vertically by array
                    break;
                case 3:
                    ModifierInstance.translate(SubsetPointer, PointerToTranslateByVector, 0);//translate both x and y directions
                    break;
                case 4:{
                    float yValOfSelectedIndice = (*PointerToTranslateByVector)[selectedIndiceTransl+1];//y-val to translate horizontally by
                    ModifierInstance.translate(SubsetPointer, yValOfSelectedIndice, 0.0);//only translate horizontally
                }
                    break;
                case 5:{
                    float yValOfSelectedIndice = (*PointerToTranslateByVector)[selectedIndiceTransl+1];
                    ModifierInstance.translate(SubsetPointer, 0.0, yValOfSelectedIndice);}
                    break;
                case 6:{//the indice provided is going to point to the x-value of the coordinate pair that we need to use for translation
                    float xValOfIndice = (*PointerToTranslateByVector)[selectedIndiceTransl+1]; float yValOfIndice = (*PointerToTranslateByVector)[selectedIndiceTransl+1];
                    ModifierInstance.translate(SubsetPointer, xValOfIndice, yValOfIndice);}
                    break;
                    
                default:
                    break;
            }
                std::copy(RawCoordsSubset.begin(), RawCoordsSubset.end(), subsetStart); //copies modified points in RawCoordsSubset for the specified primtiive into ObjectRawCoords
        }
    }
}


//object level and object position start from 1 with 1 being the lowest?? opposite to the parser
//if object already exists at indicated spot then simply adds a primtive to that object. else creates a new object with the primitive set.
void GraphingApplication::ObjectMaster::addObjectPrimitive(std::string primitiveEquation, int objectLevel, int objectPosition){
    //if object in required position does not exist resize to that position
    if(ObjectArray.size() <= objectLevel){ObjectArray.resize(objectLevel+1);}
    if(ObjectArray[objectLevel].size() <= objectPosition){ObjectArray[objectLevel].resize(objectPosition+1); ObjectArray[objectLevel][objectPosition].setInitObjectValues(xRangeOfObjectMaster,yRangeOfObjectMaster,resolOfObjectMaster);} //initializes the object with values preset if newly created
    
    ObjectArray[objectLevel][objectPosition].AddGraph(primitiveEquation); //adds primitive to a new instance of Grapher in corresponding object
    
}

void GraphingApplication::ObjectMaster::mainEqnObjectModifiers(int objectLevel, int objectPosition, std::vector<float> SliderValues, std::vector<int> levelPositionCoordsOfEqnModifierObjects){
    //if there is only one primitive then set the coords and slider values for that object to parameters. else throw error
    if(ObjectArray[objectLevel][objectPosition].GrapherPointerArray.size() == 1){
        Object& CurrentObject = ObjectArray[objectLevel][objectPosition];
        CurrentObject.ConstantModifierObjectsMainEqn = levelPositionCoordsOfEqnModifierObjects; //sets the specified object's main equation slider values to use the coordinates of the objects in the array provided
        CurrentObject.ConstantModifierSliderValuesMainEqn = SliderValues;
    }
}

//provide an equation to rotate the selected object as well as an array of x,y pairs that correspond to the objects the selected one depends on with corresponding to the various '#' in the rotation equation. Also provide slider values for '$' in the equation.
void GraphingApplication::ObjectMaster::setObjectRotationDependency(std::string rotationEquation, int objectLevel, int objectPosition, std::vector<int> levelPositionCoordsOfEqnModifierObjects, std::vector<float> SliderValues, int rotEqnMode, int IndiceToSelect){
    //if object is uninitialized then spit out error
    if(ObjectArray.size() <= objectLevel){std::cout <<"wtf?";} //super bad cuz adding rotation equation without primitive eqn???????
    if(ObjectArray[objectLevel].size() <= objectPosition){std::cout<<"wtf???";} //same prob as above
    Object& CurrentObject = ObjectArray[objectLevel][objectPosition];
    
    //sets rotation equation to given parameters and notes down coords of objects whose values to use in '#' as well as slider values
    CurrentObject.RotationEquation = rotationEquation;
    CurrentObject.ConstantModifierObjectsRotationEqn = levelPositionCoordsOfEqnModifierObjects;
    CurrentObject.ConstantModifierSliderValuesRotationEqn = SliderValues;
    CurrentObject.RotEqnMode = rotEqnMode;
    CurrentObject.selectedIndiceRot = IndiceToSelect;
}

void GraphingApplication::ObjectMaster::setObjectTranslationDependency(std::string translationEquation, int objectLevel, int objectPosition, std::vector<int> levelPositionCoordsOfEqnModifierObjects, std::vector<float> SliderValues, int translEqnMode, int IndiceToSelect){
    //if object is uninitialized then spit out error
    if(ObjectArray.size() <= objectLevel){std::cout <<"wtf?";} //super bad cuz adding rotation equation without primitive eqn???????
    if(ObjectArray[objectLevel].size() <= objectPosition){std::cout<<"wtf???";} //same prob as above
        Object& CurrentObject = ObjectArray[objectLevel][objectPosition];
    
    CurrentObject.TranslationEquation = translationEquation;
    CurrentObject.ConstantModifierObjectsTranslationEqn = levelPositionCoordsOfEqnModifierObjects;
    CurrentObject.ConstantModifierSliderValuesTranslationEqn = SliderValues;
    CurrentObject.TranslEqnMode = translEqnMode;
    CurrentObject.selectedIndiceTransl = IndiceToSelect;
}

void GraphingApplication::ObjectMaster::setCrossSectionEqn(std::string primitiveEqn){
    std::unique_ptr<Grapher> tempGrapherPointer = std::make_unique<Grapher>('P',std::sqrt(8.0),CrossSecObj.ZLookupPtr->yRangeOfHeightTable,CrossSecObj.ZLookupPtr->resolOfHeightTable);
    CrossSecObj.ZLookupPtr->CrossSecEqnAndGraph.EquationForGrapher = primitiveEqn;
    CrossSecObj.ZLookupPtr->CrossSecEqnAndGraph.GrapherPointer = std::move(tempGrapherPointer);
}
void GraphingApplication::ObjectMaster::crossSectionEqnModifiers(std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs){
    CrossSecObj.ConstantModifierObjectsMainEqn = levelPositionCoordsOfEqnModObjs;
    CrossSecObj.ConstantModifierSliderValuesMainEqn = SliderVals;
}
void GraphingApplication::ObjectMaster::setCrossSectionResol(float newResolVal){
    CrossSecObj.ZLookupPtr->resolOfHeightTable = newResolVal;
}
void GraphingApplication::ObjectMaster::setCrossSectionYrange(float newYRange){
    CrossSecObj.ZLookupPtr->yRangeOfHeightTable = newYRange;
}

void GraphingApplication::ObjectMaster::addLightEqns(std::string XYequation, std::string IntensityEqn, std::string ZEqn){
    lightInfObjArray.resize(lightInfObjArray.size()+1);//adds a light object to the existing array
    Object& newObj = lightInfObjArray[lightInfObjArray.size()-1];//reference to the object just created
    newObj.AddGraph(XYequation);//this is the main xy eqn
    
        GrapherAndEquation ZDirEqn;
        ZDirEqn.EquationForGrapher = ZEqn; ZDirEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.lightInfPtr->ZDirGraphNEqn = std::move(ZDirEqn);
        GrapherAndEquation IntensityGrapherNEqn;
        IntensityGrapherNEqn.EquationForGrapher = IntensityEqn; IntensityGrapherNEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.lightInfPtr->IntensityGraphNEqn = std::move(IntensityGrapherNEqn);
}
void GraphingApplication::ObjectMaster::addXYeqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs){
    Object& referredObj = lightInfObjArray[LightObjPos];
    referredObj.ConstantModifierSliderValuesMainEqn = SliderVals;
    referredObj.ConstantModifierObjectsMainEqn = levelPositionCoordsOfEqnModObjs;
}
void GraphingApplication::ObjectMaster::ZEqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs){
    Object& currentObj = lightInfObjArray[LightObjPos];
        currentObj.lightInfPtr->ConstModObjsZDirEqn = levelPositionCoordsOfEqnModObjs;
        currentObj.lightInfPtr->ConstModSldrValsZDirEqn = SliderVals;
}
void GraphingApplication::ObjectMaster::intensityEqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs){
    Object& currentObj = lightInfObjArray[LightObjPos];
    currentObj.lightInfPtr->ConstModObjsIntensityEqn = levelPositionCoordsOfEqnModObjs;
    currentObj.lightInfPtr->ConstModSldrValsIntensityEqn = SliderVals;
}

void GraphingApplication::ObjectMaster::addClrEqn(std::string redEqn, std::string greenEqn, std::string blueEqn,std::string reflectivityEqn){
    ClrAndRflctivityObjArray.resize(ClrAndRflctivityObjArray.size()+1);
    Object& newObj = ClrAndRflctivityObjArray[ClrAndRflctivityObjArray.size()-1];
        GrapherAndEquation RedEqn;
            RedEqn.EquationForGrapher = redEqn; RedEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.clrRflctPtr->rgbGrapherNEqn[0] = std::move(RedEqn);
        GrapherAndEquation BlueEqn;
            BlueEqn.EquationForGrapher = blueEqn; BlueEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.clrRflctPtr->rgbGrapherNEqn[2] = std::move(BlueEqn);
        GrapherAndEquation GreenEqn;
            GreenEqn.EquationForGrapher = greenEqn; GreenEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.clrRflctPtr->rgbGrapherNEqn[1] = std::move(GreenEqn);
    GrapherAndEquation ReflectivityGrapherAndEqn;
        ReflectivityGrapherAndEqn.EquationForGrapher = reflectivityEqn;
        ReflectivityGrapherAndEqn.GrapherPointer = std::make_unique<Grapher>('P',newObj.xRangeOfObject,newObj.yRangeOfObject,newObj.resolOfObject);
    newObj.clrRflctPtr->ReflectivityGrapherNEqn = std::move(ReflectivityGrapherAndEqn);
}

void GraphingApplication::ObjectMaster::colorEqnModifiers(std::vector<float> SliderValsRed,std::vector<float> SliderValsGreen, std::vector<float> SliderValsBlue, std::vector<float> SliderValsReflectivity, std::vector<int> RedModObjs, std::vector<int> GreenModObjs, std::vector<int> BlueModObjs, std::vector<int> ReflectivityModObjs, int clrNRflctObjPos){
    Object& currentObj = ClrAndRflctivityObjArray[clrNRflctObjPos];
        currentObj.clrRflctPtr->ModObjsRed = RedModObjs;
        currentObj.clrRflctPtr->ModObjsBlue = BlueModObjs;
        currentObj.clrRflctPtr->ModObjsGreen = GreenModObjs;
        currentObj.clrRflctPtr->ModObjsReflectivity = ReflectivityModObjs;
    currentObj.clrRflctPtr->ModSlidersRed = SliderValsRed;
    currentObj.clrRflctPtr->ModSlidersBlue = SliderValsBlue;
    currentObj.clrRflctPtr->ModSlidersGreen = SliderValsGreen;
    currentObj.clrRflctPtr->ModSlidersReflectivity = SliderValsReflectivity;
}



void GraphingApplication::ObjectMaster::setInitValuesForObjMstr(float xRange, float yRange, float resol, float animVal, float* MetalBufferPtr, float* ColorBufferPtr, float* LightBufferPtr, float* HeightLookupBufferPtr, float* offsetBufferPointer){
    xRangeOfObjectMaster = xRange;
    yRangeOfObjectMaster = yRange;
    resolOfObjectMaster = resol;
    animationValue = animVal;
    MetalBuffer = MetalBufferPtr;
        clrAndReflectivityBufferPtr = ColorBufferPtr;
        lightInfoBufferPtr = LightBufferPtr;
        HeightLookupTableBufferPtr = HeightLookupBufferPtr;
                MetalOffsetBuffer = offsetBufferPointer;
    setInitObjectMasterValues();//sets the same init values for all objects and their graphers
}

void GraphingApplication::ObjectMaster::setInitObjectMasterValues(){
    auto numberOfObjectLevels = ObjectArray.size(); //number of levels of objects to loop through
    for (int b = 0; b < numberOfObjectLevels; b++) {
        auto numberOfObjectsInCurrentLevel = ObjectArray[b].size(); //number of objects in the current level to loop through
        for (int w = 0; w < numberOfObjectsInCurrentLevel; w++) {
            ObjectArray[b][w].setInitObjectValues(xRangeOfObjectMaster, yRangeOfObjectMaster, resolOfObjectMaster); //sets the object values for each existing object to the provided parameters
        }
    }
}
void GraphingApplication::ObjectMaster::setInitObjectValuesByMaster(float xRange, float yRange, float resol, int objectLevel, int objectPosition){
    ObjectArray[objectLevel][objectPosition].setInitObjectValues(xRange, yRange, resol); //initializes selected object with given parameters
}

std::vector<float>& GraphingApplication::ObjectMaster::ReturnReferenceToRequestedObjectVector(int objectLevel, int objectPosition){
    /*
    int numberOfPreviousObjects = 0;
    for (int v = 0; v < objectLevel-1; v++) {
        //results in fatal error if object level & position requested is greater than whatever currently exists
        numberOfPreviousObjects += ObjectArray[v].size(); //adds the number of objects in each level until the requested one to the total number of objects
    }
    numberOfPreviousObjects += objectPosition-1; //yields the total number of objects before the requested one.
    int PointOffset = 2*(int)(xRangeOfObjectMaster/(300.0*resolOfObjectMaster)); //calculates the points per object to be used in pointer artihmetic. the multiplication by two is since each object contains both x and y points
    
    float* returnPointer = &IntermediateObjectlevelPoints[(PointOffset*numberOfPreviousObjects)+1];
    return returnPointer;
     */
    std::vector<float>& ReferenceToReturn = ObjectArray[objectLevel][objectPosition].ObjectCenterCoords;
    return ReferenceToReturn;
}

//CONVERTS TO NDC!!!
void GraphingApplication::ObjectMaster::calculateObjectValues(bool Descale, float descaleFactor){
    //clearMetalBuffer();
    ObjectMaster* PointerToThisInstance = this; //pointer to this objectMaster instance
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    ObjectArray[d][j].calculateValuesAndWriteToObjectMasterArray(PointerToThisInstance);//parameter requires pointer to this instance
                }
            }
    }
            Object* PointerToCrossSecObj = &CrossSecObj;
        CrossSecObj.ZLookupPtr->calcCrossSecAndWriteToMaster(PointerToThisInstance, PointerToCrossSecObj);
    
    Object* PtrToCurrentObj;//pointer to the object instance thats currently running a calculation
    for (int ds = 0; ds < ClrAndRflctivityObjArray.size(); ds++) {//calculate color objects and write to objet master color array in packed_float4 format
        PtrToCurrentObj = &ClrAndRflctivityObjArray[ds];
        ClrAndRflctivityObjArray[ds].clrRflctPtr->calculateNwriteToObjMstrClrAndRflctArray(PointerToThisInstance, PtrToCurrentObj);
    }
    for (int b = 0; b < lightInfObjArray.size(); b++) {//calculate light objects and write to objet master light array in packed_float4 format
        PtrToCurrentObj = &lightInfObjArray[b];
        lightInfObjArray[b].lightInfPtr->calculateNwriteToObjMstrLightInfArray(PointerToThisInstance, PtrToCurrentObj);
    }
    float xRange = xRangeOfObjectMaster;//to scale coords to NDC
    float yRange = yRangeOfObjectMaster;
    //NDC conversion
    for (int o = 0; o < (RawObjectPoints.size()/2); o++) {
        RawObjectPoints[o*2] /= (xRange/2);//divides object points by xRange/2 to output values between -1.0,1.0. we're doing xRange/2 since range should be from (-150,150) if xRange is 300
        RawObjectPoints[1+(o*2)] /= yRange; //division by yRange to obtain NDC. not yRange/2 since if we say yRange is 100 it means upto y = 100
    }
    for (int vv = 0; vv < ClrAndReflctArray.size() ; vv++) {
        ClrAndReflctArray[vv] /= (yRange/2);
    }
    for (int mn = 0; mn < LightInfArray.size(); mn++) {
        LightInfArray[mn] /= (yRange/2);
    }
    if(Descale){
        for (int u = 0; u < RawObjectPoints.size(); u++) {
            RawObjectPoints[u] /= descaleFactor;
        }
        float moveRightAndDownBy = (1.0-descaleFactor);//formula is (1-descaleFactor)/2 * LengthOfEdge and since length = 2 this is what it simplifies to
        for (int d = 0; d < (RawObjectPoints.size()/2); d++) {
            RawObjectPoints[d*2] += (3.5*moveRightAndDownBy); //goes right
            RawObjectPoints[1+(d*2)] -= (2*moveRightAndDownBy);//goes down
        }
    }
    long currentMetalOffsetBufferPosition = 0;
    long currentMetalBufferPosition = 0;
    float offset = 0.005;//smol offset cuz it will be ndc right? offset is basically supposed to be radius of cross section
    for (long e = 0; e < (RawObjectPoints.size()/2)-1; e++) {
            float point1[2] = {RawObjectPoints[(e*2)],RawObjectPoints[(e*2)+1]};//x,y coords of current point
            float point2[2] = {RawObjectPoints[(e*2)+2],RawObjectPoints[(e*2)+3]};//x,y coords of next point
            std::array<float,8> offsetPointsCoords = pointOffsetCalculator(point1, point2, offset);
            
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[6];//upper left point
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[7];
        currentMetalOffsetBufferPosition++;
        
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[2];//lower left point
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[3];
        currentMetalOffsetBufferPosition++;
        
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[0];
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[1];//lower right point
        currentMetalOffsetBufferPosition++;
        
        
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[6];//upper left point, triangle 2
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[7];
        currentMetalOffsetBufferPosition++;
        
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[4];//upper right point, triangle 2
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[5];
        currentMetalOffsetBufferPosition++;
        
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[0];//lower right point, triangle 2
        currentMetalOffsetBufferPosition++;
        *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[1];
        currentMetalOffsetBufferPosition++;
        
        
        //line seg endpoints
        *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(e*2)];//xy coords of start point of line seg
        currentMetalBufferPosition++;
        *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(e*2)+1];
        currentMetalBufferPosition++;
        
        *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(e*2)+2];//xy coords of end point of line seg
        currentMetalBufferPosition++;
        *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(e*2)+3];
        currentMetalBufferPosition++;
        //line seg endpoints
        
    }
        *(HeightLookupTableBufferPtr) = HeightLookupArray.size();//first index of metal buffer to be filled with number of height values
    for (long q = 0; q < HeightLookupArray.size(); q++) {
        *(HeightLookupTableBufferPtr+q+1) = HeightLookupArray[q];
    }
    for (long x = 0; x < ClrAndReflctArray.size(); x++) {
        *(clrAndReflectivityBufferPtr+x) = ClrAndReflctArray[x];
    }
        *(lightInfoBufferPtr) = (LightInfArray.size()/4)-1;//first index to be filled with number of light points. since its float4 number is divided by 4. and since there are .size()-1 indices excluding the first indice thats the count given here
    for (long z = 0; z < LightInfArray.size(); z++) {
        *(lightInfoBufferPtr+z+4) = LightInfArray[z];
    }
    RawObjectPoints.clear();
    HeightLookupArray.clear();
    ClrAndReflctArray.clear();
    LightInfArray.clear();
    IntermediateObjectlevelPoints.clear();//clear for next frame
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    ObjectArray[d][j].ObjectRawCoords.clear();
                    ObjectArray[d][j].ObjectCenterCoords.clear();//clear for next frame
                }
            }
    }
}
void GraphingApplication::ObjectMaster::calculateOnlyTheseObjects(std::vector<int> PosCoordsOfObjectsToDraw, bool Descale, float descaleFactor){
    //clearMetalBuffer();
    ObjectMaster* PointerToThisInstance = this; //pointer to this objectMaster instance
    std::vector<int> IndiceCoordsDrawObjs;IndiceCoordsDrawObjs.reserve(100);//record indice start and end of objects that need ot be draws
    int ObjDrawCnter = 0;//incremented each time an object to be drawn's indice coords are recorded
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    bool recordEndIndice = false;
                    //if currently going to calculate the coords of an object to be drawn, record the start of its indice and set recordEndIndice to true.
                    //if current object counter wont go out of bounds of the pos coordsof objects to draw (can happen since counter is incremented when an object is calculated but that calculated object may be the last object to be drawn) do not proceed. thats what the first statement in the && chain checks for
                    if((((ObjDrawCnter*2)+1) < PosCoordsOfObjectsToDraw.size())&&(PosCoordsOfObjectsToDraw[0+(ObjDrawCnter*2)] == d)&&(PosCoordsOfObjectsToDraw[1+(ObjDrawCnter*2)] == j)){
                        int IndexToRecord = (int)RawObjectPoints.size();//not doing size-1 since the start of the index of the object to draw will be one mroe than the last currently available index
                        IndiceCoordsDrawObjs.push_back(IndexToRecord);
                        recordEndIndice = true;
                    }
                    ObjectArray[d][j].calculateValuesAndWriteToObjectMasterArray(PointerToThisInstance);//parameter requires pointer to this instance
                    if(recordEndIndice){
                        int IndiceToRecord = (int)RawObjectPoints.size()-1/*ending indice of obj to draw*/;
                        IndiceCoordsDrawObjs.push_back(IndiceToRecord);//record ending indice of object to draw
                        recordEndIndice = false;
                        ObjDrawCnter++;
                    }
                }
            }
    }
        Object* PointerToCrossSecObj = &CrossSecObj;
    CrossSecObj.ZLookupPtr->calcCrossSecAndWriteToMaster(PointerToThisInstance, PointerToCrossSecObj);

    Object* PtrToCurrentObj;//pointer to the object instance thats currently running a calculation
    for (int ds = 0; ds < ClrAndRflctivityObjArray.size(); ds++) {//calculate color objects and write to objet master color array in packed_float4 format
    PtrToCurrentObj = &ClrAndRflctivityObjArray[ds];
    ClrAndRflctivityObjArray[ds].clrRflctPtr->calculateNwriteToObjMstrClrAndRflctArray(PointerToThisInstance, PtrToCurrentObj);
    }
    for (int b = 0; b < lightInfObjArray.size(); b++) {//calculate light objects and write to objet master light array in packed_float4 format
    PtrToCurrentObj = &lightInfObjArray[b];
    lightInfObjArray[b].lightInfPtr->calculateNwriteToObjMstrLightInfArray(PointerToThisInstance, PtrToCurrentObj);
    }
    
    
    float xRange = xRangeOfObjectMaster;//to scale coords to NDC
    float yRange = yRangeOfObjectMaster;
    //NDC conversion
    for (int o = 0; o < (RawObjectPoints.size()/2); o++) {
        RawObjectPoints[o*2] /= (xRange/2);//divides object points by xRange/2 to output values between -1.0,1.0. we're doing xRange/2 since range should be from (-150,150) if xRange is 300
        RawObjectPoints[1+(o*2)] /= yRange; //division by yRange to obtain NDC. not yRange/2 since if we say yRange is 100 it means upto y = 100
    }
    for (int vv = 0; vv < ClrAndReflctArray.size() ; vv++) {
        ClrAndReflctArray[vv] /= (yRange/2);
    }
    for (int mn = 0; mn < LightInfArray.size(); mn++) {
        LightInfArray[mn] /= (yRange/2);
    }
    if(Descale){
        for (int u = 0; u < RawObjectPoints.size(); u++) {
            RawObjectPoints[u] /= descaleFactor;
        }
        float moveRightAndDownBy = (1.0-descaleFactor);//formula is (1-descaleFactor)/2 * LengthOfEdge and since length = 2 this is what it simplifies to
        for (int d = 0; d < (RawObjectPoints.size()/2); d++) {
            RawObjectPoints[d*2] += (3.5*moveRightAndDownBy); //goes right
            RawObjectPoints[1+(d*2)] -= (2*moveRightAndDownBy);//goes down
        }
    }
    /*
    for (long e = 0; e < RawObjectPoints.size(); e++) {
        *(MetalBuffer+e) = RawObjectPoints[e];//fills metal buffer with raw object points
    }
     */
    unsigned long currentMetalBufferPosition = 0;//keep track of what position we are in.
    unsigned long currentMetalOffsetBufferPosition = 0;
    float offset = 0.005;
    for (int y = 0; y < (IndiceCoordsDrawObjs.size()/2); y++) {//loop through the points between the indice coords of the objects to draw and push to metal
        long numberOfPointsToDrawInThisIndice = (IndiceCoordsDrawObjs[1+y*2])-IndiceCoordsDrawObjs[0+(y*2)];//points to draw is endIndice-startIndice
        int& StartFrom = IndiceCoordsDrawObjs[0+(y*2)];
        for (long q = 0; q < (numberOfPointsToDrawInThisIndice/2)-1;q++) {
            float point1[2] = {RawObjectPoints[StartFrom+(q*2)],RawObjectPoints[(StartFrom+(q*2)+1)]};//x,y coords of current point
            float point2[2] = {RawObjectPoints[StartFrom+(q*2)+2],RawObjectPoints[(StartFrom+(q*2)+3)]};//x,y coords of next point
            std::array<float,8> offsetPointsCoords = pointOffsetCalculator(point1, point2, offset);
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[6];//upper left point
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[7];
            currentMetalOffsetBufferPosition++;
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[2];//lower left point
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[3];
            currentMetalOffsetBufferPosition++;
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[0];
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[1];//lower right point
            currentMetalOffsetBufferPosition++;
            
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[6];//upper left point, triangle 2
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[7];
            currentMetalOffsetBufferPosition++;
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[4];//upper right point, triangle 2
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[5];
            currentMetalOffsetBufferPosition++;
            
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[0];//lower right point, triangle 2
            currentMetalOffsetBufferPosition++;
            *(MetalOffsetBuffer+currentMetalOffsetBufferPosition) = offsetPointsCoords[1];
            currentMetalOffsetBufferPosition++;
            
            
            //line seg endpoints
            *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(StartFrom+(q*2))];//xy coords of start point of line seg
            currentMetalBufferPosition++;
            *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(StartFrom+(q*2)+1)];
            currentMetalBufferPosition++;
            
            *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(StartFrom+(q*2)+2)];//xy coords of end point of line seg
            currentMetalBufferPosition++;
            *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[(StartFrom+(q*2)+3)];
            currentMetalBufferPosition++;
            //line seg endpoints
        }
    }
        *(HeightLookupTableBufferPtr) = HeightLookupArray.size();//first index of metal buffer to be filled with number of height values
    for (long q = 0; q < HeightLookupArray.size(); q++) {
        *(HeightLookupTableBufferPtr+q+1) = HeightLookupArray[q];
    }
    for (long x = 0; x < ClrAndReflctArray.size(); x++) {
        *(clrAndReflectivityBufferPtr+x) = ClrAndReflctArray[x];
    }
        *(lightInfoBufferPtr) = (LightInfArray.size()/4)-1;//first index to be filled with number of light points. since its float4 number is divided by 4. and since there are .size()-1 indices excluding the first indice thats the count given here
    for (long z = 0; z < LightInfArray.size(); z++) {
        *(lightInfoBufferPtr+z+4) = LightInfArray[z];
    }
    RawObjectPoints.clear();
    HeightLookupArray.clear();
    ClrAndReflctArray.clear();
    LightInfArray.clear();
    IntermediateObjectlevelPoints.clear();//clear for next frame
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    ObjectArray[d][j].ObjectRawCoords.clear();
                    //ObjectArray[d][j].ObjectCenterCoords.clear(); not clearing object center coords for now since level evlaulation systm might be broken and objects re requresting coords to obejcts that havent been calculated yet
                }
            }
    }
}

inline std::array<float, 8> GraphingApplication::ObjectMaster::pointOffsetCalculator(float point1[2], float point2[2], float offset){//[x,y]
        float slopeBetweenPoints = (point2[1]-point1[1])/(point2[0]-point1[0]);
        float inverseNegSlope = -1.0f/slopeBetweenPoints;
    offset = std::abs(offset);//in case it is accidentally given as negative
    float angleOfInvNegSlope = atan2f((0.0f-(point2[0]-point1[0])),(point2[1]-point1[1]));
    float returnPoint1XOffset = cosf(angleOfInvNegSlope)*offset;
    float returnPoint1YOffset = sinf(angleOfInvNegSlope)*offset;
    
    //xy coords of four offset points, 2 coord pairs for each endpoint of the line segment
    std::array<float,8> OffsetPoints = {(point1[0]+returnPoint1XOffset),(point1[1]+returnPoint1YOffset),(point1[0]-returnPoint1XOffset),(point1[1]-returnPoint1YOffset),(point2[0]+returnPoint1XOffset),(point2[1]+returnPoint1YOffset),(point2[0]-returnPoint1XOffset),(point2[1]-returnPoint1YOffset)};
    return OffsetPoints;
}

void GraphingApplication::ObjectMaster::calculateOnlyTheseLightObjects(std::vector<int> PosCoordsOfLightObjsToDraw, bool Descale, float descaleFactor){
    ObjectMaster* PointerToThisInstance = this; //pointer to this objectMaster instance
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    ObjectArray[d][j].calculateValuesAndWriteToObjectMasterArray(PointerToThisInstance);//parameter requires pointer to this instance
                }
            }
    }
            Object* PointerToCrossSecObj = &CrossSecObj;
        CrossSecObj.ZLookupPtr->calcCrossSecAndWriteToMaster(PointerToThisInstance, PointerToCrossSecObj);
    
    Object* PtrToCurrentObj;//pointer to the object instance thats currently running a calculation
    for (int ds = 0; ds < ClrAndRflctivityObjArray.size(); ds++) {//calculate color objects and write to objet master color array in packed_float4 format
        PtrToCurrentObj = &ClrAndRflctivityObjArray[ds];
        ClrAndRflctivityObjArray[ds].clrRflctPtr->calculateNwriteToObjMstrClrAndRflctArray(PointerToThisInstance, PtrToCurrentObj);
    }
    for (int b = 0; b < lightInfObjArray.size(); b++) {//calculate light objects and write to objet master light array in packed_float4 format
        bool drawThisObj = true;
        for (int c = 0; c < PosCoordsOfLightObjsToDraw.size(); c++) {
            if (b != PosCoordsOfLightObjsToDraw[c]){drawThisObj = false;}//if the light object position to draw right not is not in the drawObj list dont draw it
        }
        if (drawThisObj){
            PtrToCurrentObj = &lightInfObjArray[b];
            lightInfObjArray[b].lightInfPtr->calculateNwriteToObjMstrLightInfArray(PointerToThisInstance, PtrToCurrentObj);
        }
    }
    float xRange = xRangeOfObjectMaster;//to scale coords to NDC
    float yRange = yRangeOfObjectMaster;
    //NDC conversion
    for (int o = 0; o < (RawObjectPoints.size()/2); o++) {
        RawObjectPoints[o*2] /= (xRange/2);//divides object points by xRange/2 to output values between -1.0,1.0. we're doing xRange/2 since range should be from (-150,150) if xRange is 300
        RawObjectPoints[1+(o*2)] /= yRange; //division by yRange to obtain NDC. not yRange/2 since if we say yRange is 100 it means upto y = 100
    }
    if(Descale){
        for (int u = 0; u < RawObjectPoints.size(); u++) {
            RawObjectPoints[u] /= descaleFactor;
        }
        float moveRightAndDownBy = (1.0-descaleFactor);//formula is (1-descaleFactor)/2 * LengthOfEdge and since length = 2 this is what it simplifies to
        for (int d = 0; d < (RawObjectPoints.size()/2); d++) {
            RawObjectPoints[d*2] += (3.5*moveRightAndDownBy); //goes right
            RawObjectPoints[1+(d*2)] -= (2*moveRightAndDownBy);//goes down
        }
    }
    long currentMetalBufferPosition = 0;
    for (long e = 0; e < RawObjectPoints.size(); e++) {
        *(MetalBuffer+currentMetalBufferPosition) = RawObjectPoints[e];
        currentMetalBufferPosition++;
    }
        *(HeightLookupTableBufferPtr) = HeightLookupArray.size();//first index of metal buffer to be filled with number of height values
    for (long q = 1; q < HeightLookupArray.size()+1; q++) {
        *(HeightLookupTableBufferPtr+q) = HeightLookupArray[q];
    }
    for (long x = 0; x < ClrAndReflctArray.size(); x++) {
        *(clrAndReflectivityBufferPtr+x) = ClrAndReflctArray[x];
    }
    int numberOfPointsPerLightObject = int(300.0f*resolOfObjectMaster);
        *(lightInfoBufferPtr) = numberOfPointsPerLightObject*PosCoordsOfLightObjsToDraw.size();//first index to be filled with number of light points
    for (long z = 1; z < LightInfArray.size()+1; z++) {
        *(lightInfoBufferPtr+z) = LightInfArray[z];
    }
    RawObjectPoints.clear();
    IntermediateObjectlevelPoints.clear();//clear for next frame
    for (int d = 0; d < ObjectArray.size(); d++) {//nested loop calculates values for every single object//pos 0 dont exist they start from 1!
            for (int j = 0; j < ObjectArray[d].size(); j++) {
                if(j!=0){
                    ObjectArray[d][j].ObjectRawCoords.clear();
                    ObjectArray[d][j].ObjectCenterCoords.clear();//clear for next frame
                }
            }
    }
}

void GraphingApplication::ObjectMaster::incrementAnimationValue(float multiplier){
    animationValue += (1.0*multiplier);//increments animation value
}

int GraphingApplication::ObjectMaster::returnPointsTerDraw(){
    int PointsTerDraw = 0;//for physical objects and their primitives only
    for (int u = 0; u < ObjectArray.size(); u++) { //loops through all the levels
        for (int g = 0; g < ObjectArray[u].size(); g++) { //loops through all the object positions
            auto numberOfPrimitives = ObjectArray[u][g].GrapherPointerArray.size();
            int pointsPerPrimitve = (int)300.0*ObjectArray[u][g].resolOfObject;
            PointsTerDraw += numberOfPrimitives*pointsPerPrimitve; //+= number of points in the current object
        }
    }
    return PointsTerDraw;
}

void GraphingApplication::ObjectMaster::clearMetalBuffer(){
    int PointsDrawn = returnPointsTerDraw();
    for (int g = 0; g < (PointsDrawn*30); g++) {//setting to x3 since lineWidth has been experimentally set to 3
        *(MetalOffsetBuffer+g) = 0.0; //fills with coords outside NDC that will be clipped. clears OFFSET buffer not main vertice buffer since thats whats actually drawn
        *(MetalBuffer+g) = 0.0; //fills with coords outside NDC that will be clipped. clears OFFSET buffer not main vertice buffer since thats whats actually drawn
    }
}


std::vector<float> Grapher::dummyVector = {0};

//create an object of the specific Graphing subclass like Polynomial graph. whatever the equation aligns to.
//Equation types: 'R' = Polar, 'T' = Trig, 'C' = Calculus, 'P' = Polynomial
Grapher::Grapher(char EquationType,float xRangeValue, float yRangeValue, float resolutionValue){ //add types of graphs as they are created as classes. stores only polynomial for now.
    
    thisGrapherPointer = this;
    ThreeDeeExpressionArrayUniquePointer = std::make_unique<ThreeDeeExpressionArray>(thisGrapherPointer);
    PointerToThreeDeeExpressionArrayInstance = ThreeDeeExpressionArrayUniquePointer.get();
    
    
    //SINCE INITIALIZED AS EMPTY CONSTRUCTOR NEED TO SET INITIAL VALUES!!!
    PolynomialGrapherInstance->setInitValues(xRangeValue, yRangeValue, resolutionValue);
    CalculusGrapherInstance->setInitValues(xRangeValue, yRangeValue, resolutionValue);
    TrigonometricGrapherInstance->setInitValues(xRangeValue, yRangeValue,resolutionValue);
    PolarGrapherInstance->setInitValues(xRangeValue, yRangeValue,resolutionValue);
    
    PointerToGraphingCoordsStruct = &GraphingCoordsStruct;
    PointerToGraphingCoordsStruct->xInterval = (xRangeValue/(300.0 * resolutionValue)); //records x-interval that will later be used by coordinates updater of subtypes
    PointerToGraphingCoordsStruct->xRange = xRangeValue;
    
    if(EquationType == 'P'){
        CurrentGraphingSubtypePointer = PolynomialGrapherInstance.get();
    } else if (EquationType == 'T') {
        CurrentGraphingSubtypePointer = TrigonometricGrapherInstance.get();
    } else if (EquationType == 'R') {
        CurrentGraphingSubtypePointer = PolarGrapherInstance.get();
    } else if (EquationType == 'C') {
        CurrentGraphingSubtypePointer = CalculusGrapherInstance.get();
    }
}

//sets subtype pointer to required subtype and returns pointer.
void Grapher::setCurrentSubtypePointer(char EquationType) {
    if(EquationType == 'P'){
        CurrentGraphingSubtypePointer = PolynomialGrapherInstance.get();
    } else if (EquationType == 'T') {
        CurrentGraphingSubtypePointer = TrigonometricGrapherInstance.get();
    } else if (EquationType == 'R') {
        CurrentGraphingSubtypePointer = PolarGrapherInstance.get();
    } else if (EquationType == 'C') {
        CurrentGraphingSubtypePointer = CalculusGrapherInstance.get();
    }
    
}

void Grapher::setInitGrapherValues(float xRange, float yRange, float resol){
    PolynomialGrapherInstance->setInitValues(xRange, yRange, resol);
    CalculusGrapherInstance->setInitValues(xRange, yRange, resol);
    TrigonometricGrapherInstance->setInitValues(xRange, yRange, resol);
    PolarGrapherInstance->setInitValues(xRange, yRange, resol);
    
    PointerToGraphingCoordsStruct->xInterval = (xRange/(300.0 * resol)); //records x-interval that will later be used by coordinates updater of subtypes
    PointerToGraphingCoordsStruct->xRange = xRange;
    PointerToGraphingCoordsStruct->yRange = yRange;
}

//if set to true, prints coords and returns. else, returns.
std::vector<float> Grapher::printOrReturnCurrentCoords(bool printOrReturn){
    auto NumberOfPointsToBePrinted = GraphingCoordsStruct.XYcoordsVector.size();
    auto xIntervalTemp = GraphingCoordsStruct.xInterval;
    float currentYvalue = 0.0;
    float currentXvalue = 0.0;
    
    if(printOrReturn){
        if(CurrentGraphingSubtypePointer == PolarGrapherInstance.get()){
            //number of points will always be even with a polar array
            for(int ty = 0; ty < (NumberOfPointsToBePrinted/2); ty++){
                
                currentYvalue = GraphingCoordsStruct.XYcoordsVector[1+(ty*2)];
                currentXvalue = GraphingCoordsStruct.XYcoordsVector[ty*2];
                    if(std::isnan(currentXvalue)||std::isnan(currentYvalue)) {
                    //NAN VALUE!!!
                    }
                std::cout << "X: ";
                std::cout << currentXvalue;
                std::cout << "   Y: ";
                std::cout << currentYvalue;
                std::cout << "\n";
            }
            return GraphingCoordsStruct.XYcoordsVector;
        }
        for(int ty = 0; ty < NumberOfPointsToBePrinted; ty++){
            
            currentYvalue = GraphingCoordsStruct.XYcoordsVector[ty];
            currentXvalue = (ty * xIntervalTemp);
                if(std::isnan(currentXvalue)||std::isnan(currentYvalue)) {
                //NAN VALUE!!!
                }
            std::cout << "X: ";
            std::cout << currentXvalue;
            std::cout << "   Y: ";
            std::cout << currentYvalue;
            std::cout << "\n";
        }
    }
        return GraphingCoordsStruct.XYcoordsVector;
    
}

void Grapher::SubtypeTesterPrintCoordinates(char EquationType, bool addOrDelete, int termPosition, float termExponent, float termCoefficient, int termIdentifier) {
    setCurrentSubtypePointer(EquationType);
    std::cout << "Set current subtype pointer to :";
    switch (EquationType) {
        case 'P':
            std::cout << "Polynomial";
            break;
        case 'T':
            std::cout << "Trigonometry";
            break;
        case 'C':
            std::cout << "Calculus";
            break;
        case 'R':
            std::cout << "Polar";
            
        default:
            //bad
            break;
    }
    if(!((EquationType == 'C') || (EquationType == 'R'))){
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(addOrDelete, termPosition, termExponent, termCoefficient, termIdentifier);
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 2, 0.3, 87.64, termIdentifier);
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(false, 2);
        std::vector<float> IntegrandArrayForCalculusSubtypeTwo = CurrentGraphingSubtypePointer->returnCoordinatesArray();
            
        //sets Grapher's vector equal to the coordinates returned
        GraphingCoordsStruct.XYcoordsVector = CurrentGraphingSubtypePointer->CoordinatesUpdater(false, dummyVector, termIdentifier);
    } else if(EquationType == 'C'){
        std::vector<float> IntegrandArrayForCalculusSubtype;
        
        std::vector<std::vector<float>> tempVectorArrayForTesting = CurrentGraphingSubtypePointer->returnExpressionArray();
        std::vector<float> dummyVector = {0};
        setCurrentSubtypePointer('P');
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 3.1, 0.75, 2);
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 2, 1.23, 4.89, 2);
        //last parameter doesnt matter. we are currently grabbing the coordinate results for 0.75x^4 to use in Calculus Integrand.
        CurrentGraphingSubtypePointer->CoordinatesUpdater(false, dummyVector, 54);
        
        IntegrandArrayForCalculusSubtype = CurrentGraphingSubtypePointer->returnCoordinatesArray();
        
        setCurrentSubtypePointer('C');
        std::vector<std::vector<float>> ExpressionArrayVectorForTesting = CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 0.0f, 0.0f, 105);
        //last parameter unused
        GraphingCoordsStruct.XYcoordsVector = CurrentGraphingSubtypePointer->CoordinatesUpdater(true, IntegrandArrayForCalculusSubtype, 12);
        
    } else if(EquationType == 'R'){
        setCurrentSubtypePointer('P');
        std::vector<std::vector<float>> dummyVector;
        std::vector<float> trigVector = {0};
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 1.0, 4.1, 0);
        //last parameter unused
        CurrentGraphingSubtypePointer->CoordinatesUpdater(false, trigVector, 53);
        trigVector = CurrentGraphingSubtypePointer->returnCoordinatesArray();
        std::vector<float> PolynomialVector = CurrentGraphingSubtypePointer->returnCoordinatesArray();
        setCurrentSubtypePointer('T');
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 0, 0, 99);
        CurrentGraphingSubtypePointer->CoordinatesUpdater(true, trigVector, 43);
        trigVector = CurrentGraphingSubtypePointer->returnCoordinatesArray();
        
        setCurrentSubtypePointer('R');
        CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 1.0, 2.73, 0);
        
        GraphingCoordsStruct.XYcoordsVector = CurrentGraphingSubtypePointer->CoordinatesUpdater(true, trigVector, 54);
        
    }
        
        //sets the graphing struct's x-interval to the x-interval currently set?
        GraphingCoordsStruct.xInterval = CurrentGraphingSubtypePointer->returnXinterval();
    
    //prints coords stored in Grapher's vector
    printOrReturnCurrentCoords(true);
}

Grapher::ThreeDeeExpressionArray::ThreeDeeExpressionArray(Grapher* thisGrapherPointer){
    ThePointerToCurrentGrapherInstance = thisGrapherPointer;
    //StringParserUniquePointer = std::make_unique<StringParser>();
    //PositionLogicUniquePointer = std::make_unique<PositionLogic>();
    //VectorDataUniquePointer = std::make_unique<VectorData>();
    inputStringCalculatorUniquePointer = std::make_unique<inputStringCalculator>(thisGrapherPointer);
    
    StringParserInstancePointer = StringParserUniquePointer.get();
    PositionLogicInstancePointer = PositionLogicUniquePointer.get();
    VectorDataInstancePointer = VectorDataUniquePointer.get();
    inputStringCalculatorInstancePointer = inputStringCalculatorUniquePointer.get();
}


void Grapher::ThreeDeeExpressionArray::StringParser::ParenthesesLogic(char ParenthesesType, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    
    
    
    switch (ParenthesesType) {
        case '(': {
            //doesnt make sesne for columns to be inserted from the start. then close brackets would come before open!!!
            
            //when a new term is created, the signParser for new term automatically creates a new row and updartes coutner to that vector and updates the 14th element of that vector (index 13) to the signage seperating rows. so if you encounter an open parantheses on level 0 you DONT want to create a new vector since one has already been created. otherwise DO create a new vector.
            if((plogicPointer->currentNestLevel != 0)&&(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][0]==0.0f)){
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][2] = plogicPointer->currentNestLevel;
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][3] = plogicPointer->currentNestPosition;
            }
            if(plogicPointer->currentNestLevel != 0){
                createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
            }
            //if on the very first iteration then initialize the vector
            if(((plogicPointer->currentRow == 0)&&(plogicPointer->currentColumn == 0))&&(VectorDataInstancePointer->TermsArray.empty())){
                createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
            }
            plogicPointer->ParenthesesClosureCounter++;
            plogicPointer->currentNestLevel++;
            std::vector<float>& VectorToBeChecked = VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn];

            if(plogicPointer->maxNestedPositionsOfNestedLevels[plogicPointer->currentRow].size() <= plogicPointer->currentNestLevel)
            {plogicPointer->maxNestedPositionsOfNestedLevels[plogicPointer->currentRow].push_back(0);}
            
            plogicPointer->currentNestPosition = plogicPointer->maxNestedPositionsOfNestedLevels[plogicPointer->currentRow][plogicPointer->currentNestLevel];
            plogicPointer->currentNestPosition++;
            //modifies first element to open bracket identifer, third to nestLevel, fourth to termPosition
            VectorToBeChecked[0] = 1.0f;
            VectorToBeChecked[2] = float(plogicPointer->currentNestLevel);
            VectorToBeChecked[3] = float(plogicPointer->currentNestPosition);
            
            //sets default values of polynomial expression and of 1x^1
            VectorToBeChecked[1] = 0.0f;
            VectorToBeChecked[4] = 1.0f;
            VectorToBeChecked[5] = 1.0f;
            
            plogicPointer->maxNestedPositionsOfNestedLevels[plogicPointer->currentRow][plogicPointer->currentNestLevel] = plogicPointer->currentNestPosition;
            
            plogicPointer->currentStringPosition++;
            break;
        }
            
        case ')':
            if(plogicPointer->ParenthesesClosureCounter == 0){
                std::cout << "bad"; //cant have closing parantheses without open parentheses
            } else{
                std::vector<float>& VectorToBeChecked = VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn];
                //doesnt make sesne for columns to be inserted from the start. then close brackets would come before open!!!
                plogicPointer->ParenthesesClosureCounter--;

                //if there is already an open brackets in the same vector then set it to identifier that allows both open and close vector in same spot
                if((VectorToBeChecked[0] != 1.0f)&&(VectorToBeChecked[0] != 3.0f)){VectorToBeChecked[0] = 2.0f;}else{VectorToBeChecked[0] = 3.0f;}
                //increment numberOfClosedParentheses counter
                VectorToBeChecked[6] += 1;
                if(StringFromSwift[plogicPointer->currentStringPosition-1] != ')'){
                    VectorToBeChecked[2] = float(plogicPointer->currentNestLevel);
                    VectorToBeChecked[3] = float(plogicPointer->currentNestPosition);
                }
                plogicPointer->currentNestLevel--;
                plogicPointer->currentNestPosition = plogicPointer->maxNestedPositionsOfNestedLevels[plogicPointer->currentRow][plogicPointer->currentNestLevel];
                //modifies first element to close bracket identifer, third to nestLevel, fourth to termPosition
                plogicPointer->currentStringPosition++;
            }
            break;
    }
}

//checks character to see if current nested level == 0 and if there are one of 3 identifiers name +-*. if so adds new row for new term.
//also adds sign to element 14 of first term so the algorithm knows what to do with individual terms after calculating them.
void Grapher::ThreeDeeExpressionArray::StringParser::signParserForNewTerm(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    if(plogicPointer->currentNestLevel != 0){
        //do nothing
    } else {
        switch (Identifier) { //if in a valid case to add a new row for a new term, increment current row and set current column to 0 since the new row contains just one vector.
                
            case '+':
                VectorDataInstancePointer->TermsArray.push_back({{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->maxNestedPositionsOfNestedLevels.push_back({0}); //push back maxNestedLevels so it doesnt go out of bounds when parentheses of new level acceses
                plogicPointer->currentRow++;
                plogicPointer->currentColumn = 0;
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][13] = float(Identifier);
                plogicPointer->currentStringPosition++;
                break;
            case '-':
                VectorDataInstancePointer->TermsArray.push_back({{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->maxNestedPositionsOfNestedLevels.push_back({0}); //push back maxNestedLevels so it doesnt go out of bounds when parentheses of new level acceses
                plogicPointer->currentRow++;
                plogicPointer->currentColumn = 0;
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][13] = float(Identifier);
                plogicPointer->currentStringPosition++;
                break;
            case '*':
                VectorDataInstancePointer->TermsArray.push_back({{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->maxNestedPositionsOfNestedLevels.push_back({0}); //push back maxNestedLevels so it doesnt go out of bounds when parentheses of new level acceses
                plogicPointer->currentRow++;
                plogicPointer->currentColumn = 0;
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][13] = float(Identifier);
                plogicPointer->currentStringPosition++;
                break;
            case '/':
                VectorDataInstancePointer->TermsArray.push_back({{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->maxNestedPositionsOfNestedLevels.push_back({0}); //push back maxNestedLevels so it doesnt go out of bounds when parentheses of new level acceses
                plogicPointer->currentRow++;
                plogicPointer->currentColumn = 0;
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][13] = float(Identifier);
                plogicPointer->currentStringPosition++;
                break;
        }
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::createNewColumnOrRowAndUpdateCounters(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer, bool columnOrRow, bool beginningOrEnd){
    //if vector is uninitialized (as in when it is starting, initialize with value of 0 and set currentCOlumn and currentRow to 0
    if(VectorDataInstancePointer->TermsArray.size()==0){VectorDataInstancePointer->TermsArray.push_back( {{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}} );
        plogicPointer->currentRow = 0;
        plogicPointer->currentColumn = 0;
    } else {
        if (columnOrRow) {
            if(beginningOrEnd){
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow].insert(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow].begin(),{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0});
                plogicPointer->currentColumn = 0;
            } else {
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow].push_back({0,0,0,0,0,0,0,0,0,0,5,0,0,0,0});
                plogicPointer->currentColumn++;
            }
        } else {
            if (!(beginningOrEnd)) {
                VectorDataInstancePointer->TermsArray.push_back({{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->currentRow++;
                plogicPointer->currentColumn = 0;
            } else {
                VectorDataInstancePointer->TermsArray.insert(VectorDataInstancePointer->TermsArray.begin(), {{0,0,0,0,0,0,0,0,0,0,5,0,0,0,0}});
                plogicPointer->currentRow = 0;
                plogicPointer->currentColumn = 0;
            }
        }
    }
}

//parses multiple digits and returns as a single float
float Grapher::ThreeDeeExpressionArray::StringParser::multiDigitParser(int& uValue, std::string StringFromSwift){
    auto SwiftStringSize = StringFromSwift.size();
    float result = 0;
    std::string stringResult = "";
    while(uValue < SwiftStringSize){
        if(((StringFromSwift[uValue] > 47)&&(StringFromSwift[uValue] < 58))||(StringFromSwift[uValue] == '.')){
            stringResult += StringFromSwift[uValue];
            uValue++;
        } else {
            break;
        }
    }
    uValue--; //decrements because the while loop stops either when u exceeds the string size or u no longer points to a digit. since polynomialLogic increments u after this we don't want u to point to a value after the digits.
    result = std::stof(stringResult);
return result;
}

//increments stringPosition while still recieving valid values and staying on the same nestedLevel and nestedPosition
//creates vector elements instead of assuming they have been created. so the very base step 1 of the process
//assumes that sign parser has already run before this and set a new row if needed and updated currentColumn and currentRow to that.
void Grapher::ThreeDeeExpressionArray::StringParser::polynomialLogic(char PolynomialIdentifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    int u = plogicPointer->currentStringPosition;
    std::vector<float> emptyVector = {0};
    
        if((PolynomialIdentifier > 47) && (PolynomialIdentifier < 58)){ //proceed if char ASCII values are between 0-10, this branch carries out coefficient logic
            //do not create new column since whenever a coefficient is expected a new column has already been created. such as with (5x +6), a new column is created when the function reads the parentheses and also with the '+' sign inside.
            float coefficient = multiDigitParser(u, StringFromSwift);
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = coefficient; //sets the termCoefficient element to the multiDigitParser result
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 0.0f; //sets termExponent to 0 so onyl the coefficient is recorded
            plogicPointer->currentStringPosition++;
            u++;
        } else if (PolynomialIdentifier == 'x'){
            
            //if the coefficient element has NOT been set, which means the x is a standalone in a nested term, set DEFAULT COEFFICIENT OF 1
            if (VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] == 0) {
                //dont create new column with same logic as for coefficient. because whenever you would have to create a new column one has already been created by virtue of the signs or parentheses
                //sets exponent to 1.0
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1.0f;
            }
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1.0f;
            
            //set to polynomial since its always polynomial at any nest level and with an 'x'
            //you DONT create a new column since at nest level > 0 parantheses logic woudlve already created a new column and updated counters.
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 0;
            plogicPointer->currentStringPosition++;
            u++;
        } else if (PolynomialIdentifier == '^'){ //the algorithm will encounter this sign when the currentColumn pointer is still on a vector with a closed parentheses. therefore it decrements a temporary counter until it finds the vector behind the nearest open parentheses with thte same nest level and sets its exponentiation identifier to true.
            int tempCurrentColumn = plogicPointer->currentColumn;
            //we need to add one nestLevel since the nest level will be one lower than needed as the closed parentheses will set the nestedLevel of the vector and then DECREMENT it.
            
            //only do the decrement logic when you DONT have an open and close parentheses in the same vector. if thats the case then simply add the exponentiation identifier to the nearest vector. however if the open-close parentheses is not of the same level as the base/exponent (which is nestLevel +1 since they are wrapped in parentheses) then proceed with conditiona
            if((VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][0] != 3.0f)||(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][2] != plogicPointer->currentNestLevel+1)){
                int tempCurrentNestLevel = plogicPointer->currentNestLevel+1; //current nestLevel PLUS 1 since the '^' will be encountered one nest level lower than base/exponent since theyre wrapped in parentheses whereas this is encountered outside the parentheses
                while(tempCurrentColumn > 0){
                    tempCurrentColumn--;
                    if((VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][0] == 1.0f)&&(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][2] == tempCurrentNestLevel)){break;}//break when reaching the element with the open parentheses matching the closed parentheses that must have been before the '^'
                }
            }

            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][12] = 1; //if exponent identifier, set element of exponent identifier to true
            plogicPointer->currentStringPosition++;
            u++;
        }
        
    
    //u stops at the exact point where it is no longer reciving valid inputs.
    //however current vector position is still at a valid input point.
    plogicPointer->currentStringPosition = u;
}

void Grapher::ThreeDeeExpressionArray::StringParser::signLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    //only do something when nest level is NOT 0. otherwise it is handled by signForNewTerm function
    
    char characterAfterIdentifier = 0;
    if(plogicPointer->currentStringPosition+1 <= StringFromSwift.size()){characterAfterIdentifier = StringFromSwift[plogicPointer->currentStringPosition+1];}
    if(plogicPointer->currentNestLevel != 0){
        int closedParenthesesInVector = VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][6];
        int tempCurrentColumn = plogicPointer->currentColumn;
        switch (Identifier) {
            case '/':
                if(closedParenthesesInVector > 0){
                    while(tempCurrentColumn >= 0){
                        //if the tempCurrentColumn vector's nested level equals the nested level that the sign corresponds to, break the loop and set that vector's addSubtractMultiplyDivide to the required identifier. this is to prevent situations like ((T(s+x))*(5x)) where the multiplicator will be assigned to the vector containing the x rather than the outer parentheses
                        
                        //TEST CASE: ((i+5(5x +2 *s))/(i(5x)))
                        
                        if(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][2] == plogicPointer->currentNestLevel+1){
                            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][10] = 3; //division identifier activated
                            //we assume that the denominator is ALREADY ENCLOSED IN PARANTHESES!!!
                            plogicPointer->currentStringPosition++;
                            break;
                        }
                        tempCurrentColumn--;
                    }
                } else {
                    VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][10] = 3; //division identifier activated
                    plogicPointer->currentStringPosition++;

                }
                //if next character is NOT an open brackets THEN create a new column.
                    if(characterAfterIdentifier != '('){
                        createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
                    }
                break;
            case '+':
                if(closedParenthesesInVector > 0){
                    while(tempCurrentColumn >= 0){
                        //if the tempCurrentColumn vector's nested level equals the nested level that the sign corresponds to, break the loop and set that vector's addSubtractMultiplyDivide to the required identifier. this is to prevent situations like ((T(s+x))*(5x)) where the multiplicator will be assigned to the vector containing the x rather than the outer parentheses
                        
                        //TEST CASE: ((i+5(5x +2 *s))/(i(5x)))
                        
                        if(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][2] == plogicPointer->currentNestLevel){
                            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][10] = 0; //division identifier activated
                            //we assume that the denominator is ALREADY ENCLOSED IN PARANTHESES!!!
                            plogicPointer->currentStringPosition++;
                            break;
                        }
                        tempCurrentColumn--;
                    }
                } else {
                    VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][10] = 0; //division identifier activated
                    plogicPointer->currentStringPosition++;

                }
                //if next character is NOT an open brackets THEN create a new column.
                    if(characterAfterIdentifier != '('){
                        createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
                    }
                break;
                
            case '-':
                if(closedParenthesesInVector > 0){
                    while(tempCurrentColumn >= 0){
                        //if the tempCurrentColumn vector's nested level equals the nested level that the sign corresponds to, break the loop and set that vector's addSubtractMultiplyDivide to the required identifier. this is to prevent situations like ((T(s+x))*(5x)) where the multiplicator will be assigned to the vector containing the x rather than the outer parentheses
                        
                        //TEST CASE: ((i+5(5x +2 *s))/(i(5x)))
                        
                        if(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][2] == plogicPointer->currentNestLevel){
                            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][10] = 1; //subtraction identifier activated
                            //we assume that the denominator is ALREADY ENCLOSED IN PARANTHESES!!!
                            plogicPointer->currentStringPosition++;
                            break;
                        }
                        tempCurrentColumn--;
                    }
                } else {
                    VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][10] = 1; //division identifier activated
                    plogicPointer->currentStringPosition++;

                }
                //if next character is NOT an open brackets THEN create a new column.
                    if(characterAfterIdentifier != '('){
                        createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
                    }
                break;
                
            case '*':
                if(closedParenthesesInVector > 0){
                    while(tempCurrentColumn >= 0){
                        //if the tempCurrentColumn vector's nested level equals the nested level that the sign corresponds to, break the loop and set that vector's addSubtractMultiplyDivide to the required identifier. this is to prevent situations like ((T(s+x))*(5x)) where the multiplicator will be assigned to the vector containing the x rather than the outer parentheses
                        
                        //TEST CASE: ((i+5(5x +2 *s))/(i(5x)))
                        
                        if(VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][2] == (plogicPointer->currentNestLevel)){
                            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][tempCurrentColumn][10] = 2; //multiplication identifier activated
                            //we assume that the denominator is ALREADY ENCLOSED IN PARANTHESES!!!
                            plogicPointer->currentStringPosition++;
                            break;
                        }
                        tempCurrentColumn--;
                    }
                } else {
                    VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][10] = 2; //division identifier activated
                    plogicPointer->currentStringPosition++;

                }
                //if next character is NOT an open brackets THEN create a new column.
                    if(characterAfterIdentifier != '('){
                        createNewColumnOrRowAndUpdateCounters(plogicPointer, VectorDataInstancePointer, true, false);
                    }
                break;
        }
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::trigonometricLogic(char Identifier, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    switch (Identifier) {
        case 's':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 1; //identifier 1 in element 2 for sine
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            //incrementing by two more since it will be of the form 'sin'!
            plogicPointer->currentStringPosition++;
            plogicPointer->currentStringPosition++;
            break;
        case 'S':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 2; //asin
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            break;
        case 'c':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 3; //cos
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            //incrementing by two more since it will be of the form 'cos'!
            plogicPointer->currentStringPosition++;
            plogicPointer->currentStringPosition++;
            break;
        case 'C':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 4; //acos
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            break;
        case 't':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 5; //tan
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            //incrementing by two more since it will be of the form 'tan'!
            plogicPointer->currentStringPosition++;
            plogicPointer->currentStringPosition++;
            break;
        case 'T':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 6; //atan
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][5] = 1; //coefficient of 1
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][4] = 1; //exponent of 1
            plogicPointer->currentStringPosition++;
            break;
    }
}

//need to change this and trig and polar to act differently when nested level DOESTN equal 0!! make it add a new vector in the current row.
void Grapher::ThreeDeeExpressionArray::StringParser::calculusLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    switch (Identifier) {
        case 'i':
            VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 7; //calculus identifier. more identifiers to be added in the future.
            plogicPointer->currentStringPosition++;
            break;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::polarLogic(char Identifier, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    char elemAfterIdentifier = 0;
    if(plogicPointer->currentStringPosition+1 <= StringFromSwift.size()){
        elemAfterIdentifier = StringFromSwift[plogicPointer->currentStringPosition+1]; }//if wont exceed bounds of string
    switch (Identifier) {
        case 'r':
            if(elemAfterIdentifier != '='){//if element after identifier isnt = which means it is not of the form r=... but rather (r(...
                VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][1] = 8; //polar identifier
            } else if(elemAfterIdentifier == '='){
                VectorDataInstancePointer->calculateAsPolar = true;//calculate the end equation as polar
                plogicPointer->currentStringPosition++;//increment now and after this since need to skip two elements, 'r='
            }
            plogicPointer->currentStringPosition++;
            break;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::whiteSpaceLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    //if identifier is a space bar
    if(Identifier == 32){
        //increment string position and don't do anything else.
        plogicPointer->currentStringPosition++;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::SliderValueLogic(char Identifier, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    if(Identifier == '$'){
        VectorDataInstancePointer->SliderValuesVectorCoords.push_back(plogicPointer->currentRow);
        VectorDataInstancePointer->SliderValuesVectorCoords.push_back(plogicPointer->currentColumn); //push back row, column coords of vector with slider value logic onto vector
        plogicPointer->currentStringPosition++;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::EquationModifierLogic(char Identifier, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    if(Identifier == '#'){
        VectorDataInstancePointer->EquationModifierCoords.push_back(plogicPointer->currentRow);
        VectorDataInstancePointer->EquationModifierCoords.push_back(plogicPointer->currentColumn); //push back row, column coords of vector with eqn modifier value logic onto vector
        int HashTagPosition = 1; //assumes hashtag to be first positio and incremented if hashtags before it.
        int tempCurrentStringPosition = plogicPointer->currentStringPosition;
        while (tempCurrentStringPosition > 0) {
            if((StringFromSwift[tempCurrentStringPosition] == '#')&&(tempCurrentStringPosition != plogicPointer->currentStringPosition)){ HashTagPosition++;} //calculates how many hashtags are before this and therefore what points to grab from slider values
            tempCurrentStringPosition--;
        }
        //records hashtag position in VectorDataInstancePointer to be used later in CoordinatesUpdater
        VectorDataInstancePointer->TermsArray[plogicPointer->currentRow][plogicPointer->currentColumn][14] = (float)HashTagPosition;
        
        plogicPointer->currentStringPosition++;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::AnimationModifierLogic(char Identifier, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    if(Identifier == '~'){
        VectorDataInstancePointer->animationModifierCoords.push_back(plogicPointer->currentRow);
        VectorDataInstancePointer->animationModifierCoords.push_back(plogicPointer->currentColumn); //push back row, column coords of vector with eqn modifier value logic onto vector
        plogicPointer->currentStringPosition++;
    }
}

void Grapher::ThreeDeeExpressionArray::StringParser::nestLevelAndPositionUpdater(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    plogicPointer->currentStringPosition = 0;

    //resets counter variables
    plogicPointer->currentRow = 0;
    plogicPointer->currentColumn = 0;
    plogicPointer->currentNestLevel = 0;
    plogicPointer->currentNestPosition = 0;
    plogicPointer->ParenthesesClosureCounter = 0;
    auto CurrentVectorRowSize = VectorDataInstancePointer->TermsArray[plogicPointer->currentRow].size();
    
    int rowCounter = 0;
    
    //runs until it runs out of terms
    while(rowCounter < VectorDataInstancePointer->TermsArray.size()){
        plogicPointer->currentColumn = 0;
        CurrentVectorRowSize = VectorDataInstancePointer->TermsArray[rowCounter].size();
        //runs while y is within the row's bounds
        //doesnt set exponentiated vectors to take argument as the exponent yet. look at pages document for reference.
        while (plogicPointer->currentColumn < CurrentVectorRowSize) {
            float Identifier = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][0];
            
            float maxNestLevel = plogicPointer->maxNestedPositionsOfNestedLevels[rowCounter].size()-1; //only works since each term is enclosed with parentheses...
            //if identifier is an open bracke
            if((Identifier == 1.0f)||(Identifier == 3.0f)){
                plogicPointer->currentNestLevel = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2];
                plogicPointer->currentNestPosition = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][3];
            }
            //if encountering open brackets, go to the vector before this and update the nestedPosition its gonna use as argument from the nestedLevel below it to the currentNestPosition
            //also checks if nest level is greater than a single open bracket. a single open bracket indicates the START of a term like '('c*5)*(T+(6.7*c))
            if(((Identifier == 1.0f)||(Identifier == 3.0f))&&(plogicPointer->currentNestLevel > 1)){
                //if the vector right behind has NOT had its addSubtractMultiplyDivide thingy set then and THEN ONLY update its nested position. this stuff is supposed to be used for COMPOSITION, not MULTIPLICATION in weird things like ((s)(c))
                float tempTermExponent = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][4];
                float tempTermCoefficient = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][5];
                float tempNestLevel = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][2];
                //if the vector before this is also NOT of the same nest level
                if((VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][10] == 5.0f)&&(VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][2] != maxNestLevel)&&(tempNestLevel != VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2])){
                    VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][11] = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][3];
                }
                //if the vector before is just an open parentheses with the equation 1x^1, then also set its nestedPosition to be used to the next vector. Vectors like these don't get their value set in the previous conditional if they have some kind of arithmetic operator applied to them in situations like ( '(' (s(2x)) ) / (c) ) since they have a division operator applied to them adn the previous conditional excludes all terms with some kind of arithmetic operator applied even though in this case we need it to have a division operator.
                if((tempTermExponent == 1)&&(tempTermCoefficient == 1)&&(tempNestLevel != VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2])){VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn-1][11] = VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][3];}
            }
                    
                //if at max nest level, set arguments it takes to 0. easy fix to bug.
                if(VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2] == maxNestLevel){
                    VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][11] = 0;
                }
            //updates vectors that don't directly border a parentheses
            if (VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2] == 0.0f){
                VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][2] = plogicPointer->currentNestLevel;
                VectorDataInstancePointer->TermsArray[rowCounter][plogicPointer->currentColumn][3] = plogicPointer->currentNestPosition;
            }
            plogicPointer->currentColumn++;
        }
        rowCounter++;
    }
    
    //resets everything
    plogicPointer->currentStringPosition = 0;

    //resets counter variables
    plogicPointer->currentRow = 0;
    plogicPointer->currentColumn = 0;
    plogicPointer->currentNestLevel = 0;
    plogicPointer->currentNestPosition = 0;
    plogicPointer->ParenthesesClosureCounter = 0;
}




void Grapher::ThreeDeeExpressionArray::inputStringParser(std::string inputString, StringParser* StringParserInstancePointer, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){

        auto stringSize = inputString.size();
        StringParserInstancePointer->StringFromSwift = inputString;
        
        plogicPointer->currentStringPosition = 0;
        
        for(int& h = plogicPointer->currentStringPosition; h < stringSize;){
            StringParserInstancePointer->ParenthesesLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->signParserForNewTerm(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->polynomialLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->trigonometricLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->calculusLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->polarLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->signLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->whiteSpaceLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->SliderValueLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->EquationModifierLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
            StringParserInstancePointer->AnimationModifierLogic(inputString[h], plogicPointer, VectorDataInstancePointer);
        }
        StringParserInstancePointer->nestLevelAndPositionUpdater(plogicPointer, VectorDataInstancePointer);
    
}



void Grapher::ThreeDeeExpressionArray::inputStringCalculator::setSubtypeAndUpdateExpression(int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer){
    float subtypeElement = VectorDataInstancePointer->TermsArray[row][column][1];
    float subtypeElementDuplicate = subtypeElement;
    if((subtypeElement >= 1.0f)&&(subtypeElement <= 6.0f)){subtypeElement = 1.0f;} //if subtype elemtn is within trig ranges set it equal to a single value for a trig identifier in the switch case statement below
    switch ((int)subtypeElement) {
        case 0:
            GrapherInstancePointer->setCurrentSubtypePointer('P'); //sets subtype to polynomial
            break;
        case 1:
            GrapherInstancePointer->setCurrentSubtypePointer('T'); //trig
            break;
        case 7:
            GrapherInstancePointer->setCurrentSubtypePointer('C'); //calculus
            break;
        case 8:
            GrapherInstancePointer->setCurrentSubtypePointer('R'); //polar
            break;
            
        default:
            //NO RECOGNIZED ELEMENTS IS VERY BAD
            break;
    }
    subtypeExpressionArrayUpdater(row, column, (int)subtypeElementDuplicate, plogicPointer, VectorDataInstancePointer); //updates expression array of subtype
}

inline void Grapher::ThreeDeeExpressionArray::inputStringCalculator::subtypeExpressionArrayUpdater(int row, int column, int expressionType, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    
    

    float termCoefficient = VectorDataInstancePointer->TermsArray[row][column][5];
    float termExponent = VectorDataInstancePointer->TermsArray[row][column][4];
    int addSubtractMultiplyDivide;
    
    int currentNestLevel = VectorDataInstancePointer->TermsArray[row][column][2]; int currentNestPosition = VectorDataInstancePointer->TermsArray[row][column][3];
    
    //if previous vector is of same nest level and position grab its identifier else set it to nil
    if((column != 0)&&(VectorDataInstancePointer->TermsArray[row][column-1][2] == currentNestLevel)&&(VectorDataInstancePointer->TermsArray[row][column-1][3]==currentNestPosition)){addSubtractMultiplyDivide = VectorDataInstancePointer->TermsArray[row][column-1][10];}else{addSubtractMultiplyDivide = 5.0f;}
    
    if((expressionType >= 1)&&(expressionType <= 6)){expressionType = 1;} //if of type trigonometric set to case 1.
    
    switch (expressionType) { //expression type passed from setSubtypeAndUpdateExpression
        case 0: //if polynomial expression then pass the following parameters
            //termPositionPolynomial++; //increment termPosition each time but is commented for now since each subtype for now only evaluates a single term
            GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, termExponent, termCoefficient, addSubtractMultiplyDivide);
            break;
        case 1:{
            //termPositionTrigonometric++;//increment termPosition each time but is commented for now since each subtype for now only evaluates a single term
            int trigID = VectorDataInstancePointer->TermsArray[row][column][1];
            GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, (float)addSubtractMultiplyDivide, termCoefficient, trigID);
        }
            break;
        case 7:{
            //termPositionCalculus++; //increment termPosition each time but is commented for now since each subtype for now only evaluates a single term
            //hardcoded while features are developed
            float upperBound = 0.0f;
            float lowerBound = 0.0f;
            int typeIdentifier = 105;
            GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, upperBound, lowerBound, typeIdentifier);
        }
            break;
        case 8:
            //termPositionPolar++; //increment termPosition each time but is commented for now since each subtype for now only evaluates a single term
            //same as polynomial
            GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, termExponent, termCoefficient, addSubtractMultiplyDivide);
            break;
        default:
            break;
    }
}

//also clears the expression array when it ends so future additions to the expression array won't result in wrong coords due to ghosts from previous calls
inline void Grapher::ThreeDeeExpressionArray::inputStringCalculator::subtypeCoordinatesUpdater(int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, std::vector<float> providedArray){ //provided array defaults to dummy vector for non-nested expressions. provide array during nested calls
    int exponentiationIdentifier = VectorDataInstancePointer->TermsArray[row][column][12]; //0 for array to be used as base 1 to be used as exponentiation in polynomial
    bool nestedArgumentOrNot = VectorDataInstancePointer->TermsArray[row][column][11];//0 if base expression and >0 if it has a nested argument
        if((VectorDataInstancePointer->TermsArray[row][column][12] ==  1.0f)&&(VectorDataInstancePointer->TermsArray[row][column][2]==plogicPointer->maxNestedPositionsOfNestedLevels[row].size()-1)) //if at maxNestLevel and on term with exponent identifier there is actually nothing being taken for argument but the nestedPositionBelowToBeUsedAsArgument will be non-zero if the base and exponent are right next to each other this is a known bug so we need to change nestedArgumentOrNot to false since nothing is being used as a nestedArgument thats being taken care of by Exponentiation edge case logic
            {nestedArgumentOrNot = false;}
                    //false if at max nest level too.
                    if(VectorDataInstancePointer->TermsArray[row][column][2] == plogicPointer->maxNestedPositionsOfNestedLevels[row].size()-1){nestedArgumentOrNot = false;}
                exponentiationIdentifier = 0; //HARDCODED TO FALSE FOR NOW-- REFER TO PAGES DOCUMENTATION!!!
    
    if(providedArray == dummyVector){nestedArgumentOrNot = false;}//if dummy vector is provided set nestedArgument to false as it should be
        
    int expressionType = (int)VectorDataInstancePointer->TermsArray[row][column][1];
    if((expressionType >= 1)&&(expressionType <= 6)){expressionType = 1.0f;}
    
    std::vector<float> CoefficientArray = {}; //used for EqnModifiers ('#')
    //if the current vector has a hashtag identifier then set CoefficientArray to provided coefficients
    if((VectorDataInstancePointer->TermsArray[row][column].size() == 15)&&(VectorDataInstancePointer->TermsArray[row][column][14] > 0)){
        int hashtagPosition = VectorDataInstancePointer->TermsArray[row][column][14];
        auto PointsToCalculate = GrapherInstancePointer->PointerToGraphingCoordsStruct->xRange/GrapherInstancePointer->PointerToGraphingCoordsStruct->xInterval;
        std::vector<float>& EqnValueArray = VectorDataInstancePointer->EquationValues;
        //grabs coefficient values corresponding to the hashtag from the equation array
        std::vector<float> EqnModifierCoefficientArray(EqnValueArray.begin()+(PointsToCalculate*(hashtagPosition-1)),EqnValueArray.begin()+(PointsToCalculate*(hashtagPosition)));
        CoefficientArray = EqnModifierCoefficientArray;
    }
    
    switch (expressionType) {
        case 0:
            GrapherInstancePointer->CurrentGraphingSubtypePointer->CoordinatesUpdater(nestedArgumentOrNot, providedArray, exponentiationIdentifier,CoefficientArray);
            break;
        case 1:
            GrapherInstancePointer->CurrentGraphingSubtypePointer->CoordinatesUpdater(nestedArgumentOrNot, providedArray, 0,CoefficientArray);//last parameter unused so hardcoded
            break;
        case 7:
            GrapherInstancePointer->CurrentGraphingSubtypePointer->CoordinatesUpdater(nestedArgumentOrNot, providedArray, 0,CoefficientArray);//last parameter unused so hardcoded
            break;
        case 8:
            GrapherInstancePointer->CurrentGraphingSubtypePointer->CoordinatesUpdater(nestedArgumentOrNot, providedArray, exponentiationIdentifier,CoefficientArray);
            break;
            
        default:
            //super bad
            break;
    }
    GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayClear();
}

//remember to pass the addSubtractMultiplyDivide identifier from the PREVIOUS vector in the same Position!!!!!!!
//if identifier doesnt match value is simply assigned
void Grapher::ThreeDeeExpressionArray::inputStringCalculator::addCoordinatesToCalculatedValuesArray(std::vector<float> coordinatesArray, int nestLevel, int position, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, int addSubtractMultiplyDivideIdentifier){
    //if # rows less than whats needed then add a row. same with nestPosition/columns
    if(VectorDataInstancePointer->CalculatedValuesForComposition.size() <= nestLevel){
        VectorDataInstancePointer->CalculatedValuesForComposition.resize(nestLevel+1, {{0}});//resize calculatedValues to accomodate max number of nest levels. will only be resized on first call since that will be with the highest nest level
    }
        if(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel].size() <= position){VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel].resize(position+1,{0});}

    switch (addSubtractMultiplyDivideIdentifier) {
        case 0: //adds elements to calculatedValues corresponding vector as you have addition identifier
            std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].end(), coordinatesArray.begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), std::plus<>());
            break;
        case 1: //subtracts existing values in CalculatedValues vectors
            std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].end(), coordinatesArray.begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), std::minus<>());
            break;
        case 2://multiplies existing values in CalculatedValues vector
            std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].end(), coordinatesArray.begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), std::multiplies<>());
            break;
        case 3: //divides values in calculated values vector by given values
            std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].end(), coordinatesArray.begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position].begin(), std::divides<>());
            break;
        default:
            VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][position] = coordinatesArray; //if no identifier (like (x) then just assign the value
            break;
    }
}

void Grapher::ThreeDeeExpressionArray::inputStringCalculator::clearCalculatedValuesArray(VectorData* VectorDataInstancePointer) {
    VectorDataInstancePointer->CalculatedValuesForComposition.clear();
}

void Grapher::ThreeDeeExpressionArray::inputStringCalculator::CoordinatesUpdateAcrossNestLevel(int termNumber, int nestLevel, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    //termNumber starts from 0. its just the row in the TermsArray
    int nestLevelPosition;
    auto numberOfTermsTotal = VectorDataInstancePointer->TermsArray[termNumber].size();
    std::vector<float> ResultVector; //to be fed into addCoordinatesToCalcualtedValuesArray function
    
    auto maxNestLevel = plogicPointer->maxNestedPositionsOfNestedLevels[termNumber].size()-1; //greatest nest level. if equal to this nest level dont pull argument from CalculatedValues
    std::vector<float> argumentVector = {0}; //to be fed in case nest level is greater than the base one
    
    
    for (int huy = 0; huy < numberOfTermsTotal; huy++) {
        if(VectorDataInstancePointer->TermsArray[termNumber][huy][2] == nestLevel){ //if the nestLevel of the current vector equals the target nest level
                nestLevelPosition = (int)VectorDataInstancePointer->TermsArray[termNumber][huy][3]; //sets the nestPosition in that specific nestLevel to the number recorded inside
                    
            //if greater than base nest level, set argument vector to previously calculated value for nest level one above and corresponding nest position
            if(nestLevel < (int)maxNestLevel){
                int argumentNestLevel = nestLevel+1;
                int argumentNestPosition = (int)VectorDataInstancePointer->TermsArray[termNumber][huy][11];
                
                argumentVector = VectorDataInstancePointer->CalculatedValuesForComposition[argumentNestLevel][argumentNestPosition];
            } else {argumentVector = dummyVector;}
            
                    //only runs this logic when not on an exponentiated term! otherwise skips.
                    if(ExponentiationEdgeCaseLogic(nestLevel, nestLevelPosition, termNumber, huy, plogicPointer, VectorDataInstancePointer, argumentVector, huy)){
                        continue;}
            
                setSubtypeAndUpdateExpression(termNumber, huy, plogicPointer, VectorDataInstancePointer); //updates respective expression array
                subtypeCoordinatesUpdater(termNumber, huy, plogicPointer, VectorDataInstancePointer, argumentVector); //updates coordinates for expression stored
                ResultVector = GrapherInstancePointer->CurrentGraphingSubtypePointer->returnCoordinatesArray(); //assigns vector to be passed as argument
            int addSubtractMultiplyDivideIdentifier = 6;
            //if the column before this is in the same nest level and position, grab its arithmetic identifier. else set it to default value where the value is semply assigned to the respective poisiton in CalculatedValuesArray
            //dont run when huy i s0 since thats the first vector and theres no vector before that.
            for (int b = huy-1; b >= 0; b--) {//search all the terms before this one in the same nestLevel/nestPosition and if it has an arithmetic identifier grab that one
                if((VectorDataInstancePointer->TermsArray[termNumber][b][2]==nestLevel)&&(VectorDataInstancePointer->TermsArray[termNumber][b][3]==nestLevelPosition)){
                    addSubtractMultiplyDivideIdentifier = (int)VectorDataInstancePointer->TermsArray[termNumber][b][10];
                    break;//break when reaching a term with an artihmetic identifier on the same nest level and position as this one and before this one
                } else{
                    addSubtractMultiplyDivideIdentifier = 6;
                }
            }
            /*if((huy!=0)&&(VectorDataInstancePointer->TermsArray[termNumber][huy-1][2] == nestLevel)&&(VectorDataInstancePointer->TermsArray[termNumber][huy-1][3] == nestLevelPosition)){
                addSubtractMultiplyDivideIdentifier = (int)VectorDataInstancePointer->TermsArray[termNumber][huy-1][10];
            } else {
                addSubtractMultiplyDivideIdentifier = 6;
            }*/
                addCoordinatesToCalculatedValuesArray(ResultVector, nestLevel, nestLevelPosition, plogicPointer, VectorDataInstancePointer, addSubtractMultiplyDivideIdentifier); //adds values to 3d array
        }
    }
    for (int yuH = (int)numberOfTermsTotal-1; yuH >= 0; yuH--) {
        //if nestLevelPosition contains division identifier, grab its nestPosition, increment it, then divide the current column nestPosition array by its next nestPosition array
        if((VectorDataInstancePointer->TermsArray[termNumber][yuH][10] == 3.0f)&&(VectorDataInstancePointer->TermsArray[termNumber][yuH][2]==nestLevel)){
            int temporaryCurrentNestLevel = VectorDataInstancePointer->TermsArray[termNumber][yuH][2];
            int temporaryCurrentNestPosition = VectorDataInstancePointer->TermsArray[termNumber][yuH][3];
            std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[temporaryCurrentNestLevel][temporaryCurrentNestPosition].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[temporaryCurrentNestLevel][temporaryCurrentNestPosition].end(), VectorDataInstancePointer->CalculatedValuesForComposition[temporaryCurrentNestLevel][temporaryCurrentNestPosition+1].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[temporaryCurrentNestLevel][temporaryCurrentNestPosition].begin(), std::divides<>());
        }
    }
}

//returns true on either position of a vector that is part of an exponentiated term!!!
//argument vector is provided if this has a nest level underneath and requires argumetns from that. otherwise a dummy vector should be provided

//FIX THIS-- REALIZE YOU ARE ONLY CALCULATING EXPRESSION OF A SINGLE COLUMN BEYOND THE EXPONENT WHERE THERE MAY BE MANY. SAME APPLIES TO BASE ITSELF ITS BEING TREATED AS ONE TERM!!!
bool Grapher::ThreeDeeExpressionArray::inputStringCalculator::ExponentiationEdgeCaseLogic(int nestLevel, int nestPosition, int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, std::vector<float> argumentVector, int& counterVariableToModify){
    
    auto maxNestLevel = plogicPointer->maxNestedPositionsOfNestedLevels[row].size()-1; //greatest nest level. if equal to this nest level dont pull argument from CalculatedValues
    
    if(VectorDataInstancePointer->TermsArray[row][column][12] == 1){//if exponent identifier
        
        int startOfExponentArrayPosition = 1; //we are going to increment forward until we reach the next nest position within the same nest level
        
        //increment until you find the first column that is of the SAME nest level but of nest position PLUS 1. such a vector MUST EXIST IF EXPONENTIATION TERM EXISTS
        while(startOfExponentArrayPosition < VectorDataInstancePointer->TermsArray[row].size()-column){
            if((VectorDataInstancePointer->TermsArray[row][column+startOfExponentArrayPosition][3] == VectorDataInstancePointer->TermsArray[row][column][3]+1)&&(VectorDataInstancePointer->TermsArray[row][column+startOfExponentArrayPosition][2] == nestLevel)){break;}
            startOfExponentArrayPosition++;
        }
        int currentPosition = startOfExponentArrayPosition;
        std::vector<float> exponentArray;
        while(currentPosition < VectorDataInstancePointer->TermsArray[row].size()-column){ //cycle through remaining columns
            if((VectorDataInstancePointer->TermsArray[row][column+currentPosition][2] == nestLevel)&&(VectorDataInstancePointer->TermsArray[row][column+currentPosition][3]==nestPosition+1)){ //if a column belongs to the same nest level and the desired nestPosition (1 above the exponentiated base) then calculate its values
                
                //the argument vector to be used for exponent term calculation in case the exponent itsel fha smore nested parentheses
                int exponentArgumentVectorNestPosition = VectorDataInstancePointer->TermsArray[row][column+currentPosition][11];
                
                std::vector<float> exponentArgumentVector;
                if((nestLevel < plogicPointer->maxNestedPositionsOfNestedLevels[row].size()-1)&&(exponentArgumentVectorNestPosition != 0)){ //if NOT at max nest level, pull argument vector. otherwise set to dummy vector also if the position to be pulled is 0 which means no argument either.
                    
                    //if the nest position doesnt makae sense (specifically pertaining to how base and exponent seem to have nested argument values that dont actually exist sometimes) then set to dummy vector. else carry on logic
                    if(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel+1].size() <= exponentArgumentVectorNestPosition){exponentArgumentVector = dummyVector;} else {
                        exponentArgumentVector = VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel+1][exponentArgumentVectorNestPosition];
                    }
                } else { exponentArgumentVector = dummyVector;}
                
                setSubtypeAndUpdateExpression(row, column+currentPosition, plogicPointer, VectorDataInstancePointer); //we are doing column +currentPosition to calculate the exponent first
                subtypeCoordinatesUpdater(row, column+currentPosition, plogicPointer, VectorDataInstancePointer, exponentArgumentVector);
                counterVariableToModify = column+currentPosition; //sets counter variable to the last vector of the exponentiation expression. this is so that when the CurrentNestLevel function continues through its current iteration it increments 'huy' to the vector right after the exponent term effectively skipping base and exponent
                exponentArray = GrapherInstancePointer->CurrentGraphingSubtypePointer->returnCoordinatesArray();
                
                int addSubtractMultiplyDivideIdentifierForExponent;
                //if there is vector before this in the same nest level and position+1 grab its arithmetic identifier else set it to default of 5.0f
                if((VectorDataInstancePointer->TermsArray[row][column+currentPosition-1][2] == nestLevel)&&(VectorDataInstancePointer->TermsArray[row][column+currentPosition-1][3] == nestPosition+1)){
                    addSubtractMultiplyDivideIdentifierForExponent = VectorDataInstancePointer->TermsArray[row][column+currentPosition-1][10];
                } else {addSubtractMultiplyDivideIdentifierForExponent = 5.0f;}
                addCoordinatesToCalculatedValuesArray(exponentArray, nestLevel, nestPosition+1, plogicPointer, VectorDataInstancePointer, addSubtractMultiplyDivideIdentifierForExponent);
            }
            currentPosition++;
        }
        //exponentiation array to be used as argument
            exponentArray = VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][nestPosition+1];
        
        std::vector<float> intermediateVector;
        
        int addSubtractMultiplyDivideIdentifier;
        int currentPositionBase = 0;
        while (currentPositionBase < VectorDataInstancePointer->TermsArray[row].size()-column) {
            
            //if the column before this is in the same nest level and position, grab its arithmetic identifier. else set it to default value where the value is semply assigned to the respective poisiton in CalculatedValuesArray
            if((column != 0)&&(VectorDataInstancePointer->TermsArray[row][column+currentPositionBase-1][2] == nestLevel)&&(VectorDataInstancePointer->TermsArray[row][column+currentPositionBase-1][3] == nestPosition)){
                addSubtractMultiplyDivideIdentifier = (int)VectorDataInstancePointer->TermsArray[row][column+currentPositionBase-1][10];
            } else {
                addSubtractMultiplyDivideIdentifier = 6;
            }
            //if in desired nest position and level corresponding to base carry out arithmetic logic and stuff
            if((VectorDataInstancePointer->TermsArray[row][column+currentPositionBase][2] == nestLevel)&&(VectorDataInstancePointer->TermsArray[row][column+currentPositionBase][3]==nestPosition)){
                //argument vector is one nest level above and the correspondign nest position to be used as argument
                setSubtypeAndUpdateExpression(row, column+currentPositionBase, plogicPointer, VectorDataInstancePointer);
                //if greater than base nest level, set argument vector to previously calculated value for nest level one above and corresponding nest position
                std::vector<float> NewArgumentVector;
                if(nestLevel < (int)maxNestLevel){
                    int NewArgumentNestLevel = nestLevel+1;
                    int NewArgumentNestPosition = (int)VectorDataInstancePointer->TermsArray[row][column+currentPositionBase][11];
                    
                    NewArgumentVector = VectorDataInstancePointer->CalculatedValuesForComposition[NewArgumentNestLevel][NewArgumentNestPosition];
                } else {argumentVector = dummyVector;}
                if(currentPositionBase != 0){argumentVector = NewArgumentVector;}//if we just potentially got the argument vector for another term then set the argument vector to the new argument vector grabbed. we are checkng for currentPositionBase != 0 since that means he while loop is only calculating for this vector
                subtypeCoordinatesUpdater(row, column+currentPositionBase, plogicPointer, VectorDataInstancePointer,argumentVector);
                intermediateVector = GrapherInstancePointer->CurrentGraphingSubtypePointer->returnCoordinatesArray();
                
                addCoordinatesToCalculatedValuesArray(intermediateVector, nestLevel, nestPosition, plogicPointer, VectorDataInstancePointer, addSubtractMultiplyDivideIdentifier);
            }
            
            currentPositionBase++;
        }
        //raises base to the power of the exponent and assigns the transformed vector to the nestPosition passed down a.k.a. the nest position of the base
        std::transform(VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][nestPosition].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][nestPosition].end(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][nestPosition+1].begin(), VectorDataInstancePointer->CalculatedValuesForComposition[nestLevel][nestPosition].begin(), [](float a, float b){return std::powf(a, b);});
        return true;
    } else {
        return false; //returns false when not on a term with an exponent identifier. we dont check for a term BEFORE with an exponent identifier as logic would dictate since the above conditional is guaranteed to have gone through a term with an exponentiation identifier and set huy to such a way that it skips the base and the exponent
    }
}

//returns final value across a row
std::vector<float> Grapher::ThreeDeeExpressionArray::inputStringCalculator::CoordinatesUpdateAcrossRow(int row, PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    auto maxNestLevel = plogicPointer->maxNestedPositionsOfNestedLevels[row].size()-1; //the maximum nest level is the size of the corresponding row in the nest level array -1 since row 0 is assumed to have no nest levels and if you have uptil nest level 1 .size() will yield 2 when you actually want 1
    auto sizeOfRow = VectorDataInstancePointer->TermsArray[row].size();
    
    //loops through nest levels and resizes calculated valeus for composition's vectors for the corresponding nest level to accomodate the maximum number of nest positions needed.
    VectorDataInstancePointer->CalculatedValuesForComposition.resize(maxNestLevel+1); //resized the 3d vector to accomodate all nest levels
    for (int gf = 0; gf < plogicPointer->maxNestedPositionsOfNestedLevels[row].size(); gf++) {
        VectorDataInstancePointer->CalculatedValuesForComposition[gf].resize(plogicPointer->maxNestedPositionsOfNestedLevels[row][gf]+1);
    }
    
    std::vector<float> rowFinalValue;
    
    for(int a = 0; a < maxNestLevel; a++){ //cycles through each nest level
        int currentTargetNestLevel = (int)maxNestLevel-a;
        CoordinatesUpdateAcrossNestLevel(row, currentTargetNestLevel, plogicPointer, VectorDataInstancePointer); //updates all the nest positions for the selected nest level
        
        
        if(currentTargetNestLevel == 0){break;}//break if no parentheses
        if(currentTargetNestLevel == 1){ //if finished calculating all nest levels, proceed with final arithmetic
            int k = 0;
            int addSubtractMultiplyDivideIdentifier;
            while (k < sizeOfRow){
                if(VectorDataInstancePointer->TermsArray[row][k][2] == 1.0f){ //if nest level is the lowest
                    //if at starting nest position, then push back first vector from CalculatedValuesForComposition. no addSubtractMultiplyDivide identifier
                    if(VectorDataInstancePointer->TermsArray[row][k][3] == 1){rowFinalValue = VectorDataInstancePointer->CalculatedValuesForComposition[1][1];}
                    else {
                        if(k!=0){
                            addSubtractMultiplyDivideIdentifier = VectorDataInstancePointer->TermsArray[row][k-1][10]; //grab identifier from previous nestPosition
                            int nestLevelPosition = VectorDataInstancePointer->TermsArray[row][k][3];
                            switch (addSubtractMultiplyDivideIdentifier) {
                                case 0: //adds value of current position in highest-order nest level to the final value
                                    std::transform(rowFinalValue.begin(), rowFinalValue.end(), VectorDataInstancePointer->CalculatedValuesForComposition[1][nestLevelPosition].begin(), rowFinalValue.begin(), std::plus<>());
                                    break;
                                case 1://subtracts value of current position in highest-order nest level to the final value
                                    std::transform(rowFinalValue.begin(), rowFinalValue.end(), VectorDataInstancePointer->CalculatedValuesForComposition[1][nestLevelPosition].begin(), rowFinalValue.begin(), std::minus<>());
                                    break;
                                case 2: //multiplies value of current position in highest-order nest level to the final value
                                    std::transform(rowFinalValue.begin(), rowFinalValue.end(), VectorDataInstancePointer->CalculatedValuesForComposition[1][nestLevelPosition].begin(), rowFinalValue.begin(), std::multiplies<>());
                                    break;
                                case 3: //divides value of current position in highest-order nest level to the final value
                                    std::transform(rowFinalValue.begin(), rowFinalValue.end(), VectorDataInstancePointer->CalculatedValuesForComposition[1][nestLevelPosition].begin(), rowFinalValue.begin(), std::divides<>());
                                    break;
                                    
                                default:
                                    //no identifier?!? what???
                                    break;
                            }
                        } else {rowFinalValue = VectorDataInstancePointer->CalculatedValuesForComposition[1][0];} //if on the first iteration, assign value to rowFinalValue
                    }
                }
                k++;
            }
            break;
        }
    }
    VectorDataInstancePointer->CalculatedValuesForComposition.clear(); //clear this everytime so it can be resued across row calls??
    return rowFinalValue;
}

std::vector<float> Grapher::ThreeDeeExpressionArray::inputStringCalculator::InputtedStringCalculator(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    for (int l = 0; l < VectorDataInstancePointer->TermsArray.size(); l++) {//goes for as many rows as there are
        if(l == 0){
            VectorDataInstancePointer->finalValueOfRows = CoordinatesUpdateAcrossRow(l, plogicPointer, VectorDataInstancePointer);
        } else {
            char rowAddSubtractMultiplyDivideIdentifier = VectorDataInstancePointer->TermsArray[l][0][13]; //the signage in BETWEEN rows
            std::vector<float> returnedArray = CoordinatesUpdateAcrossRow(l, plogicPointer, VectorDataInstancePointer);
            switch (rowAddSubtractMultiplyDivideIdentifier) {
                case '+': //add new row to previous row of points
                    std::transform(VectorDataInstancePointer->finalValueOfRows.begin(), VectorDataInstancePointer->finalValueOfRows.end(), returnedArray.begin(), VectorDataInstancePointer->finalValueOfRows.begin(), std::plus<>());
                    break;
                case '-': //subtracts new row to previous row of points
                    std::transform(VectorDataInstancePointer->finalValueOfRows.begin(), VectorDataInstancePointer->finalValueOfRows.end(), returnedArray.begin(), VectorDataInstancePointer->finalValueOfRows.begin(), std::minus<>());
                    break;
                case '*': //multiplies new row to previous row of points
                    std::transform(VectorDataInstancePointer->finalValueOfRows.begin(), VectorDataInstancePointer->finalValueOfRows.end(), returnedArray.begin(), VectorDataInstancePointer->finalValueOfRows.begin(), std::multiplies<>());
                    break;
                case '/': //divides new row to previous row of points
                    std::transform(VectorDataInstancePointer->finalValueOfRows.begin(), VectorDataInstancePointer->finalValueOfRows.end(), returnedArray.begin(), VectorDataInstancePointer->finalValueOfRows.begin(), std::divides<>());
                    break;
                    
                default:
                    //no signage between rows/terms?!?.
                    break;
            }
        }
    }
    
    if(VectorDataInstancePointer->calculateAsPolar){
        GrapherInstancePointer->setCurrentSubtypePointer('R');
        GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayUpdater(true, 1, 1, 1, 5);
        std::vector<float> ProvidedArray = VectorDataInstancePointer->finalValueOfRows;
        VectorDataInstancePointer->finalValueOfRows = GrapherInstancePointer->CurrentGraphingSubtypePointer->CoordinatesUpdater(true, ProvidedArray, 0,{});
        GrapherInstancePointer->CurrentGraphingSubtypePointer->ExpressionArrayClear();
    }

    return VectorDataInstancePointer->finalValueOfRows;
}

//converts coords vector in grapher to have x points at even slots
void Grapher::ThreeDeeExpressionArray::inputStringCalculator::convertCoordinatesToHaveBothXandYpoints(){
    float tempXinterval = GrapherInstancePointer->PointerToGraphingCoordsStruct->xInterval;
    float tempXrangeValue = GrapherInstancePointer->PointerToGraphingCoordsStruct->xRange;
    int tempPointsToCalculate = (int)(tempXrangeValue/tempXinterval);
    int tempSizeOfVector = (int)GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector.size();
    if(tempSizeOfVector < tempPointsToCalculate*2){
        GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector.reserve(tempPointsToCalculate*2);
        std::vector<float> TempXYcoordPoints;//to contain x,y coords in such a manner that metal can use them for rasterizing triangles. refer to pages for documentation. should also clip/overwrite coordinates that are part a triangle that has ANY coordinates outside of NDC boundaires to prevent clipped triangle portions leaking into the screen
        float* PointerToCoordsVector = &GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector[0];
        TempXYcoordPoints.reserve(tempPointsToCalculate*2);
        for (int d = 0; d < tempSizeOfVector; d++) {
            TempXYcoordPoints.push_back((d*tempXinterval)-tempXrangeValue/2.0f);
            TempXYcoordPoints.push_back(PointerToCoordsVector[d]);
            //GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector.insert(GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector.begin()+(d*2), ((d*tempXinterval)-tempXrangeValue/2.0f));
                    
        }
        GrapherInstancePointer->PointerToGraphingCoordsStruct->XYcoordsVector = TempXYcoordPoints;
    }
}

void Grapher::ThreeDeeExpressionArray::CoordinatesCalculator(std::string inputString, StringParser* StringParserInstancePointer, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, bool parametricOrNot, std::vector<float> SliderSpecialValues, std::vector<float>& EquationModifierSpecialValues, float animationValue){
    if(inputString == StringParserInstancePointer->StringFromSwift){
        recalculationOfSameString = true;
        resetThreeDeeArrayForSameStringRecalculation(plogicPointer, VectorDataInstancePointer, StringParserInstancePointer);
        VectorDataInstancePointer->SliderValuesVectorCoords.clear();
            VectorDataInstancePointer->SliderValues.clear();
        //VectorDataInstancePointer->EquationModifierCoords.clear();
            VectorDataInstancePointer->EquationValues.clear();
        //VectorDataInstancePointer->animationModifierCoords.clear(); //need to preserve coords from previous call!!!!!!
            VectorDataInstancePointer->animationValue = animationValue;

        ThePointerToCurrentGrapherInstance->SwiftStringSliderValueUpdater(SliderSpecialValues);
        ThePointerToCurrentGrapherInstance->SwiftStringEquationModifierUpdater(EquationModifierSpecialValues);
        ThePointerToCurrentGrapherInstance->SwiftStringAnimationModifierUpdater(animationValue);
        //inputStringParser(inputString, StringParserInstancePointer, plogicPointer, VectorDataInstancePointer);
        inputtedStringEquationModifierUpdater(plogicPointer, VectorDataInstancePointer);
        inputtedStringSliderConstantUpdater(plogicPointer, VectorDataInstancePointer);
        inputtedStringAnimationUpdater(plogicPointer, VectorDataInstancePointer);
    } else {
        recalculationOfSameString = false;
        resetThreeDeeArray(plogicPointer, VectorDataInstancePointer, StringParserInstancePointer);
        ThePointerToCurrentGrapherInstance->SwiftStringSliderValueUpdater(SliderSpecialValues);
        ThePointerToCurrentGrapherInstance->SwiftStringEquationModifierUpdater(EquationModifierSpecialValues);
        ThePointerToCurrentGrapherInstance->SwiftStringAnimationModifierUpdater(animationValue);
        inputStringParser(inputString, StringParserInstancePointer, plogicPointer, VectorDataInstancePointer);
        inputtedStringEquationModifierUpdater(plogicPointer, VectorDataInstancePointer);
        inputtedStringSliderConstantUpdater(plogicPointer, VectorDataInstancePointer);
        inputtedStringAnimationUpdater(plogicPointer, VectorDataInstancePointer);
    }
    //sets graphign coords struct result to what is outputted by inputStringCalculatorInstancePointer
    ThePointerToCurrentGrapherInstance->PointerToGraphingCoordsStruct->XYcoordsVector = inputStringCalculatorInstancePointer->InputtedStringCalculator(plogicPointer, VectorDataInstancePointer);
    inputStringCalculatorInstancePointer->convertCoordinatesToHaveBothXandYpoints();
    
    //std::cout << "Done";

}

void Grapher::ThreeDeeExpressionArray::resetThreeDeeArray(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, StringParser* StringParserInstancePointer){
    VectorDataInstancePointer->TermsArray.clear();
    VectorDataInstancePointer->CalculatedValuesForComposition.clear();
    VectorDataInstancePointer->finalValueOfRows.clear();
    
    plogicPointer->currentNestLevel = 0;
    plogicPointer->currentNestPosition = 0;
    plogicPointer->ParenthesesClosureCounter = 0;
    plogicPointer->currentColumn = 0;
    plogicPointer->currentRow = 0;
    plogicPointer->currentStringPosition = 0;
    plogicPointer->lengthOfString = 0;
    plogicPointer->CurrentPointerCoordinates[0] = 0;
    plogicPointer->CurrentPointerCoordinates[1] = 0;
    plogicPointer->CurrentPointerCoordinates[2] = 0;
    plogicPointer->maxNestedPositionsOfNestedLevels = {{0}};
    
    VectorDataInstancePointer->SliderValuesVectorCoords.clear();
        VectorDataInstancePointer->OriginalSliderCoefficientValues.clear();
        VectorDataInstancePointer->SliderValues.clear();
    VectorDataInstancePointer->EquationModifierCoords.clear();
        VectorDataInstancePointer->EquationValues.clear();
        VectorDataInstancePointer->OriginalEquationCoefficientValues.clear();
    VectorDataInstancePointer->animationModifierCoords.clear();
        VectorDataInstancePointer->animationValue = 0.0;
        VectorDataInstancePointer->OriginalAnimationCoefficientValues.clear();
    VectorDataInstancePointer->calculateAsPolar = false;
    
    StringParserInstancePointer->StringFromSwift = "";
    
}

void Grapher::ThreeDeeExpressionArray::resetThreeDeeArrayForSameStringRecalculation(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer, StringParser *StringParserInstancePointer){
    //VectorDataInstancePointer->TermsArray.clear();
    VectorDataInstancePointer->CalculatedValuesForComposition.clear();
    VectorDataInstancePointer->finalValueOfRows.clear();
    
    plogicPointer->currentNestLevel = 0;
    plogicPointer->currentNestPosition = 0;
    plogicPointer->ParenthesesClosureCounter = 0;
    plogicPointer->currentColumn = 0;
    plogicPointer->currentRow = 0;
    plogicPointer->currentStringPosition = 0;
    plogicPointer->lengthOfString = 0;
    plogicPointer->CurrentPointerCoordinates[0] = 0;
    plogicPointer->CurrentPointerCoordinates[1] = 0;
    plogicPointer->CurrentPointerCoordinates[2] = 0;
    //plogicPointer->maxNestedPositionsOfNestedLevels = {{0}};
    
    //StringParserInstancePointer->StringFromSwift = "";
}

//updates coordinates in swift array given a pointer to that array and the input expression string.
void Grapher::CoordinatesUpdaterForSwift(std::string inputExpression, bool dontConvertToXYCoords, std::vector<float> SliderSpecialValues, std::vector<float>& EquationModifierSpecialValues, float animationValue, float* pointerToBuffer){
    PointerToThreeDeeExpressionArrayInstance->CoordinatesCalculator(inputExpression, PointerToThreeDeeExpressionArrayInstance->StringParserInstancePointer, PointerToThreeDeeExpressionArrayInstance->PositionLogicInstancePointer, PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer, false, SliderSpecialValues, EquationModifierSpecialValues, animationValue);
    
    float xScaleFactor = GraphingCoordsStruct.xRange;
    float yScaleFactor = GraphingCoordsStruct.yRange;
    
    int sizeOfCoordsVector = (int)GraphingCoordsStruct.XYcoordsVector.size();

        for (int f = 0; f < sizeOfCoordsVector; f++) {
            if(dontConvertToXYCoords){
                if((f%2)==1){*(pointerToBuffer+(f/2)) = GraphingCoordsStruct.XYcoordsVector[f];}//doesnt convert to XY coords and only fills y coords that were calculated}
            } else{
                *(pointerToBuffer+f) = GraphingCoordsStruct.XYcoordsVector[f];//fills pointer to buffer to be filled with points
            }
        }

    
}

//these three functions combined are to be called by class Object for every modifier value/update. a single frame hapens when all these are updated for the current frame and CoordinatesCalculator is called.
void Grapher::SwiftStringSliderValueUpdater(std::vector<float> SliderSpecialValues){
    auto sizeOfSpecialValues = SliderSpecialValues.size();
    PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->SliderValues.clear();
    PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->SliderValues.reserve(sizeOfSpecialValues);
    for (int f = 0; f < sizeOfSpecialValues; f++) {
        PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->SliderValues.push_back(SliderSpecialValues[f]);
    }
}

void Grapher::SwiftStringEquationModifierUpdater(std::vector<float>& EquationModifierValues){
    auto sizeOfEquationValues = EquationModifierValues.size();
    PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->EquationValues.clear();
    PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->EquationValues.reserve(sizeOfEquationValues);
    for (int j = 0; j < sizeOfEquationValues; j++) {
        PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->EquationValues.push_back(EquationModifierValues[j]);
    }
}

void Grapher::SwiftStringAnimationModifierUpdater(float animationCurrentValue){
    PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer->animationValue = animationCurrentValue;
}

void Grapher::ThreeDeeExpressionArray::inputtedStringSliderConstantUpdater(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    auto SliderValuesSize = VectorDataInstancePointer->SliderValues.size();
    for (int a = 0; a < SliderValuesSize; a++) {
        //loops through slider values array which according to protocol must have corresponding points in SliderValuesCoords
        int rowToModify = VectorDataInstancePointer->SliderValuesVectorCoords[0+(a*2)]; //pulls the row that the current slider value targets
        int columnToModify = VectorDataInstancePointer->SliderValuesVectorCoords[1+(a*2)]; //pulls the column that the current slider value targets
        //if current coefficient is 0 replace by slider value. else multiply.
        if(!recalculationOfSameString){
            //if it is the first time being called, store the original coefficients in the dedicated vector and multiply current coefficient with given values
            VectorDataInstancePointer->OriginalSliderCoefficientValues.push_back(VectorDataInstancePointer->TermsArray[rowToModify][columnToModify][5]);
            VectorDataInstancePointer->TermsArray[rowToModify][columnToModify][5] *= VectorDataInstancePointer->SliderValues[a];
        } else if(recalculationOfSameString){
            //if it is NOT the first time being called like when special values are simply updated, assign the coefficient ot the original coefficient times the provided slider value.
            VectorDataInstancePointer->TermsArray[rowToModify][columnToModify][5] = VectorDataInstancePointer->SliderValues[a]*VectorDataInstancePointer->OriginalSliderCoefficientValues[a];
        }
    }
    //set firstTimeBeingCalled to false after it has been called for the first time
}

void Grapher::ThreeDeeExpressionArray::inputtedStringEquationModifierUpdater(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    //auto sizeOfEqnArray = VectorDataInstancePointer->EquationValues.size()
}

void Grapher::ThreeDeeExpressionArray::inputtedStringAnimationUpdater(PositionLogic *plogicPointer, VectorData *VectorDataInstancePointer){
    auto sizeOfAnmArray = VectorDataInstancePointer->animationModifierCoords.size();
    for (int g = 0; g < sizeOfAnmArray/2; g++) {
        int rowToMod = VectorDataInstancePointer->animationModifierCoords[0+(g*2)];
        int columnToMod = VectorDataInstancePointer->animationModifierCoords[1+(g*2)];
        if(!recalculationOfSameString){
            VectorDataInstancePointer->OriginalAnimationCoefficientValues.push_back(VectorDataInstancePointer->TermsArray[rowToMod][columnToMod][5]);
            VectorDataInstancePointer->TermsArray[rowToMod][columnToMod][5] *= VectorDataInstancePointer->animationValue;
        } else if(recalculationOfSameString){
            VectorDataInstancePointer->TermsArray[rowToMod][columnToMod][5] = VectorDataInstancePointer->animationValue*VectorDataInstancePointer->OriginalAnimationCoefficientValues[g];
        }
    }
}

void Grapher::resetGrapher(){
    PointerToThreeDeeExpressionArrayInstance->resetThreeDeeArray(PointerToThreeDeeExpressionArrayInstance->PositionLogicInstancePointer, PointerToThreeDeeExpressionArrayInstance->VectorDataInstancePointer, PointerToThreeDeeExpressionArrayInstance->StringParserInstancePointer);
    
    PolynomialGrapherInstance->CoordinatesArrayClear();
    TrigonometricGrapherInstance->CoordinatesArrayClear();
    CalculusGrapherInstance->CoordinatesArrayClear();
    PolarGrapherInstance->CoordinatesArrayClear();
    
    GraphingCoordsStruct.XYcoordsVector.clear();
    GraphingCoordsStruct.TermsAndExponents.clear();
}

PolynomialGrapher::PolynomialGrapher(float xRangeValue, float yRangeValue, float resolutionValue) {
    
    PointerToCoordStruct = &PolynomialCoordsObject;
    
    
    xRange = xRangeValue;
    yRange = yRangeValue;
    resolution = resolutionValue;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    PolynomialCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    PolynomialCoordsObject.XYcoordsVector.resize(1);

    
}

PolynomialGrapher::PolynomialGrapher(){
    
    PointerToCoordStruct = &PolynomialCoordsObject;
    
    PolynomialCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    PolynomialCoordsObject.XYcoordsVector.resize(1);
    
}

GraphingApplication::GraphingApplication(float initXRange, float initYRange, float initResol){
    ObjectMasterInstance->setInitObjectMasterValues();
}


void PolynomialGrapher::setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol){
    xRange = InitRangeX;
    yRange = InitRangeY;
    resolution = InitRangeResol;
}

//one variable/symbol at a time
std::vector<std::vector<float>> PolynomialGrapher::ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent, float termCoefficient, /*for trig Grapher ->*/ int addSubtractMultiplyorDivideWithNextTerm){
    //first element in a row corresponds to exponent, second to coefficient. default exponent needs to be 1.0 when user enters it. Cant have both coefficient and Exponent at the same time, its one or the other when updating it.
    
    //what the final parameter does is the next term reads it and decides if it should add, subtract, multiply, or divide the previous term with it.
    
    if(!(addOrDelete)){
        PointerToCoordStruct->TermsAndExponents.erase(PointerToCoordStruct->TermsAndExponents.begin() + (termPosition-1)); //when erasing a vector also pass its termPosition. termPosition should start at 0 and count from the left.
        //addOrDelete = false = DELETE
    } else {
        //inserts exponent in first element and coefficient in second element.
            std::vector<float> RowToBeInserted = {termExponent, termCoefficient, float(addSubtractMultiplyorDivideWithNextTerm)};
            PointerToCoordStruct->TermsAndExponents.insert(PointerToCoordStruct->TermsAndExponents.begin() + (termPosition - 1), RowToBeInserted);
        
    }
    
    return (PointerToCoordStruct->TermsAndExponents);
}

void PolynomialGrapher::ExpressionArrayClear(){
    //clears all of the terms
    PointerToCoordStruct->TermsAndExponents.clear();
    //resizes to prevent crashes
    PolynomialCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

void PolynomialGrapher::CoordinatesArrayClear(){
    PolynomialCoordsObject.XYcoordsVector.clear();
    PolynomialCoordsObject.XYcoordsVector.resize(1);
    PolynomialCoordsObject.TermsAndExponents.clear();
    PolynomialCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

//first parameter doesnt matter. nor does last one.
std::vector<float> PolynomialGrapher::CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, /* termIdentifier used for deciding wether provided x-array is used for exponentation or base. 0 for base and 1 for exponentiation. */ int termIdentifier, std::vector<float> providedCoordinatesForCoefficient) {
    //erases previous coordinates
    PointerToCoordStruct->XYcoordsVector.clear();
    
    
    int PointsToCalculate = int(300.0 * resolution); //converts given value to float, multiplies by resolution factor, converts back to int for use in for loop
    
    float theRangeOfX = xRange;
    
    PointerToCoordStruct->XYcoordsVector.resize(PointsToCalculate); //resizes coordinate vector to number of points needed
    auto numberOfTerms = PointerToCoordStruct->TermsAndExponents.size();
    PointerToCoordStruct->xInterval = (float(theRangeOfX) / float(PointsToCalculate)); //sets xInterval to x-axis range divided by total number of points
    
    float currentXvalue = 0.0;
    float CalculatedValue = 0.0;
    
    //refer to pages for documentation
    float addSubtractMultiplyOrDivide;
    
    for(int ou = 0; ou < PointsToCalculate; ou++) {
        
        //if term identifier signals to use x-array for base then do so.
        if((useProvidedCoordinatesArrayForXinterval)&&(termIdentifier == 0)){
            //refer to trigonometric grapher's documentation in CoordinatesUpdater in the same function for info on usage
            currentXvalue = ProvidedCoordinatesArrayForXinterval[ou];
        } else {
            currentXvalue = (float(ou) * PointerToCoordStruct->xInterval) -(xRange/2.0f); //increments x-value by xInterval each time; //increments x-value by xInterval each time
        }
            
        CalculatedValue = 0.0;
        
        for(int iw = 0; iw < (numberOfTerms -1); iw++){ //takes size of (# of terms) of expression and adds to current y-coord for each expression
            
            if(PointerToCoordStruct->TermsAndExponents.empty()){
                std::cout << "RED ALERT: EMPTY ROW!!!!!!!!!!! AT:    \n";
                std::cout << iw;
                
            }
            
            float CalculatedTermValue; //calculates y-value for a single term
            CalculatedTermValue = PointerToCoordStruct->TermsAndExponents[iw][0]; //sets a variable equal to the current term's exponent
            
                if(termIdentifier == 1){CalculatedTermValue = ProvidedCoordinatesArrayForXinterval[ou];}//works since only a single term is being calculated by an instance of PolynomialGrapher for now.
            
            CalculatedTermValue = std::powf(currentXvalue,CalculatedTermValue); //takes xValue and raises it to the power of exponent and sets it equal to Calculated Value
            
            //multiplies by coefficent, supposed to be present in second element
            CalculatedTermValue *= PointerToCoordStruct->TermsAndExponents[iw][1];
                //if coefficient array is provided as with special value eqn modifier then multiply by it.
                if(!(providedCoordinatesForCoefficient.empty())){
                    CalculatedTermValue *= providedCoordinatesForCoefficient[ou];
                }
            
            //if we are at or above second element, do add multiply subtract divide
            if(iw > 0){
                //grab identifier from third element of previous term
                addSubtractMultiplyOrDivide = PointerToCoordStruct->TermsAndExponents[(iw-1)][2];
                
                switch (int(addSubtractMultiplyOrDivide)) {
                    case 0:
                        CalculatedValue += CalculatedTermValue; //adds term's contirbution to that point's calculated value
                        break;
                        
                    case 1:
                        CalculatedValue -= CalculatedTermValue; //subtract from total point value
                        break;
                        
                    case 2:
                        CalculatedValue *= CalculatedTermValue; //multiply times total value
                        break;
                        
                    case 3:
                        CalculatedValue /= CalculatedTermValue;
                        break;
                }
            } else {
                CalculatedValue = CalculatedTermValue;
            }
        }
        
        PointerToCoordStruct->XYcoordsVector[ou] = CalculatedValue; //sets the value of the current point to what was calculated
        
    }
    
    //add part in Graphing function that adds the coordinates returned to the grapher struct!
    return PointerToCoordStruct->XYcoordsVector;
}

std::vector<std::vector<float>> PolynomialGrapher::returnExpressionArray(){
    return PointerToCoordStruct->TermsAndExponents;
}

std::vector<float> PolynomialGrapher::returnCoordinatesArray() {
    return PolynomialCoordsObject.XYcoordsVector;
}

float PolynomialGrapher::returnXinterval() {
    return PolynomialCoordsObject.xInterval;
}


TrigonometricGrapher::TrigonometricGrapher(float xRangeValue, float yRangeValue, float resolutionValue) {
    
    PointerToTrigonometricStruct = &TrigonometricCoordsObject;
    
    
    xRange = xRangeValue;
    yRange = yRangeValue;
    resolution = resolutionValue;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    TrigonometricCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    TrigonometricCoordsObject.XYcoordsVector.resize(1);

    
    
}

TrigonometricGrapher::TrigonometricGrapher(){
    
    PointerToTrigonometricStruct = &TrigonometricCoordsObject;
    
    TrigonometricCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    TrigonometricCoordsObject.XYcoordsVector.resize(1);
    
}

void TrigonometricGrapher::setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol){
    xRange = InitRangeX;
    yRange = InitRangeY;
    resolution = InitRangeResol;
}

//termPosition and termCoefficient unused in this case and in function calls
std::vector<std::vector<float>> TrigonometricGrapher::ExpressionArrayUpdater(bool addOrDelete, int termPosition, /*known as termExponent elsewhere*/float addSubtractMultiplyOrDivide, float termCoefficient, /*for trig grapher*/ int termIdentifier) {
    
    
    if(!(addOrDelete)){
            PointerToTrigonometricStruct->TermsAndExponents.erase(PointerToTrigonometricStruct->TermsAndExponents.begin() + (termPosition-1)); //when erasing a vector also pass its termPosition. termPosition should start at 0 and count from the left.
            //addOrDelete = false = DELETE
        } else {
            //converts unicode decimals to program identifiers. second element is addSubtractMultiplyOrDivide by previous term as with PolynomialGrapher
            std::vector<float> FunctionIdentifierToBeInserted = {0,0,0};
            switch (termIdentifier) {
                    //uses unicode values for referenced identifiers. refer to pages document for identifier information. do the conversion to integers in objcpp file not here.
                case 2:
                    //'S" unicode identifier to program asin identifier
                    FunctionIdentifierToBeInserted = {22,0,0};
                    break;
                    
                case 1:
                    //'s" unicode identifier to program sin identifier
                    FunctionIdentifierToBeInserted = {21,0,0};
                    break;
                    
                case 4:
                    //'C' unicode identifier to program acos identifier
                    FunctionIdentifierToBeInserted = {23,0,0};
                    break;
                    
                case 3:
                    //c to cos
                    FunctionIdentifierToBeInserted = {24,0,0};
                    break;
                    
                case 6:
                    FunctionIdentifierToBeInserted = {26,0,0};
                    break;
                    
                case 5:
                    FunctionIdentifierToBeInserted = {25,0,0};
                    break;
                    
                default:
                    std::cout << "TrigonometricGrapher::ExpressionArrayUpdater: No terms matching trig grapher identifier";
                    break;
            }
            
            FunctionIdentifierToBeInserted[1] = termCoefficient;
            
            FunctionIdentifierToBeInserted[2] = addSubtractMultiplyOrDivide;
            
                //std::vector<float> RowToBeInserted = {};
                PointerToTrigonometricStruct->TermsAndExponents.insert(PointerToTrigonometricStruct->TermsAndExponents.begin() + (termPosition - 1), FunctionIdentifierToBeInserted);
        }
    return PointerToTrigonometricStruct->TermsAndExponents;
}

void TrigonometricGrapher::ExpressionArrayClear(){
    //clears all of the terms
    PointerToTrigonometricStruct->TermsAndExponents.clear();
    //resizes to prevent crashes
    PointerToTrigonometricStruct->TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

void TrigonometricGrapher::CoordinatesArrayClear(){
    TrigonometricCoordsObject.XYcoordsVector.clear();
    TrigonometricCoordsObject.XYcoordsVector.resize(1);
    TrigonometricCoordsObject.TermsAndExponents.clear();
    TrigonometricCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

//literally just a glorified trig function outputter
std::vector<float> TrigonometricGrapher::CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, /* this termIdentifier and ExpressionArrayUpdater's are NOT the same. this converts the program codes to functions whereas the former converts unicode to program codes */ int termIdentifier, std::vector<float> providedCoordinatesForCoefficient) {
    
    int PointsToCalculate = int(300.0 * resolution); //converts given value to float, multiplies by resolution factor, converts back to int for use in for loop
    
    float theRangeOfX = xRange;
    
    PointerToTrigonometricStruct->XYcoordsVector.resize(PointsToCalculate); //resizes coordinate vector to number of points needed
    auto numberOfTerms = PointerToTrigonometricStruct->TermsAndExponents.size();
    PointerToTrigonometricStruct->xInterval = (float(theRangeOfX) / float(PointsToCalculate)); //sets xInterval to x-axis range divided by total number of points
    
    float currentXvalue = 0.0;
    float CalculatedValue = 0.0;
    
    //refer to pages for documentation
    float addSubtractMultiplyOrDivide;
    
    for(int ou = 0; ou < PointsToCalculate; ou++){
        
        //when you have the trig function's argument be another function like cos( ln(x) )
        if(useProvidedCoordinatesArrayForXinterval){
            //like when you have ln(x^5.2). the provided x interval array would be *x^5.2* !
            currentXvalue = ProvidedCoordinatesArrayForXinterval[ou];
        } else {
            currentXvalue = (float(ou) * PointerToTrigonometricStruct->xInterval)  -(xRange/2.0f); //increments x-value by xInterval each time; //increments x-value by xInterval each time
        }
        CalculatedValue = 0.0;
        
        for(int iw = 0; iw < (numberOfTerms -1); iw++){ //takes size of (# of terms) of expression and adds to current y-coord for each expression
            
            if(PointerToTrigonometricStruct->TermsAndExponents.empty()){
                std::cout << "RED ALERT: EMPTY ROW IN TRIGONOMETRIC GRAPHER!!!!!!!!!!! AT:    \n";
                std::cout << iw;
            }
            
            float CalculatedTermValue; //calculates y-value for a single term
            CalculatedTermValue = PointerToTrigonometricStruct->TermsAndExponents[iw][0]; //sets a variable equal to the current function's identifier
            
            
            switch (int(CalculatedTermValue)) {
                case 22:
                    CalculatedTermValue = std::asin(currentXvalue);
                    break;
                    
                case 21:
                    CalculatedTermValue = std::sin(currentXvalue);
                    break;
                    
                case 23:
                    CalculatedTermValue = std::acos(currentXvalue);
                    break;
                    
                case 24:
                    CalculatedTermValue = std::cos(currentXvalue);
                    break;
                    
                case 26:
                    CalculatedTermValue = std::atan(currentXvalue);
                    break;
                    
                case 25:
                    CalculatedTermValue = std::tan(currentXvalue);
                    break;
                    
                default:
                    std::cout << "TrigonometricGrapher::CoordinatesUpdater: WARNING: TERM IDENTIFIER SUPPOSEDLY FILTERED THROUGH EXPRESSION ARRAY DOESNT MATCH COORDINATESUPDATER VALUES!!! STOPPING PROGRAM...";

                    break;
            }
            //multiply calculated term value by coefficient. for example it would go from cos(x^2) to 5cos(x^2) if coefficient was 5.
            CalculatedTermValue *= PointerToTrigonometricStruct->TermsAndExponents[iw][1];
            
            if(iw > 0){
                //grab identifier from third element of previous term
                addSubtractMultiplyOrDivide = PointerToTrigonometricStruct->TermsAndExponents[(iw-1)][2];
                
                switch (int(addSubtractMultiplyOrDivide)) {
                    case 0:
                        CalculatedValue += CalculatedTermValue; //adds term's contirbution to that point's calculated value
                        break;
                        
                    case 1:
                        CalculatedValue -= CalculatedTermValue; //subtract from total point value
                        break;
                        
                    case 2:
                        CalculatedValue *= CalculatedTermValue; //multiply times total value
                        break;
                        
                    case 3:
                        CalculatedValue /= CalculatedTermValue;
                        break;
                        
                    default:
                        //bad
                        break;
                }
            } else {
                //when there's just one term, sets the y-coordinate to the calculated value. Without the else statement it bugs out cuz it looks for a next term to add, subtract, multiply, or divide with; failing to do so, doesnt set the value to be input into the vector to the current calculated value
                CalculatedValue = CalculatedTermValue;
            }
            
        }
        
        PointerToTrigonometricStruct->XYcoordsVector[ou] = CalculatedValue; //sets the value of the current point to what was calculated
        
    }
    
    return PointerToTrigonometricStruct->XYcoordsVector;
    
}

std::vector<std::vector<float>> TrigonometricGrapher::returnExpressionArray(){
    //returns raw program identifiers
    return PointerToTrigonometricStruct->TermsAndExponents;
}

float TrigonometricGrapher::returnXinterval(){
    //returns xInterval used by TrigGrapher's struct currently
    return  PointerToTrigonometricStruct->xInterval;
}

std::vector<float> TrigonometricGrapher::returnCoordinatesArray(){
    return PointerToTrigonometricStruct->XYcoordsVector;
}

CalculusGrapher::CalculusGrapher(float RangeOfX, float RangeOfY, float ResolutionValue){
    
    PointerToCalculusStruct = &CalculusCoordsObject;
    
    
    xRange = RangeOfX;
    yRange = RangeOfY;
    resolution = ResolutionValue;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    CalculusCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    CalculusCoordsObject.XYcoordsVector.resize(1);
    
}

CalculusGrapher::CalculusGrapher(){
    
    PointerToCalculusStruct = &CalculusCoordsObject;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    CalculusCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    CalculusCoordsObject.XYcoordsVector.resize(1);
    
}

void CalculusGrapher::setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol){
    xRange = InitRangeX;
    yRange = InitRangeY;
    resolution = InitRangeResol;
}

//currently defaults to ZERO ON LOWER BOUND AND X ON UPPER BOUND FURTHER FUNCTIONALITY YET TO BE DEVELOPED
//so upper and lower bound parameters currently UNUSED
//currently, MULTIPLE EXPRESSON SUPPORT HAS NOT BEEN ADDED. SO DONT ADD MOAR THAN ONE TERM.
std::vector<std::vector<float>> CalculusGrapher::ExpressionArrayUpdater(bool addOrDelete, int termPosition, float upperBound, float lowerBound, /*for calc grapher. trig grapher is different one*/ int typeIdentifier) {
    
    //if of identifier derivative, has code 32 in first element. if integral, first element = 31 and 2nd and third are upper and lower bounds respectively
    // different term identifiers if x interval array in Coordinates Updater is used as either or both upper/lower bounds
    std::vector<float> TypeToBeInserted = {0};
    if(!(addOrDelete)){
            PointerToCalculusStruct->TermsAndExponents.erase(PointerToCalculusStruct->TermsAndExponents.begin() + (termPosition-1)); //when erasing a vector also pass its termPosition. termPosition should start at 0 and count from the left.
            //addOrDelete = false = DELETE
    } else {
        //converts unicode decimal to program codes
        switch (typeIdentifier) {
            case 105:
                TypeToBeInserted = {31};
                break;
                
            case 117:
                //upper-bound is bound by x-array.
                TypeToBeInserted = {33};
                break;
                
            case 111:
                //lower-bound bound by x-array.
                TypeToBeInserted = {35};
                break;
                
            case 100:
                //derivative
                TypeToBeInserted = {32};
                break;
                
            default:
                
                break;
        }
        PointerToCalculusStruct->TermsAndExponents.insert(PointerToCalculusStruct->TermsAndExponents.begin() + (termPosition - 1), TypeToBeInserted);
    }
    return PointerToCalculusStruct->TermsAndExponents;
    
}

void CalculusGrapher::ExpressionArrayClear(){
    PointerToCalculusStruct->TermsAndExponents.clear();
    PointerToCalculusStruct->TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

void CalculusGrapher::CoordinatesArrayClear(){
    PointerToCalculusStruct->XYcoordsVector.clear();
    PointerToCalculusStruct->XYcoordsVector.resize(1);
    PointerToCalculusStruct->TermsAndExponents.clear();
    PointerToCalculusStruct->TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

//CalculusGrapher doesnt have addSubtractMultipleOrDivide capabilities unfortunately. its a glorified reimann sum calculator. and derivatives.
//depending on identifier, integral can have ProvidedCoordinatesArray be the upper x-bound of the integral, lower x-bound of the integral, or the integrand itself.
//abCalculusGrapher::CoordinatesUpdaterove has NOT been developed yet. currently defaults to 0 on lower bound and x on upper bound.
std::vector<float> CalculusGrapher::CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, int termIdentifier, std::vector<float> providedCoordinatesForCoefficient) {
    
    int PointsToCalculate = int( 300.0 * resolution); //converts given value to float, multiplies by resolution factor, converts back to int for use in for loop
    
    float theRangeOfX = xRange;
    
    PointerToCalculusStruct->XYcoordsVector.resize(PointsToCalculate); //resizes coordinate vector to number of points needed
    PointerToCalculusStruct->xInterval = (float(theRangeOfX) / float(PointsToCalculate)); //sets xInterval to x-axis range divided by total number of points
    
    float currentXvalue = 0.0;
    float CalculatedValue = 0.0;
    
    if(!(useProvidedCoordinatesArrayForXinterval)) {
        //INTEGRAND MUST BE PROVIDED!
    }
    
    float xInterval = PointerToCalculusStruct->xInterval;
    //upper bound is x, lower bound is 0. integrand MUST be provided
    int theTermIdentifier = PointerToCalculusStruct->TermsAndExponents[0][0];
    if (theTermIdentifier == 31) {
        float PreviouslyCalculatedValue = 0.0;
        float PreviousXValue = 0.0;
        
        for (int eq = 0; eq < PointsToCalculate; eq++){
                        
            CalculatedValue = 0.0;
            
            currentXvalue = (xInterval*eq)  -(xRange/2.0f); //increments x-value by xInterval each time;
            
            //curretly hardcoded to take 0 as the initial x
            CalculatedValue = AreaCalculator(currentXvalue, PreviousXValue, ProvidedCoordinatesArrayForXinterval, xInterval, PreviousXValue, PreviouslyCalculatedValue, true);
            
            PointerToCalculusStruct->XYcoordsVector[eq] = CalculatedValue;
            //sets non-local variables to values calculated so that they may function as previously calculated values on the next run of the loop. these values prevent the reimann sum calculator from recalculating already calculated points over and over in the same stretch of the graph.
            PreviouslyCalculatedValue = CalculatedValue;
            PreviousXValue = currentXvalue;
            
        }
    } else {
        //bad
    }
    return PointerToCalculusStruct->XYcoordsVector;
}

float CalculusGrapher::AverageReimannSum (float x1, float x2, float y1) {
    float width = x2-x1;
    
    return y1*width;
}

//other classes dont support starting from negative numbers yet
//calculates area between a section of the graph returning it as a float. NOT FOR EACH POINT.
inline float CalculusGrapher::AreaCalculator(float upperBound, float lowerBound, std::vector<float> integrand, float xInterval, float initialX, float previousValue, bool previousValueSet) {
    
    float upperCalculatedValue = 0.0;
    float lowerCalculatedValue = 0.0;
    
    bool lowerCalculatedValueHasBeenSet = false;
    float minX = (0.0f-(xRange/2.0f));//the minimum x-value of the coordinates vector/integrand
    
    float currentX1;
    float currentX2;
    
    float currentValue = 0.0;
    
    float numberOfPointsUPPER = (upperBound - initialX)/xInterval;
    float numberOfPointsLOWER = (lowerBound - initialX)/xInterval;
    if(previousValueSet){
        //takes the difference between the initial x-value that the integral is to start calculating at and the starting x-value and divides that by the x-interval to get the points crossed to set as the indice number for the y-value
        //believe it finds the difference between the minimum x-value of the coords vector provided and the initialX to start from and finds out how many points have been jumped to reach that initialX and finds out the reimann sum right after that
        //adding the pointer to calculus addition part is to convert (-150,150) type coordinates to (0,300) coordinate indice
        int capturedIndiceFromIntegrandArray = (((initialX - minX)/xInterval)+(xRange/2.0f));
        
        //returns previous value plus one reimann Sum
        return ((xInterval*integrand[capturedIndiceFromIntegrandArray]) + previousValue);
    } else {
        //if upper bound and lower bound are the same, return 0.
        if(upperBound == lowerBound) {return 0.0f;}
        for (int a = 0; a < int(numberOfPointsUPPER+1); a++) {
            currentX1 = (xInterval * a)+initialX;
            currentX2 = (xInterval * (a+1))+initialX;
            currentValue += AverageReimannSum(currentX1,currentX2, integrand[0+a]);
            if (a == numberOfPointsLOWER) {
                if (numberOfPointsLOWER == 0) {lowerCalculatedValue = 0; lowerCalculatedValueHasBeenSet = true;}
                else {
                    lowerCalculatedValue = currentValue;
                    lowerCalculatedValueHasBeenSet = true;
                }
            }
        }
        if(lowerCalculatedValueHasBeenSet == false) {
            //bad
        }
        
        //previous value takes the value calculated for the same graph in an earlier call except for an x-interval that ended earlier and adds it so it doesnt do unnecessary work with the initial-X being set to the lower bound.
        upperCalculatedValue = currentValue+previousValue;
        lowerCalculatedValue += previousValue;
        
        
        return (upperCalculatedValue - lowerCalculatedValue);
    }
}

std::vector<std::vector<float>> CalculusGrapher::returnExpressionArray(){
    return PointerToCalculusStruct->TermsAndExponents;
}

float CalculusGrapher::returnXinterval(){
    return PointerToCalculusStruct->xInterval;
}

std::vector<float> CalculusGrapher::returnCoordinatesArray(){
    return PointerToCalculusStruct->XYcoordsVector;
}


PolarGrapher::PolarGrapher(float RangeOfX, float RangeOfY, float ResolutionValue) {
    PointerToPolarStruct = &PolarCoordsObject;
    
    
    xRange = RangeOfX;
    yRange = RangeOfY;
    resolution = ResolutionValue;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    PolarCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    PolarCoordsObject.XYcoordsVector.resize(1);
}

PolarGrapher::PolarGrapher(){
    PointerToPolarStruct = &PolarCoordsObject;
    
    //sets Terms And Exponents to a non-zero default and other arrays too
    
    PolarCoordsObject.TermsAndExponents.resize(1, std::vector<float>{0.0f});
    PolarCoordsObject.XYcoordsVector.resize(1);
}

void PolarGrapher::setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol){
    xRange = InitRangeX;
    yRange = InitRangeY;
    resolution = InitRangeResol;
}

std::vector<std::vector<float>> PolarGrapher::ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent, float termCoefficient, int addSubtractMultiplyOrDivideWithNextTerm) {
    if(!(addOrDelete)){
        PointerToPolarStruct->TermsAndExponents.erase(PointerToPolarStruct->TermsAndExponents.begin() + (termPosition-1)); //when erasing a vector also pass its termPosition. termPosition should start at 0 and count from the left.
        //addOrDelete = false = DELETE
    } else {
        //inserts exponent in first element and coefficient in second element.
        std::vector<float> RowToBeInserted = {termExponent, termCoefficient, float(addSubtractMultiplyOrDivideWithNextTerm)};
            PointerToPolarStruct->TermsAndExponents.insert(PointerToPolarStruct->TermsAndExponents.begin() + (termPosition - 1), RowToBeInserted);
        
    }
    
    return (PointerToPolarStruct->TermsAndExponents);
}

void PolarGrapher::ExpressionArrayClear(){
    //clears all of the terms
    PointerToPolarStruct->TermsAndExponents.clear();
    //resizes to prevent crashes
    PointerToPolarStruct->TermsAndExponents.resize(1, std::vector<float>{0.0f});
}

void PolarGrapher::CoordinatesArrayClear(){
    PointerToPolarStruct->XYcoordsVector.clear();
    PointerToPolarStruct->XYcoordsVector.resize(1);
    PointerToPolarStruct->TermsAndExponents.clear();
    PointerToPolarStruct->TermsAndExponents.resize(1, std::vector<float>{0.0f});
}



//if provided coords for coefficient is provided mutliply coefficient by the value as the array is looped through
std::vector<float> PolarGrapher::CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, int termIdentifier, std::vector<float> providedCoordinatesForCoefficient) {
    PointerToPolarStruct->XYcoordsVector.clear();
    
    
    int PointsToCalculate = int(300.0 * resolution); //converts given value to float, multiplies by resolution factor, converts back to int for use in for loop
    
    float theRangeOfX = xRange;
    
    PointerToPolarStruct->XYcoordsVector.resize(PointsToCalculate); //resizes coordinate vector to number of points needed
    auto numberOfTerms = PointerToPolarStruct->TermsAndExponents.size();
    PointerToPolarStruct->xInterval = (float(theRangeOfX) / float(PointsToCalculate)); //sets xInterval to x-axis range divided by total number of points
    
    float currentXvalue = 0.0;
    float CalculatedValue = 0.0;
    
    float xInterval = PointerToPolarStruct->xInterval;
    
    //refer to pages for documentation
    float addSubtractMultiplyOrDivide;
    
    for(int ou = 0; ou < PointsToCalculate; ou++) {
        
        if(useProvidedCoordinatesArrayForXinterval){
            //refer to trigonometric grapher's documentation in CoordinatesUpdater in the same function for info on usage
            currentXvalue = ProvidedCoordinatesArrayForXinterval[ou];
        } else {
            currentXvalue = (float(ou) * xInterval)  -(xRange/2.0f); //increments x-value by xInterval each time; //increments x-value by xInterval each time
        }
            
        CalculatedValue = 0.0;
        
        for(int iw = 0; iw < (numberOfTerms -1); iw++){ //takes size of (# of terms) of expression and adds to current y-coord for each expression
            
            if(PointerToPolarStruct->TermsAndExponents.empty()){
                std::cout << "RED ALERT: EMPTY ROW!!!!!!!!!!!";
                std::cout << iw;
            }
            
            float CalculatedTermValue; //calculates y-value for a single term
            CalculatedTermValue = PointerToPolarStruct->TermsAndExponents[iw][0]; //sets a variable equal to the current term's exponent
            CalculatedTermValue = std::powf(currentXvalue,CalculatedTermValue); //takes xValue and raises it to the power of exponent and sets it equal to Calculated Value
            
            //multiplies by coefficent, supposed to be present in second element
            CalculatedTermValue *= PointerToPolarStruct->TermsAndExponents[iw][1];
                //if there is a coefficient array provided then multiply by it. Used for special values.
                if(!(providedCoordinatesForCoefficient.empty())){
                CalculatedTermValue *= providedCoordinatesForCoefficient[ou]; //
                }
            
            //if we are at or above second element, do add multiply subtract divide
            if(iw > 0){
                //grab identifier from third element of previous term
                addSubtractMultiplyOrDivide = PointerToPolarStruct->TermsAndExponents[(iw-1)][2];
                
                switch (int(addSubtractMultiplyOrDivide)) {
                    case 0:
                        CalculatedValue += CalculatedTermValue; //adds term's contirbution to that point's calculated value
                        break;
                        
                    case 1:
                        CalculatedValue -= CalculatedTermValue; //subtract from total point value
                        break;
                        
                    case 2:
                        CalculatedValue *= CalculatedTermValue; //multiply times total value
                        break;
                        
                    case 3:
                        CalculatedValue /= CalculatedTermValue;
                        break;
                        
                    default:
                        //bad
                        break;
                }
            } else {
                CalculatedValue = CalculatedTermValue;
            }
            
        }
        
        PointerToPolarStruct->XYcoordsVector[ou] = CalculatedValue; //sets the value of the current point to what was calculated
        
    }
    std::vector<float> UnmodifiedPolynomialArray = PointerToPolarStruct->XYcoordsVector;
    //even elements are x-coords, odd elements are y-coords
    std::vector<float> ModifiedPolarArray(PointsToCalculate*2);
    
    PointerToPolarStruct->XYcoordsVector.resize(PointsToCalculate*2);
    
    for (int u = 0; u < (PointsToCalculate*2); u++) {
        
            currentXvalue = (float(int((u/2))) * xInterval) - (xRange/2.0f); //increments theta by xInterval each time
        switch ((u % 2)) {
                //when u is even, input current X-coord into an even slot of the array to be returned
            case 0:
                ModifiedPolarArray[u] = (UnmodifiedPolynomialArray[(u/2)]*std::cos(currentXvalue));
                break;
            
            case 1:
                ModifiedPolarArray[u] = (UnmodifiedPolynomialArray[(u/2)]*std::sin(currentXvalue));
                break;
        }
    }
    PointerToPolarStruct->XYcoordsVector = ModifiedPolarArray;
    
    return PointerToPolarStruct->XYcoordsVector;
}

float PolarGrapher::returnXinterval() {
    return PointerToPolarStruct->xInterval;
}

std::vector<std::vector<float>> PolarGrapher::returnExpressionArray() {
    return PointerToPolarStruct->TermsAndExponents;
}

std::vector<float> PolarGrapher::returnCoordinatesArray() {
    return PointerToPolarStruct->XYcoordsVector;
}




