//
//  Segmentation.h
//
//  Created by Salik Syed on 8/16/11.
//  GPL License
//

#pragma once
#include <opencv/cv.h>
#include <vector>
using namespace std;

typedef struct {
    int x;
    int y;
}Pixel;

void segmentImage(IplImage* input, vector<vector<Pixel> > & resultBuffer);

