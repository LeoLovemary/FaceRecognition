// FaceRec_LBP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/ml/ml.hpp"
#include "ml.h"

#include "register.h"
#include "identifier.h"
#include <stdexcept>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;

Rect detectROI = Rect(170,60,480,450);
Detector det;
cv::Ptr<cv::FaceRecognizer> lbpModel;
Normalizator iNorm;
cv::Ptr<cv::ml::SVM> svmClassifier;
bool ifSpoofingFlag = false;            //�Ƿ���жԿ������⹦�ܣ�Ĭ�Ϸ�

bool initializeModel(const std::string detModelFile = "haarcascade_frontalface_alt.xml", const std::string keyPointModelFile = "shape_predictor_68_face_landmarks.dat",
	                 const std::string lbpModelFile = "model.xml", const std::string svmClassifierFile = "SVMClassifier.xml");

int main() 
{
	if (initializeModel())
	{
		Register reg;
		Identifier ident;
		// ע��
	    //Mat image1 = imread("00001.jpg");
		//reg.registerFaceFromPIC(image1,false);
		//Mat image2 = imread("00015.jpg");
		//reg.registerFaceFromPIC(image2,true);
		// ʶ��
		Mat image3 = imread("00060.jpg");
		ident.identificationFaceFromPIC(image3);
		imshow("result",image3);
		waitKey(0);
		return 0;
	} 
	else
	{
		printf("Initialize fail!!!");
		return -1;
	}
}


bool initializeModel(const std::string detModelFile, const std::string keyPointModelFile, const std::string lbpModelFile, const std::string svmClassifierFile)
{
	if ((_access(detModelFile.c_str(),0) != -1)&&(_access(keyPointModelFile.c_str(),0) != -1))
	{
		det.initializeModel(detModelFile,keyPointModelFile);            //�������ʼ��
	}
	else
	{
		std::cout <<"No face detector model is found in the current path, initialize fail."<<std::endl;
		return false;
	}
	lbpModel = createLBPHFaceRecognizer();    //ʶ������ʼ��
	if ((_access(lbpModelFile.c_str(),0) != -1))
	{
		lbpModel->load(lbpModelFile);
	}
	else
	{
		std::cout <<"No LBP model is found in the current path, and you need to start training again."<<std::endl;
	}
	if ((access(svmClassifierFile.c_str(), 0) != -1))
	{
		svmClassifier = cv::ml::StatModel::load<cv::ml::SVM>(svmClassifierFile);   //����������ʼ��
		std::cout << "This face recognition system include anti-spoofing function, you can use it by setting parameter ifSpoofing==true." << std::endl;
		return true;
	}
	else
	{
		std::cout << "This face recognition system no anti-spoofing function." << std::endl;
		return true;
	}
}

