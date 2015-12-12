#include <QtGui>
#include<string.h>
#include <QtWidgets/QApplication>
#include<QLabel>
#include<QMessageBox>
#include <QScrollArea>
#include "imagewindow.h"
#include "ui_imagewindow.h"
#include "ui_resizedialog.h"

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);

    ui->imagelabel->setBackgroundRole (QPalette::Dark);
    ui->imagelabel->setAutoFillBackground (true);
    setCentralWidget (ui->imagelabel);

    createActions ();
    createMenus ();

    statusBar ()->showMessage (tr("Ready"), 2000);

    connect (&thread, SIGNAL(transactionStarted(const QString &)), ui->statusBar, SLOT(showMessage(const QString &)));
    connect (&thread, SIGNAL(allTransactionsDone()), this, SLOT(allTransactionsDone()));
    mirrorView = NULL;
    filterList << "*.png"<<"*.jpeg"<<"*.jpg"<<"*.tiff";
    //connect(ui->action_About,SIGNAL(clicked()),this,SLOT(browse()));
    connect (ui->actionFile, SIGNAL(triggered()), this, SLOT(browse()));
    //connect(ui.searchBox,SIGNAL(clicked()),this,SLOT(ignoreChanged()));
    connect (ui->actionHd, SIGNAL(triggered()),this,SLOT(startShow()));
    QDesktopWidget desktop;
    this->move(desktop.width()/2 - this->width()/2,
               desktop.height()/2 - this->height()/2);
    setCurrentFile ("");  //设置标题栏和菜单栏，及标记文档是否保存


}

void ImageWindow::closeEvent (QCloseEvent *event)
{
    if(okToContune ())
        event->accept ();
    else
        event->ignore ();
}

void ImageWindow::open ()
{
    if(okToContune ())
    {
        QString fileName = QFileDialog::getOpenFileName (this);
        if(!fileName.isEmpty ())
            loadFile (fileName);
    }
}

bool ImageWindow::save ()
{
    if(curFile.isEmpty ())
        return saveAs ();
    else
    {
        saveFile (curFile);
        return true;
    }
}

bool ImageWindow::saveAs ()
{
    QString fileName = QFileDialog::getSaveFileName (this, tr("另存为"), curFile);
    if(QFile::exists (fileName))//如果文件名已存在，则提示是否要覆盖
    {
        int r = QMessageBox::warning (this, tr("Image Pro"), tr("文件%1已存在\n需要覆盖吗？").arg (QDir::toNativeSeparators (fileName)), QMessageBox::Yes | QMessageBox::No);
        if(r == QMessageBox::No)
            return true;
    }
    if(!fileName.isEmpty ())
        saveFile (fileName);
    return true;
}


void ImageWindow::flipHorizontally ()
{
    addTransaction (new FlipTransaction(Qt::Horizontal));
}

void ImageWindow::flipVertically ()
{
    addTransaction (new FlipTransaction(Qt::Vertical));
}

void ImageWindow::resizeImage ()
{
    QDialog dialog;
    Ui::ResizeDialog Dui; //更改大小对话框
    Dui.setupUi (&dialog);

    Dui.widthSpinBox->setValue (ui->imagelabel->pixmap ()->width ());
    Dui.heightSpinBox->setValue (ui->imagelabel->pixmap ()->height ());

    if(dialog.exec ())
    {
        QSize newSize(Dui.widthSpinBox->value(), Dui.heightSpinBox->value());
        addTransaction (new ResizeTransaction(newSize));
    }
}

void ImageWindow::convertTo32Bit ()
{
    addTransaction (new ConvertDepthTransaction(32));
}

void ImageWindow::convertTo8Bit ()
{
    addTransaction (new ConvertDepthTransaction(8));
}

void ImageWindow::convertTo1Bit ()
{

    addTransaction (new ConvertDepthTransaction(1));

}
void ImageWindow::gray ()
{

    addTransaction (new GrayTransaction());

}

void ImageWindow::RUIHUA ()
{

    addTransaction (new RUIHUATransaction());

}

void ImageWindow::PINGHUA ()
{

    addTransaction (new PINGHUATransaction());

}

void ImageWindow::SOBEL ()
{

    addTransaction (new SOBELTransaction());

}

void ImageWindow::addTransaction (Transaction *transact)
{
    thread.addTransaction (transact);
    ui->action_Open->setEnabled (false);
    ui->action_Save->setEnabled (false);
    ui->actionSave_As->setEnabled (false);


}
void ImageWindow::gs()
{

  addTransaction (new ConvertGOSSTransaction(1));
}

void ImageWindow::about ()
{
    QMessageBox::about(this, tr("关于"), tr("<h2>Image Pro 1.1</h2>"
                       "<p>本程序是一个小的图片处理程序，展示了多线程技术的使用"
                       "<p>制作者：梁堂皎 尤传跃"));
}

void ImageWindow::allTransactionsDone ()
{
    ui->action_Open->setEnabled (true);
    ui->action_Save->setEnabled (true);
    ui->actionSave_As->setEnabled (true);
    ui->imagelabel->setPixmap (QPixmap::fromImage (thread.image ()));
    setWindowModified (true);  //设置文档未保存的标记位
    ui->statusBar->showMessage (tr("Ready"), 2000);
}

void ImageWindow::createActions ()
{
    connect (ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
    connect (ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
    connect (ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect (ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
    connect (ui->actionFlip_Horizontally, SIGNAL(triggered()), this, SLOT(flipHorizontally()));
    connect (ui->actionFlip_Vertically, SIGNAL(triggered()), this, SLOT(flipVertically()));
    connect (ui->action32_bit, SIGNAL(triggered()), this, SLOT(convertTo32Bit()));
    connect (ui->action8_bit, SIGNAL(triggered()), this, SLOT(convertTo8Bit()));
    connect (ui->action1_bit, SIGNAL(triggered()), this, SLOT(convertTo1Bit()));
    connect (ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect (ui->action_Resize, SIGNAL(triggered()), this, SLOT(resizeImage()));
    connect (ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect (ui->actionNiuqu, SIGNAL(triggered()), this, SLOT(gs()));
    connect (ui->actionGray, SIGNAL(triggered()), this, SLOT(gray()));
    connect (ui->actionRUIHUA, SIGNAL(triggered()), this, SLOT(RUIHUA()));
    connect (ui->actionPINGHUA, SIGNAL(triggered()), this, SLOT(PINGHUA()));
    connect (ui->actionSOBEL, SIGNAL(triggered()), this, SLOT(SOBEL()));
}



void ImageWindow::createMenus ()
{

}

bool ImageWindow::okToContune ()
{
    if(isWindowModified ())
    {
        int r = QMessageBox::warning (this, tr("Image Pro"), tr("图片有修改，你想保存修改吗？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(r == QMessageBox::Yes)
            return save();
        else if(r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void ImageWindow::loadFile (const QString &fileName)
{
    QImage newImage;
    QApplication::setOverrideCursor (Qt::WaitCursor);
    bool loaded = newImage.load (fileName);
    QApplication::restoreOverrideCursor ();


    if(loaded)
    {
        thread.setImage (newImage);
        imageFormat = QImageReader::imageFormat (fileName);
        ui->imagelabel->setPixmap (QPixmap::fromImage (newImage));
        setCurrentFile (fileName);
        statusBar ()->showMessage (tr("文件已加载"), 2000);
    }else
    {
        QMessageBox::warning (this, tr("Image Pro"), tr( "加载文件出错"));
        statusBar ()->showMessage (tr("Loading canceled"), 2000);
    }
}

void ImageWindow::saveFile (const QString &fileName)
{
    QImage image = thread.image ();


    QApplication::setOverrideCursor (Qt::WaitCursor);
    bool saved = image.save (fileName, imageFormat);
    QApplication::restoreOverrideCursor ();

    if(saved)
    {
        setCurrentFile (fileName);
        statusBar ()->showMessage (tr("文件已保存"), 2000);
    }else
    {
        QMessageBox::warning (this, tr("Image Pro"), tr("保存出错"));
        statusBar ()->showMessage (tr("保存已取消"), 2000);
    }
}


void ImageWindow::setCurrentFile (const QString &fileName)
{
    curFile = fileName;
    setWindowModified (false);

    bool hasImage = !curFile.isEmpty ();

    if(hasImage)
        setWindowTitle (tr("%1[*] - %2").arg (strippedName (curFile)).arg (tr("Image Pro")));
    else
        setWindowTitle (tr("Image Pro"));

    ui->actionSave_As->setEnabled (hasImage);
    ui->action_Save->setEnabled (hasImage);
    ui->actionFlip_Horizontally->setEnabled (hasImage);
    ui->actionFlip_Vertically->setEnabled (hasImage);
    ui->action_Resize->setEnabled (hasImage);
    ui->action1_bit->setEnabled (hasImage);
    ui->action8_bit->setEnabled (hasImage);
    ui->action32_bit->setEnabled (hasImage);
ui->actionNiuqu->setEnabled(hasImage);
     }

QString ImageWindow::strippedName (const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName ();
}

void ImageWindow::browse()
{
    QString path = QFileDialog::getExistingDirectory(this,"","");
    if(path != "")
    {
        //ui.browseLine->setText(path);
        pictureMap.clear();
        //ui.startButton->setEnabled(false);
        findeFiles(QDir(path));
        //ui.foundLabel->setText(tr("Images Found: %1").arg(pictureMap.count()));
        //ui.startButton->setEnabled(true);
    }
}

void ImageWindow::ignoreChanged()
{
    /*if(ui.browseLine->text().trimmed() != "")
    {
        //pictureMap.clear();
        //ui.startButton->setEnabled(false);
        //findeFiles(QDir(ui.browseLine->text()));
        //ui.foundLabel->setText(tr("Images Found: %1").arg(pictureMap.count()));
        //ui.startButton->setEnabled(true);
    }*/
}

void ImageWindow::findeFiles(QDir folder)
{
    QStringList dirFilter;
    dirFilter<<"*";
    if(1)
    {
        foreach(const QFileInfo file, folder.entryInfoList(dirFilter,QDir::AllDirs))
        {
            if(file.absolutePath()==folder.absolutePath())
            {
                findeFiles(QDir(file.absoluteFilePath()));
            }
        }
    }
    QList<QFileInfo> fileList = folder.entryInfoList(filterList);
    while(fileList.count() > 0)
        pictureMap[pictureMap.count()] = fileList.takeFirst();
}

void ImageWindow::startShow()
{
    if(pictureMap.count() > 0)
    {
        mirrorView = new MirrorView();
        connect(mirrorView,SIGNAL(neededPhoto(int)),this,SLOT(photoNeeded(int)));
        connect(mirrorView,SIGNAL(hideMe()),this,SLOT(hideMirrorView()));
        mirrorView->setImageList(pictureMap.keys());
        int imageCount = 7;
        if(imageCount > pictureMap.keys().count())
            imageCount  = pictureMap.keys().count();
        //ui.progressBar->setValue(0);
        //ui.progressBar->setMaximum(imageCount);
        for(int i = 0; i < imageCount; ++i)
        {
            mirrorView->loadImage(pictureMap.keys().at(i),
                                  QPixmap(pictureMap.value(pictureMap.keys()
                                                           .at(i))
                                          .absoluteFilePath()));
           // ui.progressBar->setValue(i);
        }
        mirrorView->showFullScreen();
    }
}

void ImageWindow::hideMirrorView()
{
    //ui.progressBar->setValue(0);
    mirrorView->close();
    delete mirrorView;
    mirrorView = NULL;
}

void ImageWindow::photoNeeded(int index)
{
    if(pictureMap.keys().contains(index))
    {
        if(pictureMap.value(index).exists())
            mirrorView->loadImage(index,QPixmap(pictureMap.value(index)
                                                .absoluteFilePath()));
        else
            mirrorView->removeIndex(index);
    }
}

ImageWindow::~ImageWindow()
{
    delete ui;
}
