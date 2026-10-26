//
//  MultiGraphManager.swift
//  ArduiMatorNew
//
//  Created by AnshuDiya on 9/18/26.
//

//Intended for the Grapher workspace where only a single-object is still beign manipulated but multiple primitives are being operated. initializes a new GraphView for every primitive provided providing an EquationMetadata Struct construction as a parameter. then looks at all those parameters to pass to a single object in ObjectMaster.

import SwiftUI

//stores graph equations for all existing objects
class GraphsMetadata: ObservableObject{
    static let GraphsMtdt = GraphsMetadata()
    var MetadataOfEquations: Array<EquationMetadata> = [EquationMetadata()] //array of structs EquationMetadata. each element contains data for one equation/primitive.
        var drawTheseObjects: Array<EquationMetadata?> = []//objects to be drawn
            var rgbReflObjects: [RGBreflMetadata] = [RGBreflMetadata()]
            var lightObjects: [lightInfMetadata] = [lightInfMetadata()]
                            var CrossSecObject: CrossSecMetadata = CrossSecMetadata()//contains singular object
    var AnimationValue: Float = 0.0
    var animationMultiplier: Double = 1.0
    var durationOfAnimation: Float = 10.0 //in seconds
    var stopStaticAnimation = false //set to true when button is hoevred over. animate object is set to true when button is CLICKED. this is set to false when button is not hovered over whereas animate object is set to false when button is clicked again
    var animateObject = false //set to true when user clicks animate button. when false metal calls once a static draw version of the animation that is scaled down and with its NDC's moved such that it sits in the bottom left corner of the screen.
    var hasBeenClickedAtleastOnce = false//set to true by animation button and dont animate even statically before this is clicked
    var showAllObjects = false
    
    var selectedObjectID: UUID = UUID()//to be set to the current object selected by object switcher
    
    @Published var xRangeOfApp: Float = 300.0
    @Published var yRangeOfApp: Float = 1000.0
    @Published var resolOfApp: Float = 1.0
    
    var currentProgressCounter = 0.0//ticks along with the progress counter that drives animation
    
    
}

//still in single object construction view but switch to viewing different objects
struct ObjectSwitcherView: View {
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    @State private var selectedObjectID: UUID
    @State private var SavingObject = false
        @State private var SaveObjectAlert = false
            @State private var tempObjName = ""
    
    @ObservedObject var Swift2ObMstr = SwiftToObjMstr.CoordsAnimInstance
    @ObservedObject var AnmInst = Animation.AnimInstance
    
    var rgbReflMtdt = RGBreflMetadata()//for testing
    
    init(){
        var firstID: UUID
        if Animation.AnimInstance.CurrentObjectViewType == 0{firstID = GraphsMetadata.GraphsMtdt.MetadataOfEquations.first?.id ?? UUID()}else if Animation.AnimInstance.CurrentObjectViewType == 1{
            firstID = GraphsMetadata.GraphsMtdt.rgbReflObjects[0].id
        }else{firstID = GraphsMetadata.GraphsMtdt.lightObjects[0].id}
        self._selectedObjectID = State(initialValue: firstID)
        Graphs.selectedObjectID = selectedObjectID
    }
    var body: some View {
        ZStack{//depending on which object type is currently selected, passes the appropriate equationMetadataInstance initializer and rgb/light object initialzier to the graph view
            if AnmInst.CurrentObjectViewType == 0{
                let matchedObject = (Graphs.MetadataOfEquations.firstIndex(where: { $0.id == selectedObjectID }) ?? 0)
                GraphView(workspaceBackgroundColor: Color(red:97/255,green:100/255,blue:117/255), GraphInstance: Graphs, EquationMetadataInstance: Graphs.MetadataOfEquations[matchedObject],ObjectType: 0)
                    .id(selectedObjectID)
            } else if AnmInst.CurrentObjectViewType == 1{
                let matchedObject = (GraphsMetadata.GraphsMtdt.rgbReflObjects.firstIndex(where: {$0.id == selectedObjectID}) ?? 0)
                ColorGraphView(GraphInstance: Graphs, rgbMtdt: GraphsMetadata.GraphsMtdt.rgbReflObjects[matchedObject])
                    .id(selectedObjectID)
            } else if AnmInst.CurrentObjectViewType == 2{
                let matchedObject = (Graphs.lightObjects.firstIndex(where: {$0.id == selectedObjectID}) ?? 0)
                LightGraphView(GraphInstance: Graphs, lightMtdt: Graphs.lightObjects[matchedObject])
                //GraphView(workspaceBackgroundColor: Color(red:177/255,green:100/255,blue:117/255), GraphInstance: Graphs, EquationMetadataInstance: Graphs.lightObjects[matchedObject ?? 0].mainEqn,ObjectType: 2,lightMtdt: Graphs.lightObjects[matchedObject])
                    .id(selectedObjectID)
            } else{
                CrossSecGraphView(crossSecMtdt: Graphs.CrossSecObject)
            }
            MetalView()
                .frame(width: 750, height: 750)
            
            /*if !Graphs.MetadataOfEquations.isEmpty {
             // If something gets deleted or lost, fall back to the first available item
             GraphView(GraphInstance: Graphs, EquationMetadataInstance: Graphs.MetadataOfEquations[0],ObjectType: 1,rgbMtdt: [rgbReflMtdt])//values initialized for testing
             }*/
            // .frame(width: 500, height: 500)
            HStack{
                VStack{
                    Button{Graphs.showAllObjects.toggle(); updateCPPtoShowAllObjects()}label:{Image(systemName: "greetingcard.fill") .imageScale(.small)}
                        .padding(.top,20)
                        .padding(.leading,40)
                    Spacer()
                }
                Spacer()
            }
            VStack{
                Spacer()
                //creates a new object
                if AnmInst.CurrentObjectViewType != 3{//no appending cross section
                    Button{appendCorrectObjType(); selectedObjectID = Graphs.MetadataOfEquations.last?.id ?? UUID()}label:{Image(systemName: "plus.diamond")} .buttonStyle(.borderedProminent)
                        .padding(.leading,1550)
                }
                HStack{
                    Spacer()
                    Button{
                        withAnimation(.easeIn(duration: 0.3), {SavingObject = true})
                        
                    }label:{Image(systemName: "externaldrive.fill.badge.plus").foregroundStyle(.black)
                    }
                    .buttonStyle(.plain)
                    Picker(selection: $selectedObjectID, label: Text("")) {
                        Group{
                            if AnmInst.CurrentObjectViewType == 0{
                                //picker of all existing objects along with their names
                                ForEach(Graphs.MetadataOfEquations){ k in
                                    Text("\(k.nameOfEqn)")
                                        .tag(k.id as UUID?)
                                }
                            } else if AnmInst.CurrentObjectViewType == 1{
                                ForEach(Graphs.rgbReflObjects){ k in
                                    Text("Color: \(k.mainEqn.nameOfEqn)")
                                        .tag(k.id as UUID?)
                                }
                            } else if AnmInst.CurrentObjectViewType == 2{
                                ForEach(Graphs.lightObjects){ k in
                                    Text("Light: \(k.mainEqn.nameOfEqn)")
                                        .tag(k.id as UUID?)
                                }
                            } else{
                                Text("Cross Section: \(Graphs.CrossSecObject.nameOfEqn)")
                            }
                        }
                        .background(Color.gray)
                        .tint(.white)
                        .clipShape(RoundedRectangle(cornerRadius: 5))
                    }
                    .frame(maxWidth: 100, maxHeight: 50)
                    //.padding(.leading, 1475)
                }
            }
            //.padding(.leading, 1475)
                if SavingObject {
                    ZStack{
                        RoundedRectangle(cornerRadius: 30.0)
                            .fill(.primary)
                        VStack{
                            TextField("New obj", text: $tempObjName)
                                .frame(maxWidth: 500)
                                .textFieldStyle(.roundedBorder)
                            Button("Save Object", action:{
                                if let currentObjIndex = $Graphs.MetadataOfEquations.firstIndex(where: {$0.id == selectedObjectID}) {
                                    Graphs.MetadataOfEquations[currentObjIndex].nameOfEqn = tempObjName
                                    SavingObject = false
                                    SaveObjectAlert = true
                                } else if let currentObjIndex = $Graphs.rgbReflObjects.firstIndex(where: {$0.id == selectedObjectID}) {
                                    Graphs.rgbReflObjects[currentObjIndex].mainEqn.nameOfEqn = tempObjName
                                    SavingObject = false
                                    SaveObjectAlert = true
                                } else if let currentObjIndex = $Graphs.lightObjects.firstIndex(where: {$0.id == selectedObjectID}){
                                    Graphs.lightObjects[currentObjIndex].mainEqn.nameOfEqn = tempObjName
                                    SavingObject = false
                                    SaveObjectAlert = true
                                } else if AnmInst.CurrentObjectViewType == 3{
                                    Graphs.CrossSecObject.nameOfEqn = tempObjName
                                    SavingObject = false
                                    SaveObjectAlert = true
                                }
                            })
                        }
                    }
                    .transition(.opacity)
                }
            
        }
        .transition(.opacity)
        .onAppear(perform: {//default id's
            if AnmInst.CurrentObjectViewType == 0{
                selectedObjectID = Graphs.MetadataOfEquations[0].id
            } else if AnmInst.CurrentObjectViewType == 1{
                selectedObjectID = Graphs.rgbReflObjects[0].id
            } else if AnmInst.CurrentObjectViewType == 2{
                selectedObjectID = Graphs.lightObjects[0].id
            }
            tempObjName = tempObjName
        })
        .onChange(of: Graphs.xRangeOfApp, perform: {_ in Swift2ObMstr.updateObjMstrInitVals()})
        .onChange(of: Graphs.yRangeOfApp, perform: {_ in
            Swift2ObMstr.updateObjMstrInitVals()
        })
        .onChange(of: Graphs.resolOfApp, perform: {_ in
            Swift2ObMstr.updateObjMstrInitVals()
        })
        .alert(isPresented: $SaveObjectAlert){
            let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == selectedObjectID})
            return Alert(title: Text(saveObjectAlertTextReturn()), message: Text("Good to go."), dismissButton: .cancel(Text("Bruh why are you telling me this")))
        }
        
    }
    private func saveObjectAlertTextReturn()->String{
        var objectName: String = ""
        if AnmInst.CurrentObjectViewType == 0{
            let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == selectedObjectID})
            objectName = currentObj?.nameOfEqn ?? "Unkown Object"
        } else if AnmInst.CurrentObjectViewType == 1{
            let currentObj = Graphs.rgbReflObjects.first(where: {$0.id == selectedObjectID})
            objectName = currentObj?.mainEqn.nameOfEqn ?? "Unkown Object"
        } else if AnmInst.CurrentObjectViewType == 2{
            let currentObj = Graphs.lightObjects.first(where: {$0.id == selectedObjectID})
            objectName = (currentObj?.mainEqn.nameOfEqn)!
        } else{
            objectName = Graphs.CrossSecObject.nameOfEqn
        }
        objectName = "Saved \(objectName)"
        return objectName
    }
    private func appendCorrectObjType(){
        let objType = AnmInst.CurrentObjectViewType
        switch objType {
        case 0:
            Graphs.MetadataOfEquations.append(EquationMetadata())
            selectedObjectID = Graphs.MetadataOfEquations.last!.id
        case 1:
            Graphs.rgbReflObjects.append(RGBreflMetadata())
            selectedObjectID = Graphs.rgbReflObjects.last!.id
        case 2:
            Graphs.lightObjects.append(lightInfMetadata())
            selectedObjectID = Graphs.lightObjects.last!.id
        default:
            break;
        }
    }
    private func updateCPPtoShowAllObjects(){
        if Graphs.showAllObjects{//toggled by button
            Swift2ObMstr.updateCPPwithAllObjects()
        } else {
            let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == selectedObjectID})
            if currentObj != nil{
                let arrayOfOneObj: Array<UUID> = [currentObj!.id]
                Swift2ObMstr.updateObjectsToDraw(ObjectIdArray: arrayOfOneObj, AnmInstMode: AnmInst.CurrentObjectViewType)
            }
        }
    }
    
}

struct MultiGraphManager_Previews: PreviewProvider {
    static var previews: some View {
        ObjectSwitcherView()
    }
}
