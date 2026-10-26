//
//  File.swift
//  ArduiMatorNew
//
//  Created by AnshuDiya on 9/23/26.
//

import Foundation
import SwiftUI
import MetalKit
import Metal
import Cocoa
import QuartzCore

//all the info needed to output coords for a singular object
struct ObjectInfo{
    var ObjectLevel: Int32 //Int32 for compatability
    var ObjectPosition: Int32
    var PrimitivesEquations: [CChar] = [] //null terminated char* array
    var numberOfPrimitives: Int32 //to be set by the parser function
        var MainEqnConstModsObjCoords: Array<Int32>
        var SliderValsMainEqn: Array<Float>
    var ObjectRotEqn: [CChar] = []
        var rotModObjCoords: Array<Int32>
        var SliderValsRotEqn: [Float]
        var rotEqnMode: Int32
            var IndiceUserSelectedRot: Int32 = 0//indice selected by user in case of special rot mode
    var ObjectTranslEqn: [CChar] = []
        var translModObjCoords: Array<Int32>
        var SliderValsTranslEqn: [Float]
        var translEqnMode: Int32
            var IndiceUserSelectedTransl: Int32 = 0//indice selected by user in case of special transl mode
}
struct ColorObjectInfo{
        var ColorObjectPosition: Int32//color objects only need a position
    var RedEqn: [CChar] = []
        var GreenEqn: [CChar] = []
        var BlueEqn: [CChar] = []
        var ReflEqn: [CChar] = []
    var RedModObjs: [Int32]
        var GreenModObjs: [Int32]
        var BlueModObjs:[Int32]
        var ReflModObjs: [Int32]
    var RedSldrs: [Float]
        var GreenSldrs: [Float]
        var BlueSldrs: [Float]
        var ReflSldrs: [Float]
}
struct LightObjectInfo{
        var LightObjectPosition: Int32
    var XYeqn: [CChar] = []
        var ZDirEqn: [CChar] = []
        var IntensityEqn: [CChar] = []
    var XYeqnModObjs: [Int32]
        var ZDirModObjs: [Int32]
        var IntensityModObjs: [Int32]
    var XYeqnSldrs: [Float]
        var ZDirSldrs: [Float]
        var IntensitySldrs: [Float]
    
}
struct CrossSecObjectInfo: Equatable{
    var CrossSecEqn: [CChar]
    var CrossSecModObjs: [Int32]
    var CrossSecSldrs: [Float]
}

class Animation: ObservableObject{
    static var AnimInstance = Animation()
    @ObservedObject var Swift2ObMstr = SwiftToObjMstr.CoordsAnimInstance
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    var vertexBufferPtr: UnsafeMutableRawPointer? = nil
        var colorBufferPtr: UnsafeMutableRawPointer? = nil
        var lightBufferPtr: UnsafeMutableRawPointer? = nil
        var heightLookupBufferPtr: UnsafeMutableRawPointer? = nil
                var offsetsBufferPtr: UnsafeMutableRawPointer? = nil
    var pointsTerDraw = 0
    
    @Published var CurrentObjectViewType = 0//0 for physical eqns, 1 for rgbRefl, 2 for lightObjs
    
    func setInitVals(initXrange: Float, initYrange: Float, initResol: Float){
        let FloatVertexBufferPtr: UnsafeMutablePointer<Float> = vertexBufferPtr!.assumingMemoryBound(to: Float.self)
            let Float4ColorVertexBufferPtr: UnsafeMutablePointer<Float> = colorBufferPtr!.assumingMemoryBound(to: Float.self)
            let Float4LightBufferPtr: UnsafeMutablePointer<Float> = lightBufferPtr!.assumingMemoryBound(to: Float.self)
            let heightLookupPtr: UnsafeMutablePointer<Float> = heightLookupBufferPtr!.assumingMemoryBound(to: Float.self)
                    let DaOffsetBufferPtr: UnsafeMutablePointer<Float> = offsetsBufferPtr!.assumingMemoryBound(to: Float.self)
        Swift2ObMstr.setInitObjMstrVals(xRange: initXrange, yRange: initYrange, resol: initResol, animVal: 0.0, metalBufferPtr: FloatVertexBufferPtr, clrBufferPtr: Float4ColorVertexBufferPtr, lightBufferPtr: Float4LightBufferPtr, heightLookupPointer: heightLookupPtr,offstBufferPointer: DaOffsetBufferPtr)
    }
}

    class Renderer: NSObject, MTKViewDelegate {
        var device: MTLDevice!;
        var commandQueue: MTLCommandQueue!;
        var pipelineState: MTLRenderPipelineState!;
        var vertexBuffer: MTLBuffer!
            var colorBuffer: MTLBuffer!//packed_float4 array for color equation. there must be a color point for each corresponding vertice
            var lightBuffer: MTLBuffer!//packed_float4 format with the first index corresponding to how many light objects there are and the corresponding indices xyz & intensity
            var heightLookupBuffer: MTLBuffer!//float1 format since only contains y-coords of heights to look up with a set range of (0, sqrt(8))
                    var offsetsBuffer: MTLBuffer!
        var AnmInst = Animation.AnimInstance //animation instance
        var depthDescriptor: MTLDepthStencilDescriptor!
            var depthStencilState: MTLDepthStencilState!
        
        @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
        @ObservedObject var Swift2ObMstr = SwiftToObjMstr.CoordsAnimInstance
        
        var pointsToDraw: UInt32
        init(pointsTerDraw: UInt32) {
            pointsToDraw = pointsTerDraw //intializes number of points to draw a.k.a. the portion of the 100mb buffer that has actually been filled
            super.init()
            device = MTLCreateSystemDefaultDevice();
            commandQueue = device.makeCommandQueue();
            
            
            let library = device.makeDefaultLibrary();
            let vertexFunction = library?.makeFunction(name: "basic_vertex");
            let fragmentFunction = library?.makeFunction(name: "basicFragment");
            let pipelineStateScribe = MTLRenderPipelineDescriptor();
            pipelineStateScribe.vertexFunction = vertexFunction;
            pipelineStateScribe.fragmentFunction = fragmentFunction;
            pipelineStateScribe.colorAttachments[0].pixelFormat = .bgra8Unorm;
            pipelineStateScribe.depthAttachmentPixelFormat = .depth32Float
            /*print("Pipelien state format 1: ", pipelineStateScribe.colorAttachments[0].pixelFormat.rawValue)
            print("Pipeline State format 2: ", pipelineStateScribe.depthAttachmentPixelFormat.rawValue)*/
            pipelineState = try? device.makeRenderPipelineState(descriptor: pipelineStateScribe);
            
            depthDescriptor = MTLDepthStencilDescriptor()
            depthDescriptor.depthCompareFunction = .lessEqual//.less
            depthDescriptor.isDepthWriteEnabled = true//true
                depthStencilState = device.makeDepthStencilState(descriptor: depthDescriptor)
            
            vertexBuffer = device.makeBuffer(length: 100_000_00, options: .storageModeShared)//initializes 10mb buffer for c++ to write into
                colorBuffer = device.makeBuffer(length: 100_000_00, options: .storageModeShared)
                lightBuffer = device.makeBuffer(length: 100_000_0, options: .storageModeShared)//1mb buffer for light vertices
                heightLookupBuffer = device.makeBuffer(length: 100_000_0,options: .storageModeShared)//1mb buffer for height lookup table
                        offsetsBuffer = device.makeBuffer(length: 100_000_00, options: .storageModeShared)//for the offset points
            if vertexBuffer != nil && colorBuffer != nil && lightBuffer != nil && heightLookupBuffer != nil{
                AnmInst.vertexBufferPtr = vertexBuffer.contents()
                    AnmInst.colorBufferPtr = colorBuffer.contents()
                    AnmInst.lightBufferPtr = lightBuffer.contents()
                    AnmInst.heightLookupBufferPtr = heightLookupBuffer.contents()
                            AnmInst.offsetsBufferPtr = offsetsBuffer.contents()
                Swift2ObMstr.vertexBufferPtr = vertexBuffer.contents()
                    Swift2ObMstr.clrBffrPtr = colorBuffer.contents()
                    Swift2ObMstr.lightBffrPtr = lightBuffer.contents()
                    Swift2ObMstr.heightLookupBffrPtr = heightLookupBuffer.contents()
                            Swift2ObMstr.offsetBffrPtr = offsetsBuffer.contents()
                let xRange = Graphs.xRangeOfApp; let yRange = Graphs.yRangeOfApp; let resolOfApp = Graphs.resolOfApp;
                AnmInst.setInitVals(initXrange: xRange, initYrange: yRange, initResol: resolOfApp) //INITIALIZES METAL BUFFER POINTER TO NON-NIL VALUE!!!
            }
            
        }
        func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
            //to be changed later if needed
        }
        func draw(in view: MTKView){
            if vertexBuffer != nil && colorBuffer != nil && lightBuffer != nil && heightLookupBuffer != nil{//does not run if any buffers failed to allocate
            pointsToDraw = UInt32(returnPointsTerDrawAlias())
                if pointsToDraw <= 2 {pointsToDraw = 2}
                guard let drawable = view.currentDrawable else {return}
                guard let renderPassDescriptor = view.currentRenderPassDescriptor else {return;}//MTLRenderPassDescriptor();
                renderPassDescriptor.colorAttachments[0].texture = drawable.texture
                renderPassDescriptor.colorAttachments[0].loadAction = .clear
                renderPassDescriptor.colorAttachments[0].clearColor = //MTLClearColor(red:247/255,green:247/255,blue:255/255,alpha: 0.5)
                MTLClearColor(red: 0.0/255.0, green: 0.0/255.0,
                              blue: 0.0/255.0, alpha: 0.0)
                
                let commandBuffer = commandQueue.makeCommandBuffer();
                
                if Graphs.hasBeenClickedAtleastOnce{
                    let animMult = Float(Graphs.animationMultiplier); let duration = Graphs.durationOfAnimation;
                    logChronoStart();
                    Swift2ObMstr.objMstrCalculator(animMutliplier: animMult, duration: duration)
                }
                let renderEncoder = commandBuffer?.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
                renderEncoder?.setRenderPipelineState(pipelineState);
                renderEncoder?.setDepthStencilState(depthStencilState)
                renderEncoder?.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
                renderEncoder?.setVertexBuffer(offsetsBuffer, offset: 0, index: 4)
                renderEncoder?.setFragmentBuffer(lightBuffer, offset: 0, index: 1)
                renderEncoder?.setFragmentBuffer(heightLookupBuffer, offset: 0, index: 2)
                renderEncoder?.setVertexBuffer(colorBuffer, offset: 0, index: 3)
                var actualPointsToDraw: UInt32 = (pointsToDraw*6)-5
                renderEncoder?.setVertexBytes(&actualPointsToDraw, length: MemoryLayout<UInt32>.size, index: 5)
                renderEncoder?.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: Int(pointsToDraw*6)-6);//pointstoDraw*2 since two triangles per point
                /*print("View format: ",view.colorPixelFormat.rawValue)
                print("Drawable format: ", drawable.texture.pixelFormat.rawValue)
                print("Depth stencil format:", view.depthStencilPixelFormat.rawValue)
                print("Drawable size:",drawable.texture.width,drawable.texture.height)*/
                //print("Pipeline state validity", pipelineState != nil)
                renderEncoder?.endEncoding()
                commandBuffer?.present(drawable);
                commandBuffer?.commit();
                logChronoStopAndPrint();
            }
        }
    }
    
    struct MetalView: NSViewRepresentable{
        
        func makeCoordinator() -> Renderer {
            Renderer(pointsTerDraw: returnPointsTerDrawAlias())
        }
        func makeNSView(context: Context) -> MTKView {
            let view = MTKView();
            view.device = context.coordinator.device;
            view.delegate = context.coordinator;
            view.colorPixelFormat = .bgra8Unorm;
            view.depthStencilPixelFormat = .depth32Float
            view.framebufferOnly = false;
            view.wantsLayer = true
            view.layer?.backgroundColor = NSColor.clear.cgColor
            view.layer?.isOpaque = false
            if let metalLayer = view.layer as? CAMetalLayer {
                    metalLayer.isOpaque = false
                }
            return view;
        }
        func updateNSView(_ nsView: MTKView, context: Context) {
            //to be changed later when updates from swift trigger metal changes
        }
    }

//converts an array of EquationMetadata() structs to ObjectInfo C++-Friendly structs for further processing
class SwiftToObjMstr: ObservableObject{
    static let CoordsAnimInstance = SwiftToObjMstr()
    
    @ObservedObject var Graphs = GraphsMetadata.GraphsMtdt
    var vertexBufferPtr: UnsafeMutableRawPointer? = nil
            var clrBffrPtr: UnsafeMutableRawPointer? = nil
            var lightBffrPtr: UnsafeMutableRawPointer? = nil
            var heightLookupBffrPtr: UnsafeMutableRawPointer? = nil
                    var offsetBffrPtr: UnsafeMutableRawPointer? = nil
        var DrawObjsLvlAndPosCoords: Array<Int32> = []//to be filled with level and position of objects to be drawn if only certain objects are selected to be drawn
    
    private var ObjInfoArray: Array<ObjectInfo> = [] //to be filled with ObjectInfo structs that physically modify the canvas
            private var ColorObjInfoArray: Array<ColorObjectInfo> = [] //to be filled with ObjectInfo structs that modify color
            private var LightObjInfoArray: Array<LightObjectInfo> = [] //to be filled with ObjectInfo structs that act as point lights
    private var CrossSecObj: [CrossSecObjectInfo] = [] //to be filled with the cross-section object.
    //CrossSecObjInfo array only contains a single cross sec object. it is array for unwrapping purposes
    
        var stopAnimation = true //to be set to true by the CLASS's FUNCTION!!! and FALSE by clicking on the BUTTON!
    var stopStaticAnimation = false //to be set to false/reset by the CLASS's FUNCTION andddd SwiftUI when hvoering over!
    var MetalHasBeenCleared = false
    
    
    //append all existing objects to swift-cpp class array/this class to array ObjInfoArray.
    func updateCPPwithAllObjects(){
        var arrayOfAllPhysicalObjs: Array<EquationMetadata> = []; for obj in Graphs.MetadataOfEquations{arrayOfAllPhysicalObjs.append(obj)}
        let arrayOfAllLightObjs: [lightInfMetadata] = Graphs.lightObjects;
        let CrossSecObj: CrossSecMetadata = Graphs.CrossSecObject
        let arrayOfAllColorObjs: [RGBreflMetadata] = Graphs.rgbReflObjects;
        updateCppObjInfo(MtdtArray: arrayOfAllPhysicalObjs, ClrObjArray: arrayOfAllColorObjs, LightObjArray: arrayOfAllLightObjs, DerCrossSecObj: CrossSecObj); Graphs.drawTheseObjects = [];DrawObjsLvlAndPosCoords = [];}//clearing draw these objects so a future function can check if it is clear to know that all objects need to be drawn.
    
    //update the objects that actually show up on screen versus the function above which just ensures all objects are calculated
    func updateObjectsToDraw(ObjectIdArray: Array<UUID>, AnmInstMode: Int){//parameter is the array of UUIDs of objects to be drawn!
        Graphs.drawTheseObjects = []//clear previous objects to be drawn
        DrawObjsLvlAndPosCoords = []
        
        for derID in ObjectIdArray{
                Graphs.drawTheseObjects.append(Graphs.MetadataOfEquations.first(where: {$0.id == derID}))//only append the objects that need to be drawn
        }
        for derObj in Graphs.drawTheseObjects{
            let DaObjLvl = (Graphs.MetadataOfEquations.first(where: {$0.id == derObj?.id}))!.objLvl
            var objPos = 1
            for currentObj in Graphs.MetadataOfEquations{
                if(currentObj.id == derObj?.id){break;}//break when reaching current object.
                if currentObj.id != derObj?.id && currentObj.objLvl == derObj?.objLvl {objPos += 1}//if an object of the same lvl but of different ID is found increment object level
            }
            DrawObjsLvlAndPosCoords.append(Int32(DaObjLvl))
            DrawObjsLvlAndPosCoords.append(Int32(objPos))
        }
    }
    
    
    private func ObjInfoArrayFiller(MtdtArray: Array<EquationMetadata>, rgbMtdtArray: Array<RGBreflMetadata>, lightMtdtArray: Array<lightInfMetadata>, CrossSectionObjectMetadata: CrossSecMetadata){ //give it all the objects to be animated and it fills the object info array with correct objects
        ObjInfoArray = [] //clears array of existing objects
        CrossSecObj = []
        ColorObjInfoArray = []
        LightObjInfoArray = []
        for object in MtdtArray{
            EqnMtdtToObjInfoStruct(with: object)
        }
                crossSecMtdtToCrossSecObjectInfoStruct(crossSecMtdt: CrossSectionObjectMetadata)
        for colorObject in rgbMtdtArray{
            rgbMtdtToColorObjInfoStruct(rgbMtdt: colorObject)
        }
        for lightObject in lightMtdtArray{
            lightMtdtToLightObjectInfoStruct(lightMtdt: lightObject)
        }
    }
    private func EqnMtdtToObjInfoStruct(with Mtdt: EquationMetadata){ //one single EqnMtdt to an object info struct inside the object info array. appends an ObjectInfo to the array.
        let objLvl = Int32(Mtdt.objLvl)
        var tempObjPos: Int32 = 1
        for objects in Graphs.MetadataOfEquations {
            if(objects.id == Mtdt.id){break;}//if reaching this object then break the loop and stop incrementing object position
            if (objects.objLvl == objLvl) {tempObjPos += 1} // for every object with the same level as the one currently being checked increment object position by one
        }
        let objPos: Int32 = tempObjPos
        var MainEqn: [CChar] = []//Array(Mtdt.EquationString[0].utf8CString)
        for primitive in Mtdt.EquationString{
            MainEqn.append(contentsOf:(Array(primitive.utf8CString)))//appends the strings of all existing primitives to main Equation. SINCE MAIN EQUATION IS NOT FILLED WITH SEPERATE STRINGS BUT RATHER NULL-TERMINATED STRINGS ALL FLATTENED INTO A SINGLE GROUP OBJCPP FUNCTION MUST ACCOMODATE THIS!!!
        }
        let numberOfPrimitives: Int32 = Int32(Mtdt.EquationString.count)
        var MainEqnObjCoords: Array<Int32> = []
        for UUID in Mtdt.EquationModifierObjectsMainStr{
            let currentObjRfrncd = Graphs.MetadataOfEquations.first(where: {$0.id == UUID})
            let ObjRfrncdLvl = currentObjRfrncd?.objLvl
            var tempPos = 0
            for objects in Graphs.MetadataOfEquations {
                if (objects.objLvl == ObjRfrncdLvl) {tempPos += 1} // for every object with the same level as the one currently being checked increment object position by one. break when reaching this object
                if(objects.id == currentObjRfrncd?.id){break;}
            }
            MainEqnObjCoords.append(Int32(ObjRfrncdLvl!))
            MainEqnObjCoords.append(Int32(tempPos)) //appends Level,Pos of all the object coords this is modified by
        }
        let rotEqn: [CChar] = Array(Mtdt.rotEqn.utf8CString)
        var rotEqObjCoords: Array<Int32> = []
        for UUID in Mtdt.rotEqnModObjs{
            let currentObjRfrncd = Graphs.MetadataOfEquations.first(where: {$0.id == UUID})
            let ObjRfrncdLvl = currentObjRfrncd?.objLvl
            var tempPos = 0
            for objects in Graphs.MetadataOfEquations {
                if (objects.objLvl == ObjRfrncdLvl) {tempPos += 1} // for every object with the same level as the one currently being checked increment object position by one
            }
            rotEqObjCoords.append(Int32(ObjRfrncdLvl!))
            rotEqObjCoords.append(Int32(tempPos)) //appends Level,Pos of all the object coords this is modified by
        }
        let translEqn: [CChar] = Array(Mtdt.translEqn.utf8CString)
        var translEqnModCoords: [Int32] = []
        for UUID in Mtdt.translEqnModObjs{
            let currentObjRfrncd = Graphs.MetadataOfEquations.first(where: {$0.id == UUID})
            let ObjRfrncdLvl = currentObjRfrncd?.objLvl
            var tempPos = 0
            for objects in Graphs.MetadataOfEquations {
                if (objects.objLvl == ObjRfrncdLvl) {tempPos += 1} // for every object with the same level as the one currently being checked increment object position by one
            }
            translEqnModCoords.append(Int32(ObjRfrncdLvl!))
            translEqnModCoords.append(Int32(tempPos)) //appends Level,Pos of all the object coords this is modified by
        }
        
        let SliderValsMain = Mtdt.SliderValuesMainStr[0] ?? []
        let SliderValsRot = Mtdt.SliderValuesRotEqn
        let SliderValsTransl = Mtdt.SliderValuesTranslEqn//at index 0 cuz if having modifiers cna only be one view
        
        let SelectedRotEqnMode: Int32 = Int32(Mtdt.rotEqnMode)
        let SelectedTranslEqnMode: Int32 = Int32(Mtdt.translEqnMode)
        
        var newlyMadeObj = ObjectInfo(ObjectLevel: objLvl, ObjectPosition: objPos, PrimitivesEquations: MainEqn, numberOfPrimitives: numberOfPrimitives, MainEqnConstModsObjCoords: MainEqnObjCoords, SliderValsMainEqn: SliderValsMain, ObjectRotEqn: rotEqn, rotModObjCoords: rotEqObjCoords, SliderValsRotEqn: SliderValsRot, rotEqnMode: SelectedRotEqnMode, ObjectTranslEqn: translEqn, translModObjCoords: translEqnModCoords, SliderValsTranslEqn: SliderValsTransl, translEqnMode: SelectedTranslEqnMode)
        var DerRotIndice = Int32(Mtdt.SelectedRotIndice);
        var DerTranslIndice = Int32(Mtdt.SelectedTranslIndice);
        DerRotIndice *= 2;/*convert to indice that only points to x-values!*/DerTranslIndice *= 2;//convert to indice that only points to x-values!
        newlyMadeObj.IndiceUserSelectedRot = DerRotIndice; newlyMadeObj.IndiceUserSelectedTransl = DerTranslIndice;//update indice the user selected!
        
        //apends a new Object Info object with all the right initializers and updated indice selections
        ObjInfoArray.append(newlyMadeObj)
    }
            private func rgbMtdtToColorObjInfoStruct(rgbMtdt: RGBreflMetadata){//one rgbMtdt struct to appending it to its array
                var TempObjectPosition: Int32 = 1;
                for rgbObject in Graphs.rgbReflObjects{
                    if rgbObject.id == rgbMtdt.id{break;}
                    TempObjectPosition += 1;//increment object position by 1 for each object before this one
                }
                
                var RedEqn: [CChar] = []; RedEqn.append(contentsOf:(Array(rgbMtdt.RGBrefl[0].DerString.utf8CString)))//converts red, green, blue, and refl equations to C-style strings
                var GreenEqn: [CChar] = []; GreenEqn.append(contentsOf: (Array(rgbMtdt.RGBrefl[1].DerString.utf8CString)))
                var BlueEqn: [CChar] = []; BlueEqn.append(contentsOf: (Array(rgbMtdt.RGBrefl[2].DerString.utf8CString)))
                var ReflEqn: [CChar] = []; ReflEqn.append(contentsOf: (Array(rgbMtdt.RGBrefl[3].DerString.utf8CString)))
                
                var RedModObjs: [Int32] = []; var GreenModObjs: [Int32] = []; var BlueModObjs: [Int32] = []; var ReflModObjs: [Int32] = [];
                for object in rgbMtdt.RGBrefl[0].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    RedModObjs.append(Int32(currentObj!.objLvl))
                    RedModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                for object in rgbMtdt.RGBrefl[1].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    GreenModObjs.append(Int32(currentObj!.objLvl))
                    GreenModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                for object in rgbMtdt.RGBrefl[2].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    BlueModObjs.append(Int32(currentObj!.objLvl))
                    BlueModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                for object in rgbMtdt.RGBrefl[3].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    ReflModObjs.append(Int32(currentObj!.objLvl))
                    ReflModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                
                var RedSldrs: [Float] = []; var BlueSldrs: [Float] = []; var GreenSldrs: [Float] = []; var ReflSldrs: [Float] = []
                for sliderVal in rgbMtdt.RGBrefl[0].Sldrs{
                    RedSldrs.append(sliderVal)
                }
                for sliderVal in rgbMtdt.RGBrefl[1].Sldrs{
                    GreenSldrs.append(sliderVal)
                }
                for sliderVal in rgbMtdt.RGBrefl[2].Sldrs{
                    BlueSldrs.append(sliderVal)
                }
                for sliderVal in rgbMtdt.RGBrefl[3].Sldrs{
                    ReflSldrs.append(sliderVal)
                }
                let colorObjPos: Int32 = Int32(ColorObjInfoArray.count)//the position of the color object to be appended
                //appends a color object
                ColorObjInfoArray.append(ColorObjectInfo(ColorObjectPosition: colorObjPos, RedEqn: RedEqn, GreenEqn: GreenEqn, BlueEqn: BlueEqn, ReflEqn: ReflEqn, RedModObjs: RedModObjs, GreenModObjs: GreenModObjs, BlueModObjs: BlueModObjs, ReflModObjs: ReflModObjs, RedSldrs: RedSldrs, GreenSldrs: GreenSldrs, BlueSldrs: BlueSldrs, ReflSldrs: ReflSldrs))
            }
            private func lightMtdtToLightObjectInfoStruct(lightMtdt: lightInfMetadata){
                var TempObjectPosition: Int32 = 1;
                for lightObject in Graphs.lightObjects{
                    if lightObject.id == lightMtdt.id{break;}
                    TempObjectPosition += 1;//increment object position by 1 for each object before this one
                }
                
                var XYeqn: [CChar] = []; XYeqn.append(contentsOf:(Array(lightMtdt.mainEqn.EquationString[0].utf8CString)))//converts red, green, blue, and refl equations to C-style strings
                var ZDirEqn: [CChar] = []; ZDirEqn.append(contentsOf: (Array(lightMtdt.ZDirAndIntesnity[0].DerString.utf8CString)))
                var IntensityEqn: [CChar] = []; IntensityEqn.append(contentsOf: (Array(lightMtdt.ZDirAndIntesnity[1].DerString.utf8CString)))
                
                var XYmodObjs: [Int32] = []; var ZDirModObjs: [Int32] = []; var IntensityModObjs: [Int32] = [];
                for object in lightMtdt.mainEqn.EquationModifierObjectsMainStr{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    XYmodObjs.append(Int32(currentObj!.objLvl))
                    XYmodObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                for object in lightMtdt.ZDirAndIntesnity[0].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    ZDirModObjs.append(Int32(currentObj!.objLvl))
                    ZDirModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                for object in lightMtdt.ZDirAndIntesnity[1].ModObjs{
                    let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
                    var tempModObjPos: Int32 = 0
                    for ModObject in Graphs.MetadataOfEquations{//conditional triggers when a physical object id matches the modifier object id
                        if ModObject.id == currentObj!.id{break;}else{tempModObjPos += 1;}//increments the position counter of the selected modifier object
                    }
                    IntensityModObjs.append(Int32(currentObj!.objLvl))
                    IntensityModObjs.append(tempModObjPos)//appends the object level of the modifier object followed by the calculated position of it
                }
                
                var XYsldrs: [Float] = []; var ZDirSldrs: [Float] = []; var IntensitySldrs: [Float] = [];
                let SliderValsMainStr: [Float] = lightMtdt.mainEqn.SliderValuesMainStr[0] ?? []
                for sliderVal in SliderValsMainStr{
                    XYsldrs.append(sliderVal)
                }
                for sliderVal in lightMtdt.ZDirAndIntesnity[0].Sldrs{
                    ZDirSldrs.append(sliderVal)
                }
                for sliderVal in lightMtdt.ZDirAndIntesnity[1].Sldrs{
                    IntensitySldrs.append(sliderVal)
                }
                let LightObjPos: Int32 = Int32(LightObjInfoArray.count)//the position of the color object to be appended
                
                LightObjInfoArray.append(LightObjectInfo(LightObjectPosition: LightObjPos, XYeqn: XYeqn, ZDirEqn: ZDirEqn, IntensityEqn: IntensityEqn, XYeqnModObjs: XYmodObjs, ZDirModObjs: ZDirModObjs, IntensityModObjs: IntensityModObjs, XYeqnSldrs: XYsldrs, ZDirSldrs: ZDirSldrs, IntensitySldrs: IntensitySldrs))
            }
    private func crossSecMtdtToCrossSecObjectInfoStruct(crossSecMtdt: CrossSecMetadata){
        var CrossSecEqn: [CChar] = []
        CrossSecEqn.append(contentsOf: (Array(crossSecMtdt.CrossSecString.utf8CString)))
        
        var CrossSecModObjs: [Int32] = []
        for object in crossSecMtdt.EquationModifierObjectsMainStr{
            let currentObj = Graphs.MetadataOfEquations.first(where: {$0.id == object})
            var tempObjPos: Int32 = 1
            for DaObject in Graphs.MetadataOfEquations{
                if DaObject.id == currentObj!.id{break;}else{tempObjPos += 1;}
            }
            CrossSecModObjs.append(Int32(currentObj!.objLvl))
            CrossSecModObjs.append(tempObjPos)
        }
            
            var CrossSecSldrs: [Float] = crossSecMtdt.SliderValues
            
            CrossSecObj.append(CrossSecObjectInfo(CrossSecEqn: CrossSecEqn, CrossSecModObjs: CrossSecModObjs, CrossSecSldrs: CrossSecSldrs))
    }
    
    func updateObjMstrInitVals(){
        let xRange = Graphs.xRangeOfApp; let yRange = Graphs.yRangeOfApp; let resol = Graphs.resolOfApp;
        let FloatVertexBufferPtr: UnsafeMutablePointer<Float> = vertexBufferPtr!.assumingMemoryBound(to: Float.self)
            let Float4ClrBffrPtr: UnsafeMutablePointer<Float> = clrBffrPtr!.assumingMemoryBound(to: Float.self)
            let Float4LightBffrPtr: UnsafeMutablePointer<Float> = lightBffrPtr!.assumingMemoryBound(to: Float.self)
            let heightLookupPtr: UnsafeMutablePointer<Float> = heightLookupBffrPtr!.assumingMemoryBound(to: Float.self)
                    let DerOffsetBffrPtr: UnsafeMutablePointer<Float> = offsetBffrPtr!.assumingMemoryBound(to: Float.self)
        setInitObjMstrVals(xRange: xRange, yRange: yRange, resol: resol, animVal: 0.0, metalBufferPtr: FloatVertexBufferPtr, clrBufferPtr: Float4ClrBffrPtr, lightBufferPtr: Float4LightBffrPtr, heightLookupPointer: heightLookupPtr, offstBufferPointer: DerOffsetBffrPtr)
    }
    
    
    func updateCppObjInfo(MtdtArray: Array<EquationMetadata>, ClrObjArray: [RGBreflMetadata], LightObjArray: [lightInfMetadata], DerCrossSecObj: CrossSecMetadata){ //global function to update all cpp objects
        ObjInfoArray = [] //clears/resets both arrays of data
            //for now, clears whole object master but this will probably cause performance issues donw the road so if it does add logic to only clear required objects:
            resetObjMstrAlias()
        updateObjMstrInitVals()//updates with new xRange yRange and resol
        ObjInfoArrayFiller(MtdtArray: MtdtArray, rgbMtdtArray: ClrObjArray, lightMtdtArray: LightObjArray, CrossSectionObjectMetadata: DerCrossSecObj)
        for info in ObjInfoArray {
            let objLvl: Int32 = info.ObjectLevel
            let objPos: Int32 = info.ObjectPosition
            info.PrimitivesEquations.withUnsafeBufferPointer({pointer in
                let baseAdress = pointer.baseAddress
                let numOfPrim: Int32 = info.numberOfPrimitives
                primitiveAdder(baseAdress, numOfPrim, objLvl, objPos) //single primitive adder objcpp function for all the primtiives
            })//primitives
            info.MainEqnConstModsObjCoords.withUnsafeBufferPointer({MainObjPointer in
                let ModObjCoordsPtr = MainObjPointer.baseAddress
                let numberOfObjects: Int32 = Int32((info.MainEqnConstModsObjCoords.count/2)) //number of objects. .count/2 since level,pos (2 slots) per object
                info.SliderValsMainEqn.withUnsafeBufferPointer({derPointer in
                    let SldrFltPtr = derPointer.baseAddress //base address of main eqn slider values
                    let SldrCnt: Int32 = Int32(info.SliderValsMainEqn.count) //number of sliders
                    mainEqnObjectModifiersAlias(objLvl, objPos, SldrFltPtr, SldrCnt, ModObjCoordsPtr, numberOfObjects) //calls dependency function for main quation
                })
            })//main eqn dependencies
            info.ObjectRotEqn.withUnsafeBufferPointer({rotEqnPtr in
                let rotEqnBaseAdress = rotEqnPtr.baseAddress
                info.rotModObjCoords.withUnsafeBufferPointer({modObjPtr in
                    let rotModObjBaseAdrss = modObjPtr.baseAddress
                    let numberOfModObj = Int32(info.rotModObjCoords.count/2)
                    info.SliderValsRotEqn.withUnsafeBufferPointer({SldrPtr in
                        let rotSldrPtrBaseAddress = SldrPtr.baseAddress
                        let numberOfSldrs = Int32(info.SliderValsRotEqn.count)
                        let indiceThatsSelected = Int32(info.IndiceUserSelectedRot)
                        let rotEqnMode = Int32(info.rotEqnMode)
                        setObjectRotationDependencyAlias(rotEqnBaseAdress, objLvl, objPos, rotModObjBaseAdrss, numberOfModObj, rotSldrPtrBaseAddress, numberOfSldrs, rotEqnMode, indiceThatsSelected)
                    })
                })
            }) //rotation equation and dependencies
            info.ObjectTranslEqn.withUnsafeBufferPointer({eqnPtr in
                let translEqnBaseAdress = eqnPtr.baseAddress
                info.SliderValsTranslEqn.withUnsafeBufferPointer({SldrPtr in
                    let SldrPtrBaseAdress = SldrPtr.baseAddress
                    let numOfSldrs = Int32(info.SliderValsTranslEqn.count)
                    info.translModObjCoords.withUnsafeBufferPointer({modObjPtr in
                        let modObjBaseAdrss = modObjPtr.baseAddress
                        let numberOfModObj = Int32(info.translModObjCoords.count/2)
                        let indiceThatsSelected = Int32(info.IndiceUserSelectedTransl)
                        let translEqnMode = Int32(info.translEqnMode)
                        setObjectTranslationDependencyAlias(translEqnBaseAdress, objLvl, objPos, modObjBaseAdrss, numberOfModObj, SldrPtrBaseAdress, numOfSldrs, translEqnMode, indiceThatsSelected)
                    })
                })
            })//translation equation and dependencies
        }
        
        //Cross-sec
        for daCrossSecObj in CrossSecObj{
            daCrossSecObj.CrossSecEqn.withUnsafeBufferPointer({TempCrossSecEqnPtr in
                let CrossSecEqnPtr = TempCrossSecEqnPtr.baseAddress
                let yRangeVal: Float = Graphs.CrossSecObject.yRange
                let resolVal: Float = Graphs.CrossSecObject.resol
                setCrossSectionObjectAlias(CrossSecEqnPtr, yRangeVal, resolVal)
                    daCrossSecObj.CrossSecSldrs.withUnsafeBufferPointer({TempSldrPtr in
                        let SldrPtr = TempSldrPtr.baseAddress
                        let numSldrs: Int32 = Int32(daCrossSecObj.CrossSecSldrs.count)
                            daCrossSecObj.CrossSecModObjs.withUnsafeBufferPointer({TempModObjsPtr in
                                let ModObjsPtr = TempModObjsPtr.baseAddress
                                let ModObjCnt: Int32 = Int32(daCrossSecObj.CrossSecModObjs.count)
                                    setCrossSectionObjDependencyAlias(SldrPtr, numSldrs, ModObjsPtr, ModObjCnt)
                            })
                    })
            })
        }
        //Cross-sec
        
        //color
        for colorObj in ColorObjInfoArray{
            colorObj.RedEqn.withUnsafeBufferPointer({TempRedEqnPtr in
                colorObj.GreenEqn.withUnsafeBufferPointer({TempGreenEqnPtr in
                    colorObj.BlueEqn.withUnsafeBufferPointer({TempBlueEqnPtr in
                            colorObj.ReflEqn.withUnsafeBufferPointer({TempReflEqnPtr in
                                let RedEqnPtr = TempRedEqnPtr.baseAddress
                                let GreenEqnPtr = TempGreenEqnPtr.baseAddress
                                let BlueEqnPtr = TempBlueEqnPtr.baseAddress
                                let ReflEqnPtr = TempReflEqnPtr.baseAddress
                                    addClrObjectAlias(RedEqnPtr, GreenEqnPtr, BlueEqnPtr, ReflEqnPtr)
                                
                                withExtendedLifetime((TempRedEqnPtr,TempGreenEqnPtr,TempBlueEqnPtr,TempReflEqnPtr)){}
                            })
                    })
                })
            })
            colorObj.RedSldrs.withUnsafeBufferPointer({TempRedSldrPtr in
                let RedSldrPtr = TempRedSldrPtr.baseAddress
                let numRedSldrs = Int32(colorObj.RedSldrs.count)
                colorObj.GreenSldrs.withUnsafeBufferPointer({TempGreenSldrPtr in
                    let GreenSldrPtr = TempGreenSldrPtr.baseAddress
                    let numGreenSldrs = Int32(colorObj.GreenSldrs.count)
                    colorObj.BlueSldrs.withUnsafeBufferPointer({TempBlueSldrPtr in
                        let BlueSldrPtr = TempBlueSldrPtr.baseAddress
                        let numBlueSldrs = Int32(colorObj.BlueSldrs.count)
                        colorObj.ReflSldrs.withUnsafeBufferPointer({TempReflSldrPtr in
                            let ReflSldrPtr = TempReflSldrPtr.baseAddress
                            let numReflSldrs = Int32(colorObj.ReflSldrs.count)
                            colorObj.RedModObjs.withUnsafeBufferPointer({TempRedModObjsPtr in
                                let RedModObjsPtr = TempRedModObjsPtr.baseAddress
                                let numRedModObjs = Int32(colorObj.RedModObjs.count)
                                colorObj.GreenModObjs.withUnsafeBufferPointer({TempGreenModObjsPtr in
                                    let GreenModObjsPtr = TempGreenModObjsPtr.baseAddress
                                    let numGreenModObjs = Int32(colorObj.GreenModObjs.count)
                                    colorObj.BlueModObjs.withUnsafeBufferPointer({TempBlueModObjsPtr in
                                        let BlueModObjsPtr = TempBlueModObjsPtr.baseAddress
                                        let numBlueModObjs = Int32(colorObj.BlueModObjs.count)
                                            colorObj.ReflModObjs.withUnsafeBufferPointer({TempReflModObjsPtr in
                                                let ReflModObjsPtr = TempReflModObjsPtr.baseAddress
                                                let numReflModObjs = Int32(colorObj.ReflModObjs.count)
                                                    let colorObjPos: Int32 = Int32(colorObj.ColorObjectPosition)
                                                            colorEqnModifiersAlias(RedSldrPtr, numRedSldrs, GreenSldrPtr, numGreenSldrs, BlueSldrPtr, numBlueSldrs, ReflSldrPtr, numReflSldrs, RedModObjsPtr, numRedModObjs, GreenModObjsPtr, numGreenModObjs, BlueModObjsPtr, numBlueModObjs, ReflModObjsPtr, numReflModObjs, colorObjPos)
                                            })
                                    })
                                })
                            })
                        })
                    })
                })
            })
        }
        //color
        
        //light
        for lightObj in LightObjInfoArray{
            lightObj.XYeqn.withUnsafeBufferPointer({TempXYeqnPtr in
                let XYeqnPtr = TempXYeqnPtr.baseAddress
                lightObj.ZDirEqn.withUnsafeBufferPointer({TempZDirEqnPtr in
                    let ZDirEqnPtr = TempZDirEqnPtr.baseAddress
                    lightObj.IntensityEqn.withUnsafeBufferPointer({TempIntensityEqnPtr in
                        let IntensityEqnPtr = TempIntensityEqnPtr.baseAddress
                            addLightObjectAlias(XYeqnPtr, ZDirEqnPtr, IntensityEqnPtr)
                    })
                })
            })
            lightObj.XYeqnSldrs.withUnsafeBufferPointer({TempXYeqnSldrPtr in
                let XYeqnSldrPtr = TempXYeqnSldrPtr.baseAddress
                let sldrCnt: Int32 = Int32(lightObj.XYeqnSldrs.count)
                lightObj.XYeqnModObjs.withUnsafeBufferPointer({TempXYeqnModObjsPtr in
                    let XYeqnModObjsPtr = TempXYeqnModObjsPtr.baseAddress
                    let numModObjs = Int32(lightObj.XYeqnModObjs.count)
                        let lightObjPos = lightObj.LightObjectPosition
                            addXYeqnDependencyAlias(lightObjPos, XYeqnSldrPtr, sldrCnt, XYeqnModObjsPtr, numModObjs)
                })
            })
            lightObj.ZDirSldrs.withUnsafeBufferPointer({TempZDirSldrPtr in
                let ZDirSldrPtr = TempZDirSldrPtr.baseAddress
                let sldrCnt = Int32(lightObj.ZDirSldrs.count)
                lightObj.ZDirModObjs.withUnsafeBufferPointer({TempZDirModObjsPtr in
                    let ZDirModObjsPtr = TempZDirModObjsPtr.baseAddress
                    let modObjsCnt = Int32(lightObj.ZDirModObjs.count)
                        let lightObjPos = Int32(lightObj.LightObjectPosition)
                            addZDirDependencyAlias(lightObjPos, ZDirSldrPtr, sldrCnt, ZDirModObjsPtr, modObjsCnt)
                })
            })
            lightObj.IntensitySldrs.withUnsafeBufferPointer({TempIntensitySldrPtr in
                let IntensitySldrPtr = TempIntensitySldrPtr.baseAddress
                let sldrCnt = Int32(lightObj.IntensitySldrs.count)
                    lightObj.IntensityModObjs.withUnsafeBufferPointer({TempIntensityModObjsPtr in
                        let IntensityModObjsPtr = TempIntensityModObjsPtr.baseAddress
                        let modObjCnt = Int32(lightObj.IntensityModObjs.count)
                            let lightPos = lightObj.LightObjectPosition
                                addIntensityDependencyAlias(lightPos, IntensitySldrPtr, sldrCnt, IntensityModObjsPtr, modObjCnt)
                    })
            })
        }
        //light
    }
    
    //sets init values for object master and also updates for every single object!!!
    func setInitObjMstrVals(xRange: Float, yRange: Float, resol: Float, animVal: Float, metalBufferPtr: UnsafeMutablePointer<Float>, clrBufferPtr: UnsafeMutablePointer<Float>, lightBufferPtr: UnsafeMutablePointer<Float>, heightLookupPointer: UnsafeMutablePointer<Float>, offstBufferPointer: UnsafeMutablePointer<Float>){
        setInitValuesForObjectMasterAlias(xRange, yRange, resol, animVal, metalBufferPtr, clrBufferPtr, lightBufferPtr, heightLookupPointer, offstBufferPointer) //set init values for object master and update for all objects too
        setInitobjectMstrValuesAlias()
    }
    
    //returns true when animation finishes running
    func objMstrCalculator(animMutliplier: Float, duration: Float) -> Void{
        if !stopAnimation{ //if stopAnimation is FALSE, continue animating. can only be reset by user clicking triangle button. if the graph//workspace toggle is currently at workspace metal should skip this function and run a static animation function. the static animation function should run as long as another boolean is true and the boolean is set to false when the static animation function is run once and can only be set to true again whem the workspace/graph toggle is clicked again and it switches to the graph workspace however as you should remember it is not run by metal yet since the function is only run when the graph/workspace toggle is leaning towards workspace until then this function is called.
            var animResult = 99
            if Graphs.drawTheseObjects.isEmpty{//if there are no specifications to draw specific objects, draw all of them. else call the special function.
                animResult = (Int(animateObjMstr(animMutliplier, duration, Int32(1), Float(Graphs.currentProgressCounter))))//animate object master for the given duration. needs to be called every frame of animation to animate!
            } else {
                let objCnt = DrawObjsLvlAndPosCoords.count/2;//number of objects to draw
                DrawObjsLvlAndPosCoords.withUnsafeBufferPointer({ptr in
                    if let baseAdressToObjToDrawCoords = ptr.baseAddress{
                        animResult = (Int(animateSelectedObjects(animMutliplier, duration, baseAdressToObjToDrawCoords, Int32(objCnt), Int32(1), Float(Graphs.currentProgressCounter))))
                    }
                })
                 
            }
            if animResult != 0 {stopAnimation = true; MetalHasBeenCleared = false;}//when animation is supposed to be done with a round, clear metal.//true; //when animationResult returns 1 a.k.a. it is finished, set stopAnimation to true. stopAnimation can be reset by reclicking the calculate button.
        } else{
            clearMetal()
        }
        
    }
    func objectMstrStaticCalculator(){ //static animation
        MetalHasBeenCleared = false;
        if !stopStaticAnimation{
            if Graphs.drawTheseObjects.isEmpty{//if there are no specifications to draw specific objects, draw all of them. else call the special function.
                animateStaticObjMstr(Float(Graphs.currentProgressCounter))//animate object master for the given duration. needs to be called every frame of animation to animate!
            } else {
                let objCnt = DrawObjsLvlAndPosCoords.count/2;//number of objects to draw
                DrawObjsLvlAndPosCoords.withUnsafeBufferPointer({ptr in
                    if let baseAdressToObjToDrawCoords = ptr.baseAddress{
                        //animateStaticObjMstr(Float(Graphs.currentProgressCounter))//animate object master for the given duration. needs to be called every frame of animation to animate!
                        animateSelectedStaticObjects(baseAdressToObjToDrawCoords, Int32(objCnt)) //commented out for now for testing
                    }
                })
            }
            //stopStaticAnimation = true //sets to true so cant call again
        }
    }
    func clearMetal(){
        clearMetalAlias()//clears metal buffer of points drawn
        MetalHasBeenCleared = true
    }
}



