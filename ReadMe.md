# Motion Video Recorder

Motion Video Recorder is a simple command line tool written in C++ (Standard C++11) using OpenCV 4.3.0. 

## Requirements
- C++ installed
-  OpenCV installed and configured with C++
- bgsegm module from OpenCV-Contrib repository installed and configured
- A linux environment

## Files
- ```mvc.cpp``` : Contains the motionVideoRecorder function that performs the task.
- ```main.cpp``` : Contains the main program that parses arguments and runs motionVideoRecorder in parallel threads.
- ```utils.cpp` : Contains structures and utility functions used.

## Usage
Compiling the program
```
g++ main.cpp -o recorder `pkg-config --cflags --libs opencv4` -lpthread
```
Parameters

        -h, --help (value:true)
                Print usage
        --input
                Path to a video
        --num_threads (value:4)
                Number of threads to run program on
        --thresh (value:5000)
                Threshold value to determine whether a motion was detected in a frame or not. This will be compared to the count of white pixels in the foreground mask

## Methodology
Background subtraction is used to create a foreground mask which is then in turn used to detect motion within a frame. There are different algorithms for background subtraction, but in this project I have used BackgroundSubtractorCNT [[Link](https://github.com/sagi-z/BackgroundSubtractorCNT)] due to its performance gains on low end machines. The program works in the following way:
1. Read input video file
2. Loop over the frames of input video
3. Use each frame to update the background model and create a foreground mask for the frame
4. Count the number of white pixels in the foreground mask and compare it with the provided threshold to determine whether there was a motion in this frame or not
5. If there was a motion then write frame to the output file

This function is run on N parallel threads where N is the number of threads argument that is provided when running the function.

## Results
The program was tested with a 29 second video with 30 FPS. The resultant video was 22 second long and the average frames processed per second were 19.6. The program was run with num_threads = 4.

