//
//  MasterView.swift
//  ArduiMatorNew
//
//  Created by AnshuDiya on 10/5/26.
//

import Foundation
import SwiftUI
import MetalKit
import Metal
import Cocoa
import QuartzCore

struct MasterView: View{
    @State private var animOrObjView = false//true when animView and false when objView
    @ObservedObject var AnmInst = Animation.AnimInstance
    var body: some View{
        ZStack{
            if AnmInst.CurrentObjectViewType == 4{
                AnimView()
                    .transition(.opacity)
            } else {
                ObjectSwitcherView()
                    .transition(.opacity)
            }
            VStack{
                Spacer()
                HStack{
                    Button(action:{
                        if AnmInst.CurrentObjectViewType < 4{AnmInst.CurrentObjectViewType += 1}else{AnmInst.CurrentObjectViewType = 0}//cycles through object/view types
                        //animOrObjView.toggle()
                    }) {
                        Image(systemName: "terminal.fill")
                            .imageScale(.small)
                            .background(.thickMaterial)
                            .frame(width:5, height: 5)
                    }
                    Spacer()
                }
            }
        }
        .animation(.easeInOut(duration: 0.6), value: animOrObjView)
    }
}
