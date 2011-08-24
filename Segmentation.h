//
//  Segmentation.h
//  SliceOCR
//
//  Created by Salik Syed on 8/16/11.
//  Copyright 2011 Stanford. All rights reserved.
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

