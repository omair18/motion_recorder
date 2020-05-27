# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM gcc:9.3.0

## These commands will setup the OpenCV 4 environment required to compile and run the program
RUN apt-get update && apt-get install -y build-essential cmake git pkg-config libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev openexr libatlas-base-dev python3-dev python3-numpy libtbb2 libtbb-dev libdc1394-22-dev

RUN mkdir /usr/src/opencv_build
WORKDIR /usr/src/opencv_build
RUN git clone https://github.com/opencv/opencv.git
RUN git clone https://github.com/opencv/opencv_contrib.git
WORKDIR  /usr/src/opencv_build/opencv
RUN mkdir build
WORKDIR /usr/src/opencv_build/opencv/build

RUN cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D OPENCV_GENERATE_PKGCONFIG=ON -D OPENCV_EXTRA_MODULES_PATH=/usr/src/opencv_build/opencv_contrib/modules/bgsegm -D BUILD_EXAMPLES=ON ..

RUN make -j4
RUN make install


RUN mkdir /var/video_files
RUN mkdir /var/video_files/inputs
RUN mkdir /var/video_files/outputs


# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/motion-video-recorder
WORKDIR /usr/src/motion-video-recorder

# This command compiles your app using GCC, adjust for your source codes
RUN g++ main.cpp -o recorder `pkg-config --cflags --libs opencv4` -lpthread -std=c++11

LABEL Name=motion_recorder Version=0.0.1
