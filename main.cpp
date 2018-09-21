#include "gst_pipeline.hpp"
#include "helper.hpp"
#include "VisionPipeline.h"
#include "networktables/NetworkTable.h"

#define OUTPUT_VALUES true

int device = 0;
int width = 1080;
int height = 720;
int framerate = 30;
bool mjpeg = false;
int bitrate = 600000;
int port = 5805;

int main()
{
	CvCapture_GStreamer mycam;
    string read_pipeline = createReadPipelineSplit (
            device, width, height, framerate, mjpeg, 
            bitrate, videoSinkIP, port);

	mycam.open (CV_CAP_GSTREAMER_FILE, read_pipeline.c_str());
	printf ("Succesfully opened camera with dimensions: %dx%d\n",
            width, height);

	cv::Mat currentFrame;
	VisionPipeline* pipeline;
	std::vector<std::vector<cv::Point> >* contours
	cv::Rect br;
	
	NetworkTable::SetClientMode();
	NetworkTable::SetTeam(3674);
	NetworkTable::SetIPAddress("10.36.74.61");
	table = NetworkTable::GetTable("vision");
	
	while(true)
	{
		IplImage* img = mycam.retrieveFrame(0);
		currentFrame = cv::cvarrToMat(img).clone();
		pipeline->Process(currentFrame);
		contours = pipeline->GetFilterContoursOutput();
		br = boundingRect(contours[0]);
		table->PutNumber("x", br.x+br.width/2);
		table->PutNumber("y", br.y+br.height/2); 
		table->PutNumber("area", 0);
		if(OUTPUT_VALUES)
		{
			std::cout << "x: " << br.x+br.width/2 << " , y: "<< br.y+br.height/2 << std::endl;
			
		}
	}
	return 0;
}