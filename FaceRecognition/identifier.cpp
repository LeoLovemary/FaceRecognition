#include "identifier.h"

// ********ʶ������*****************
// ���뵥��ͼƬ��������ʶ��
void Identifier::identificationFaceFromPIC(cv::Mat &image)
{
	//��ʶ��֮ǰ���ж�lbpModel�Ƿ�Ϊ�գ�Ϊ������Ҫ���¿�ʼѵ��Model��ע�ᣩ
	if (lbpModel.empty()) 
	{
		std::cerr << "Error in identification! LBP model is empty, you must register firstly." << std::endl;
	}
	Mat imgRoi = image(detectROI);
	rectangle(image,detectROI,Scalar(0,0,255));
	Mat grayImgRoi;
	switch (imgRoi.type())
	{
	case 0:
		imgRoi.copyTo(grayImgRoi);  
		break;
	case 16:
		cv::cvtColor(imgRoi,grayImgRoi,CV_BGR2GRAY);
		break;
	default:
		printf("Image type error, input image must be gray or rgb!");
		break;
	}
	m_data.image = grayImgRoi;
	//m_faceData = det.fetchFace(m_data.image);  //��ǶȲ�׽����
	m_faceData = det.findFace(m_data.image);
	size_t num_fD = m_faceData.size();
	if (num_fD != 0)
	{
		for (size_t i=0; i<num_fD; ++i)
		{
			cv::Mat normalizedImg = iNorm.normalize(m_faceData[i]);

			// �б��Ƿ��ǻ�������
			if (!ifSpoofingFlag || svmClassifier.empty())
			{
				printf("no anti-spoofing function.\n");
			}
			else
			{
				Mat lbp_image = my_elbp(normalizedImg, 1, 8);
				// get spatial histogram from this lbp image
				Mat featureVector = spatial_histogram(
					lbp_image, /* lbp_image */
					static_cast<int>(std::pow(2.0, static_cast<double>(8))), /* number of possible patterns */
					8, /* grid size x */
					8, /* grid size y */
					true);    /*normal*/
				int predictResult = svmClassifier->predict(featureVector);
				if (predictResult == 1)
				{
					putText(image, "This is spoofing face, please put real face in the rectangle", Point2f(detectROI.x - 330.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2); //��ʾ���
					continue;
				}
			}
			rectangle(image, Rect(m_faceData[i].detectFace.x + detectROI.x, m_faceData[i].detectFace.y + detectROI.y, m_faceData[i].detectFace.width, m_faceData[i].detectFace.height), cv::Scalar(0, 255, 0), 2);     //������⵽������   
			int predictedLabel;
			double dist;
			lbpModel->predict(normalizedImg, predictedLabel, dist);       //��normalizedImg��ȡLBP����,����Ϊ�Ҷ�ͼ����
			//! [Print results]
			if (dist < I_THRESHOLD)
			{
				std::vector<string> classNames = readClassNames();
				char preName[100];
				sprintf(preName, "%s", classNames.at(predictedLabel).c_str());
				putText(image, preName, Point2f(detectROI.x + m_faceData[i].detectFace.x + m_faceData[i].detectFace.width / 2 - 40.0, detectROI.y + m_faceData[i].detectFace.y - 10.0), 3, 0.8, Scalar(255, 0, 0), 1);         //д��ʶ������            
			}
			else
			{
				putText(image, "No registration information, no identifiacation", Point2f(detectROI.x - 220.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2);   //��ʾ���
			}
		}
	}
	else
	{
		putText(image,"�뽫�������ھ��ο���",Point2f(detectROI.x + detectROI.width/2 - 30.0, detectROI.y - 10.0),3,0.8,Scalar(0,0,255),1);    //��ʾ���
	}
}

// ������ͷ��ʽ����ͼ�����ʶ��
void Identifier::identificationFaceFromCAM()
{
	cv::VideoCapture capture(0);
	while(1)  
	{  
		cv::Mat frame;   //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
		capture>>frame;  //��ȡ��ǰ֡
		identificationFaceFromPIC( frame);
		int key=cvWaitKey(50);    //��ȡ��ֵ��key������
		if (key==27) break;       //�������Esc�������˳�ʶ��
	}   
}

// ����Ƶ��ʽ����ͼ�����ʶ��
void Identifier::identificationFaceFromVIDEO(char *videoName)
{
	cv::VideoCapture capture;
	capture.open(videoName);
	cv::Mat frame;
	if (capture.isOpened())
	{
		for (;;)
		{
			capture >> frame;
			if (frame.empty())                 //��������Ƶ���һ֡ʱ���ж��Ƿ�Ϊ�գ�Ϊ��������
			{
				break;                        
			}
			identificationFaceFromPIC( frame);
			int key=cvWaitKey(50);    //��ȡ��ֵ��key������
			if (key==27) break;       //�������Esc�������˳�ʶ��
		}
	}
}

