//
//  File.metal
//  ArduiMatorNew
//
//  Created by AnshuDiya on 9/14/26.
//

#include <metal_stdlib>
using namespace metal;

//converts a given point from viewSpace into NDC
inline float viewSpaceToNDC(bool xCoordsOrYcoords, float viewSpaceCoords, int SwiftFrameSize){
    if(xCoordsOrYcoords){
        return ((viewSpaceCoords/SwiftFrameSize)-1.0);
    } else{
        return (1.0-(viewSpaceCoords/SwiftFrameSize));
    }
}

struct NormalVectorMetadata{//refer to pages for documentation
    float2 currentPixelCoords;
    float slopeBetweenOrigPoints;
    float invSlope;
    float b2;
    float b1;
    float xCoordOfIntersection;
        float yCoordOfIntersection;
    float distFromLineSeg;
    float pixelLookupIndiceVal;
    float dzdx;
    float dzdy;
    float intersectionPointCoords;
        float3 T1;
        float3 T2;
            float3 NormalVector;
};

struct FragInfoStruct{
    float4 position [[position]];
    float pointSize [[point_size]] = 5;
    float2 SwiftFrameSize;
    float2 point1 [[flat]];
    float2 point2 [[flat]];
    float reflectivity [[flat]];//hardcoded for now
    float3 surfaceClr [[flat]];//hardcoded for now
};

struct LightSourceInfo{
    float lightIntensity;
    float3 lightPosition;
};

struct DerVertexOutput{
    float4 position [[position]];
    float pointSize [[point_size]];
};

vertex FragInfoStruct basic_vertex(
//offset points array contain the point offsets for the current point. they are what actually comprise the triangle vertices. vertexArray simply contains the corresponding vertex points which will be passed on as points 1 and 2. snce two triangles per point we have some updating to do in c++ and swift along with changng how the buffers are written to.
const device packed_float2* vertexArray[[buffer(0)]], const device packed_float4* clrAndReflectivity [[buffer(3)]]/*contains clr and reflecivity values for that specific point*/, const device packed_float2* offsetPointsArray[[buffer(4)]],
unsigned int vertexGid [[vertex_id]], constant unsigned int& totalVerticeCount[[buffer(5)]]) {
    unsigned int realVertexID = vertexGid;//clamp(vertexGid,0u,1790u);
    FragInfoStruct FragInf;
    FragInf.position = float4(offsetPointsArray[vertexGid],1.0,1.0);
    
        //vertex_id offsets for point1 and point2 depending on which vertexGid we are currently on
    float4 LineSegmentEndPoints = float4(vertexArray[clamp(((vertexGid/6)*2),0u,1790u/3u)], vertexArray[clamp(((vertexGid/6)*2)+1u,0u,1790u/3u)]);
    FragInf.point1 = float2(LineSegmentEndPoints.x,LineSegmentEndPoints.y);//point 1 of the line segment
    FragInf.point2 = float2(LineSegmentEndPoints.z,LineSegmentEndPoints.w);//endpoint of the line segment

    if((FragInf.point2.x > 1.0)||(FragInf.point1.x > 1.0)){FragInf.point2 = 10.0; FragInf.point1 = 10.0;}//if the line segment's point 2 or point 1 reached is the end of the screen clip both
    FragInf.SwiftFrameSize = float2(750.0f,750.0f);//hardcoded here
    FragInf.reflectivity = clrAndReflectivity[realVertexID/6].x;
   if(FragInf.reflectivity < 0.0f){
       FragInf.reflectivity = pow(abs(FragInf.reflectivity),FragInf.reflectivity)-1.0f;//1.0f/(FragInf.reflectivity*FragInf.reflectivity);
    }
    FragInf.surfaceClr = float3(clrAndReflectivity[realVertexID/6].y,clrAndReflectivity[realVertexID/6].z,clrAndReflectivity[realVertexID/6].w);//clr values
    if(FragInf.surfaceClr.x < 0.0f){
        FragInf.surfaceClr.x = pow(abs(FragInf.surfaceClr.x),FragInf.surfaceClr.x)-1.0f;//1.0f/(FragInf.surfaceClr.x*FragInf.surfaceClr.x);
    }
    if(FragInf.surfaceClr.y < 0.0f){
        FragInf.surfaceClr.y = pow(abs(FragInf.surfaceClr.y),FragInf.surfaceClr.y)-1.0f;//1.0f/(FragInf.surfaceClr.y*FragInf.surfaceClr.y);
    }
    if(FragInf.surfaceClr.z < 0.0f){
        FragInf.surfaceClr.z = pow(abs(FragInf.surfaceClr.z),FragInf.surfaceClr.z)-1.0f;//1.0f/(FragInf.surfaceClr.z*FragInf.surfaceClr.z);
    }
    return FragInf;
}


inline float2 dzDXDYcalc(float currentX, float currentY, float maxDist, const device float* HeightLookupTable, float HeightTableCount, float minValOfDistFromLineSeg, float maxValOfDistFromLineSeg, FragInfoStruct InfoStruct, float previousLookupVal){
    
    const float epsilon = 0.005;
    float2 returnDerivative = float2(0.0f,0.0f);
    
    for(int b = 0; b < 2; b++){
        NormalVectorMetadata dzDXDYcalculation;
        if (b == 0){
            dzDXDYcalculation.currentPixelCoords = float2((currentX+epsilon),currentY);//epsilon of 0.005 for testing
        } else {
            dzDXDYcalculation.currentPixelCoords = float2(currentX,(currentY+epsilon));//epsilon of 0.005 for testing
        }
        
        dzDXDYcalculation.slopeBetweenOrigPoints = ((InfoStruct.point2.y-InfoStruct.point1.y)/(InfoStruct.point2.x-InfoStruct.point1.x));
        dzDXDYcalculation.invSlope = -1.0f/dzDXDYcalculation.slopeBetweenOrigPoints;
        dzDXDYcalculation.b2 = dzDXDYcalculation.currentPixelCoords.y-(dzDXDYcalculation.invSlope*dzDXDYcalculation.currentPixelCoords.x);//intercept of the perpendicular line
        dzDXDYcalculation.b1 = InfoStruct.point1.y - (dzDXDYcalculation.slopeBetweenOrigPoints*InfoStruct.point1.x);//intercept of line going thorugh line segment
        
        
        dzDXDYcalculation.xCoordOfIntersection = ((dzDXDYcalculation.b1-dzDXDYcalculation.b2)/(dzDXDYcalculation.invSlope-dzDXDYcalculation.slopeBetweenOrigPoints));
        dzDXDYcalculation.yCoordOfIntersection = (dzDXDYcalculation.slopeBetweenOrigPoints*dzDXDYcalculation.xCoordOfIntersection)+dzDXDYcalculation.b1;
        dzDXDYcalculation.distFromLineSeg = distance(float2(dzDXDYcalculation.xCoordOfIntersection,dzDXDYcalculation.yCoordOfIntersection),dzDXDYcalculation.currentPixelCoords);//distance from point of intersection and current pixel
        if (isnan(dzDXDYcalculation.distFromLineSeg)||isinf(dzDXDYcalculation.distFromLineSeg)){dzDXDYcalculation.distFromLineSeg = 0.0f;}
        float indiceToLookupDZDX = (clamp(dzDXDYcalculation.distFromLineSeg,minValOfDistFromLineSeg,maxValOfDistFromLineSeg)*(HeightTableCount/maxDist));//returns the correct indice to look up inside the heightTableArray
        dzDXDYcalculation.pixelLookupIndiceVal = HeightLookupTable[(int)indiceToLookupDZDX];
        dzDXDYcalculation.pixelLookupIndiceVal += (indiceToLookupDZDX-(int)indiceToLookupDZDX)*(HeightLookupTable[int(indiceToLookupDZDX)+1]-HeightLookupTable[(int)indiceToLookupDZDX]);
        
        float derivative = (dzDXDYcalculation.pixelLookupIndiceVal-previousLookupVal)/(epsilon);//the derivative to return
        if(b==0){
            returnDerivative.x = derivative;
        } else{
            returnDerivative.y = derivative;
        }
    }
    return returnDerivative;
}


fragment float4 basicFragment(
FragInfoStruct InfoStruct [[stage_in]], const device packed_float4* lightInfoBuffer [[buffer(1)]], const device float* HeightLookupTable [[buffer(2)]]){
    float4 lightInfBufferFirstElement = lightInfoBuffer[0];
        float lightCount = lightInfBufferFirstElement.x;//the first index of lightInfoBuffer contains the number of lights. Could also be zero!
    float HeightTableCount = (int)HeightLookupTable[0];//first index contains number of height values.
        float maxDist = sqrt(8.0f);//the maximum distance that is permissible between a line segment and a point
    NormalVectorMetadata Nvec;
    float currentX = viewSpaceToNDC(true, InfoStruct.position.x, InfoStruct.SwiftFrameSize.x);
    float currentY = viewSpaceToNDC(false, InfoStruct.position.y, InfoStruct.SwiftFrameSize.y);
    Nvec.currentPixelCoords = float2(currentX,currentY);
    
    Nvec.slopeBetweenOrigPoints = ((InfoStruct.point2.y-InfoStruct.point1.y)/(InfoStruct.point2.x-InfoStruct.point1.x));
        Nvec.invSlope = -1.0f/Nvec.slopeBetweenOrigPoints;
    Nvec.b2 = Nvec.currentPixelCoords.y-(Nvec.invSlope*Nvec.currentPixelCoords.x);//intercept of the perpendicular line
    Nvec.b1 = InfoStruct.point1.y - (Nvec.slopeBetweenOrigPoints*InfoStruct.point1.x);//intercept of line going thorugh line segment
    
    
    Nvec.xCoordOfIntersection = ((Nvec.b1-Nvec.b2)/(Nvec.invSlope-Nvec.slopeBetweenOrigPoints));
        Nvec.yCoordOfIntersection = (Nvec.slopeBetweenOrigPoints*Nvec.xCoordOfIntersection)+Nvec.b1;
    Nvec.distFromLineSeg = distance(float2(Nvec.xCoordOfIntersection,Nvec.yCoordOfIntersection),Nvec.currentPixelCoords);//distance from point of intersection and current pixel
    if (isnan(Nvec.distFromLineSeg)||isinf(Nvec.distFromLineSeg)){Nvec.distFromLineSeg = 0.0f;}
    float minValOfDistFromLineSeg = 1.05f/(HeightTableCount/maxDist);//the minimum value that x must be in order to be of indice atleast 1. the actual formula only needs a 1.0 in the numerator but due to floating point errors we are using 1.05 so that it never ends up just below 1.0 and integer truncation cuts the decimal off leading to an index access of 0 which doesnt correspond to light data
                float maxValOfDistFromLineSeg = (HeightTableCount-2)/(HeightTableCount/maxDist); //since there is an indice+1 op performed further down
    float indiceToLookup = (clamp(Nvec.distFromLineSeg,minValOfDistFromLineSeg,maxValOfDistFromLineSeg)*(HeightTableCount/maxDist));//returns the correct indice to look up inside the heightTableArray
    Nvec.pixelLookupIndiceVal = HeightLookupTable[(int)indiceToLookup];
    Nvec.pixelLookupIndiceVal += (indiceToLookup-(int)indiceToLookup)*(HeightLookupTable[int(indiceToLookup)+1]-HeightLookupTable[(int)indiceToLookup]);
    
    float2 dxdy = dzDXDYcalc(currentX, currentY, maxDist, HeightLookupTable, HeightTableCount, minValOfDistFromLineSeg, maxValOfDistFromLineSeg, InfoStruct, Nvec.pixelLookupIndiceVal);//stores dzdx and dzdy in its two elements
    
    Nvec.dzdx = dxdy.x;//dfdx(1.73f/*Nvec.pixelLookupIndiceVal*/); //0.01f;
    Nvec.dzdy = dxdy.y;//dfdy(Nvec.pixelLookupIndiceVal);
        Nvec.T1 = float3(1,0,Nvec.dzdx);
        Nvec.T2 = float3(0,1,Nvec.dzdy);
    Nvec.NormalVector = cross(Nvec.T1, Nvec.T2);
    Nvec.NormalVector = normalize(Nvec.NormalVector);
    
    //InfoStruct.surfaceClr.x = 0.6f;
    
    float totalBrightness = 0.0f;
    for(unsigned int k = 1; k < lightCount; k += 5){
        /*if (k == lightCount-17){
            //InfoStruct.reflectivity *= totalBrightness;
            return float4(InfoStruct.surfaceClr.x*InfoStruct.reflectivity*totalBrightness,InfoStruct.surfaceClr.y*InfoStruct.reflectivity*totalBrightness,InfoStruct.surfaceClr.z*InfoStruct.reflectivity*totalBrightness,1.0);
            break;
        }*/
        //when performing lighting calculations, make a new struct with every light source
        LightSourceInfo LSinfo;
        LSinfo.lightPosition = float3(lightInfoBuffer[k].x,lightInfoBuffer[k].y,lightInfoBuffer[k].z);
        if(isnan(LSinfo.lightPosition.x)){
            LSinfo.lightPosition.x = 0.0f;
        }
        if(isnan(LSinfo.lightPosition.y)){
            LSinfo.lightPosition.y = 0.0f;
        }
        if(isnan(LSinfo.lightPosition.z)){
            LSinfo.lightPosition.z = 0.0f;
        }
        LSinfo.lightPosition = float3((LSinfo.lightPosition.x - Nvec.currentPixelCoords.x),(LSinfo.lightPosition.y - Nvec.currentPixelCoords.y),(LSinfo.lightPosition.z - Nvec.pixelLookupIndiceVal));
                float distToLight = length(LSinfo.lightPosition);
                float distanceDropoffCoefficient = 1.0f/(distToLight*distToLight);//1/r^2 dropoff in light brghtness with relation to distance
        LSinfo.lightPosition = normalize(LSinfo.lightPosition);
        float brightness = max(dot(Nvec.NormalVector, LSinfo.lightPosition),0.0);
        LSinfo.lightIntensity = lightInfoBuffer[k].w;//light intensity stored in the 4th element
        if(LSinfo.lightIntensity < 0.0f){
            LSinfo.lightIntensity = pow(abs(LSinfo.lightIntensity),LSinfo.lightIntensity);
        }
        totalBrightness += brightness*LSinfo.lightIntensity*distanceDropoffCoefficient;
    }
    float3 finalPxlClr = abs(InfoStruct.reflectivity*InfoStruct.surfaceClr*totalBrightness);//InfoStruct.reflectivity*
    //return float4(0.2*Nvec.dzdx,0.6,0.7*InfoStruct.reflectivity,1.0);
    //return float4(lightCount*Nvec.dzdx*finalPxlClr.x/650.0,0.5,HeightTableCount/650.0,1.0);
    return float4(finalPxlClr,1.0);
}


