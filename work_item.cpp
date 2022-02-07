#include "work_item.h"
#include "work.h"
#include "QImageReader"
#include "QFile"
#include "QFileInfo"
#include "QApplication"
#include "QProcess"

QMutex Work_Item::m_stat;


#ifdef __linux__
    extern "C"
    {
    #include <libavdevice/avdevice.h>
    #include <libavutil/imgutils.h>
    #include <libavformat/avformat.h>
    #include <libavutil/opt.h>
    }
#elif _WIN32

#else

#endif

Work_Item::Work_Item()
{
    stop=false;
    col=0;
    connect(this,&Work_Item::obrItem_emit,this,&Work_Item::obrItem);
    Mythread=new QThread();
    moveToThread(Mythread);
    Mythread->start();
}

int Work_Item::getCol()
{
    m.lock();
    int i=col;
    m.unlock();
    return i;
}

void Work_Item::addCol()
{
    m.lock();
    col++;
    m.unlock();
}

#ifdef __linux__
int save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, char *fileout) {
    AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    if (!jpegCodec) {
        return -1;
    }
    AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
    if (!jpegContext) {
        return -1;
    }

    jpegContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
    jpegContext->codec_id = AV_CODEC_ID_MJPEG;
    jpegContext->time_base.num = pCodecCtx->time_base.num;
    jpegContext->time_base.den = pCodecCtx->time_base.den;

    //jpegContext->pix_fmt = pCodecCtx->pix_fmt;
    jpegContext->height = pFrame->height;
    jpegContext->width = pFrame->width;

    if (avcodec_open2(jpegContext, jpegCodec, NULL) < 0) {
        return -1;
    }
    FILE *JPEGFile;

    AVPacket packet;
    packet.data = NULL;
    packet.size = 0;
    av_init_packet(&packet);
    int gotFrame;

    if (avcodec_encode_video2(jpegContext, &packet, pFrame, &gotFrame) < 0) {
        return -1;
    }

    JPEGFile = fopen(fileout, "wb");
    fwrite(packet.data, 1, packet.size, JPEGFile);
    fclose(JPEGFile);

    av_free_packet(&packet);
    avcodec_close(jpegContext);
    return 0;
}

int readsave_frames(int videoStreamIdx
                , AVFormatContext *pFormatCtx
                , AVCodecContext  *pCodecCtx
                , char *fileout
)
{
    int             i;
    AVPacket        packet;
    int             frameFinished;
    AVFrame        *pFrame;
    // Allocate video frame
    pFrame = av_frame_alloc();
    int j=0;
    for(i=0; av_read_frame(pFormatCtx, &packet) >= 0;)
    {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStreamIdx) {
            i++;

            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            if(frameFinished)
            {
                j++;
                // Write pixel data
                save_frame_as_jpeg(pCodecCtx, pFrame, fileout);
                av_free_packet(&packet);
                // Free the packet that was allocated by av_read_frame
                av_free(pFrame);
                return 0;
            }
        }
    }
    return 0;
}

int openVideo(char *url,char *fileout)
{
    AVFormatContext *pFormatCtx;
    int             videoStreamIdx;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    // Register all formats and codecs
    av_register_all();
    avdevice_register_all();
    avcodec_register_all();

    pFormatCtx = avformat_alloc_context();

    // Open video file
    if(avformat_open_input(&pFormatCtx, url, 0, NULL) != 0)
    {
        printf("avformat_open_input failed: Couldn't open file\n");
        return -1; // Couldn't open file
    }

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        printf("avformat_find_stream_info failed: Couldn't find stream information\n");
        return -1; // Couldn't find stream information
    }

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, url, 0);


    // Find the first video stream
    {
        int i = 0;
        videoStreamIdx=-1;
        for(i=0; i<pFormatCtx->nb_streams; i++)
        {
            qDebug() << pFormatCtx->streams[i]->codec->codec_type;
            if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) { //CODEC_TYPE_VIDEO
                videoStreamIdx=i;
                break;
            }
        }
    }
    // Check if video stream is found
    if(videoStreamIdx==-1)
        return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtx = pFormatCtx->streams[videoStreamIdx]->codec;

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder( pCodecCtx->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }

    // Open codec
    if( avcodec_open2(pCodecCtx, pCodec, NULL) < 0 )
        return -1; // Could not open codec

    // Read frames and save them to disk
    if (readsave_frames(videoStreamIdx, pFormatCtx, pCodecCtx,fileout) < 0)
    {
        return -1;
    }

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
}
#elif _WIN32

#else

#endif

void funIMG(QString fileName,QImage& img)
{
    QByteArray b;
    for ( int row = 0; row < img.height(); ++row )
        for ( int col = 0; col < img.width(); ++col )
        {
            char gray = qGray(img.pixel(row, col));
            b.append(gray);
        }
    Work::insertMap(fileName,b);
}

void Work_Item::obrItem(QStringList fileNamess)
{
    if(stop)
        return;
    foreach (QString fileName, fileNamess)
    {
        if(stop)
            return;
        bool is_est=Work::contMap(fileName);
        int n=fileName.lastIndexOf(".");
        QString suf;
        if(n>0)
        {
            QString s=fileName;
            suf=s.mid(n+1);
        }
        suf=suf.toLower();
        if(suf=="avi" || suf=="mp4" || suf=="3gp"
                || suf=="mov" || suf=="m4v" || suf=="mkv")
        {

            if(!is_est)
            {
                #ifdef __linux__
                QString ss=Work::D+"000";
                m_stat.lock();
                openVideo(fileName.toUtf8().data(),QString(ss).toUtf8().data());
                QImage img(ss);
                m_stat.unlock();
                funIMG(fileName,img);
                #elif _WIN32

                #else

                #endif
            }
            else
            {
                Work::insertMap_prov(fileName);
            }
        }
        else
        {
            if(!is_est)
            {
                QImageReader imageReader(fileName);
                if (imageReader.supportsOption(QImageIOHandler::Size))
                {
                    m_stat.lock();
                    imageReader.setScaledSize(QSize(16, 16));
                    QImage img=imageReader.read();
                    m_stat.unlock();
                    funIMG(fileName,img);
                }
                else
                {
                    qDebug() << fileName;
                }
            }
            else
            {
                Work::insertMap_prov(fileName);
            }
        }
    }
    m.lock();
    col--;
    m.unlock();
    emit MyEmit1();
}
