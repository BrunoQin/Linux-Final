#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QMap>
#include <QDesktopWidget>
#include "transactionthread.h"
#include "mirrorview.h"
namespace Ui {
    class ImageWindow;
}

class ImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageWindow(QWidget *parent = 0);
    ~ImageWindow();

protected:
    void closeEvent (QCloseEvent *event);

private slots:
    void open();
    bool save();
    bool saveAs();
    void flipHorizontally();
    void flipVertically();
    void resizeImage();
    void convertTo32Bit();
    void convertTo8Bit ();
    void convertTo1Bit ();
    void about();
    void gs();
    void allTransactionsDone();
    void browse();
    void startShow();
    void hideMirrorView();
    void photoNeeded(int index);
    void ignoreChanged();
    void gray();
    void RUIHUA();
    void PINGHUA();
    void SOBEL();


private:
    Ui::ImageWindow *ui;

    void createActions();
    void createMenus();
    bool okToContune();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void addTransaction(Transaction *transact);

    TransactionThread thread;
    QByteArray imageFormat;
//    QImage::Format imageFormat;
    QString curFile;
    void findeFiles(QDir folder);
   // Ui::MirrorPhotoViewer_ui ui;
    QMap<int,QFileInfo> pictureMap;
    QStringList filterList;
    MirrorView* mirrorView;
};

#endif // IMAGEWINDOW_H
