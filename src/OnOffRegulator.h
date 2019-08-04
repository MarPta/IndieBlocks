#ifndef _ONOFFREGULATOR_H_
#define _ONOFFREGULATOR_H_

class OnOffRegulator{

    float hystUp = 0.0;
    float hystDown = 0.0;
    float maxW = 0.0;
    float minW = 0.0;
    bool isOutputOn = false;

public:
    void setParameters(float aHystUp, float aHystDown, float aMaxW, float aMinW){
        if(aHystUp < 0.0 || aHystDown < 0.0)
            return; //ERROR
        hystUp = aHystUp;
        hystDown = aHystDown;
        maxW = aMaxW;
        minW = aMinW;
    }
    
    bool getRegAction(float aW, float aY){
        if(aW > maxW)
            aW = maxW;
        else if(aW < minW)
            aW = minW;
        
        if(isOutputOn == false && aY < aW - hystDown)
            isOutputOn = true;
        else if(isOutputOn == true && aY > aW + hystUp)
            isOutputOn = false;

        return isOutputOn;
    }
};

#endif /*_ONOFFREGULATOR_H_*/