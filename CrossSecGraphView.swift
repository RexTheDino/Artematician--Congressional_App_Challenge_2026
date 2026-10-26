//
//  CrossSecGraphView.swift
//  ArduiMatorNew
//
//  Created by AnshuDiya on 10/8/26.
//

import SwiftUI
import MetalKit
import Metal
import Cocoa
import QuartzCore

struct CrossSecMetadata: Equatable, Identifiable{
    let id = UUID()
    var CrossSecString: String = "(x)"
    var yRange: Float = 50.0//no xRange since it is fixed at sqrt(8)
    var resol: Float = 1.0
        var SliderValues: Array<Float> = []
        var EquationModifierObjectsMainStr: Array<UUID> = [] //UUIDS of modifier objects
            var MainStrLegalObjects:Array<UUID> = [] //all legal modifier objects (not what has been selected but what CAN be selected)
    var lastModifiedPicker: String = ""

    var SliderColor = Color.blue
    var nameOfEqn = "New Cross Section"
}

struct CrossSecGraphView: View {
    let dummyUUID: UUID = UUID()
    
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    //switched with graphBackground idk y
        let graphBackgroundColor = Color(red:247/255,green:247/255,blue:255/255)
    var workspaceBackgroundColor = Color(red:77/255,green:133/255,blue:117/255)
    @State private var graphToWorkspaceToggle = false //triggered when trngle button is hovered over
        @State private var graphWorkspaceClickToggle = false //triggered when tirangle button is clicked
    @State private var sidebarIsExpanded = false
    @State private var ModifierEquationsIsExpanded = false
    @State private var showAnimVal = false

    @State private var beingReadForFirstTime = true
    
    @State var topBarBooleansInstance = topBarImageTapBooleans()
        @State var crossSecMtdt: CrossSecMetadata
    @ObservedObject var Swift2ObMstr = SwiftToObjMstr.CoordsAnimInstance
    @ObservedObject var AnmInst = Animation.AnimInstance
    
    var body: some View {
        ZStack{
            let hashtagCount = hashtagCounterReturn()//rgbMtdt.mainEqn.EquationString[0].filter { $0 == "#"}.count //count of # in first object which must be tehe onnly one containing modifiers if modifiers are present; cant have multiple primitives when one primitive alredy has a modifier
            let sldrCntr = sldrCntrReturn()
             graphBackgroundColor
             .ignoresSafeArea()
             workspaceBackgroundColor
             .ignoresSafeArea()
             .modifier(GraphToWorkspaceAnimator(
             toggle: graphToWorkspaceToggle,
             progress: (graphToWorkspaceToggle || graphWorkspaceClickToggle) ? 0.01 : 1.0
             ))
            
             VStack{
             
             HStack{
             Group{
             Button(action:{ //when clicked, needs to trigger start of animation. needs to be reprogrammed to only display color obejcts
             graphWorkspaceClickToggle.toggle()
                 Graphs.animateObject.toggle()
                 Swift2ObMstr.stopAnimation = false
                 Graphs.hasBeenClickedAtleastOnce = true
                 updateCPPToDrawThisObject() //only perform when button is clicked!!
             }) {
             Image(systemName:"triangle.circle.fill")
             .onHover(perform: { hover in
             withAnimation(.spring(response: 0.8, dampingFraction: 0.9)){
             graphToWorkspaceToggle = hover
                 Graphs.stopStaticAnimation = hover//static animation stopped when hovered over and started when not hovering over anymore
                 Swift2ObMstr.stopStaticAnimation = false; //everytime it is hovered over stop static animation is reset!
                 showAnimVal = hover
                 crossSecMtdt.lastModifiedPicker = "animMult"
                 crossSecMtdt.SliderColor = Color.blue
             }
             
             })
             .imageScale(.large)
             .foregroundColor(.white)
             .frame(width:20,height:20)
             }
             .buttonStyle(.plain)
             .padding(.top,10)
             .padding(.leading, 10)
             .padding(.trailing,30)
             .padding(.bottom, 30)
             }
             ZStack{
             RoundedRectangle(cornerRadius: 10)
             .fill(.thinMaterial)
             .frame(maxWidth: graphToWorkspaceToggle ? 800 : 1300, maxHeight: 50)
             .frame(alignment: .leading)
             .padding(10)
             HStack{
                     if !graphToWorkspaceToggle { Spacer() }
                     //use .onTapGesture to control images like buttons
                     primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.lineTap, iconName: "line.diagonal.arrow",funcString: "(x)", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                 primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.circleTap, iconName: "circle", funcString: "(r(50))", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                     primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.parabolaTap, iconName: "function", funcString: "((x)^(1.5))", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                     primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.cardioidTap, iconName: "suit.heart", funcString: "(5x(1+c))", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                     primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.flowerTap, iconName: "snowflake", funcString: "(r(5(s(5x))))", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                     primitiveIconsCrossSecGraph(boolTerModify: $topBarBooleansInstance.spiralTap, iconName: "hurricane", funcString: "(r(10x))", CrossSectionMtdt: $crossSecMtdt, topBarBooleansInstance: $topBarBooleansInstance);
                 verticalSliderForAnimValMultAndRotTranslModePickersCrossSec(CrossSecMetadata: $crossSecMtdt, isVisible: graphToWorkspaceToggle || graphWorkspaceClickToggle)
                         .padding(.leading,10)
                     
             }
             .padding(.trailing, graphToWorkspaceToggle ? 0 : 650)
             }
             Spacer()
             }
             
             
             HStack{
             Form{
                 
             Section{
                 DisclosureGroup("Equation Metadata",isExpanded: $sidebarIsExpanded){
                     
                     Group{
                         //     Add a ‘$’ in the places where constants controlled by sliders will be used
                         //   Add a ‘#’ in places where constants controlled by equations will be used
                         TextField(TextFieldStringReturn(), text: TextFieldBindingReturn())
                         
                             .textFieldStyle(.roundedBorder)
                             .background(.thinMaterial)
                             .padding(5)
                         
                         HStack{
                             Text("Cross-Section")
                                 .italic()
                             
                         }
                         VStack{
                             
                             Divider()
                             //GraphEqnModifierSldrsView(EquationMetadataInst: $rgbMtdt.mainEqn, currentPrimitiveView: currentPrimitiveView, rgbMtdt: $rgbMtdt, lightMtdt: $lightMtdt)
                             //GraphEqnModifierSldrsView(theEqnMtdtInstGraphsView: Binding.constant(rgbMtdt.mainEqn))
                             SliderReturn()
                             if crossSecMtdt.SliderValues != []{
                                 Divider()
                             }
                             ForEach(0..<hashtagCount,id:\.self){number in
                                 Picker(selection: PickerBindingReturn(with: number), label: Text("e")){
                                     //picker of all existing legal objects along with their names
                                     PickerObjectsDisplayReturn()
                                     
                                 }
                                 .pickerStyle(.menu)
                                 .labelsHidden()
                                 .padding(.bottom,5)
                             }
                         }
                         if(crossSecMtdt.EquationModifierObjectsMainStr != []){
                             Divider()
                         }
                         
                         
                         Group{
                             Slider(value: $Graphs.resolOfApp, in: 0.01...50.0)
                                 .padding(5)
                             Text("Resolution: \(Int(Graphs.resolOfApp*300.0)) points")
                                 .italic()
                                 .padding(.bottom,20)
                             
                             Slider(value: $Graphs.xRangeOfApp, in: 0.0...10_000.0)
                                 .padding(5)
                             Text("X-Range: \(Int(Graphs.xRangeOfApp))")
                                 .italic()
                                 .padding(.bottom,20)
                             
                             Slider(value: $Graphs.yRangeOfApp, in: 0.0...10_000.0)
                                 .padding(5)
                             Text("Y-Range: \(Int(Graphs.yRangeOfApp))")
                                 .italic()
                                 .padding(.bottom,5)
                         }
                         
                         Divider()
                     }
             }
             
             .padding(10)
             }
                  
             .background(.thinMaterial)
             .clipShape(RoundedRectangle(cornerRadius: 20))
             .frame(maxWidth: ModifierEquationsIsExpanded || sidebarIsExpanded ? 300 : 200, maxHeight: .infinity, alignment: .topLeading)
             .padding(.leading, 10)
             
             }
             Spacer()
             }
             
              
             Spacer()
             }
        }
        .frame(width: 1600,height: 900)
        
        .onAppear(perform: {
            MainEqnResizer()
            //levelsUpdaterAndCheckerRotation()
            //rotEqResizer()
            //levelsUpdaterAndCheckerTranslation()
            //translEqResizer() commented out for now
            updateGlobalCrossSecMtdt()
        })
        
        
        //if last character is a special value then append default values to slider and eqnModifier arrays so forEach can insert corresponding sliders
        .onChange(of: crossSecMtdt, perform: {newValue in
            updateTextField(with: crossSecMtdt.CrossSecString)
            MainEqnResizer()
            //rgbMtdt.RGBrefl[rgbMtdt.currentTextFieldMode].Sldrs = newValue.currentSldrArray//updates the equation metadata thingy inside rgbMtdt to the one stored locally. since both ened to be synced.
            //levelsUpdaterAndCheckerRotation()
            //rotEqResizer()
            //levelsUpdaterAndCheckerTranslation()
            //translEqResizer() commented out for now
            updateGlobalCrossSecMtdt()
        })
    }
    
    private func resetImageTapBooleans(){
        topBarBooleansInstance.lineTap = false
        topBarBooleansInstance.circleTap = false
        topBarBooleansInstance.cardioidTap = false
        topBarBooleansInstance.parabolaTap = false
        topBarBooleansInstance.flowerTap = false
        topBarBooleansInstance.spiralTap = false
        topBarBooleansInstance.plusTap = false
        topBarBooleansInstance.minusTap = false
    }
    @ViewBuilder
    private func SliderReturn()-> some View{
        let sldrCntr = sldrCntrReturn()
        ForEach(0..<sldrCntr, id: \.self){number in
            HStack{
                Slider(value: $crossSecMtdt.SliderValues[number], in: 0...100.0)
                Text(String(format: "%.2f", crossSecMtdt.SliderValues[number]))
            }
           }
    }
    private func updateGlobalCrossSecMtdt(){//updates the correct object array based on the CurrentObjectType
        Graphs.CrossSecObject = crossSecMtdt
    }
    private func updateTextField(with newValue: String){
        var localCopy = self.crossSecMtdt
        let Eqn = newValue
            
        //if Eqn.isEmpty {localCopy.EquationModifierObjectsMainStr = [];}
        var hashtagCounter = 0
        var sliderCounter = 0
             
        for char in Eqn{
        if char == "$" {sliderCounter += 1} else if char == "#" {hashtagCounter += 1}
        }
        for _ in 0..<hashtagCounter{
            localCopy.EquationModifierObjectsMainStr.append(dummyUUID)
        }
        self.crossSecMtdt = localCopy
        //rgbMtdt.currentSldrArray = localCopy.Sldrs
    }
    //returns string that text field is to display as label
    private func TextFieldStringReturn()->String{
        var TextFieldString = "C.S."
        return TextFieldString
    }
    //returns string for text field to update
    private func TextFieldBindingReturn()->Binding<String>{
        var NewTextVal: Binding<String> = $crossSecMtdt.CrossSecString
        return Binding<String>(
            get: { () -> String in
                return NewTextVal.wrappedValue
            }, set: {(newValue: String) in
                NewTextVal.wrappedValue = newValue
                self.updateTextField(with: newValue)
                self.MainEqnResizer()
                //levelsUpdaterAndCheckerRotation()
                //rotEqResizer()
                //levelsUpdaterAndCheckerTranslation()
                //translEqResizer() commented out for now
                updateGlobalCrossSecMtdt()
            }
        )
    }
    private func hashtagCounterReturn()->Int{//returns the number of hashtags
        return crossSecMtdt.CrossSecString.filter {$0 == "#"}.count
    }
    private func sldrCntrReturn()->Int{
        return crossSecMtdt.CrossSecString.filter {$0 == "$"}.count
    }
    private func PickerBindingReturn(with IndexInModObjArray: Int)-> Binding<UUID>{
        crossSecMtdt.EquationModifierObjectsMainStr = []
        for object in Graphs.MetadataOfEquations{
            crossSecMtdt.EquationModifierObjectsMainStr.append(object.id)//apends all legal objects for hte next picker viewbuilder function to use
        }
        return $crossSecMtdt.EquationModifierObjectsMainStr[IndexInModObjArray]
    }
    @ViewBuilder //returns valid objects for picker to display
    private func PickerObjectsDisplayReturn() -> some View {
        let GoodUUIDs = crossSecMtdt.MainStrLegalObjects.compactMap({$0})
        ForEach(GoodUUIDs, id: \.self){ID in
            let object = Graphs.MetadataOfEquations.first(where: {$0.id == ID})
            Text("\(object?.nameOfEqn ?? "NewObject")")
                .tag(ID as UUID?) //tags with UUID
        }
    }
    private func SliderBindingReturn(Indice: Int)->Binding<Float>{
        return Binding<Float>(get: {return crossSecMtdt.SliderValues[Indice]},
                              set: {newValue in
            crossSecMtdt.SliderValues[Indice] = newValue
        })
    }
    private func MainEqnResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr and number of $
        var hashtagCounter: Int
        var dollarCounter: Int
        var differenceInSize: Int
        var differenceInSliders: Int
        hashtagCounter = crossSecMtdt.CrossSecString.filter { $0 == "#"}.count
            dollarCounter = crossSecMtdt.CrossSecString.filter { $0 == "$"}.count
        differenceInSize = hashtagCounter - crossSecMtdt.EquationModifierObjectsMainStr.count
            differenceInSliders = dollarCounter - crossSecMtdt.SliderValues.count
        //let hashtagCounter = rgbMtdt.mainEqn.EquationString[0].filter { $0 == "#"}.count
        //let differenceInSize = hashtagCounter - rgbMtdt.mainEqn.EquationModifierObjectsMainStr.count //how many dummyUUIDs to append
        if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
            for _ in 0..<differenceInSize {
                crossSecMtdt.EquationModifierObjectsMainStr.append(dummyUUID)
            }
        } else {
            for _ in differenceInSize..<0{
                crossSecMtdt.EquationModifierObjectsMainStr.popLast()
            }
        }
        if differenceInSliders > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
            for _ in 0..<differenceInSliders {
                crossSecMtdt.SliderValues.append(0.0)
            }
        } else {
            for _ in differenceInSliders..<0{
                crossSecMtdt.SliderValues.popLast()
            }
        }
    }
        /*private func levelsUpdaterAndCheckerRotation(){
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                var EqnStringCopy: Array<UUID> = []
                for object in Graphs.MetadataOfEquations{
                    if object.id != rgbMtdt.mainEqn.id {EqnStringCopy.append(object.id)} //fills EquationMetadata with all the objects there are currently to be filtered later
                }
                //the highest level object this is dependent on
                let highestObjLvlDep = rgbMtdt.mainEqn.rotEqnModObjs.max(by: {objA, objB in
                    let ObjectA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let ObjectB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return ObjectA?.objLvl ?? 0 < ObjectB?.objLvl ?? 0 //closure returns the object with the highest level
                })
                let highestObj = Graphs.MetadataOfEquations.first(where: {$0.id == highestObjLvlDep})
                let highestLvl = highestObj?.objLvl
                //if current objLvl is equal to or lower than max lvl of objects it is dependent on adjust lvl to be one higher than the highest level of the objects it is dependent on
                if(highestLvl != nil){
                    if(rgbMtdt.mainEqn.objLvl <= highestLvl!){rgbMtdt.mainEqn.objLvl = highestLvl!+1}
                }
                
                var UUIDsOfObjDependentOnThisOne: Array<UUID> = []
                
                for m in Graphs.MetadataOfEquations{
                    let nonOptionalUUIDs = m.EquationModifierObjectsMainStr.compactMap { $0 }
                    let rotIDs = m.rotEqnModObjs.compactMap { $0 }
                    let translIDs = m.translEqnModObjs.compactMap { $0 }
                    for z in nonOptionalUUIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if z == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(z) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for c in rotIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if c == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(c) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for d in translIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if d == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(d) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                }
                
                let ObjDependentOnThisWithMinLvl = UUIDsOfObjDependentOnThisOne.max(by: {objA, objB in
                    let objReferencedA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let objReferencedB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return (objReferencedA?.objLvl ?? 0) > (objReferencedB?.objLvl ?? 0) //closure returns UUID of object with min level that is dependent on this one
                })
                
                //level of object dependent on this with the minimum object Level
                let minLvlOfObjDependentOnThis = Graphs.MetadataOfEquations.first(where: {$0.id == ObjDependentOnThisWithMinLvl})?.objLvl
                
                //safe UUIDs of all objects currently existing (Since the array hasnt been modified yet and was initially set to all objects)
                let unOptionalUUIDs = EqnStringCopy.compactMap({$0})
                for derUUID in unOptionalUUIDs {
                    //grab the object referenced by the current UUID
                    let objReferenced = Graphs.MetadataOfEquations.first(where: {$0.id == derUUID})
                    //if the objectLevel of the object currently being checked is equal to or greater than the minimum level of all the objects that are dependent on this object, remove that object from the list of possible objects to select from
                    if(objReferenced!.objLvl >= minLvlOfObjDependentOnThis ?? 10000){
                        let indexToBeRemoved = EqnStringCopy.firstIndex(where: {$0 == derUUID})
                        EqnStringCopy.remove(at: indexToBeRemoved!)
                    }
                }
                for ID in rgbMtdt.mainEqn.rotEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        let indexToPrune = rgbMtdt.mainEqn.rotEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){
                            rgbMtdt.mainEqn.rotEqnModObjs.remove(at: indexToPrune!)
                        }
                    }
                }
                for ID in EqnStringCopy{
                    //since eqnInstance already contains only legal values, legalOrIllegalElmnt is nil when EqnModifier doesnt contain all legal values
                    let legalOrIllegalElmnt = rgbMtdt.mainEqn.MainStrLegalObjects.first(where: {$0 == ID})
                    if legalOrIllegalElmnt == nil {
                        rgbMtdt.mainEqn.MainStrLegalObjects.append(ID)
                    }
                }
                for ID in rgbMtdt.mainEqn.rotEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        //if index is not nil exit
                        let indexToPrune = rgbMtdt.mainEqn.rotEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){IllegalDependencyAlert = true} //everything should have been pruned in steps before so this is alert
                    }
                }
                if let minObjLvl = EqnStringCopy.max(by: {objA, objB in //returns MIN not MAX!!!
                    let objReferencedA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let objReferencedB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return objReferencedA?.objLvl ?? 0 > objReferencedB?.objLvl ?? 0
                }){
                    if let LvlRfrncd = Graphs.MetadataOfEquations.first(where: {$0.id == minObjLvl}){
                        if LvlRfrncd.objLvl > 1 {
                            let LvlsToDecrementBy = LvlRfrncd.objLvl-1
                            for index in Graphs.MetadataOfEquations.indices {
                                Graphs.MetadataOfEquations[index].objLvl -= LvlsToDecrementBy //decrements by the levels needed so that base object always sits at 1.
                            }
                        }
                    }
                }
                rgbMtdt.mainEqn.MainStrLegalObjects = EqnStringCopy //sets array of possible values belonging to object to local array
            }
        } //legal object logic for rotation eqn*/
        private func rotEqResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr
            /*if Animation.AnimInstance.CurrentObjectViewType == 0{
                let hashtagCounter = rgbMtdt.mainEqn.rotEqn.filter { $0 == "#"}.count
                // This will reveal if there is a hidden character like "\n" or a space inside!
                let differenceInSize = hashtagCounter - rgbMtdt.mainEqn.rotEqnModObjs.count //how many dummyUUIDs to append
                if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
                    for _ in 0..<differenceInSize {
                        rgbMtdt.mainEqn.rotEqnModObjs.append(dummyUUID) //appends dummyUUID to Equation array of objects user has picked
                    }
                } else {
                    for _ in differenceInSize..<0{
                        rgbMtdt.mainEqn.rotEqnModObjs.popLast() //removes extra UUIDs that dont correspond to a hashtag
                    }
                }
            } commented out for now*/
        } //resizer for rotation eqn
        private func levelsUpdaterAndCheckerTranslation(){
            /*if Animation.AnimInstance.CurrentObjectViewType == 0{
                var EqnStringCopy: Array<UUID> = []
                for object in Graphs.MetadataOfEquations{
                    if object.id != rgbMtdt.mainEqn.id {EqnStringCopy.append(object.id)} //fills EquationMetadata with all the objects there are currently to be filtered later
                }
                //the highest level object this is dependent on
                let highestObjLvlDep = rgbMtdt.mainEqn.translEqnModObjs.max(by: {objA, objB in
                    let ObjectA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let ObjectB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return ObjectA?.objLvl ?? 0 < ObjectB?.objLvl ?? 0 //closure returns the object with the highest level
                })
                let highestObj = Graphs.MetadataOfEquations.first(where: {$0.id == highestObjLvlDep})
                let highestLvl = highestObj?.objLvl
                //if current objLvl is equal to or lower than max lvl of objects it is dependent on adjust lvl to be one higher than the highest level of the objects it is dependent on
                if(highestLvl != nil){
                    if(rgbMtdt.mainEqn.objLvl <= highestLvl!){rgbMtdt.mainEqn.objLvl = highestLvl!+1}
                }
                
                var UUIDsOfObjDependentOnThisOne: Array<UUID> = []
                
                for m in Graphs.MetadataOfEquations{
                    let nonOptionalUUIDs = m.EquationModifierObjectsMainStr.compactMap { $0 }
                    let rotIDs = m.rotEqnModObjs.compactMap { $0 }
                    let translIDs = m.translEqnModObjs.compactMap { $0 }
                    for z in nonOptionalUUIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if z == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(z) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for c in rotIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if c == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(c) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for d in translIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if d == rgbMtdt.mainEqn.id && m.id != rgbMtdt.mainEqn.id {
                            UUIDsOfObjDependentOnThisOne.append(d) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                }
                
                let ObjDependentOnThisWithMinLvl = UUIDsOfObjDependentOnThisOne.max(by: {objA, objB in
                    let objReferencedA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let objReferencedB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return (objReferencedA?.objLvl ?? 0) > (objReferencedB?.objLvl ?? 0) //closure returns UUID of object with min level that is dependent on this one
                })
                
                //level of object dependent on this with the minimum object Level
                let minLvlOfObjDependentOnThis = Graphs.MetadataOfEquations.first(where: {$0.id == ObjDependentOnThisWithMinLvl})?.objLvl
                
                //safe UUIDs of all objects currently existing (Since the array hasnt been modified yet and was initially set to all objects)
                let unOptionalUUIDs = EqnStringCopy.compactMap({$0})
                for derUUID in unOptionalUUIDs {
                    //grab the object referenced by the current UUID
                    let objReferenced = Graphs.MetadataOfEquations.first(where: {$0.id == derUUID})
                    //if the objectLevel of the object currently being checked is equal to or greater than the minimum level of all the objects that are dependent on this object, remove that object from the list of possible objects to select from
                    if(objReferenced!.objLvl >= minLvlOfObjDependentOnThis ?? 10000){
                        let indexToBeRemoved = EqnStringCopy.firstIndex(where: {$0 == derUUID})
                        EqnStringCopy.remove(at: indexToBeRemoved!)
                    }
                }
                for ID in rgbMtdt.mainEqn.translEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        let indexToPrune = rgbMtdt.mainEqn.translEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){
                            rgbMtdt.mainEqn.translEqnModObjs.remove(at: indexToPrune!)
                        }
                    }
                }
                for ID in EqnStringCopy{
                    //since eqnInstance already contains only legal values, legalOrIllegalElmnt is nil when EqnModifier doesnt contain all legal values
                    let legalOrIllegalElmnt = rgbMtdt.mainEqn.MainStrLegalObjects.first(where: {$0 == ID})
                    if legalOrIllegalElmnt == nil {
                        rgbMtdt.mainEqn.MainStrLegalObjects.append(ID)
                    }
                }
                for ID in rgbMtdt.mainEqn.translEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        //if index is not nil exit
                        let indexToPrune = rgbMtdt.mainEqn.translEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){IllegalDependencyAlert = true} //everything should have been pruned in steps before so this is alert
                    }
                }
                if let minObjLvl = EqnStringCopy.max(by: {objA, objB in //returns MIN not MAX!!!
                    let objReferencedA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let objReferencedB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return objReferencedA?.objLvl ?? 0 > objReferencedB?.objLvl ?? 0
                }){
                    if let LvlRfrncd = Graphs.MetadataOfEquations.first(where: {$0.id == minObjLvl}){
                        if LvlRfrncd.objLvl > 1 {
                            let LvlsToDecrementBy = LvlRfrncd.objLvl-1
                            for index in Graphs.MetadataOfEquations.indices {
                                Graphs.MetadataOfEquations[index].objLvl -= LvlsToDecrementBy //decrements by the levels needed so that base object always sits at 1.
                            }
                        }
                    }
                }
                rgbMtdt.mainEqn.MainStrLegalObjects = EqnStringCopy //sets array of possible values belonging to object to local array
            }commented out for now*/
        } //legal object logic for rotation eqn
       /* private func translEqResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                let hashtagCounter = rgbMtdt.mainEqn.translEqn.filter { $0 == "#"}.count
                // This will reveal if there is a hidden character like "\n" or a space inside!
                let differenceInSize = hashtagCounter - rgbMtdt.mainEqn.translEqnModObjs.count //how many dummyUUIDs to append
                if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
                    for _ in 0..<differenceInSize {
                        rgbMtdt.mainEqn.translEqnModObjs.append(dummyUUID) //appends dummyUUID to Equation array of objects user has picked
                    }
                } else {
                    for _ in differenceInSize..<0{
                        rgbMtdt.mainEqn.translEqnModObjs.popLast() //removes extra UUIDs that dont correspond to a hashtag
                    }
                }
            }
        }*/ //resizer for translation eqn. commented out for now
    
    private func updateCPPToDrawThisObject(){
        if !Graphs.showAllObjects{//does not execute if showAllObjects is toggled
            //need to update cpp to calculate all objects first. drawing specified objects is a seperate function!
            Swift2ObMstr.updateCPPwithAllObjects()//fill with all objects to calculate first since there might be dependencies that need to be calculated but not rendered
            //call function to draw only this object
            let arrayOfOnlyThisObj: Array<UUID> = [Graphs.MetadataOfEquations[0].id]//temporarily set to first eqnMtdt object being animated when any other mode is selected with only one obj to be drawn  [crossSecMtdt.id]
            Swift2ObMstr.updateObjectsToDraw(ObjectIdArray: arrayOfOnlyThisObj,AnmInstMode: AnmInst.CurrentObjectViewType)
        } else {
            Swift2ObMstr.updateCPPwithAllObjects()
        }
    }
}

struct primitiveIconsCrossSecGraph: View{
    @Binding var boolTerModify: Bool
    var iconName: String
    var funcString: String
    @Binding var CrossSectionMtdt: CrossSecMetadata
    @Binding var topBarBooleansInstance: topBarImageTapBooleans
    
    var body: some View{
        ZStack{
            RoundedRectangle(cornerRadius: 10)
                .fill(RadialGradient(colors: [.blue,.green, .red], center: .center, startRadius: 0, endRadius: 10))
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .fill(boolTerModify ? .thickMaterial : .ultraThinMaterial)
                )
                .frame(maxWidth: 25, maxHeight: 20)
            Image(systemName: iconName)
                .onTapGesture {
                    topBarBooleansInstance.lineTap = false
                    topBarBooleansInstance.circleTap = false
                    topBarBooleansInstance.cardioidTap = false
                    topBarBooleansInstance.parabolaTap = false
                    topBarBooleansInstance.flowerTap = false
                    topBarBooleansInstance.spiralTap = false
                    topBarBooleansInstance.plusTap = false
                    topBarBooleansInstance.minusTap = false
                    boolTerModify = true
                    CrossSectionMtdt.CrossSecString = funcString
                    
                }
                .padding(15)
        }
    }
}

//shows vertical sllider to adjust animation value multiplier as well as pickers to select the current mode of rotation and translation equations for that specific object
struct verticalSliderForAnimValMultAndRotTranslModePickersCrossSec: View{
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    @Binding var CrossSecMetadata: CrossSecMetadata //to be provided as parameter
    var isVisible: Bool
    var body: some View{
        HStack{
            if isVisible{
                Slider(value:$Graphs.animationMultiplier,in:0.0...10.0)
                    .background(CrossSecMetadata.SliderColor.opacity(0.1))
                    .clipShape(Capsule())
                    .frame(width:200)
                    .padding(.leading,10)
                    .transition(.asymmetric(insertion: .scale, removal: .opacity))
            }
        }
            
                .frame(width: isVisible ? 200 : 0, height: isVisible ? 30 : 0)
        }
        /*private func WhatValueSliderModifies() -> Binding<Double> {//returns what value slider is to modify
            if CrossSecMetadata.lastModifiedPicker == "animMult"{
                return $Graphs.animationMultiplier
            } else if CrossSecMetadata.lastModifiedPicker == "rotEqnMode" {
                return $CrossSecMetadata.SelectedRotIndice
            } else if CrossSecMetadata.lastModifiedPicker == "translEqnMode"{
                return $CrossSecMetadata.SelectedTranslIndice
            } else {
                print("bad: WhatValueSliderModifies() did notfind a valid binding<double> to return to the slider")
                return $Graphs.animationMultiplier
            }
        }*/
        /*private func AllowedRangeOfSlider() -> Double{//outputs the range that the slider is supposed to be using based on if its currently selecting an indice or modifying the animation multiplier. if selecting indice calculates the number of x-points a single primitive currently outputs by (300.0*resol)/2
            let maxRange: Int = (Int(300.0*Graphs.resolOfApp)/2)//the points a single primitive outputs
            if CrossSecMetadata.lastModifiedPicker != "animMult"{//if currently selecting an indice and NOT selecting the animation multiplier, the maxRange should be the max number of x-indices per primitive
                return Double(maxRange)
            } else {//if currenttly modifying the aniamtion multiplier output ordinary max range of 10.0
                return 10.0
            }
        }*/
        func RevertSliderColor(){CrossSecMetadata.SliderColor = Color.blue; CrossSecMetadata.lastModifiedPicker = "animMult"}
    }
