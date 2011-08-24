//
//  Segmentation.cpp
//
//  Created by Salik Syed on 8/16/11.
//  

/*
Copyright (C) 2010 Salik Syed

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

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