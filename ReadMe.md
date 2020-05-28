# Motion Video Recorder

Motion Video Recorder is a simple command line tool written in C++ (Standard C++11) using OpenCV 4.3.0.

## Requirements

- Docker installed

## Files

- `mvc.cpp` : Contains the motionVideoRecorder function that performs the task.
- `main.cpp` : Contains the main program that parses arguments and runs motionVideoRecorder in parallel threads. 
- `utils.cpp` : Contains structures and utility functions used. 
- `Dockerfile` : Contains the commands to build a docker image

## Usage

1. Building the docker image: In order to build the docker image run the following command in the directory containing the files mentioned above: `docker build -t motion_recorder .`

2. Setting up the shared directory folder: In your working directory run the following commands:
   `mkdir video_files video_files/inputs video_files/outputs`. This directory will be used to share data between the host machine and the docker container.

3. Running the docker container: To run the container and enter a working environment with all dependcies installed, run the following command: `docker run -it motion_recorder -v ~/Desktop/video_files:/var/video_files /bin/bash` .

4. Linking the shared libraries: Once the terminal has been loaded run the following command: `export LD_LIBRARY_PATH=/usr/local/lib`

5. Running the program: Finally, run the program using the following command: `./recorder` with the appropriate parameters

6. The processed output files should be in `video_files/outputs` in the host machine. Copy those to another directory in order to view them.

Parameters for the `./recorder` program

        -h, --help (value:true)
                Print usage
        --input
                FILENAME of the video. This file should be placed in the video_files/inputs shared directory
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

The program was tested with a 29 second video with 30 FPS. The resultant video was 22 second long and the average frames processed per second were 22. The program was run with num_threads = 4.

\n
