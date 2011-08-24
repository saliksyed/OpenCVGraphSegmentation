//
//  Segmentation.cpp
//  SliceOCR
//
//  Created by Salik Syed on 8/16/11.
//  Copyright 2011 Stanford. All rights reserved.
//

#include "Segmentation.h"
#include "segment-image.h"
#include "pnmfile.h"

// minimum number of pixels for an individual blob in segmentation
#define MIN_SEGMENTATION_BLOB_SIZE 450  
// sigma smoothing value when running graph image segmentation
#define SIGMA 1.2f

image<rgb>* convertIplToNativeImage(IplImage * input){
    int w = input->width;
    int h = input->height;
    image<rgb> *im = new image<rgb>(w,h);
    for(int i =0; i<w; i++){
        for(int j=0; j<h; j++){
            CvScalar s = cvGet2D(input,j,i);
            rgb curr;
            curr.r = s.val[0];
            curr.g = s.val[1];
            curr.b = s.val[2];
            im->data[i+j*w] = curr;
        }
    }
    return im;
}


IplImage* convertNativeToIplImage(image<rgb>* input){
    int w = input->w;
    int h = input->h;
    IplImage * output = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 3);
    for(int i =0; i<w; i++){
        for(int j=0; j<h; j++){
            CvScalar s;
            rgb curr =input->data[i+j*w];
            s.val[0] = curr.r;
            s.val[1] = curr.g;
            s.val[2] = curr.b;
            cvSet2D(output, j, i, s);
        }
    }
    return output;
}


void segmentImage(IplImage* input, vector<vector<Pixel> > & resultBuffer){
    image<rgb> * converted= convertIplToNativeImage(input);
    segment_image(converted, SIGMA, 500, MIN_SEGMENTATION_BLOB_SIZE, NULL,resultBuffer); 
    delete converted;
}