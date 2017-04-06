#include "stereovision.h"

#include <iostream>

using namespace cv;
using namespace std;

StereoVision::StereoVision(QObject *parent) :
    QObject(parent),
    m_captureTimer(new QTimer(this))
{
    namedWindow("frame1");
    namedWindow("frame2");
    namedWindow("disparity");
}

StereoVision::~StereoVision()
{
    destroyAllWindows();
}

void StereoVision::startRecording()
{
    connect(m_captureTimer, &QTimer::timeout,
            this, &StereoVision::captureFrame);

    calibrateCamera();

    m_camera.startCameras();
    m_captureTimer->start(40);
}

void StereoVision::captureFrame()
{
    auto frames = m_camera.capture();

    if (!frames.first.empty())
        imshow("frame1", frames.first);
    else
        return;

    if (!frames.second.empty())
        imshow("frame2", frames.second);
    else
        return;

    emit provideImage(frames.first);
    processFrames(frames);
}

void StereoVision::processFrames(const FramePair &frames)
{
    Frame left, right, leftRect, rightRect, disparity;
    cvtColor(frames.first, left, COLOR_RGB2GRAY);
    cvtColor(frames.second, right, COLOR_RGB2GRAY);

    if(left.empty() || right.empty())
    {
        qDebug() << "Error: captured images are not valid.";
        return;
    }

    remap(left, leftRect, m_stereoSystem[0], m_stereoSystem[1], INTER_LINEAR);
    remap(right, rightRect, m_stereoSystem[2], m_stereoSystem[3], INTER_LINEAR);

    SBMPtr sbm = StereoBM::create(128, 41);
    sbm->setPreFilterSize(41);
    sbm->setPreFilterCap(31);
    sbm->setMinDisparity(-64);
    sbm->setTextureThreshold(10);
    sbm->setUniquenessRatio(15);

    sbm->compute(leftRect, rightRect, disparity);

    Frame dispDisparity;
    normalize(disparity, dispDisparity, 0, 255, CV_MINMAX, CV_8U);
    imshow("disparity" , dispDisparity);
}

void StereoVision::calibrateCamera()
{
    Size boardSize(10, 7);
    vector<string> imagelist{
        "/home/nador/devel/motiondetector/stereocalib/left1.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right1.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left2.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right2.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left3.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right3.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left4.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right4.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left5.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right5.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left6.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right6.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left7.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right7.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left8.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right8.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left9.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right9.jpg",
        "/home/nador/devel/motiondetector/stereocalib/left10.jpg",
        "/home/nador/devel/motiondetector/stereocalib/right10.jpg"
    };

    if( imagelist.size() % 2 != 0 )
    {
        qDebug() << "Error: the image list contains odd (non-even) number of elements";
        return;
    }

    const int maxScale = 2;
    const float squareSize = 25.f;  // Set this to your actual square size

    // ARRAY AND VECTOR STORAGE:
    vector<vector<Point2f> > imagePoints[2];
    vector<vector<Point3f> > objectPoints;
    Size imageSize;

    int i, j, k, nimages = (int)imagelist.size()/2;

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    vector<string> goodImageList;

    for( i = j = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            const string& filename = imagelist[i*2+k];
            Frame img = imread(filename, 0);
            if(img.empty())
                break;
            if( imageSize == Size() )
                imageSize = img.size();
            else if( img.size() != imageSize )
            {
                qDebug() << "The image " << filename.c_str() << " has the size different from the first image size. Skipping the pair";
                break;
            }
            bool found = false;
            vector<Point2f>& corners = imagePoints[k][j];
            for( int scale = 1; scale <= maxScale; scale++ )
            {
                Frame timg;
                if( scale == 1 )
                    timg = img;
                else
                    resize(img, timg, Size(), scale, scale);
                found = findChessboardCorners(timg, boardSize, corners,
                                              CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
                if( found )
                {
                    if( scale > 1 )
                    {
                        Mat cornersMat(corners);
                        cornersMat *= 1./scale;
                    }
                    break;
                }
            }
            if( !found )
                break;
            cornerSubPix(img, corners, Size(11,11), Size(-1,-1),
                         TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,
                                      30, 0.01));
        }
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[i*2]);
            goodImageList.push_back(imagelist[i*2+1]);
            j++;
        }
    }
    qDebug() << j << "pairs have been successfully detected.";
    nimages = j;
    if( nimages < 2 )
    {
        qDebug() << "Error: too little pairs to run the calibration.";
        return;
    }

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    objectPoints.resize(nimages);

    for( i = 0; i < nimages; i++ )
    {
        for( j = 0; j < boardSize.height; j++ )
            for( k = 0; k < boardSize.width; k++ )
                objectPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
    }

    qDebug() << "Running stereo calibration...";

    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = Mat::eye(3, 3, CV_64F);
    Mat R, T, E, F;

    double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
            cameraMatrix[0], distCoeffs[0],
            cameraMatrix[1], distCoeffs[1],
            imageSize, R, T, E, F,
            CALIB_FIX_ASPECT_RATIO +
            CALIB_ZERO_TANGENT_DIST +
            CALIB_SAME_FOCAL_LENGTH,
            TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );
    qDebug() << "Done with RMS error:" << rms;

    Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];

    stereoRectify(cameraMatrix[0], distCoeffs[0],
                  cameraMatrix[1], distCoeffs[1],
                  imageSize, R, T, R1, R2, P1, P2, Q,
                  CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

    Mat rmap[2][2];

    //Precompute maps for cv::remap()
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    m_transformationMatrix = Q;
    m_cameras.push_back(cameraMatrix[0]);
    m_cameras.push_back(cameraMatrix[1]);
    m_stereoSystem.push_back(rmap[0][0]);
    m_stereoSystem.push_back(rmap[0][1]);
    m_stereoSystem.push_back(rmap[1][0]);
    m_stereoSystem.push_back(rmap[1][1]);

    cout << "Transformation matrix Q:" << endl
         << m_transformationMatrix << endl;
}
