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

using namespace cv;
using namespace std;
using namespace std::chrono;
using namespace cv::bgsegm;

int motionVideoRecorder(string input_file, string output_file, int thresh) {
    auto start = high_resolution_clock::now();
    int frames_processed = 0;
    VideoCapture input_video(samples::findFile(input_file));
    if (!input_video.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open: " << input_file << endl;
        return -1;
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
        return -1;
    }
    
    Mat frame, fg_mask;
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorCNT();
    while (true) {
        input_video >> frame;
        pBackSub->apply(frame, fg_mask);
        if (frame.empty()) {
            break;
        }
        resize(frame, frame, Size(640,480));
        

        double sum_delta = sum(fg_mask)[0] / 255;
        //std::cout << "Sum Delta: " << sum_delta << endl;
        if (sum_delta > (thresh)) {
            // Motion detected
            std::cout << "Writing frame to video" << endl;
            output_video.write(frame);
        }
        frames_processed++;
    }
    input_video.release();
    output_video.release();
    std::cout << "Finished writing: " << output_file << endl;
    return frames_processed;
}

int main()
{
    string input = "inputs/video.mp4";
    // string output = "C:\\Users\\umair\\Desktop\\motion_detector\\second_video1.avi";
    int N = 4;
    std::vector<std::thread> threads;
    for(int i=0;i<N;i++){
        output_file = "output_video" + str(i) + ".avi";
        threads.push_back(std::thread(input,output_file,30000));
    }

    for(auto& t : threads){
        t.join();
    }
    
    int frames_processed = motionVideoRecorder(input, output, 30000);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Frame processed per second: " << double(frames_processed)/duration.count() << endl;
    
}

