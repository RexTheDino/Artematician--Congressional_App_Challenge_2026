//
//  ContentView.swift
//  ArduiMatorNew
//
//  Created by AnshuDiya on 8/20/26.
//

import SwiftUI
import MetalKit
import Metal
import Cocoa
import QuartzCore

struct topBarImageTapBooleans{
    var lineTap = false
    var circleTap = false
    var parabolaTap = false
    var cardioidTap = false
    var flowerTap = false //snowflake
    var spiralTap = false //hurrican
    var plusTap = false
    var minusTap = false
} //booleans show in UI which primitive is currently selected

struct StringModObjsAndSldrs: Equatable{
    var DerString = "(x)"
    var ModObjs: Array<UUID> = []
    var Sldrs: Array<Float> = []
}

struct EquationMetadata: Identifiable, Equatable{
        let id = UUID()
    var EquationString: Array<String> = ["(x)"]
    var xRange: Float = 300.0
        //the x-value where the slider range starts at (and the picker!)
    var yRange: Float = 1000.0
    var resol: Float = 1.0
        var SliderValuesMainStr: [Int:Array<Float>] = [:]
        var EquationModifierObjectsMainStr: Array<UUID> = [] //UUIDS of modifier objects
            var MainStrLegalObjects:Array<UUID> = [] //all legal modifier objects (not what has been selected but what CAN be selected)
    var rotEqn = ""
        var SliderValuesRotEqn: Array<Float> = []
        var rotEqnModObjs: Array<UUID> = [] //UUIDs of rot mod objs
    var translEqn = ""
        var SliderValuesTranslEqn: Array<Float> = []
        var translEqnModObjs: Array<UUID> = [] //UUIDs of transl mod objs
    
        var rotEqnMode: Int = 1 //refer to pages for further documentation on these five
            var SelectedRotIndice = 0.0
        var translEqnMode = 1
            var SelectedTranslIndice = 0.0
        var lastModifiedPicker: String = "rotEqnMode"//changes string to the ModePicker that was last hovered on.
    
    var SliderColor = Color.blue
    
    var objLvl: Int = 0
    
    var nameOfEqn = "New Object"
        var FinalEqnString: Array<String> = []
}


struct GraphView: View {
    let dummyUUID: UUID = UUID()
    
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    //switched with graphBackground idk y
        let graphBackgroundColor = Color(red:247/255,green:247/255,blue:255/255)
    var workspaceBackgroundColor: Color
    //workspaceBackgroundColor = Color(red:97/255,green:100/255,blue:117/255)
    @State private var graphToWorkspaceToggle = false //triggered when trngle button is hovered over
        @State private var graphWorkspaceClickToggle = false //triggered when tirangle button is clicked
    @State private var sidebarIsExpanded = false
    @State private var ModifierEquationsIsExpanded = false
    @State private var showAnimVal = false
    @State private var previousEqnString: Array<String> = [""]
    
    //set to true when user tries to create more than one primitive with a slider in the main eqn
    @State private var IllegalPrimitiveAlrt = false
    @State private var beingReadForFirstTime = true
    @State private var IllegalDependencyAlert = false
    
    @State var topBarBooleansInstance = topBarImageTapBooleans()
    
    @ObservedObject var GraphInstance: GraphsMetadata
    @State var EquationMetadataInstance: EquationMetadata
        @State private var currentPrimitiveView = 0
        @State private var showAllPrimitives = true
    
    @State var ObjectType: Int //0 for physicalEqn, 1 for RGBrefl, 2 for lightEqns(z-dir and intensity and xy)
    @State var lightMtdt: lightInfMetadata = lightInfMetadata()
    @State var rgbMtdt: RGBreflMetadata = RGBreflMetadata()
    
    @ObservedObject var Swift2ObMstr = SwiftToObjMstr.CoordsAnimInstance
    
    
    var body: some View {
        
        ZStack{
            
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
             Button(action:{ //when clicked, needs to trigger start of animation
             graphWorkspaceClickToggle.toggle()
                 Graphs.animateObject.toggle()
                 Swift2ObMstr.stopAnimation = false
                 Graphs.hasBeenClickedAtleastOnce = true
                 updateCPPToDrawThisObject() //only perform when button is clicked!!
             }) {
             Image(systemName:"play.fill")//triangle.circle.fill
             .imageScale(.large)
             .foregroundColor(.white)
             .frame(width:20,height:20)
             }
             .onHover(perform: { hover in
             triangleButtonHoverLogic(hover: hover)
             })
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
             primitiveIcons(boolTerModify: $topBarBooleansInstance.lineTap, iconName: "line.diagonal.arrow",funcString: "(x)", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
                primitiveIcons(boolTerModify: $topBarBooleansInstance.circleTap, iconName: "circle", funcString: "(r(50))", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
                 primitiveIcons(boolTerModify: $topBarBooleansInstance.parabolaTap, iconName: "function", funcString: "(sin(300-0.02x))*(100)", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
                 primitiveIcons(boolTerModify: $topBarBooleansInstance.cardioidTap, iconName: "suit.heart", funcString: "(5x(1+c))", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
                 primitiveIcons(boolTerModify: $topBarBooleansInstance.flowerTap, iconName: "snowflake", funcString: "(r(5(s(5x))))", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
                 primitiveIcons(boolTerModify: $topBarBooleansInstance.spiralTap, iconName: "hurricane", funcString: "(r(10x))", EqnMtdt: $EquationMetadataInstance, currentPrimView: currentPrimitiveView, topBarBooleansInstance: $topBarBooleansInstance);
             Group{
             ZStack{
             RoundedRectangle(cornerRadius: 10)
             .fill(RadialGradient(colors: [.blue,.green, .red], center: .center, startRadius: 0, endRadius: 10))
             .overlay(
             RoundedRectangle(cornerRadius: 10)
             .fill(topBarBooleansInstance.plusTap ? .thickMaterial : .ultraThinMaterial)
             )
             .frame(maxWidth: 25, maxHeight: 20)
             Image(systemName: "plus.circle")
             .onTapGesture {
             //if there havent been any primitives with modifier equations yet set execute usual logic
             if((EquationMetadataInstance.SliderValuesMainStr[0] == [])&&(EquationMetadataInstance.EquationModifierObjectsMainStr.isEmpty)){
             resetImageTapBooleans()
             topBarBooleansInstance.plusTap = true
             EquationMetadataInstance.EquationString.append("")
             previousEqnString.append("")
             } else { //if a primitive with modifiers has already been created dont do nothing and pop alert
             IllegalPrimitiveAlrt = true
             }
             
             }
             .padding(15)
             }
             ZStack{
             RoundedRectangle(cornerRadius: 10)
             .fill(RadialGradient(colors: [.blue,.green, .red], center: .center, startRadius: 0, endRadius: 10))
             .overlay(
             RoundedRectangle(cornerRadius: 10)
             .fill(topBarBooleansInstance.minusTap ? .thickMaterial : .ultraThinMaterial)
             )
             .frame(maxWidth: 25, maxHeight: 20)
             Image(systemName: "minus.circle")
             .onTapGesture {
             resetImageTapBooleans()
             topBarBooleansInstance.minusTap = true
             EquationMetadataInstance.EquationString.popLast()
             if EquationMetadataInstance.EquationString.isEmpty {EquationMetadataInstance.EquationString.append("")}
             //wipes out last primitive
             if let lastKey = Array(EquationMetadataInstance.SliderValuesMainStr.keys).last {
             EquationMetadataInstance.SliderValuesMainStr[lastKey] = nil
             }
             if currentPrimitiveView >= EquationMetadataInstance.EquationString.count {currentPrimitiveView = EquationMetadataInstance.EquationString.count-1}
             
             }
             .padding(15)
             }
             .padding(.trailing,40)
                 verticalSliderForAnimValMultAndRotTranslModePickers(EqnMtdtInst: $EquationMetadataInstance, isVisible: graphToWorkspaceToggle || graphWorkspaceClickToggle)
                     .padding(.leading,10)
             .padding(.trailing, graphToWorkspaceToggle ? 0 : 500)
                 
             }
             }
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
                             Text(StepperStringReturn())
                                 .italic()
                             Divider()
                             Stepper(value: StepperBindingReturn(), in: 0...StepperRangeReturn()){}
                                 .padding(10)
                                 .padding(10)
                             
                         }
                         VStack{
                             
                             Divider()
                             //GraphEqnModifierSldrsView(EquationMetadataInst: $EquationMetadataInstance, currentPrimitiveView: currentPrimitiveView, rgbMtdt: $rgbMtdt, lightMtdt: $lightMtdt)
                             //GraphEqnModifierSldrsView(theEqnMtdtInstGraphsView: Binding.constant(EquationMetadataInstance))
                             SliderReturn()
                             
                             if EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] != []{
                                 Divider()
                             }
                             let hashtagCount = hashtagCounterReturn()//EquationMetadataInstance.EquationString[0].filter { $0 == "#"}.count //count of # in first object which must be tehe onnly one containing modifiers if modifiers are present; cant have multiple primitives when one primitive alredy has a modifier
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
                         if(EquationMetadataInstance.EquationModifierObjectsMainStr != []){
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
                     if ObjectType == 0{
                         TextField("Rotation Equation", text: Binding(get: {self.EquationMetadataInstance.rotEqn}, set: {newValue in
                             
                             var localCopy = self.EquationMetadataInstance
                             let defaultID  = GraphInstance.MetadataOfEquations[0].id
                             localCopy.rotEqn = newValue
                             let Eqn = newValue
                             if Eqn.isEmpty {previousEqnString[currentPrimitiveView] = ""; localCopy.SliderValuesRotEqn = [];localCopy.rotEqnModObjs = [];}
                             var hashtagCounter = 0
                             var sliderCounter = 0
                             for char in Eqn{
                                 if char == "$" {sliderCounter += 1} else if char == "#" {hashtagCounter += 1}
                             }
                             localCopy.SliderValuesRotEqn = [];
                             for _ in 0..<sliderCounter {
                                 localCopy.SliderValuesRotEqn.append(0.0)
                             }
                             self.EquationMetadataInstance = localCopy
                             levelsUpdaterAndChecker() //level checking for main, rotation, and translation
                             levelsUpdaterAndCheckerRotation()
                             rotEqResizer()
                             levelsUpdaterAndCheckerTranslation()
                             translEqResizer()
                             MainEqnResizer()
                             updateGlobalEqnMtdt()
                             
                             
                         }))
                         .textFieldStyle(.roundedBorder)
                         .background(.thinMaterial)
                         .padding(5)
                     }
                     VStack{
                         
                         Divider()
                         //let sliderArray = EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] ?? []
                         ForEach(0..<(EquationMetadataInstance.SliderValuesRotEqn.count), id: \.self){v in
                             HStack{
                                 Slider(value: Binding(get: {self.EquationMetadataInstance.SliderValuesRotEqn[v]}, set: {self.EquationMetadataInstance.SliderValuesRotEqn[v] = $0}), in: 0...100)
                                     .padding(.bottom,5)
                                 Text("\(EquationMetadataInstance.SliderValuesRotEqn[v])")
                                     .bold()
                             }
                         }
                         if EquationMetadataInstance.SliderValuesRotEqn != []{
                             Divider()
                         }
                         ForEach(0..<(EquationMetadataInstance.rotEqnModObjs.count),id: \.self){number in
                             Picker(selection: $EquationMetadataInstance.rotEqnModObjs[number], label: Text("e")){
                                 //picker of all existing objects along with their names
                                 PickerObjectsDisplayReturn()
                             }
                             .pickerStyle(.menu)
                             .labelsHidden()
                             .padding(.bottom,5)
                         }
                         if(EquationMetadataInstance.rotEqnModObjs != []){
                             Divider()
                         }
                     }
                        TextField("Translation Equation", text: Binding(get: {self.EquationMetadataInstance.translEqn}, set: {newValue in
                            
                            var localCopy = self.EquationMetadataInstance
                            let defaultID  = GraphInstance.MetadataOfEquations[0].id
                            localCopy.translEqn = newValue
                            let Eqn = newValue
                            if Eqn.isEmpty {previousEqnString[currentPrimitiveView] = ""; localCopy.SliderValuesTranslEqn = [];localCopy.translEqnModObjs = [];}
                            var hashtagCounter = 0
                            var sliderCounter = 0
                            for char in Eqn{
                                if char == "$" {sliderCounter += 1} else if char == "#" {hashtagCounter += 1}
                            }
                            localCopy.SliderValuesTranslEqn = [];
                            for _ in 0..<sliderCounter {
                                localCopy.SliderValuesTranslEqn.append(0.0)
                            }
                            self.EquationMetadataInstance = localCopy
                            levelsUpdaterAndChecker() //level checking for main, rotation, and translation
                            levelsUpdaterAndCheckerRotation()
                            rotEqResizer()
                            levelsUpdaterAndCheckerTranslation()
                            translEqResizer()
                            MainEqnResizer()
                            updateGlobalEqnMtdt()
                            
                            
                        }))
                            .textFieldStyle(.roundedBorder)
                            .background(.thinMaterial)
                            .padding(5)
             VStack{
             
             Divider()
             //let sliderArray = EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] ?? []
             ForEach(0..<(EquationMetadataInstance.SliderValuesTranslEqn.count), id: \.self){v in
             HStack{
             Slider(value: Binding(get: {self.EquationMetadataInstance.SliderValuesTranslEqn[v]}, set: {self.EquationMetadataInstance.SliderValuesTranslEqn[v] = $0}), in: 0...100)
             .padding(.bottom,5)
             Text("\(EquationMetadataInstance.SliderValuesTranslEqn[v])")
             .bold()
             }
             }
             if EquationMetadataInstance.SliderValuesTranslEqn != []{
             Divider()
             }
             ForEach(0..<(EquationMetadataInstance.translEqnModObjs.count),id: \.self){number in
             Picker(selection: $EquationMetadataInstance.translEqnModObjs[number], label: Text("e")){
             //picker of all existing objects along with their names
                 PickerObjectsDisplayReturn()
             }
             .pickerStyle(.menu)
             .labelsHidden()
             .padding(.bottom,5)
             }
             if(EquationMetadataInstance.translEqnModObjs != []){
             Divider()
             }
             
             }
             }
             
             .padding(10)
             }
                  
             .background(.thinMaterial)
             .clipShape(RoundedRectangle(cornerRadius: 20))
             .frame(maxWidth: ModifierEquationsIsExpanded || sidebarIsExpanded ? 400 : 200, maxHeight: .infinity, alignment: .topLeading)
             .padding(.leading, 10)
             
             }
             Spacer()
             }
             
              
             Spacer()
             }
        }
        .frame(width: 1600,height: 900)
        
        .onAppear(perform: {
            levelsUpdaterAndChecker() //level checking for main, rotation, and translation
            levelsUpdaterAndCheckerRotation()
            rotEqResizer()
            levelsUpdaterAndCheckerTranslation()
            translEqResizer()
            MainEqnResizer()
            updateGlobalEqnMtdt()
        })
        
        .alert(isPresented: $IllegalPrimitiveAlrt){
            Alert(title: Text("Illegal Primitive"), message: Text("Cannot have more than one primitive with modifiers in the main Equation"), dismissButton: .destructive(Text("Ok lol")))
        }
        .alert(isPresented: $IllegalDependencyAlert){
            Alert(title: Text("Illegal Dependency"), message: Text("Found illegal primitive when all of them should have been removed, this will cause undefined behavior"), dismissButton: .cancel(Text("Bruh")))
        }
        
        
        //if last character is a special value then append default values to slider and eqnModifier arrays so forEach can insert corresponding sliders
        .onChange(of: EquationMetadataInstance, perform: {newValue in
            updateTextField(with: newValue.EquationString[currentPrimitiveView])
            levelsUpdaterAndChecker() //level checking for main, rotation, and translation
            levelsUpdaterAndCheckerRotation()
            rotEqResizer()
            levelsUpdaterAndCheckerTranslation()
            translEqResizer()
            MainEqnResizer()
            updateGlobalEqnMtdt()
            
        })
    }
    
    private func triangleButtonClickLogic(){
        graphWorkspaceClickToggle.toggle()
            Graphs.animateObject.toggle()
        Swift2ObMstr.stopAnimation = false;
            Graphs.hasBeenClickedAtleastOnce = true
            updateCPPToDrawThisObject() //only perform when button is clicked!!
    }
    private func triangleButtonHoverLogic(hover: Bool){
        withAnimation(.spring(response: 0.8, dampingFraction: 0.9)){
        graphToWorkspaceToggle = hover
            Graphs.stopStaticAnimation = hover//static animation stopped when hovered over and started when not hovering over anymore
            Swift2ObMstr.stopStaticAnimation = false; //everytime it is hovered over stop static animation is reset!
            showAnimVal = hover
            EquationMetadataInstance.lastModifiedPicker = "animMult"
            EquationMetadataInstance.SliderColor = Color.blue
        }
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
    private func updateGlobalEqnMtdt(){//updates the correct object array based on the CurrentObjectType
        if Animation.AnimInstance.CurrentObjectViewType == 0{
            if let globalMtdtCorrespondIndex = GraphsMetadata.GraphsMtdt.MetadataOfEquations.firstIndex(where: {$0.id == EquationMetadataInstance.id}){
                GraphsMetadata.GraphsMtdt.MetadataOfEquations[globalMtdtCorrespondIndex] = EquationMetadataInstance
            }
        } else if Animation.AnimInstance.CurrentObjectViewType == 1{
            if let globalMtdtCorrespondIndex = GraphsMetadata.GraphsMtdt.rgbReflObjects.firstIndex(where: {$0.id == rgbMtdt.id}){//the first object that corresponds to{ dis color ob
                GraphsMetadata.GraphsMtdt.rgbReflObjects[globalMtdtCorrespondIndex] = rgbMtdt
            }
        } else {
            if let globalMtdtCorrespondIndex = GraphsMetadata.GraphsMtdt.lightObjects.firstIndex(where: {$0.id == lightMtdt.id}){
                GraphsMetadata.GraphsMtdt.lightObjects[globalMtdtCorrespondIndex] = lightMtdt
            }
        }
    }
    private func updateTextField(with newValue: String){
        var localCopy = self.EquationMetadataInstance
        var stringToCheck: String = ""
            stringToCheck = EquationMetadataInstance.EquationString[currentPrimitiveView]
        localCopy.EquationString[currentPrimitiveView] = stringToCheck
        let Eqn = newValue
            
        if Eqn.isEmpty {previousEqnString[currentPrimitiveView] = ""; /*localCopy.SliderValuesMainStr[currentPrimitiveView] = [];localCopy.EquationModifierObjectsMainStr = [];*/}
        var hashtagCounter = 0
        var sliderCounter = 0
             
        for char in Eqn{
        if char == "$" {sliderCounter += 1} else if char == "#" {hashtagCounter += 1}
        }
        self.EquationMetadataInstance = localCopy
    }
    //returns string that text field is to display as label
    private func TextFieldStringReturn()->String{
        var TextFieldString = ""
            if ObjectType == 0{TextFieldString = "y ="}else if ObjectType == 1{
                if rgbMtdt.currentTextFieldMode == 0{TextFieldString = "red";}else if rgbMtdt.currentTextFieldMode == 1{TextFieldString = "green";} else if rgbMtdt.currentTextFieldMode == 2{TextFieldString = "blue";}else{TextFieldString = "RFL";}
            } else{
                if lightMtdt.currentTextFieldMode == 0{TextFieldString = "y =";}else if lightMtdt.currentTextFieldMode == 1{TextFieldString = "Z-Dir";}else{TextFieldString = "Reflectivity";}
            }
        return TextFieldString
    }
    //returns string for text field to update
    private func TextFieldBindingReturn()->Binding<String>{
        var NewTextVal: Binding<String> = $EquationMetadataInstance.EquationString[currentPrimitiveView]
            NewTextVal = $EquationMetadataInstance.EquationString[currentPrimitiveView]
        return Binding<String>(
            get: { () -> String in
                return NewTextVal.wrappedValue
            }, set: {(newValue: String) in
                NewTextVal.wrappedValue = newValue
                self.updateTextField(with: newValue)
                self.levelsUpdaterAndChecker()
                levelsUpdaterAndCheckerRotation()
                rotEqResizer()
                levelsUpdaterAndCheckerTranslation()
                translEqResizer()
                self.MainEqnResizer()
                updateGlobalEqnMtdt()
            }
        )
    }
    //returns string stepper is to display
    private func StepperStringReturn()->String{
        var StepperString = ""
        StepperString = "Primitive \(currentPrimitiveView)"
        return StepperString
    }
    //returns the currentTextFieldMode stepper needs to bind to
    private func StepperBindingReturn()->Binding<Int>{
        if ObjectType == 0{return $currentPrimitiveView} else if ObjectType == 1{return $rgbMtdt.currentTextFieldMode}else{return $lightMtdt.currentTextFieldMode}
    }
    private func StepperRangeReturn()->Int{
        if ObjectType == 0{return EquationMetadataInstance.EquationString.count-1}else if ObjectType == 1{return 3}else{return 2}//the range that currentTextFieldMode for each ObjectType can range in
    }
    @ViewBuilder
    private func SliderReturn()->some View{
        let sldrCntr = sldrCntrReturn()
            ForEach(0..<sldrCntr, id: \.self){sldr in
                Slider(value: Binding<Float>(get: {
                    guard let array = EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView],array.indices.contains(sldr) else {return 0.0}
                    return array[sldr]
                }
                                             , set: {newVal in
                    if var array = EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView],array.indices.contains(sldr){
                        array[sldr] = newVal
                        EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] = array
                    }
                }), in: 0...100.0)
            }
    }
    private func sldrCntrReturn()->Int{
        return (EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView]?.count ?? 0)
    }
    private func levelsUpdaterAndChecker(){
        if Animation.AnimInstance.CurrentObjectViewType == 0{
            var EqnStringCopy: Array<UUID> = []
            for object in Graphs.MetadataOfEquations{
                if object.id != EquationMetadataInstance.id {EqnStringCopy.append(object.id)} //fills EquationMetadata with all the objects there are currently to be filtered later
            }
            //the highest level object this is dependent on
            let highestObjLvlDep = EquationMetadataInstance.EquationModifierObjectsMainStr.max(by: {objA, objB in
                let ObjectA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                let ObjectB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                return ObjectA?.objLvl ?? 0 < ObjectB?.objLvl ?? 0 //closure returns the object with the highest level
            })
            let highestObj = Graphs.MetadataOfEquations.first(where: {$0.id == highestObjLvlDep})
            let highestLvl = highestObj?.objLvl
            //if current objLvl is equal to or lower than max lvl of objects it is dependent on adjust lvl to be one higher than the highest level of the objects it is dependent on
            if(highestLvl != nil){
                if(EquationMetadataInstance.objLvl <= highestLvl!){EquationMetadataInstance.objLvl = highestLvl!+1}
            }
            
            var UUIDsOfObjDependentOnThisOne: Array<UUID> = []
            
            for m in Graphs.MetadataOfEquations{
                let nonOptionalUUIDs = m.EquationModifierObjectsMainStr.compactMap { $0 }
                let rotIDs = m.rotEqnModObjs.compactMap { $0 }
                let translIDs = m.translEqnModObjs.compactMap { $0 }
                for z in nonOptionalUUIDs{
                    //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                    if z == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                        UUIDsOfObjDependentOnThisOne.append(z) //appends UUIDs of objects that are dependent on this one
                    }
                }
                for c in rotIDs{
                    //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                    if c == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                        UUIDsOfObjDependentOnThisOne.append(c) //appends UUIDs of objects that are dependent on this one
                    }
                }
                for d in translIDs{
                    //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                    if d == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
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
            for ID in EquationMetadataInstance.EquationModifierObjectsMainStr{
                //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                if ID != dummyUUID{//do not execute logic in case its a dummyUUID which is a UUID just appended to EqnMtdtInst
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        let indexToPrune = EquationMetadataInstance.EquationModifierObjectsMainStr.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){
                            EquationMetadataInstance.EquationModifierObjectsMainStr.remove(at: indexToPrune!)
                        }
                    }
                }
            }
            for ID in EqnStringCopy{
                //since eqnInstance already contains only legal values, legalOrIllegalElmnt is nil when EqnModifier doesnt contain all legal values
                let legalOrIllegalElmnt = EquationMetadataInstance.MainStrLegalObjects.first(where: {$0 == ID})
                if legalOrIllegalElmnt == nil {
                    EquationMetadataInstance.MainStrLegalObjects.append(ID)
                }
            }
            for ID in EquationMetadataInstance.EquationModifierObjectsMainStr{
                //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                if legalOrIllegalElement == nil {
                    //if index is not nil exit
                    let indexToPrune = EquationMetadataInstance.EquationModifierObjectsMainStr.firstIndex(where:{$0 == ID})
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
            EquationMetadataInstance.MainStrLegalObjects = EqnStringCopy //sets array of possible values belonging to object to local array
        } else {
            EquationMetadataInstance.MainStrLegalObjects = []
            for object in Graphs.MetadataOfEquations{
                EquationMetadataInstance.MainStrLegalObjects.append(object.id)//appends all existing object IDs to EquationMtdt array to be used by PickerObjectsDisplayReturn when object is of type light or color in order to display all objects that are physical as possible dependencies.
            }
        }
    }
    private func hashtagCounterReturn()->Int{//returns the number of hashtags
        return EquationMetadataInstance.EquationString[0].filter { $0 == "#"}.count
    }
    private func PickerBindingReturn(with IndexInModObjArray: Int)-> Binding<UUID>{
        return $EquationMetadataInstance.EquationModifierObjectsMainStr[IndexInModObjArray]
    }
    @ViewBuilder //returns valid objects for picker to display
    private func PickerObjectsDisplayReturn() -> some View {
        let GoodUUIDs: [UUID] = EquationMetadataInstance.MainStrLegalObjects.compactMap({$0})
        ForEach(GoodUUIDs, id: \.self){ID in
            let object = Graphs.MetadataOfEquations.first(where: {$0.id == ID})
            Text("\(object?.nameOfEqn ?? "NewObject")")
                .tag(ID as UUID?) //tags with UUID
        }
    }
    private func MainEqnResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr
        let hashtagCounter = EquationMetadataInstance.EquationString[0].filter { $0 == "#"}.count
        let differenceInSize = hashtagCounter - EquationMetadataInstance.EquationModifierObjectsMainStr.count //how many dummyUUIDs to append
        if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
            for _ in 0..<differenceInSize {
                EquationMetadataInstance.EquationModifierObjectsMainStr.append(dummyUUID) //appends dummyUUID to Equation array of objects userhas picked
            }
        } else {
            for _ in differenceInSize..<0{
                EquationMetadataInstance.EquationModifierObjectsMainStr.popLast() //removes extra UUIDs that dont correspond to a hashtag
            }
        }
        let dollarCounter = EquationMetadataInstance.EquationString[0].filter {$0 == "$"}.count; let differenceInSliders = dollarCounter - (EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView]?.count ?? 0)//sliders previously stored for current view
        var localCopyOfSliderArray: Array<Float> = EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] ?? []
            if differenceInSliders > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
                for _ in 0..<differenceInSliders {
                    localCopyOfSliderArray.append(0.0) //appends dummyUUID to Equation array of objects userhas picked
                }
            } else {
                for _ in differenceInSliders..<0{
                    localCopyOfSliderArray.popLast() //removes extra UUIDs that dont correspond to a hashtag
                }
            }
            EquationMetadataInstance.SliderValuesMainStr[currentPrimitiveView] = localCopyOfSliderArray
        
    }
        private func levelsUpdaterAndCheckerRotation(){
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                var EqnStringCopy: Array<UUID> = []
                for object in Graphs.MetadataOfEquations{
                    if object.id != EquationMetadataInstance.id {EqnStringCopy.append(object.id)} //fills EquationMetadata with all the objects there are currently to be filtered later
                }
                //the highest level object this is dependent on
                let highestObjLvlDep = EquationMetadataInstance.rotEqnModObjs.max(by: {objA, objB in
                    let ObjectA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let ObjectB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return ObjectA?.objLvl ?? 0 < ObjectB?.objLvl ?? 0 //closure returns the object with the highest level
                })
                let highestObj = Graphs.MetadataOfEquations.first(where: {$0.id == highestObjLvlDep})
                let highestLvl = highestObj?.objLvl
                //if current objLvl is equal to or lower than max lvl of objects it is dependent on adjust lvl to be one higher than the highest level of the objects it is dependent on
                if(highestLvl != nil){
                    if(EquationMetadataInstance.objLvl <= highestLvl!){EquationMetadataInstance.objLvl = highestLvl!+1}
                }
                
                var UUIDsOfObjDependentOnThisOne: Array<UUID> = []
                
                for m in Graphs.MetadataOfEquations{
                    let nonOptionalUUIDs = m.EquationModifierObjectsMainStr.compactMap { $0 }
                    let rotIDs = m.rotEqnModObjs.compactMap { $0 }
                    let translIDs = m.translEqnModObjs.compactMap { $0 }
                    for z in nonOptionalUUIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if z == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                            UUIDsOfObjDependentOnThisOne.append(z) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for c in rotIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if c == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                            UUIDsOfObjDependentOnThisOne.append(c) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for d in translIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if d == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
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
                for ID in EquationMetadataInstance.rotEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        let indexToPrune = EquationMetadataInstance.rotEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){
                            EquationMetadataInstance.rotEqnModObjs.remove(at: indexToPrune!)
                        }
                    }
                }
                for ID in EqnStringCopy{
                    //since eqnInstance already contains only legal values, legalOrIllegalElmnt is nil when EqnModifier doesnt contain all legal values
                    let legalOrIllegalElmnt = EquationMetadataInstance.MainStrLegalObjects.first(where: {$0 == ID})
                    if legalOrIllegalElmnt == nil {
                        EquationMetadataInstance.MainStrLegalObjects.append(ID)
                    }
                }
                for ID in EquationMetadataInstance.rotEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        //if index is not nil exit
                        let indexToPrune = EquationMetadataInstance.rotEqnModObjs.firstIndex(where:{$0 == ID})
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
                EquationMetadataInstance.MainStrLegalObjects = EqnStringCopy //sets array of possible values belonging to object to local array
            }
        } //legal object logic for rotation eqn
        private func rotEqResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                let hashtagCounter = EquationMetadataInstance.rotEqn.filter { $0 == "#"}.count
                // This will reveal if there is a hidden character like "\n" or a space inside!
                let differenceInSize = hashtagCounter - EquationMetadataInstance.rotEqnModObjs.count //how many dummyUUIDs to append
                if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
                    for _ in 0..<differenceInSize {
                        EquationMetadataInstance.rotEqnModObjs.append(dummyUUID) //appends dummyUUID to Equation array of objects user has picked
                    }
                } else {
                    for _ in differenceInSize..<0{
                        EquationMetadataInstance.rotEqnModObjs.popLast() //removes extra UUIDs that dont correspond to a hashtag
                    }
                }
            }
        } //resizer for rotation eqn
        private func levelsUpdaterAndCheckerTranslation(){
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                var EqnStringCopy: Array<UUID> = []
                for object in Graphs.MetadataOfEquations{
                    if object.id != EquationMetadataInstance.id {EqnStringCopy.append(object.id)} //fills EquationMetadata with all the objects there are currently to be filtered later
                }
                //the highest level object this is dependent on
                let highestObjLvlDep = EquationMetadataInstance.translEqnModObjs.max(by: {objA, objB in
                    let ObjectA = Graphs.MetadataOfEquations.first(where: {$0.id == objA})
                    let ObjectB = Graphs.MetadataOfEquations.first(where: {$0.id == objB})
                    return ObjectA?.objLvl ?? 0 < ObjectB?.objLvl ?? 0 //closure returns the object with the highest level
                })
                let highestObj = Graphs.MetadataOfEquations.first(where: {$0.id == highestObjLvlDep})
                let highestLvl = highestObj?.objLvl
                //if current objLvl is equal to or lower than max lvl of objects it is dependent on adjust lvl to be one higher than the highest level of the objects it is dependent on
                if(highestLvl != nil){
                    if(EquationMetadataInstance.objLvl <= highestLvl!){EquationMetadataInstance.objLvl = highestLvl!+1}
                }
                
                var UUIDsOfObjDependentOnThisOne: Array<UUID> = []
                
                for m in Graphs.MetadataOfEquations{
                    let nonOptionalUUIDs = m.EquationModifierObjectsMainStr.compactMap { $0 }
                    let rotIDs = m.rotEqnModObjs.compactMap { $0 }
                    let translIDs = m.translEqnModObjs.compactMap { $0 }
                    for z in nonOptionalUUIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if z == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                            UUIDsOfObjDependentOnThisOne.append(z) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for c in rotIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if c == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
                            UUIDsOfObjDependentOnThisOne.append(c) //appends UUIDs of objects that are dependent on this one
                        }
                    }
                    for d in translIDs{
                        //if NOT on this object (checked with UUID m not equaling this object) and another object is using this object as a modifier (checked with z which is the value of all the UUIDs of the objects the other object is referencing) and objectLevel of the object referencing this object is equal to or lesser than this object, remove value from array
                        if d == EquationMetadataInstance.id && m.id != EquationMetadataInstance.id {
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
                for ID in EquationMetadataInstance.translEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        let indexToPrune = EquationMetadataInstance.translEqnModObjs.firstIndex(where:{$0 == ID})
                        if(indexToPrune != nil){
                            EquationMetadataInstance.translEqnModObjs.remove(at: indexToPrune!)
                        }
                    }
                }
                for ID in EqnStringCopy{
                    //since eqnInstance already contains only legal values, legalOrIllegalElmnt is nil when EqnModifier doesnt contain all legal values
                    let legalOrIllegalElmnt = EquationMetadataInstance.MainStrLegalObjects.first(where: {$0 == ID})
                    if legalOrIllegalElmnt == nil {
                        EquationMetadataInstance.MainStrLegalObjects.append(ID)
                    }
                }
                for ID in EquationMetadataInstance.translEqnModObjs{
                    //checks each element in array of possible and legal values for a matching element in real array. returns nil if existing array contains an illegal value
                    let legalOrIllegalElement = EqnStringCopy.first(where: {$0 == ID})
                    if legalOrIllegalElement == nil {
                        //if index is not nil exit
                        let indexToPrune = EquationMetadataInstance.translEqnModObjs.firstIndex(where:{$0 == ID})
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
                EquationMetadataInstance.MainStrLegalObjects = EqnStringCopy //sets array of possible values belonging to object to local array
            }
        } //legal object logic for rotation eqn
        private func translEqResizer(){ //resizes main eqn array of values user picked for object dependencies to number of hashtags in MainStr
            if Animation.AnimInstance.CurrentObjectViewType == 0{
                let hashtagCounter = EquationMetadataInstance.translEqn.filter { $0 == "#"}.count
                // This will reveal if there is a hidden character like "\n" or a space inside!
                let differenceInSize = hashtagCounter - EquationMetadataInstance.translEqnModObjs.count //how many dummyUUIDs to append
                if differenceInSize > 0 { //if containing less hashtags than needed like when user adds a hashtag, append the number of empty elements needed so picker for object does not go out of bounds
                    for _ in 0..<differenceInSize {
                        EquationMetadataInstance.translEqnModObjs.append(dummyUUID) //appends dummyUUID to Equation array of objects user has picked
                    }
                } else {
                    for _ in differenceInSize..<0{
                        EquationMetadataInstance.translEqnModObjs.popLast() //removes extra UUIDs that dont correspond to a hashtag
                    }
                }
            }
        } //resizer for translation eqn
    
    private func updateCPPToDrawThisObject(){
        if !Graphs.showAllObjects{//does not execute if showAllObjects is toggled
            //need to update cpp to calculate all objects first. drawing specified objects is a seperate function!
            Swift2ObMstr.updateCPPwithAllObjects()//fill with all objects to calculate first since there might be dependencies that need to be calculated but not rendered
            //call function to draw only this object
            let arrayOfOnlyThisObj: Array<UUID> = [EquationMetadataInstance.id]
            Swift2ObMstr.updateObjectsToDraw(ObjectIdArray: arrayOfOnlyThisObj, AnmInstMode: Animation.AnimInstance.CurrentObjectViewType)
        } else {
            Swift2ObMstr.updateCPPwithAllObjects()
        }
    }
    
}




//declare as var plz
//custom clip shape to create animation of graph flowing in arc to reveal workspace
struct GraphToWorkspaceCustomClipFolder: Shape{
    //control point to be passed in parameters
    //how far along you have come across the animation. to be initialized by a counter that triggers inside var body
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    var pointInCurve: CGFloat
    func path(in rect: CGRect) -> Path {
        //move in a diagonal path downwards as the counter inside var body ticks up
        let middlePointOfCurve = CGPoint(x: rect.minX+(pointInCurve*(4/3)),y:rect.minY+pointInCurve)
        var path = Path()
        //move to the bottom left
        path.move(to: CGPoint(x:rect.minX,y:rect.maxY))
        let topRight = CGPoint(x:rect.maxX,y:rect.minY)
        path.addQuadCurve(to: topRight, control: middlePointOfCurve)
        path.addLine(to: CGPoint(x: rect.minX, y: rect.minY))
        path.addLine(to: CGPoint(x:rect.minX,y:rect.maxY))
        Graphs.currentProgressCounter = 1-(pointInCurve/1000.0)
        return path;
    }
}
//turns above clipper into an animation
struct GraphToWorkspaceAnimator: AnimatableModifier{
    let toggle: Bool
    var progress: CGFloat
    var animatableData: CGFloat{
        get{progress}set{progress = newValue}
    }
    func body(content: Content) -> some View {
        content
            .clipShape(GraphToWorkspaceCustomClipFolder(pointInCurve: progress*1000))
    }
}

struct primitiveIcons: View{
    @Binding var boolTerModify: Bool
    var iconName: String
    var funcString: String
    @Binding var EqnMtdt: EquationMetadata
    var currentPrimView: Int
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
                    EqnMtdt.EquationString[currentPrimView] = funcString
                    
                }
                .padding(15)
        }
    }
}

//shows vertical sllider to adjust animation value multiplier as well as pickers to select the current mode of rotation and translation equations for that specific object
struct verticalSliderForAnimValMultAndRotTranslModePickers: View{
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    @Binding var EqnMtdtInst: EquationMetadata //to be provided as parameter
    var isVisible: Bool
    var body: some View{
        HStack{
            if isVisible{
                Slider(value:WhatValueSliderModifies(),in:0.0...AllowedRangeOfSlider())
                    .background(EqnMtdtInst.SliderColor.opacity(0.1))
                    .clipShape(Capsule())
                    .frame(width:200)
                    .padding(.leading,10)
                    .transition(.asymmetric(insertion: .scale, removal: .opacity))
                
                
                Picker(selection: $EqnMtdtInst.rotEqnMode, label: Text("")){//refer to pages documentation
                    Text("Rotate by Atan")
                        .tag(1)
                    Text("Rotate by Y-Val")
                        .tag(2)
                    Text("Rotate by Atan of Indice")
                        .tag(3)
                    Text("Rotate by Y-Val of Indice")
                        .tag(4)
                }
                .pickerStyle(.menu)
                .labelsHidden()
                .frame(width:50)
                Picker(selection: $EqnMtdtInst.translEqnMode, label: Text("")){//refer to pages documentation
                    Text("Translate Horizontally by Array")
                        .tag(1)
                    Text("Translate Vertically by Array")
                        .tag(2)
                    Text("Translate Bidirectionally by Array")
                        .tag(3)
                    Text("Translate Horizontally by Indice")
                        .tag(4)
                    Text("Translate Vertically by Indice")
                        .tag(5)
                    Text("Translate Bidirectionally by Indice")
                        .tag(6)
                }
                .pickerStyle(.menu)
                .labelsHidden()
                .frame(width:50)
            }
        }
            
                .frame(width: isVisible ? 200 : 0, height: isVisible ? 30 : 0)
                .onChange(of: EqnMtdtInst.translEqnMode, perform: {newValue in EqnMtdtInst.lastModifiedPicker = "translEqnMode";
                    DispatchQueue.main.async{
                        if newValue == 4 || newValue == 5 || newValue == 6{EqnMtdtInst.SliderColor = Color.green}else{EqnMtdtInst.SliderColor = Color.blue}
                    }
                })
                .onChange(of: EqnMtdtInst.rotEqnMode, perform: {newValue in EqnMtdtInst.lastModifiedPicker = "rotEqnMode"
                    DispatchQueue.main.async{
                        if newValue == 3 || newValue == 4 {EqnMtdtInst.SliderColor = Color.red}else{EqnMtdtInst.SliderColor = Color.blue}
                    }
                })
        }
        private func WhatValueSliderModifies() -> Binding<Double> {//returns what value slider is to modify
            if EqnMtdtInst.lastModifiedPicker == "animMult"{
                return $Graphs.animationMultiplier
            } else if EqnMtdtInst.lastModifiedPicker == "rotEqnMode" {
                return $EqnMtdtInst.SelectedRotIndice
            } else if EqnMtdtInst.lastModifiedPicker == "translEqnMode"{
                return $EqnMtdtInst.SelectedTranslIndice
            } else {
                print("bad: WhatValueSliderModifies() did notfind a valid binding<double> to return to the slider")
                return $Graphs.animationMultiplier
            }
        }
        private func AllowedRangeOfSlider() -> Double{//outputs the range that the slider is supposed to be using based on if its currently selecting an indice or modifying the animation multiplier. if selecting indice calculates the number of x-points a single primitive currently outputs by (300.0*resol)/2
            let maxRange: Int = (Int(300.0*Graphs.resolOfApp)/2)//the points a single primitive outputs
            if EqnMtdtInst.lastModifiedPicker != "animMult"{//if currently selecting an indice and NOT selecting the animation multiplier, the maxRange should be the max number of x-indices per primitive
                return Double(maxRange)
            } else {//if currenttly modifying the aniamtion multiplier output ordinary max range of 10.0
                return 10.0
            }
        }
        func RevertSliderColor(){EqnMtdtInst.SliderColor = Color.blue; EqnMtdtInst.lastModifiedPicker = "animMult"}
    }
    

