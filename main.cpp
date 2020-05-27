#include "mvc.cpp"
#include <pthread.h>

using namespace std;
using namespace cv;

const char *params = "{ help h         |           | Print usage }"
                     "{ input          |  | Full filename of the video. Make sure directory structure is followed }"
                     "{ thresh           | 5000      | Threshold value to determine whether a motion was detected in a frame or not. This will be compared to the count of white pixels in the foreground mask }"
                     "{ num_threads | 4 | Number of threads to run program on }";

int main(int argc, char *argv[])
{

    CommandLineParser parser(argc, argv, params);
    parser.about("This is a program implemented using OpenCV that detects motion"
                 " in videos using background subtraction (BackgroundSubtractionCNT from opencv-contrib bgsegm module)"
                 "and will then save N streams of output videos containing motion only where N is the number of threads provided.\n");

    if (parser.has("help"))
    {
        parser.printMessage();
        exit(1);
    }

    string input = parser.get<string>("input");
    int N = parser.get<int>("num_threads");
    int thresh = parser.get<int>("thresh");

    string filename = split_string(input, ".").front();
    pthread_t threads[N];
    mvcArgs thread_args[N];

    int rc;
    for (int i = 0; i < N; i++)
    {
        thread_args[i].input_file = "/var/video_files/inputs/" + input;
        thread_args[i].output_file = "/var/video_files/outputs/" + filename + to_string(i) + ".avi";
        thread_args[i].thresh = thresh;

        rc = pthread_create(&threads[i], NULL, motionVideoRecorder, (void *)&thread_args[i]);

        if (rc)
        {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    cout << "Exiting..." << endl;
    return 0;
}