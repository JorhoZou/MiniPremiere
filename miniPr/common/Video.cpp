#include "Video.h"

using namespace std;
using namespace cv;

Video::Video()
{
}

Video::Video(const string & video_path)
{
	this->capture(video_path);
}

Video::~Video()
{
}

bool Video::capture(const string &video_path)
{
	VideoCapture capture(video_path);
	if (!capture.isOpened()) {
		return false;
	}

	fps = capture.get(CAP_PROP_FPS);
	wSize = windowSize(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	fCnt = capture.get(CAP_PROP_FRAME_COUNT);

	Mat frame;
	while (capture.read(frame))
	{
		shared_ptr<Mat> frameP(new Mat);
		frame.copyTo(*frameP);
		frames.push_back(frameP);
	}
	capture.release();

	return true;
}

void Video::play()
{
	int slidp = 0;

	namedWindow("display", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("framenum", "display", &slidp, fCnt);

	for (int i = 0; i < fCnt; i++)
	{
		bool isEscape = false;
		setTrackbarPos("framenum", "display", slidp);
		imshow("display", *frames[i]);
		slidp++;

		switch (waitKey((double)1000 / fps)) {
		case 32:
			switch (waitKey()) {
			case 27:
				isEscape = true;
				break;
			default:
				break;
			}
			break;
		case 27:
			isEscape = true;
			break;
		}

		if (isEscape) break;
	}
	cvDestroyWindow("display");
}

bool Video::cut(int start_frame, int end_frame)
{
	if (start_frame > fCnt) {
		return false;
	}

	frames.erase(frames.begin(), frames.begin() + start_frame);
	frames.erase(frames.begin() + (end_frame - start_frame), frames.end());
	fCnt = end_frame - start_frame;
	return true;
}

bool Video::changeSpeed(double rate)
{
	if (rate < 1) {
		fps = fps * rate;
	} else {
		fCnt = (int)(double(fCnt) / rate);
		for (int i = 0; i < fCnt; i++)
		{
			frames[i] = frames[(int)(i * rate)];
		}
	}
	return true;
}

bool Video::link(shared_ptr<Video> next_video, int mode)
{
	if (wSize.width < next_video->wSize.width) {
		wSize.width = next_video->wSize.width;
	}
	if (wSize.height < next_video->wSize.height) {
		wSize.height = next_video->wSize.height;
	}

	int trans_begin = fCnt - (fCnt < 10 ? fCnt : 10);
	int trans_end = fCnt + (next_video->fCnt < 10 ? next_video->fCnt : 10);

	frames.insert(frames.end(), next_video->frames.begin(), next_video->frames.end());
	fCnt += next_video->fCnt;

	if(mode != -1) {
		this->addFilter(mode, trans_begin, trans_end);
	}
	return true;
}

bool Video::addFilter(int mode, int start_frame, int end_frame)
{
	if (start_frame == -1) {
		start_frame = 0;
	}
	if (end_frame == -1) {
		end_frame = fCnt;
	}

	for (int i = start_frame; i < end_frame; i++)
	{
		double p = (double)(i - start_frame) / (end_frame - start_frame);
		double arg = (0.5 - fabs(p - 0.5)) * 2;
		switch (mode) {
		case 0:
			filter_mirror(*frames[i]);
			break;
		case 1:
			filter_rotation(*frames[i]);
			break;
		case 2: // unavailable
			filter_feather(*frames[i], 1 - arg);
			break;
		case 3:
			filter_fudiao(*frames[i]);
			break;
		case 4:
			filter_diaoke(*frames[i]);
			break;
		case 5:
			filter_kuozhang(*frames[i]);
			break;
		case 6:
			filter_jiya(*frames[i]);
			break;
		case 7:
			filter_bolang(*frames[i]);
			break;
		case 8:
			filter_jingxiangsuofang(*frames[i], (int)(arg * 40) + 1);
			break;
		case 9:
			filter_jingxiangxuanzhuan(*frames[i], (int)(arg * 40) + 1);
			break;
		case 10:
			filter_feng(*frames[i], (int)(arg * 20) + 1);
			break;
		case 11:
			filter_xuanwo(*frames[i], arg * 20);
			break;
		case 12:
			filter_sumiao(*frames[i]);
			break;
		case 13:
			filter_huaijiu(*frames[i]);
			break;
		case 14:
			filter_qiangguang(*frames[i]);
			break;
		default:
			break;
		}
	}
	return true;
}

bool Video::addSubtitle(const string &text, int start_frame, int end_frame, double x, double y, double r, double g, double b, double fontSize, bool isbold)
{
	if (start_frame > fCnt) {
		return false;
	}

	for (int i = start_frame; i < end_frame; i++)
	{
		putText(*frames[i], text, Point(x, y), FONT_HERSHEY_COMPLEX, fontSize, Scalar(r, g, b), isbold ? 2 : 1, 8, 0);
	}
	return true;
}

bool Video::addSticker(const string& image_path, int start_frame, int end_frame, double pos_x, double pos_y)
{
	Mat image = imread(image_path);

	for (int i = start_frame; i < end_frame; i++)
	{
		int width, height;
		if ((*frames[i]).rows < image.rows + pos_y) {
			height = (*frames[i]).rows - pos_y;
		} else {
			height = image.rows;
		}
		if ((*frames[i]).cols < image.cols + pos_x) {
			width = (*frames[i]).cols - pos_x;
		} else {
			width = image.cols;
		}

		for (int y = 0; y < height; y++)
		{
			uchar* bkgP = (*frames[i]).ptr<uchar>(y);
			uchar* imgP = image.ptr<uchar>(y);
			for (int x = 0; x < width; x++)
			{
				bkgP[3 * (int)(x + pos_x)] = imgP[3 * x];
				bkgP[3 * (int)(x + pos_x) + 1] = imgP[3 * x + 1];
				bkgP[3 * (int)(x + pos_x) + 2] = imgP[3 * x + 2];
			}
		}
	}
	return true;
}

bool Video::writer(const string &video_path)
{
	VideoWriter writer(video_path, CV_FOURCC('M', 'J', 'P', 'G'), fps, Size(wSize.width, wSize.height));
	for (int i = 0; i < fCnt; i++) {
		writer.write(*frames[i]);
	}
	writer.release();
	return true;
}
