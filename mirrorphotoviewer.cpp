#include "mirrorphotoviewer.h"

void MirrorPhotoViewer::browse()
{
    QString path = QFileDialog::getExistingDirectory(this,"","");
    if(path != "")
    {
        ui.browseLine->setText(path);
        pictureMap.clear();
        ui.startButton->setEnabled(false);
        findeFiles(QDir(path));
        ui.foundLabel->setText(tr("Images Found: %1").arg(pictureMap.count()));
        ui.startButton->setEnabled(true);
    }
}

void MirrorPhotoViewer::ignoreChanged()
{
    if(ui.browseLine->text().trimmed() != "")
    {
        pictureMap.clear();
        ui.startButton->setEnabled(false);
        findeFiles(QDir(ui.browseLine->text()));
        ui.foundLabel->setText(tr("Images Found: %1").arg(pictureMap.count()));
        ui.startButton->setEnabled(true);
    }
}

void MirrorPhotoViewer::findeFiles(QDir folder)
{
    QStringList dirFilter;
    dirFilter<<"*";
    if(!ui.searchBox->isChecked())
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

void MirrorPhotoViewer::startShow()
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
        ui.progressBar->setValue(0);
        ui.progressBar->setMaximum(imageCount);
        for(int i = 0; i < imageCount; ++i)
        {
            mirrorView->loadImage(pictureMap.keys().at(i),
                                  QPixmap(pictureMap.value(pictureMap.keys()
                                                           .at(i))
                                          .absoluteFilePath()));
            ui.progressBar->setValue(i);
        }
        mirrorView->showFullScreen();
    }
}

void MirrorPhotoViewer::hideMirrorView()
{
    ui.progressBar->setValue(0);
    mirrorView->close();
    delete mirrorView;
    mirrorView = NULL;
}

void MirrorPhotoViewer::photoNeeded(int index)
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


MirrorPhotoViewer::MirrorPhotoViewer(QWidget *parent)
    :  QWidget(parent)
{
    ui.setupUi(this);
    mirrorView = NULL;
    filterList << "*.png"<<"*.jpeg"<<"*.jpg"<<"*.tiff";
    connect(ui.browseButton,SIGNAL(clicked()),this,SLOT(browse()));
    connect(ui.searchBox,SIGNAL(clicked()),this,SLOT(ignoreChanged()));
    connect(ui.startButton,SIGNAL(clicked()),this,SLOT(startShow()));
    QDesktopWidget desktop;
    this->move(desktop.width()/2 - this->width()/2,
               desktop.height()/2 - this->height()/2);
}

MirrorPhotoViewer::~MirrorPhotoViewer()
{
    if(mirrorView != NULL)
        delete mirrorView;
}
