#ifndef EPOLLPOLLER_H__
#define EPOLLPOLLER_H__

#include <vector>
#include <map>

class EventLoop;
class Channel;


class EpollPoller {
public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller();
    bool hasChannel(Channel *channel) const;
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
private:
    typedef std::map<int, Channel*>ChannelMap;
    EventLoop*    m_pLoop;
    int           m_iFd;
    ChannelMap    m_mapChannel;
};


#endif //EPOLLPOLLER_H__