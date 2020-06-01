QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 PRECOMPILED_HEADER

PRECOMPILED_HEADER += precompile.h

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Algorithm/MaskRcnn/MaskRcnnProcess.cpp \
    Algorithm/OpenPose/OpenPoseProcess.cpp \
    Algorithm/algorithmbase.cpp \
    Algorithm/inifile.cpp \
    algorithmconfig.cpp \
    algorithmsetting.cpp \
    main.cpp \
    mainwindow.cpp \
    mediaplay.cpp \
    mylabel.cpp \
    myslider.cpp \
    mytoolbutton.cpp \
    mywidget.cpp \
    playcontrolwidget.cpp

HEADERS += \
    Algorithm/MaskRcnn/MaskRcnnProcess.h \
    Algorithm/OpenPose/OpenPoseProcess.h \
    Algorithm/algorithmbase.h \
    Algorithm/inifile.h \
    Common.h \
    Singleton.h \
    algorithmconfig.h \
    algorithmsetting.h \
    mainwindow.h \
    mediaplay.h \
    mylabel.h \
    myslider.h \
    mytoolbutton.h \
    mywidget.h \
    playcontrolwidget.h \
    precompile.h

INCLUDEPATH += E:/Github/mediaplayer/3rdparty/opencv/include
INCLUDEPATH += E:/Github/mediaplayer/Algorithm


CONFIG(debug, debug|release){
    LIBS += -LE:/Github/mediaplayer/3rdparty/opencv/x64/vc15/lib \
    -lopencv_world420d \
    -lopencv_img_hash420d
}
else
{
    LIBS += -LE:/Github/mediaplayer/3rdparty/opencv/x64/vc15/lib \
    -lopencv_world420 \
    -lopencv_img_hash420
}

FORMS += \
    algorithmconfig.ui \
    algorithmsetting.ui \
    mainwindow.ui \
    playcontrolwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    Algorithm/MaskRcnn/config/colors.txt \
    Algorithm/MaskRcnn/config/config.ini \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/checkpoint \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/model.ckpt.data-00000-of-00001 \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/model.ckpt.index \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/model.ckpt.meta \
    Algorithm/MaskRcnn/config/mask_rcnn_inception_v2_coco_2018_01_28/saved_model/saved_model.pb \
    Algorithm/MaskRcnn/config/mscoco_labels.names \
    Algorithm/OpenPose/config/coco/pose_deploy_linevec.prototxt \
    Algorithm/OpenPose/config/coco/pose_iter_440000.caffemodel \
    Algorithm/OpenPose/config/config.ini \
    Algorithm/OpenPose/config/mpi/pose_deploy_linevec.prototxt \
    Algorithm/OpenPose/config/mpi/pose_deploy_linevec_faster_4_stages.prototxt \
    Algorithm/OpenPose/config/mpi/pose_iter_160000.caffemodel
