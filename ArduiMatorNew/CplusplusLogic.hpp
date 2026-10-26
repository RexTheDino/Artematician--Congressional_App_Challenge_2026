//
//  CplusplusLogic.hpp
//  ArduiMatorNew
//
//  Created by AnshuDiya on 8/20/26.
//

#ifndef CplusplusLogic_hpp
#define CplusplusLogic_hpp

#include <stdio.h>

#endif /* CplusplusLogic_hpp */
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <chrono>
#include <array>

#pragma once



struct XYcoords{
    std::vector<float> XYcoordsVector; //assumes each y-coord is a fixed distance away. pass this interval as an argument
    float xInterval;
    float xRange;
    float yRange;
    
    std::vector<std::vector<float>> TermsAndExponents; //each vector corresponds to the term Number and the vector element contains the term's power. Should be dynamically resized with the number of terms
};


class GraphingApplication;
class PolynomialGrapher;
class TrigonometricGrapher;
class CalculusGrapher;
class Grapher;
class GraphingSubtypeInterface;
class PolarGrapher; //use PolynomialGrapher to calculate r in something like 2theta^2 and then use seperate functions to calculate x and y and return x coordinates and y coordinates in a SINGLE vector. maybe have x-coords in odd spaces and y-coords in even or pre-calculate which coords have the same x-values and input them into a vector idc what happens

class PositionLogic;
class StringParser;
class VectorData;
class inputStringCalculator;

class ObjectMaster;

class GraphingApplication {

    int xRangeOfApplication;
    int yRangeOfApplication;
    
    
    friend class Grapher;
    
public:
    
    
    //adds a grapher instance and sets current pointer to that one
    void AddGrapherInstance(char EquationType, float xRangeValue, float yRangeValue, float resolutionValue = 1.0);
    
    //probably broken
    void CalculatePolynomialCoordsAndPrint(float term1Exponent, float term2Exponent, float term3Exponent, float xRangeValue, float yRangeValue, int theRangeOfX, float resolutionValue = 1.0, int PointsToCalculate = 300);
    
    void PrintCurrentCoordinates();
    
    GraphingApplication(float initXRange, float initYRange, float initResol);
    
    std::vector<float> returnCurrentInstanceCalculatedCoordinates();
    
    Grapher* CurrentGraphingInstance = nullptr;
    
    GraphingApplication* thisApplication = this;
    
    class ObjectMaster{ //controls all the objects underneath like their relations to each other and outputs final coordinates to shared buffer
            //one object as created in the Grapher swiftUI Workspace
        
        friend class GraphingApplication;
        
        struct GrapherAndEquation{
            std::unique_ptr<Grapher> GrapherPointer;
            std::string EquationForGrapher;
        };
        
        
        std::vector<float> IntermediateObjectlevelPoints; //stores all the x,y points calculated by intermediate objects of all levels except the very top. x,y points written by obects are not just all the primitive coordinate points but instead a fixed number of coordinates per object where th e object averages the primitives coordinates to find global object coordinates and inserts that  into the vector. The number of points per object is (xRangeOfMaster*(300*resol)) temporarily until beta features implementing per-object resoltuion and range values are implemented. Top level object calculations are written directly into the metal buffer. passes pointers and vector sizes to the corresponding objects to objects that request the coordinates of a specific object.
        
        std::vector<float> RawObjectPoints; //stores all the x,y points of each object except the difference from the above is that these arent an average of all the primitives but a raw array of every primitive stacked
        
        std::vector<float> HeightLookupArray;//stores values for the height values of the cross seciton
        std::vector<float> LightInfArray;//stores coords for lightInf coords. packed_float4 format
        std::vector<float> ClrAndReflctArray;//stores coords for color and reflectivvity of vertice cross sections. packed_float4 format
        
        float xRangeOfObjectMaster = 300.0;
        float yRangeOfObjectMaster = 10000.0;
        float resolOfObjectMaster = 1.0;
        
            float animationValue = 0.0;
        
        GraphingApplication* ReferenceToThisGraphApp;
            class Object{
                //TEMPORARILY PUBLIC FOR TESTING!!!
                GraphingApplication* ReferenceToGraphApp;
                //Grapher Metadata to set for each grapher in GrapherPointerArray
                float xRangeOfObject = 300.0; //default values
                float yRangeOfObject = 10000.0;
                float resolOfObject = 1.0;
                
                Grapher* CurrentObjectGrapherInstance = nullptr;
                
                    std::vector<GrapherAndEquation> GrapherPointerArray; //NONE of these graphers AFFECT EACH OTHER!!! Purely for primitives.
                
                friend class ObjectMaster;
                    int objectMode = 0;//to be changed to 0 for physical equation, 1 for lightEqns, 2 for clrAndRflct Eqns, and 3 for CrossSecEqn
                
                std::vector<float> ObjectCenterCoords; //centered coords of object
                std::vector<float> ObjectRawCoords; //raw coords of object
            
                void setInitObjectValues(float xRange, float yRange, float resol);
                void AddGraph(std::string eqnForGrapherToAdd); //adds a graph to the object array
                    void ChangeGrapherEquation(std::string ChangedEquation, int GrapherPosition); //change the equation of the grapher in the indicated position
                
                std::string RotationEquation = "";
                std::vector<int> ConstantModifierObjectsRotationEqn; //xy coords in pairs for the objects that will modify its constants ('#' in eqn)
                std::vector<float> ConstantModifierSliderValuesRotationEqn; //slider values for the constants in eqn ('$')
                int RotEqnMode;//refer to pages for documentation
                int selectedIndiceRot;
                
                std::string TranslationEquation = ""; //Equation for translation
                std::vector<int> ConstantModifierObjectsTranslationEqn; //xy coords in pairs for the objects that will modify its constants ('#' in eqn)
                std::vector<float> ConstantModifierSliderValuesTranslationEqn; //slider values for the constants in eqn ('$')
                int TranslEqnMode;//refer to pages for documentation
                int selectedIndiceTransl;
                
                //for constants in the main equation
                //ONLY AVAILABLE IF THE ENTIRE OBJECT CONSISTS OF A SINGULAR EQUATION!!!!!!!!!!!!!!!
                std::vector<int> ConstantModifierObjectsMainEqn; //xy coords in pairs for the objects that will modify its constants ('#' in eqn)
                std::vector<float> ConstantModifierSliderValuesMainEqn; //slider values for the constants in eqn ('$')
                
                class heightTableFuncs{
                    friend class ObjectMaster;
                    GrapherAndEquation CrossSecEqnAndGraph;
                    float resolOfHeightTable;//to be adjusted by slider seperate from ObjResol
                    float yRangeOfHeightTable;
                    void calcCrossSecAndWriteToMaster(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);//writes cross section values into object master. the max x-range in metal of thsi array is treated as sqrt(8).
                public:
                    //heightTableFuncs();
                };
                class lightInfoObjFuncs{//contains extra data and functions to be used if the object is set to be an equation that acts as point light sources
                    friend class ObjectMaster;
                    GrapherAndEquation ZDirGraphNEqn;
                    GrapherAndEquation IntensityGraphNEqn;
                    
                    std::vector<float> ZDirCoords;//does NOT store an x-value only z-values corresponding to the x,y coords
                    std::vector<float> IntensityVals;//stores respective of intensity that correspond to x,y,z values. does NOT store an 'x-value' for each intensity coord.
                    
                    std::vector<int> ConstModObjsZDirEqn;//mod objs for z-dir eqn. can only be dependent on other objects, no objects can be dependent on this eqn. can only be dependent on objects graphed as lines.
                    std::vector<float> ConstModSldrValsZDirEqn;
                        std::vector<int> ConstModObjsIntensityEqn;//modifier objs for intensity eqn. same rules as z-dir eqn no objs can depend on this.
                        std::vector<float> ConstModSldrValsIntensityEqn;
                    
                        void calculateNwriteToObjMstrLightInfArray(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);//calculates and writes in packed_float4 format. also calls Object's functions to first calculate the x,y coords
                    void calculateMainEqns(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);//calculate x,y eqns and store it only in this object
                            void calculateZdirEqn(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);
                            void calculateIntensityEqn(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);
                public:
                    //lightInfoObjFuncs();
                };
                class clrAndRflctFuncs{
                    friend class ObjectMaster;
                    GrapherAndEquation rgbGrapherNEqn[3];
                    GrapherAndEquation ReflectivityGrapherNEqn;
                    
                        std::vector<float> RedClrCoords;//seperate strings for each color value
                            std::vector<int> ModObjsRed;
                            std::vector<float> ModSlidersRed;
                        std::vector<float> GreenClrCoords;
                            std::vector<int> ModObjsGreen;
                            std::vector<float> ModSlidersGreen;
                        std::vector<float> BlueClrCoords;
                            std::vector<int> ModObjsBlue;
                            std::vector<float> ModSlidersBlue;
                                std::vector<float> ReflectivityCoords;
                                    std::vector<int> ModObjsReflectivity;
                                    std::vector<float> ModSlidersReflectivity;
                    void calculateNwriteToObjMstrClrAndRflctArray(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);
                        void calculateClrsAndWrite(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);
                        void reflectivityCalcAndWrite(ObjectMaster* PointerToObjectMasterInstance, Object* ThisObjPtr);
                public:
                    //clrAndRflctFuncs();
                };
                
                    void calculateValuesAndWriteToObjectMasterArray(ObjectMaster* PointerToObjectMasterInstance); //calculate values of primitives and write to object master arrays (both raw primitives array and object avg coords array)
                        void calculateMainEqn(ObjectMaster* PointerToObjectMasterInstance); //calculate main eqn and store it in object master array
                            void calculateRotation(ObjectMaster* PointerToObjectMasterInstance);
                            void calculateTranslation(ObjectMaster* PointerToObjectMasterInstance);
                
                std::unique_ptr<heightTableFuncs> ZLookupPtr = std::make_unique<heightTableFuncs>();
                std::unique_ptr<lightInfoObjFuncs> lightInfPtr = std::make_unique<lightInfoObjFuncs>();
                std::unique_ptr<clrAndRflctFuncs> clrRflctPtr = std::make_unique<clrAndRflctFuncs>();
                
            public:
                Object(); //initializes a new object with a grapher equation set that is yet unfilled
            };

        
        std::vector<std::vector<Object>> ObjectArray; //2d array of object classes. base level goes on top.
            Object CrossSecObj;//to contain the object data for the cross-section equation. can only contain one primitive, cannot have anything else dependent on this object. cannot be polar or parametric or have rotation or translation modifiers applied either. CAN have animation and slider and eqnModifers applied to it.
                    std::vector<Object> lightInfObjArray;//this, height table, and ClrRflct are to be calculated AFTER the physical objects have been calculated
                    std::vector<Object> ClrAndRflctivityObjArray;
                
        
    public:
        
        float* MetalBuffer;//pointer to the Metal buffer!
                float* MetalOffsetBuffer;
            float* lightInfoBufferPtr;//packed_float4. first three elements store light point coords and last element stores light intensity.
            float* HeightLookupTableBufferPtr;//only single float. no x-coords, only y-coords since indice determines x-interval. start to end of buffer models how cross section should look as it goes from one end to another end of the screen diagonally
            float* clrAndReflectivityBufferPtr;//packed_float4 (reflectiviry, r, g, b)
        
        ObjectMaster();
        //if object exists at indicated spot, adds a primitive to that object. else creates a new object at indicated spot with the primitive.
        void addObjectPrimitive(std::string primitiveEquation, int objectLevel, int objectPosition); //add a primitive to an object. provide objectLevel and objectPosition
            //provide coords for objects that modify main eqn ('#') as well as sliders. throws an erorr if object conssists of more than one primitive since these modifiers have to apply object-wide like rotationa nd translation and not per graph.
            void mainEqnObjectModifiers(int objectLevel, int objectPosition, std::vector<float> SliderValues,std::vector<int> levelPositionCoordsOfEqnModifierObjects);
        //changes the rotation equation for the specified object
        void setObjectRotationDependency(std::string rotationEquation, int objectLevel, int objectPosition, std::vector<int> levelPositionCoordsOfEqnModifierObjects, std::vector<float> SliderValues, int rotEqnMode, int IndiceToSelect = 0); //provide an equation to rotate the selected object as well as an array of x,y pairs that correspond to the objects the selected one depends on with corresponding to the various '#' in the rotation equation. Also provide slider values for '$' in the equation.
        void setObjectTranslationDependency(std::string translationEquation, int objectLevel, int objectPosition, std::vector<int> levelPositionCoordsOfEqnModifierObjects, std::vector<float> SliderValues, int translEqnMode, int IndiceToSelect= 0); //same structure as above
        
        void setCrossSectionEqn(std::string primitiveEqn);//no more than one primitive
            void crossSectionEqnModifiers(std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs);
            void setCrossSectionResol(float newResolVal);
        void setCrossSectionYrange(float newYRange);
        
        void addLightEqns(std::string XYequation, std::string IntensityEqn, std::string ZEqn);//no more than one primitive
            void addXYeqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs);
            void ZEqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs);
            void intensityEqnModifiers(int LightObjPos, std::vector<float> SliderVals, std::vector<int> levelPositionCoordsOfEqnModObjs);
        
        void addClrEqn(std::string redEqn, std::string greenEqn, std::string blueEqn, std::string reflectivityEqn);//no more than one primitive
            void colorEqnModifiers(std::vector<float> SliderValsRed,std::vector<float> SliderValsGreen, std::vector<float> SliderValsBlue, std::vector<float> SliderValsReflectivity, std::vector<int> RedModObjs, std::vector<int> GreenModObjs, std::vector<int> BlueModObjs, std::vector<int> ReflectivityModObjs, int clrNRflctObjPos);
        
        
        void setInitValuesForObjMstr(float xRange, float yRange, float resol, float animVal, float* MetalBufferPtr, float* ColorBufferPtr, float* LightBufferPtr, float* HeightLookupBufferPtr, float* offsetBufferPointer); //THIS Is the function that sets Object Master's init values NOT the ones below!
        
        //sets the same init values for all existing objects
        void setInitObjectMasterValues();
        //the function below is experimental-- do not use for now
            void setInitObjectValuesByMaster(float xRange, float yRange, float resol, int objectLevel, int objectPosition); //sets individual resol and range values for the selected object
        
        std::vector<float>& ReturnReferenceToRequestedObjectVector(int objectLevel, int objectPosition); //returns a pointer to the center coords of the requested object
        float& returnAnimationValue(); //returns current Animation Value
        
        void resetObjectMaster();//blank slate
            void resetAnimationValue();//resets animation value
            void resetObjects(); //clears object array
            void resetObject(int objectLvl, int objectPosition); //resets a single object and removes in from the array
        
        //descale and descale factor to be used for syncing this animation with that of the progress counter animation in swift
        void calculateObjectValues(bool Descale = false, float descaleFactor = 1.0);//calculates values and writes to metal buffer
            void calculateOnlyTheseObjects(std::vector<int> PosCoordsOfObjectsToDraw, bool Descale, float descaleFactor);//only calculate and draw these objects.
            void calculateOnlyTheseLightObjects(std::vector<int> PosCoordsOfLightObjsToDraw, bool Descale, float descaleFactor);
                void incrementAnimationValue(float multiplier);
                            inline std::array<float, 8> pointOffsetCalculator(float point1[2], float point2[2], float offset);//return the two offset-points that need to be added to the current point in metal buffer
        
        int returnPointsTerDraw();
        
        void clearMetalBuffer();//clears metal buffer of points that were drawn.
        
    };
    std::unique_ptr<ObjectMaster> ObjectMasterInstance = std::make_unique<ObjectMaster>();
    
    
};

//modifies the coordinates of a graph by rotating or translating either through given parameters or through the coordinates/theta of another graph using atan to calculate theta
//this class contains static modifiers that just modify graph coordinates statically. to be incorporated in a bigger graph capable of animation
class GrapherStaticModifiers{
public:
    //rotate about point x,y? default = 0,0
    void rotate(std::vector<float>* PointerToArrayToBeModified, float theta, float rotateAboutX, float rotateAboutY);
        void rotate(std::vector<float>* PointerToArrayToBeModified, float x, float y); //calculates theta using atan/overload of above func
        void rotate(std::vector<float>* PointerToArrayToBeModified, std::vector<float>* PointerToArrayToModifyBy); //rotates using atan of corresponding points in provided array
        void rotateByArrayValue(std::vector<float>* PointerToArrayToBeModified, std::vector<float>* PointerToArrayToModifyBy); //instead of rotating by atan of provided array rotates by y-values of provides array
    
    void translate(std::vector<float>* PointerToArrayToBeModified, float xTranslation, float yTranslation);
        void translate(std::vector<float>* PointerToArrayToBeModified, std::vector<float>* PointerToArrayToModifyBy, int translMode);//0- BiDirection; 1- Horizontal; 2- Vertical
};

//put XYcoords struct in here for child classes to use.
//graph constructor also have resolution, xRange, and yRange as arguments. when user clicks zoom button the coordinates are passed into a function that map the given coordinates into pixel coords that are recalculated with every zoom in/out/. when zoom exceeds graph's original x/y values graph is destroyed and reconstructed as a new objecct with greater x/y ranges as original paameters.
class Grapher { //to contain all kinds of graphing classes like trig, calculus, polynomials. initialize them at the START. also have a mixed class for different combos.
    std::unique_ptr<PolynomialGrapher> PolynomialGrapherInstance = std::make_unique<PolynomialGrapher>();
    std::unique_ptr<TrigonometricGrapher> TrigonometricGrapherInstance = std::make_unique<TrigonometricGrapher>();
    std::unique_ptr<CalculusGrapher> CalculusGrapherInstance = std::make_unique<CalculusGrapher>();
    std::unique_ptr<PolarGrapher> PolarGrapherInstance = std::make_unique<PolarGrapher>();
    
    
    XYcoords GraphingCoordsStruct; //current classes create their seperate structs but upload final data to this struct
    XYcoords* PointerToGraphingCoordsStruct;
    
    void setCurrentSubtypePointer(char EquationType);
    
    Grapher* thisGrapherPointer;
    
public:
    
    GraphingSubtypeInterface* CurrentGraphingSubtypePointer;
    
    std::vector<float> printOrReturnCurrentCoords(bool printOrReturn);
    
    //wotnt work on calculus grapher without pre-provided x-interval array
    void SubtypeTesterPrintCoordinates(char EquationType, bool addOrDelete, int termPosition, float termExponent = 1.0, float termCoefficient = 1.0, int termIdentifier = 0);
    
    static std::vector<float> dummyVector;
    
    void setInitGrapherValues(float xRange, float yRange, float resol);
    
    
    class ThreeDeeExpressionArray{
    public:
        bool recalculationOfSameString = false;
        
        struct VectorData{
            bool calculateAsPolar = false;
            //initializes a 1x1x1 cube
            std::vector<std::vector<std::vector<float>>> TermsArray;
            
            //access the array to be used in composition arguments through CalculatedValuesForComposition[NestPosition][NestLevel]
            //needs to be cleared after a row is finished with!
            std::vector<std::vector<std::vector<float>>> CalculatedValuesForComposition;
            
            std::vector<int> SliderValuesVectorCoords; //contains row,column coords to the coefficient element of termsArray vectors that have a '$' so that the slider values can be updated smoothly
                std::vector<float> SliderValues; //values of corresponding slider values. to be updated thorugh function. THIS AND OTHERS NEED TO HAVE CORRESPONDING VALUES!!!
                std::vector<float> OriginalSliderCoefficientValues; //stores the values of the original coefficients modified by sliders!!
            std::vector<int> EquationModifierCoords; //contains row,column coords to the coefficient element vectors that have a '#'
                std::vector<float> EquationValues; //values corresponding to equation modifiers. to be updated thorugh function.
                std::vector<float> OriginalEquationCoefficientValues; //stores the values of the original coefficients modifided by Equations
            std::vector<int> animationModifierCoords; //contains row,column coords to the coefficient element of vectors that have a '~'
                float animationValue; //value of current aniamtion counter
                std::vector<float> OriginalAnimationCoefficientValues; //stores the original value of coefficients modified by animation modifiers
            
            std::vector<float> finalValueOfRows; //to be updated with the final value for each row
            
        }; //VectorDataInstance;
        
        
        struct PositionLogic{
            //logic is stored in current variables before they become valid (like when parentheses arent closed) and are updated to permanent values when the values become valid
            //initalized at 0 and startNestedLevel is ONE '1'
            int currentNestLevel = 0;
            int currentNestPosition = 0;
            int ParenthesesClosureCounter = 0;
            
            //element position corresponds to nested level and element value corresponds to max positions
            //row position corresponds to which rows the max positions correspond to
            std::vector<std::vector<int>> maxNestedPositionsOfNestedLevels = {{0}};
            
            int lengthOfString = 0;
            int currentStringPosition = 0;
            
            int currentColumn = 0;
            int currentRow = 0;
            
            //a kind of x, y, z coordinates setter for the current position in TermsArray
            int CurrentPointerCoordinates[3] = {0, 0, 0};
        }; //plogic;
        
        
        
        class StringParser{
        friend class ThreeDeeExpressionArray;
            void createNewColumnOrRowAndUpdateCounters(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, bool columnOrRow, bool beginningOrEnd);
            
            std::string StringFromSwift = ""; //to be set to string from swift
            void signParserForNewTerm(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer); //if the identifier is '+-*' and nest level is currently 0 (base-level) returns true to construct a new term
            
            void ParenthesesLogic(char ParenthesesType, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);

            void polynomialLogic(char PolynomialIdentifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
                float multiDigitParser(int& uValue, std::string StringFromSwift);
            
            void signLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
            void trigonometricLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
            //to be expanded in the future
            void calculusLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
            void polarLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
            void whiteSpaceLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
                void SliderValueLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer); //for '$'
                void EquationModifierLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer); //for '#'
                void AnimationModifierLogic(char Identifier, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer); //for '~'
            
            //updates nest levels and positions for each vector in each row.
            void nestLevelAndPositionUpdater(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
        };//StringParserInstance;
        
        
        void inputStringParser(std::string inputString, StringParser* StringParserInstancePointer, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
        
        
        class inputStringCalculator {
            Grapher* GrapherInstancePointer;
            //these store term poisitions for the expression array updaters
            int termPositionPolynomial = 0;
            int termPositionPolar = 0;
            int termPositionCalculus = 0;
            int termPositionTrigonometric = 0;
                
        public:
            inputStringCalculator(Grapher* PointerToTheGrapherInstance): GrapherInstancePointer(PointerToTheGrapherInstance){};
            
        std::vector<float> InputtedStringCalculator(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer); //assuming we are calling it AFTER parsing string
            void convertCoordinatesToHaveBothXandYpoints(); //converts coords stored in Graphing coords struct to have both x and y points in case its non-polar and only contains y-points at set x-interval.
            
            void setSubtypeAndUpdateExpression(int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
                        inline void subtypeExpressionArrayUpdater(int row, int column, int expressionType, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            
            std::vector<float> CoordinatesUpdateAcrossRow(int row, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);//updates coordinates across entire row
            //updates data across a whole nestLevel
                void CoordinatesUpdateAcrossNestLevel(int termNumber, int nestLevel, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
                    //if exponentiation identifier then execute special logic where you cannot just do simple addSubtractMultiply
                    bool ExponentiationEdgeCaseLogic(int nestLevel, int nestPosition, int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, std::vector<float> argumentVector, int& counterVariableToModify); //sets counter variable to the last vector of the exponent
                        inline void subtypeCoordinatesUpdater(int row, int column, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, std::vector<float> providedArray = dummyVector);
                        void addCoordinatesToCalculatedValuesArray(std::vector<float> coordinatesArray, int nestLevel, int position, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, int addSubtractMultiplyDivideIdentifier);
                            void clearCalculatedValuesArray(VectorData* VectorDataInstancePointer);
            
        };
        
        
        std::unique_ptr<Grapher::ThreeDeeExpressionArray::StringParser> StringParserUniquePointer = std::make_unique<Grapher::ThreeDeeExpressionArray::StringParser>();
        std::unique_ptr<PositionLogic> PositionLogicUniquePointer = std::make_unique<PositionLogic>();
        std::unique_ptr<VectorData> VectorDataUniquePointer = std::make_unique<VectorData>();
        std::unique_ptr<inputStringCalculator> inputStringCalculatorUniquePointer;
        
        StringParser* StringParserInstancePointer;
        PositionLogic* PositionLogicInstancePointer;
        VectorData* VectorDataInstancePointer;
        inputStringCalculator* inputStringCalculatorInstancePointer;
        
        Grapher* ThePointerToCurrentGrapherInstance; //stores pointer to the grapher instance currently running to pass to nested classes
        ThreeDeeExpressionArray(Grapher* PointerToCurrentGrapherInstance);
        
        void CoordinatesCalculator(std::string inputString, StringParser* StringParserInstancePointer, PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, bool parametricOrNot, std::vector<float> SliderSpecialValues, std::vector<float>& EquationModifierSpecialValues, float animationValue);
            void inputtedStringSliderConstantUpdater(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            void inputtedStringEquationModifierUpdater(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
            void inputtedStringAnimationUpdater(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer);
        
        void resetThreeDeeArray(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, StringParser* StringParserInstancePointer);
            void resetThreeDeeArrayForSameStringRecalculation(PositionLogic* plogicPointer, VectorData* VectorDataInstancePointer, StringParser* StringParserInstancePointer);
        
    };
    ThreeDeeExpressionArray* PointerToThreeDeeExpressionArrayInstance;
    std::unique_ptr<ThreeDeeExpressionArray> ThreeDeeExpressionArrayUniquePointer;
    
    void CoordinatesUpdaterForSwift(std::string inputExpression, bool dontConvertToXYCoords, std::vector<float> SliderSpecialValues, std::vector<float>& EquationModifierSpecialValues, float animationValue, float* pointerToBuffer);
        void SwiftStringSliderValueUpdater(std::vector<float> SliderSpecialValues);
        void SwiftStringEquationModifierUpdater (std::vector<float>& EquationModifierValue);
        void SwiftStringAnimationModifierUpdater(float animationCurrentValue);
    
    
    //set pointer to XYcoords struct as well
    //SINCE CHILD CLASSES INITIALIZED WITHOUT DEFAULT VALUES NEEDS TO ALSO PASS DEFAULT VALUES TO CHILD CLASSES!
    Grapher(char EquationType,float xRangeValue, float yRangeValue, float resolutionValue = 1.0); //constructor-- sets parent pointer to the child class thats been already initialized.
    
    void resetGrapher();
};



class GraphingSubtypeInterface {
public:
    //we'll simplify all of these NVI's into one simple one later. it doesnt matter for now that these NVI's arent non-virtual since they do not modify anything.
    virtual std::vector<std::vector<float>> ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent = 1.0, float termCoefficient = 1.0, /*for trig grapher*/ int termIdentifier = 0) = 0;
    
    virtual std::vector<float> CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, /* this termIdentifier and ExpressionArrayUpdater's are NOT the same. this converts the program codes to functions whereas the former converts unicode to program codes */ int termIdentifier, std::vector<float> providedCoordinatesForCoefficient = {}) = 0;
    
    virtual void setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol = 1.0) = 0;
    
    virtual void ExpressionArrayClear() = 0;
    
    virtual std::vector<std::vector<float>> returnExpressionArray() = 0;
    
    virtual std::vector<float> returnCoordinatesArray() = 0;
    
    virtual float returnXinterval() = 0;
    
    virtual void CoordinatesArrayClear() = 0;
    
};

class PolynomialGrapher: public GraphingSubtypeInterface { //to output coordinates for a graph of a polynomial
    float xRange;
    float yRange;
    float resolution;  //at resolution of 1.0, there is an interval of 0.01x for every y-coordinate point
    
    
    
    
    XYcoords PolynomialCoordsObject;
    XYcoords* PointerToCoordStruct;
    
    //also used for FIRST TIME CONSTRUCTION not just updating
    //ExpressionArrayUpdater is called every single time a term/exponent is updated
    //addOrdelete is true when user adds a term and false when a term is deleted. NOT exponent change, TERM change. unchanged when signs like +/-/*/ '/' / are added
    //termPosition indicates the term that was modified not including SIGNS as position indices. termExponent returns current Exponent of modified term. if term is deleted addOrDelete is FALSE
    //returns the 2D vector expression array from struct XYcoords

    std::vector<std::vector<float>> ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent = 1.0, float termCoefficient = 1.0, /*for trig grapher*/ int termIdentifier = 0) override;
    
    std::vector<float> CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, /* this termIdentifier and ExpressionArrayUpdater's are NOT the same. this converts the program codes to functions whereas the former converts unicode to program codes */ int termIdentifier, std::vector<float> providedCoordinatesForCoefficient = {}) override; //calculates 300 points by default values
public:
    PolynomialGrapher(float xRangeValue, float yRangeValue, float resolutionValue = 1.0);
    PolynomialGrapher();
    
    void setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol = 1.0) override;
    
    //we'll simplify all these individual return functions later. kinda takes away from the simplicity of polymorphism
    std::vector<std::vector<float>> returnExpressionArray() override;
    
    std::vector<float> returnCoordinatesArray() override;
    
    float returnXinterval() override;
    
    void ExpressionArrayClear() override;
    
    void CoordinatesArrayClear() override;

    
};

class TrigonometricGrapher: public GraphingSubtypeInterface {
    float xRange;
    float yRange;
    float resolution;  //at resolution of 1.0, there is an interval of 0.01x for every y-coordinate point
    
    XYcoords TrigonometricCoordsObject;
    XYcoords* PointerToTrigonometricStruct;
    
    std::vector<std::vector<float>> ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent = 1.0, float termCoefficient = 1.0, /*for trig grapher*/ int termIdentifier = 0) override;
    
    std::vector<float> CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, /* this termIdentifier and ExpressionArrayUpdater's are NOT the same. this converts the program codes to functions whereas the former converts unicode to program codes */ int termIdentifier, std::vector<float> providedCoordinatesForCoefficient = {}) override;
    
    
public:
    TrigonometricGrapher(float RangeOfX, float RangeOfY, float ResolutionValue);
    TrigonometricGrapher();
    
    void setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol = 1.0) override;
    
    std::vector<std::vector<float>> returnExpressionArray() override;
    
    float returnXinterval() override;
    
    std::vector<float> returnCoordinatesArray() override;
    
    void ExpressionArrayClear() override;
    
    void CoordinatesArrayClear() override;
    
};



class CalculusGrapher: public GraphingSubtypeInterface {
    float xRange;
    float yRange;
    float resolution;  //at resolution of 1.0, there is an interval of 0.01x for every y-coordinate point
    
    XYcoords CalculusCoordsObject;
    XYcoords* PointerToCalculusStruct;
    
    std::vector<std::vector<float>> ExpressionArrayUpdater(bool addOrDelete, int termPosition, float upperBound = 0.0, float lowerBound = 0.0, /*for calculus grapher. different logic than trig Grapher.*/ int typeIdentifier = 0) override;
    
    std::vector<float> CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, int termIdentifier, std::vector<float> providedCoordinatesForCoefficient = {}) override;
    //calculates 300 points by default values
    
    float AverageReimannSum (float x1, float x2, float y1);
    
    //either upperBound OR lowerBound OR integrand has a vector attached to it; others are constant.
    inline float AreaCalculator (float upperBound, float lowerBound, std::vector<float> integrand, float xInterval, float initialX = 0.0, float previousValue = 0.0, bool previousValueSet = false);
    
public:
    CalculusGrapher(float RangeOfX, float RangeOfY, float ResolutionValue);
    CalculusGrapher();
    
    void setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol = 1.0) override;
    
    std::vector<std::vector<float>> returnExpressionArray() override;
    
    float returnXinterval() override;
    
    std::vector<float> returnCoordinatesArray() override;
    
    void ExpressionArrayClear() override;
    
    void CoordinatesArrayClear() override;
    
    
};



class PolarGrapher: public GraphingSubtypeInterface {
    
    PolynomialGrapher PolarGraphersPolynomialGrapher;
    
    float xRange;
    float yRange;
    float resolution;  //at resolution of 1.0, there is an interval of 0.01x for every y-coordinate point
    
    XYcoords PolarCoordsObject;
    XYcoords* PointerToPolarStruct;
    
    std::vector<float> CoordinatesUpdater(bool useProvidedCoordinatesArrayForXinterval, std::vector<float> ProvidedCoordinatesArrayForXinterval, int termIdentifier, std::vector<float> providedCoordinatesForCoefficient = {}) override;
    
    std::vector<std::vector<float>> ExpressionArrayUpdater(bool addOrDelete, int termPosition, float termExponent = 0.0, float termCoefficient = 0.0, int addSubtractMultiplyOrDivideWithNextTerm = 0) override;
    
public:
    PolarGrapher(float RangeOfX, float RangeOfY, float ResolutionValue);
    PolarGrapher();
    
    void setInitValues(float InitRangeX, float InitRangeY, float InitRangeResol = 1.0) override;
    
    std::vector<std::vector<float>> returnExpressionArray() override;
    
    std::vector<float> returnCoordinatesArray() override;
    
    float returnXinterval() override;
    
    void ExpressionArrayClear() override;
    
    void CoordinatesArrayClear() override;
    
};

