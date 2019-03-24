#include<opencv4/opencv2/opencv.hpp>
#include<opencv4/opencv2/core.hpp>
#include<opencv4/opencv2/imgproc.hpp>
#include<opencv4/opencv2/highgui.hpp>
#include<opencv4/opencv2/videoio.hpp>
#include<opencv4/opencv2/video.hpp>
#include<opencv4/opencv2/objdetect.hpp>
#include<opencv4/opencv2/bgsegm.hpp>


#include<string>
#include<vector>
static cv::Scalar randomColor( cv::RNG& );


int main(int argc, char *argv[])
{
    cv::VideoCapture cap(0);
    cv::Mat frame;
    cv::Mat frameGray;
    cv::Mat maskMOG2;
    cv::Mat labels;
    cv::Mat objects1;
    cv::Mat mask;
    int obj1N;


    char ch;

    cv::Ptr<cv::BackgroundSubtractorMOG2> bMOG2;

    bMOG2 = cv::createBackgroundSubtractorMOG2(600,60,true); //threshold (second term) can be modified

    cv::namedWindow("video",cv::WINDOW_NORMAL);
    cv::namedWindow("videoGray",cv::WINDOW_NORMAL);
    cv::namedWindow("MOG2",cv::WINDOW_NORMAL);

    ch = '0';
    cv::RNG rng(0xFFFFFF);

    while(ch!=27)
    {
        cap >> frame;
        cv::cvtColor(frame,frameGray,cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frameGray,frameGray);
        bMOG2->apply(frameGray,maskMOG2);

        cv::resize(maskMOG2,maskMOG2,cv::Size(),0.125,0.125);
        cv::medianBlur(maskMOG2,maskMOG2,5);
        mask = cv::Mat::zeros(maskMOG2.rows,maskMOG2.cols,CV_8UC3);
        //cv::resize(maskMOG2,maskMOG2,cv::Size(),6,6);
        obj1N = cv::connectedComponents(maskMOG2,labels);
        objects1 = cv::Mat::zeros(maskMOG2.rows,maskMOG2.cols,CV_8UC3);

        if(obj1N>1)
        {
            rng = cv::RNG(0xFFCCFF);
            for(auto j = 1; j<obj1N;j++)
            {
                mask = labels==j;
                objects1.setTo(randomColor(rng),mask);
            }
            std::cout<<"\"objects\": "<<obj1N<<std::endl; //may not be actual objects
        }
        else
        {
            std::cout<<"no objects"<<std::endl;
        }
        //cv::resize(objects1,objects1,cv::Size(),10,10);
        //cv::resize(maskMOG2,maskMOG2,cv::Size(),8,8);
        cv::imshow("videoGray",frameGray);
        cv::imshow("MOG2",objects1);
        cv::imshow("video",frame);
        ch = cv::waitKey(30);

    }

    return 1;
}

static cv::Scalar randomColor( cv::RNG& rng )
{
    auto icolor = (unsigned) rng;
    return cv::Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}
