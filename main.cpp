//
//  main.cpp
//  SliceOCR
//
//  Created by Salik Syed on 8/9/11.
//  Copyright 2011 Stanford. All rights reserved.
//

#import <tesseract/baseapi.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "Threshold.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace tesseract;

#define TESSERACT_DATA_PATH "/opt/local/share/tessdata"

int main (int argc, const char * argv[])
{
    if(argc<2){
        printf("Usage: SliceOCR <image-file-name> <output-file-name> (optional)  <output-text-file-name> (optional)\n");
        exit(0);
    }
        printf("Arguments: %d  %s\n",argc,argv[3]);
    // Load Image into buffer:
    IplImage * rawImage = NULL;
    rawImage=cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    if(!rawImage){
        printf("Could not load image file: %s\n",argv[1]);
        exit(0);
    }
    
    // Pre-process image
    ThresholdImage(rawImage);
    IplImage * img = cvCreateImage(cvSize(rawImage->height, rawImage->width), IPL_DEPTH_8U, 1);
    cvTranspose(rawImage, img);
    cvFlip(img,img,1);
    cvReleaseImage(&rawImage);
    
    
    if(argc==3){
        cvSaveImage(argv[2], img);
        return 0;
    }
    
    // Setup tesseract
    TessBaseAPI * api = new TessBaseAPI();
    api->Init(TESSERACT_DATA_PATH, "eng");
    api->SetPageSegMode(PSM_SINGLE_COLUMN);
    api->SetAccuracyVSpeed(AVS_MOST_ACCURATE);
    api->SetImage((unsigned char *)img->imageData, img->width, img->height, img->nChannels*sizeof(char),img->widthStep);

    
    // Tesseract first detects blocks (bounding boxes of lines)
    BLOCK_LIST * blocks = api->FindLinesCreateBlockList();
    
    // Run both passes
    PAGE_RES * p1 = api->RecognitionPass1(blocks);
    PAGE_RES*  p2= api->RecognitionPass2(blocks,p1);
    

    
    char * text; // buffer holding result text
    int * lengths; // lengths?? not sure what this is.
    float * costs; // accuracies for each character
    // arrays holding bbox corners of each character
    int * x0; 
    int * y0;
    int * x1;
    int * y1;
    int num = api->TesseractExtractResult(&text,&lengths,&costs,&x0,&y0,&x1,&y1,p2);    
    api->End();
    
    // output results to stdout:
    IplImage * img2 = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
    cvCvtColor(img, img2, CV_GRAY2BGR);
    int xLast = -9999;
    char lastChar = ' ';
    
    ofstream out;
    bool useFileOutput = false;
    if(argc>3){
        useFileOutput = true;
        out.open (argv[3]);
    }
    
    
    for(int i =0; i<num; i++){
        
//        printf("%d,%d,%d,%d\n",x0[i],x1[i],y0[i],y1[i]);
        
        if(useFileOutput)out<<text[i];
        if(x0[i]>x1[i]) continue;
        
        if(x0[i]<xLast && lastChar!=' '){
            if(useFileOutput)out<<endl;
        }
        
        CvScalar color = cvScalarAll(0);
        costs[i] = 100-costs[i];
        if(costs[i]<15){
            color.val[0]=125;
        }else if(costs[i]<33){
            color.val[0]=255;
        }else if(costs[i]<50){
            color.val[1]=125;
        }else if(costs[i]<66){
            color.val[1]=255;
        }else if(costs[i]<75){
            color.val[2]=125;
        }else{
            color.val[2]=255;
        }
        //cvRectangle(img2, cvPoint(x0[i], img->height-y0[i]), cvPoint(x1[i], img->height-y1[i]), color,3);
        //cout<<costs[i]<<endl;
        xLast = x0[i];
        lastChar = text[i];
        /*
        if(text[i]!='\n')cout<<text[i];
        if(i>0 && x0[i]<x0[i-1] && x0[i-1]!=32767 && x0[i]!=32767 && x0[i-1]!=-32767 && x0[i]!=-32767){
            cout<<endl;
        }*/
        //cout<<","<<x0[i]<<","<<y0[i]<<x1[i]<<","<<y1[i]<<","<<costs[i]<<endl;
    }
    
    if(argc>2){
        cvSaveImage(argv[2], img2);
    }
    
    if(argc>3){
        out.close();
    }

    
    cvReleaseImage(&img2);
    // free mem alloc'd by tesseract
    delete x0;
    delete x1;
    delete y0;
    delete y1;
    delete text;
    delete costs;
    delete lengths;
    
    
    return 0;
}

