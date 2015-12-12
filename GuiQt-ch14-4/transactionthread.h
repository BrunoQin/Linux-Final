#ifndef TRANSACTIONTHREAD_H
#define TRANSACTIONTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

class Transaction  //基类
{
public:
    virtual ~Transaction() { }

    virtual QImage apply( QImage &image) = 0;  //=0表示是纯虚函数
    virtual QString message() = 0;
};

class FlipTransaction : public Transaction  //翻转类
{
public:
    FlipTransaction(Qt::Orientation orientation);//确定翻转方向

    QImage apply ( QImage &image);
    QString message ();

private:
    Qt::Orientation orientation;
};

class GrayTransaction:public Transaction{

public:
    GrayTransaction();
    QImage apply(QImage &image);
    QString message();

};

class RUIHUATransaction:public Transaction{

public:
    RUIHUATransaction();
    QImage apply(QImage &image);
    QString message();

};

class PINGHUATransaction:public Transaction{

public:
    PINGHUATransaction();
    QImage apply(QImage &image);
    QString message();
    void MoreClearPic(QRgb *tmpPixel,int yanKind,int width,int height);
    void YanMo1(int *tmpPixel, int width, int height);
};

class SOBELTransaction:public Transaction{

public:
    SOBELTransaction();
    QImage apply(QImage &image);
    QString message();
    void Template(QImage &SobelImage, QImage &image);
};

class ResizeTransaction : public Transaction
{
public:
    ResizeTransaction(const QSize &size);

    QImage apply ( QImage &image);
    QString message ();


private:
    QSize size;

};

class ConvertDepthTransaction : public Transaction
{
public:
    ConvertDepthTransaction(int depth);

    QImage apply ( QImage &image);
    QString message ();

private:
    int depth;
};


class ConvertGOSSTransaction: public Transaction
{
public:
    ConvertGOSSTransaction(int count);

    QImage apply ( QImage &image);
    QString message ();
private:
    int width;
    int height;
    int count;
}
;
class TransactionThread : public QThread  //线程类
{
    Q_OBJECT

public:
    explicit TransactionThread(QObject *parent = 0);
    ~TransactionThread();

    void addTransaction(Transaction *transact);
    void setImage( QImage &image);
    QImage image();

signals:
    void transactionStarted(const QString &message);
    void allTransactionsDone();

protected:
    void run();

private:
    QImage currentImage;
    QQueue<Transaction *> transactions;  //事物队列
    QWaitCondition transactionAdded;
    QMutex mutex;
};

#endif // TRANSACTIONTHREAD_H
