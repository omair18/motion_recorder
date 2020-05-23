// motionVideoRecorder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <sstream>
#include <chrono>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

#include "utils.cpp"

using namespace cv;
using namespace std;
using namespace std::chrono;
using namespace cv::bgsegm;


void *motionVideoRecorder(void *all_args) {
    auto start = high_resolution_clock::now();
    mvcArgs *parsed_args;
    parsed_args = (struct mvcArgs *)(all_args);

    string input_file = parsed_args->input_file;
    string output_file = parsed_args->output_file;
    int thresh = parsed_args->thresh;

    int frames_processed = 0;
    VideoCapture input_video(samples::findFile(input_file));
    if (!input_video.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open: " << input_file << endl;
        return;
    }
    
    int ex = static_cast<int>(input_video.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form
    // Transform from int to char via Bitwise operators
    char EXT[] = { (char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0 };
    Size S = Size(640,480);

    int input_fps = input_video.get(CAP_PROP_FPS);
    cout << "Input Video FPS: " << input_fps;
    VideoWriter output_video(output_file,ex,input_fps,S,true);
    if (!output_video.isOpened()) {
        cout << "Could not open the output video for write: " << output_file << endl;
        return;
    }
    
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorCNT();

    Mat frame, fg_mask;
    while (true) {
        input_video >> frame;
        if (frame.empty()) {
            break;
        }
        resize(frame, frame, Size(640,480));
        pBackSub->apply(frame, fg_mask);
        
        double white_area = sum(fg_mask)[0] / 255;
        if (white_area > (thresh)) {
            // Motion detected
            output_video.write(frame);
        }
        frames_processed++;
    }
    input_video.release();
    output_video.release();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Finished writing: " << output_file << endl;
    cout << "Frame processed per second: " << double(frames_processed)/duration.count() << endl;
    return;
}

