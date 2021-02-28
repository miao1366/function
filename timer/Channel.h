#ifndef CHANNEL_H__
#define CHANNEL_H__

class EventLoop;

class Channel {
public:
    Channel(EventLoop *loop,int iFd);
private:
    EventLoop    *m_pLoop;
    const int     m_iFd;

};

#endif  //CHANNEL_H__