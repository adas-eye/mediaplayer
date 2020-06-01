#ifndef CMEDIAPLAY_H
#define CMEDIAPLAY_H

#include "algorithmbase.h"
#include <QObject>
#include <mutex>
#include <QString>
#include <opencv2/opencv.hpp>
#include <map>
#include <QLabel>
#include <QTimer>


//typedef std::vector<CAlgorithmBase*> AlgorithmVec;

typedef enum
{
    PLAYING,
    PAUSED,
    STOPPED
}PlayStateEnum;

class MediaPlay: public QObject
{
public:
    MediaPlay();
    ~MediaPlay();

    void Add(const QStringList& strPathlist);

    void Add(const QString& strPath);

    void Delete(unsigned index);

    void Delete(const QString& strPath);

    int Play();

    int Play(unsigned index);

    void PlayPrevious();

    void PlayNext();

    void Pause();

    void Stop();

    void VolumeUp();

    void VolumeDown();

    unsigned GetCurrentPlayIndex();

    int GetCurrentState();

    int GetCurrentTotalFrames();

    unsigned GetCurrentFrame();

    const std::map<unsigned, std::string>& GetPlayList();

    void SetPlayPosition(int pos);

    void SetPlayIndex(qint16 index);

    int SetShowLabel(QLabel* pUi);

    void SetAlgorithmType(AlgorithmEnum type);

    bool IsStoped();

    bool IsPaused();

public slots:
    void RunCapture();

private:


    int InitCapture();

    void ReleaseCapture();

    void LoadAlgorithm(AlgorithmEnum type);

private:

    AlgorithmEnum m_AlgorithmType;

    int m_PlayState;

    unsigned int m_currentPlayFrame;

    unsigned m_total;

    unsigned m_currentIndex;

    cv::VideoCapture*  m_pCapture;

    std::map<unsigned, std::string> m_playMap;

    std::mutex m_mutex;

    QLabel *m_LabelShow;

    QTimer* m_pTimer;

    CAlgorithmBase* m_pAlgorithm;

    //AlgorithmVec m_AlgorithmObjVec;

};

#endif // CMEDIAPLAY_H
