#include "register.h"

// **********ע������*************
// ��ͼƬ��ʽ�����ע������
// ���������ͼƬ��ѵ����ʽ�Ƿ����Ը��·�ʽ
void Register::registerFaceFromPIC(cv::Mat &image, bool update)  
{
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
	m_faceData = det.findFace(m_data.image);
	//m_faceData = det.fetchFace(m_data.image);     //ͨ����ת����Ƕȼ�⵽����
	if (!m_faceData.empty())
	{
		m_data = det.biggestFace(m_faceData);
		rectangle(image, Rect(m_data.detectFace.x + detectROI.x, m_data.detectFace.y + detectROI.y, m_data.detectFace.width, m_data.detectFace.height), cv::Scalar(0, 255, 0), 2);     //������⵽������
		cv::Mat normalizedImg = iNorm.normalize(m_data);
		if (!ifSpoofingFlag || svmClassifier.empty())           //�����ж��Ƿ�Ϊ����
		{
			printf("no anti-spoofing function.\n");
			int label = 0;
			string name;
			printf("Please input your name:");
			getline(cin, name); 			              //����ע��������
			acquireLabel(const_cast<char*>(name.c_str()), "label_name.txt", label);
			std::vector<cv::Mat> imageData;
			std::vector<int> labels;
			imageData.push_back(normalizedImg);                   //ע�⣺normalizedImg����Ϊ�Ҷ�ͼ
			labels.push_back(label);
			if (update)
			{
				lbpModel->update(imageData, labels);
			}
			else
			{
				lbpModel->train(imageData, labels);
			}
			lbpModel->write("model.xml");
		}
		else                      //�������⹦��
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
				putText(image, "This is spoofing face, you can not register", Point2f(detectROI.x - 130.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2); //��ʾ���
			}
			else
			{
				int label = 0;
				string name;
				printf("Please input your name.\n");
				std::cin >> name; 	                                     //����ע��������
				acquireLabel(const_cast<char*>(name.c_str()), "label_name.txt", label);
				std::vector<cv::Mat> imageData;
				std::vector<int> labels;
				imageData.push_back(normalizedImg);                   //ע�⣺normalizedImg����Ϊ�Ҷ�ͼ
				labels.push_back(label);
				if (update)
				{
					lbpModel->update(imageData, labels);
				}
				else
				{
					lbpModel->train(imageData, labels);
				}
				lbpModel->write("model.xml");
			}
		}
	}
	else
	{
		putText(image,"�뽫�������ھ��ο���",Point2f(detectROI.x + detectROI.width/2 - 30.0, detectROI.y - 10.0),3,0.8,Scalar(0,0,255),1);    //��ʾ���
		imshow("Image Windows",image);
		waitKey(1000);
	}
}

// ������ͷ��ʽ�����ע������
void Register::registerFaceFromCAM(bool update)
{
	//��1��������ͷ������Ƶ
	cv::VideoCapture capture(0);
	//��2��ѭ����ʾÿһ֡
	while(1)  
	{  
		cv::Mat frame;   //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
		capture>>frame;  //��ȡ��ǰ֡
		registerFaceFromPIC( frame,update);
		int key=cvWaitKey(50);    //��ȡ��ֵ��key������
		if (key==27) break;       //�������Esc�������˳�ע��
	}   
}

// ����Ƶ��ʽ�����ע������
void Register::registerFaceFromVIDEO(char *videoName, bool update)
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
			registerFaceFromPIC( frame,update);
			int key=cvWaitKey(50);    //��ȡ��ֵ��key������
			if (key==27) break;       //�������Esc�������˳�ע��
		}
	}
}