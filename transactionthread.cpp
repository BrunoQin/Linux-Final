#include <QtGui>

#include "transactionthread.h"

Transaction * const endTransaction = 0;

FlipTransaction::FlipTransaction(Qt::Orientation orientation)
{
    this->orientation = orientation;
}

QImage FlipTransaction::apply (QImage &image)
{
    return image.mirrored (orientation == Qt::Horizontal, orientation == Qt::Vertical);  //翻转操作
}

QString FlipTransaction::message ()
{
    if(orientation == Qt::Horizontal)
        return QObject::tr ("Flipping image horiontally...");
    else
        return QObject::tr ("Flipping image vertically...");
}

ResizeTransaction::ResizeTransaction(const QSize &size)
{
    this->size = size;
}

QString ResizeTransaction::message ()
{
    return QObject::tr ("Resizing image...");
}

QImage ResizeTransaction::apply (QImage &image)
{
    return image.scaled (size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //忽略长宽比，平滑变换
}

GrayTransaction::GrayTransaction()
{
}

QString GrayTransaction::message ()
{
    return QObject::tr ("graying image...");
}

QImage GrayTransaction::apply (QImage &image)
{
        QImage ImageAverage;
        ImageAverage=QImage(image.width(),image.height(),QImage::Format_RGBA8888_Premultiplied);
        int i,j;
        int width,height;
        width=image.width();
        height=image.height();
        QRgb rgb;
        int r[256],g[256],b[256];//原图各个灰度数量的统计
        int rtmp,gtmp,btmp,rj,gj,bj;
        float rPro[256],gPro[256],bPro[256];//原图各个灰度级的概率
        float rTemp[256],gTemp[256],bTemp[256];//均衡化后各个灰度级的概率
        int rJun[256],gJun[256],bJun[256];//均衡化后对应像素的值
        memset(r,0,sizeof(r));
        memset(g,0,sizeof(g));
        memset(b,0,sizeof(b));

        //获取原图各个灰度的数量
        for(i=0;i<width;i++)
        {
            for(j=0;j<height;j++)
            {
                rgb=image.pixel(i,j);
                r[qRed(rgb)]++;
                g[qGreen(rgb)]++;
                b[qBlue(rgb)]++;
            }
        }

        //获取原图各个灰度级的概率
        for(i=0;i<256;i++)
        {
            rPro[i]=(r[i]*1.0)/(width*height);
            gPro[i]=(g[i]*1.0)/(width*height);
            bPro[i]=(b[i]*1.0)/(width*height);
        }

        //均衡化后各个灰度级的概率，同时获取均衡化后对应像素的值
        for(i=0;i<256;i++)
        {
            if(i==0)
            {
                rTemp[0]=rPro[0];
                gTemp[0]=gPro[0];
                bTemp[0]=bPro[0];
            }
            else
            {
                rTemp[i]=rTemp[i-1]+rPro[i];
                gTemp[i]=gTemp[i-1]+gPro[i];
                bTemp[i]=bTemp[i-1]+bPro[i];
            }
            rJun[i]=(int)(255*rTemp[i]+0.5);
            gJun[i]=(int)(255*gTemp[i]+0.5);
            bJun[i]=(int)(255*bTemp[i]+0.5);
        }

        for(i=0;i<width;i++)
        {
            for(j=0;j<height;j++)
            {
                rgb=image.pixel(i,j);
                rtmp=qRed(rgb);
                gtmp=qGreen(rgb);
                btmp=qBlue(rgb);
                rj=rJun[rtmp];
                gj=gJun[gtmp];
                bj=bJun[btmp];
                ImageAverage.setPixel(i,j,qRgb(rj,gj,bj));
            }
        }
       // ui->label_2->resize(ImageAverage.width(),ImageAverage.height());
       // ui->label_2->setPixmap(QPixmap::fromImage(ImageAverage));
    return ImageAverage;
}
RUIHUATransaction::RUIHUATransaction()
{
}

QString RUIHUATransaction::message ()
{
    return QObject::tr ("RUIHUAing image...");
}


QImage RUIHUATransaction::apply (QImage &image)
{
    QImage ImageBetter;
    ImageBetter=QImage(image.width(),image.height(),QImage::Format_ARGB32);
    int i,j;
    int r,g,b,tmpR,tmpG,tmpB;
    QRgb rgb00,rgb01,rgb10;
    int width=image.width(),height=image.height();
    int betterValue=70;
    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            if(image.valid(i,j)&&image.valid(i+1,j)&&image.valid(i,j+1))
            {
                rgb00=image.pixel(i,j);
                rgb01=image.pixel(i,j+1);
                rgb10=image.pixel(i+1,j);
                r=qRed(rgb00);
                g=qGreen(rgb00);
                b=qBlue(rgb00);
                tmpR=abs(qRed(rgb00)-qRed(rgb01))+abs(qRed(rgb00)-qRed(rgb10));
                tmpG=abs(qGreen(rgb00)-qGreen(rgb01))+abs(qGreen(rgb00)-qGreen(rgb10));
                tmpB=abs(qBlue(rgb00)-qBlue(rgb01))+abs(qBlue(rgb00)-qBlue(rgb10));
                if((tmpR+120)<255)
                {
                    if(tmpR>betterValue)
                    {
                        r=tmpR+120;
                    }
                }
                else
                {
                    r=255;
                }

                if((tmpG+120)<255)
                {
                    if(tmpG>betterValue)
                    {
                        g=tmpG+120;
                    }
                }
                else
                {
                    g=255;
                }

                if((tmpB+120)<255)
                {
                    if(tmpB>betterValue)
                    {
                        b=tmpB+120;
                    }
                }
                else
                {
                    b=255;
                }
                ImageBetter.setPixel(i,j,qRgb(r,g,b));
        }
        }
    }
    return ImageBetter;
    //ui->label_2->resize(ImageBetter.width(),ImageBetter.height());
    //ui->label_2->setPixmap(QPixmap::fromImage(ImageBetter));
    //delete image;
    //delete dialog;
}

PINGHUATransaction::PINGHUATransaction()
{
}

QString PINGHUATransaction::message ()
{
    return QObject::tr ("PINGHUAing image...");
}


QImage PINGHUATransaction::apply (QImage &image)
{
    int i,j,k,num;
    int width=image.width(),height=image.height();
    QImage MoreClear;
    MoreClear=QImage(width,height,QImage::Format_ARGB32);
    num=width*height;
    QRgb tmpPixel[num];
    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            k=i*height+j;
            tmpPixel[k]=image.pixel(i,j);
        }
    }
    MoreClearPic(tmpPixel,1,width,height);
    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            k=i*height+j;
            MoreClear.setPixel(i,j,tmpPixel[k]);
        }
    }
    return MoreClear;
}

void PINGHUATransaction::MoreClearPic(QRgb *tmpPixel,int yanKind,int width,int height)
{
    int i,j,k;
    int num=width*height;
    int rtmpPixel[num],gtmpPixel[num],btmpPixel[num];
    for(i=0;i<width;i++)
    {
       for(j=0;j<height;j++)
       {
           k=i*height+j;
           rtmpPixel[k]=qRed(tmpPixel[k]);
           gtmpPixel[k]=qGreen(tmpPixel[k]);
           btmpPixel[k]=qBlue(tmpPixel[k]);
       }
    }
    if(yanKind==1)
    {
        YanMo1(rtmpPixel,width,height);
        YanMo1(gtmpPixel,width,height);
        YanMo1(btmpPixel,width,height);
    }
    for(i=1;i<width-1;i++)
    {
        for(j=1;j<height-1;j++)
        {
            k=i*height+j;
            tmpPixel[k]=qRgb(rtmpPixel[k],gtmpPixel[k],btmpPixel[k]);
        }
    }
}
//邻域平均法平滑化函数
void PINGHUATransaction::YanMo1(int *tmpPixel, int width, int height)
{
    float Template[9]={1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9};
    int i,j;
    for(i=1;i<width-1;i++)
    {
       for(j=1;j<height-1;j++)
       {
         tmpPixel[i*height+j]=tmpPixel[(i-1)*height+j-1]*Template[0]+tmpPixel[(i-1)*height+j]*Template[1]+
         tmpPixel[(i-1)*height+j+1]*Template[2]+tmpPixel[i*height+j-1]*Template[3]+tmpPixel[i*height+j]*Template[4]
         +tmpPixel[i*height+j+1]*Template[5]+tmpPixel[(i+1)*height+j-1]*Template[6]+tmpPixel[(i+1)*height+j]*Template[7]
         +tmpPixel[(i+1)*height+j+1]*Template[8];
       }
    }
}

SOBELTransaction::SOBELTransaction()
{
}

QString SOBELTransaction::message ()
{
    return QObject::tr ("SOBELing image...");
}


QImage SOBELTransaction::apply (QImage &image)
{
    QImage SobelIma;
    SobelIma=QImage(image.width(),image.height(),QImage::Format_ARGB32);
    Template(SobelIma, image);
    return SobelIma;
}

void SOBELTransaction::Template(QImage &SobelImage, QImage &image)
{
    int width=image.width(),height=image.height();
    int pixelNum=width*height;
    int i,j,k,l;
    float rResult,gResult,bResult;
    float sobel1[9]={1,2,1,0,0,0,-1,-2,-1},sobel2[9]={1,0,-1,2,0,-2,1,0,-1};
    QRgb pixelOld[pixelNum],pixelTemp1[pixelNum],pixelTemp2[pixelNum];
    int rtmp,gtmp,btmp;
    memset(pixelTemp1,255,pixelNum);
    memset(pixelTemp2,255,pixelNum);
    QRgb tmpRgb;
    for(j=0;j<height;j++)
    {
        for(i=0;i<width;i++)
        {
            tmpRgb=image.pixel(i,j);
            pixelOld[j*width+i]=tmpRgb;
        }
    }
    for(j=1;j<height-1;j++)
    {
        for(i=1;i<width-1;i++)
        {
            rResult=0;
            gResult=0;
            bResult=0;
            for(k=0;k<3;k++)
            {
                for(l=0;l<3;l++)
                {
                    rResult+=qRed(pixelOld[(j-1+k)*width+(i-1+l)])*sobel1[k*3+l];
                    gResult+=qGreen(pixelOld[(j-1+k)*width+(i-1+l)])*sobel1[k*3+l];
                    bResult+=qBlue(pixelOld[(j-1+k)*width+(i-1+l)])*sobel1[k*3+l];
                }
            }
            rResult=(float)fabs(rResult);
            gResult=(float)fabs(gResult);
            bResult=(float)fabs(bResult);
            if(rResult>255)
            {
                rtmp=255;
            }
            else
                rtmp=(int)(rResult+0.5);
            if(gResult>255)
                gtmp=255;
            else
                gtmp=(int)(gResult+0.5);
            if(bResult>255)
                btmp=255;
            else
                btmp=(int)(bResult+0.5);
            pixelTemp1[j*width+i]=qRgb(rtmp,gtmp,btmp);
        }
    }

    for(j=1;j<height-1;j++)
    {
        for(i=1;i<width-1;i++)
        {
            rResult=0;
            gResult=0;
            bResult=0;
            for(k=0;k<3;k++)
            {
                for(l=0;l<3;l++)
                {
                    rResult+=qRed(pixelOld[(j-1+k)*width+(i-1+l)])*sobel2[k*3+l];
                    gResult+=qGreen(pixelOld[(j-1+k)*width+(i-1+l)])*sobel2[k*3+l];
                    bResult+=qBlue(pixelOld[(j-1+k)*width+(i-1+l)])*sobel2[k*3+l];
                }
            }
            rResult=(float)fabs(rResult);
            gResult=(float)fabs(gResult);
            bResult=(float)fabs(bResult);
            if(rResult>255)
                rtmp=255;
            else
                rtmp=(int)(rResult+0.5);
            if(gResult>255)
                gtmp=255;
            else
                gtmp=(int)(gResult+0.5);
            if(bResult>255)
                btmp=255;
            else
                btmp=(int)(bResult+0.5);
            pixelTemp2[j*width+i]=qRgb(rtmp,gtmp,btmp);
        }
    }

    for(i=0;i<pixelNum;i++)
    {
        if(pixelTemp2[i]>pixelTemp1[i])
            pixelTemp1[i]=pixelTemp2[i];
    }
    for(j=0;j<height;j++)
    {
        for(i=0;i<width;i++)
        {
            SobelImage.setPixel(i,j,pixelTemp1[j*width+i]);
        }
    }
}

ConvertDepthTransaction::ConvertDepthTransaction(int depth)
{
    this->depth = depth;
}

QImage ConvertDepthTransaction::apply (QImage &image)
{
    QImage::Format format;

    switch(depth)
    {
    case 1:
        format = QImage::Format_Mono;
        break;
    case 8:
        format = QImage::Format_Indexed8;
        break;
    case 32:
    default:
        format = QImage::Format_RGB32;
    }

    return image.convertToFormat (format);
}

QString ConvertDepthTransaction::message ()
{
    return QObject::tr ("Converting image depth...");
}

ConvertGOSSTransaction::ConvertGOSSTransaction(int count)
{
this->count=count;
}

QImage ConvertGOSSTransaction::apply (QImage &image)
{
    width=(image.width()*32+31)/32;
    height=image.height();
   uchar *data=image.bits();
   double sigma=8;
    int radius=3;
    double *gaussMatrix, gaussSum = 0.0, _2sigma2 = 2 * sigma * sigma;
        int x, y, xx, yy, xxx, yyy;
        double *pdbl, a, r, g, b, d;
        unsigned char *bbb, *pout, *poutb;
        poutb =(unsigned char*)malloc(image.byteCount());
    pout = poutb;
        gaussMatrix = (double *)malloc( (radius * 2 + 1) * (radius * 2 + 1) * sizeof(double)*2);
    pdbl = gaussMatrix;
        for (y = -radius; y <= radius; y++) {
            for (x = -radius; x <= radius; x++) {
                a = exp(-(double)(x * x + y * y) / _2sigma2);
                *pdbl++ = a;
                gaussSum += a;
            }
        }
        pdbl = gaussMatrix;
        for (y = -radius; y <= radius; y++) {
            for (x = -radius; x <= radius; x++) {
                *pdbl++ /= gaussSum;
            }
        }
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                a = r = g = b = 0.0;
                pdbl = gaussMatrix;
                for (yy = -radius; yy <= radius; yy++) {
                    yyy = y + yy;
                    if (yyy >= 0 && yyy < height) {
                        for (xx = -radius; xx <= radius; xx++) {
                            xxx = x + xx;
                            if (xxx >= 0 && xxx < width) {
                                bbb = (unsigned char *)&data[xxx*4 + yyy * width*4];
                                d = *pdbl;
                                b += d * bbb[0];
                                g += d * bbb[1];
                                r += d * bbb[2];
                                a += d * bbb[3];
                                a +=0.2;
                            }
                            pdbl++;
                        }
                    } else {
                        pdbl += (radius * 2 + 1);
                    }
                }
                *pout++ = (unsigned char)b;
                *pout++ = (unsigned char)g;
                *pout++ = (unsigned char)r;
                *pout++ = (unsigned char)a;
            }
        }
        memcpy(data, poutb, image.byteCount() );
        free(gaussMatrix);
        free(poutb);




    return image;
}

QString ConvertGOSSTransaction::message ()
{
    return QObject::tr ("Converting image Goss...");
}



TransactionThread::TransactionThread(QObject *parent) :
    QThread(parent)
{
    start ();
}

TransactionThread::~TransactionThread ()
{
    {
        QMutexLocker locker(&mutex);  //使用互斥量，保护对变量的访问
        while(!transactions.isEmpty ())
            delete transactions.dequeue ();
        transactions.enqueue (endTransaction);
        transactionAdded.wakeOne ();  //唤醒阻塞的线程
    }
    wait ();
}

void TransactionThread::addTransaction (Transaction *transact)
{
    QMutexLocker locker(&mutex);
    transactions.enqueue (transact);
    transactionAdded.wakeOne ();
}

void TransactionThread::run ()
{
    Transaction *transact = 0;
    QImage oldImage;

    forever  //无限循环
    {
        {
            QMutexLocker locker(&mutex);

            if(transactions.isEmpty ())
                transactionAdded.wait (&mutex);  //阻塞线程
            transact = transactions.dequeue ();
            if(transact == endTransaction)
                break;

            oldImage = currentImage;
        }

        emit transactionStarted (transact->message ());
        QImage newImage = transact->apply (oldImage);  // 对图片进行处理
        delete transact;

        {
            QMutexLocker locker(&mutex);
            currentImage = newImage;
            if(transactions.isEmpty ())
                emit allTransactionsDone ();
        }
    }
}

void TransactionThread::setImage (QImage &image)
{
    QMutexLocker locker(&mutex);
    currentImage = image;
}

QImage TransactionThread::image ()
{
    QMutexLocker locker(&mutex);
    return currentImage;
}
