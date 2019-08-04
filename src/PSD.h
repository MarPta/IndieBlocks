#ifndef _PSD_
#define _PSD_

class PSD{
    float kP;   //proportional gain
    float kS;   //summative gain
    float kD;   //differential gain

    float e = 0.0;  //regulation error
    float prevE = 0.0;  //previous regulation error
    float x = 0.0;  //regulation action

    float sum = 0.0;
    float sumSaturation = 0.0;
    float diff = 0.0;
    float maxX = 0.0;
    float minX = 0.0;
    float maxW = 0.0;
    float minW = 0.0;
public:
    PSD(float aKP, float aKS, float aKD, float aSumSaturation, float aMaxX, float aMinX, float aMaxW, float aMinW){
        kP = aKP;
        kS = aKS;
        kD = aKD;
        sumSaturation = aSumSaturation;
        maxX = aMaxX;
        minX = aMinX;
        maxW = aMaxW;
        minW = aMinW;
    }
    float getRegAction(float aW, float aY){
        if(aW > maxW)
            aW = maxW;
        else if(aW < minW)
            aW = minW;
        e = aW - aY;

        sum += e;
        if(sum > sumSaturation)
            sum = sumSaturation;
        else if(sum < -sumSaturation)
            sum = -sumSaturation;

        diff = e - prevE;
        prevE = e;

        x = kP*e + kS*sum + kD*diff;

        if(x > maxX)
            x = maxX;
        else if(x < minX)
            x = minX;
        return x;
    }
    void reset(){
        sum = 0.0;
        prevE = 0;
    }
};

#endif /*_PSD_*/