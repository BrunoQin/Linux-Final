#ifndef MIRRORPHOTOVIEWER_H
#define MIRRORPHOTOVIEWER_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QMap>
#include <QDesktopWidget>


class MirrorPhotoViewer: public QWidget
{
    Q_OBJECT

public:
    MirrorPhotoViewer(QWidget *parent = 0);
    ~MirrorPhotoViewer();

public slots:
    void browse();
    void startShow();
    void hideMirrorView();
    void photoNeeded(int index);
    void ignoreChanged();

private:
    void findeFiles(QDir folder);
    Ui::MirrorPhotoViewer_ui ui;
    QMap<int,QFileInfo> pictureMap;
    QStringList filterList;
    MirrorView* mirrorView;
};

#endif // MIRRORPHOTOVIEWER_H
