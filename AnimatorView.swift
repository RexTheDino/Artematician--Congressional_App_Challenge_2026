//
//  AnimatorView.swift
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

//create a rounded rect shape of frame 500x500 with a very light red background over the rest of the space
struct AnimView: View{
    var body: some View{
        let superLightRedBckgrnd = Color(Color.RGBColorSpace.sRGB, red: 250.0/255.0, green: 26.0/255.0, blue: 12.0/255.0, opacity: 0.18)
        ZStack{
            superLightRedBckgrnd
            RoundedRectangle(cornerRadius: 20, style: .continuous)
                .fill(.thickMaterial)
                .frame(width: 750, height: 750)
        }
        .transition(.opacity)
    }
}
