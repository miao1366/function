#include <sys/epoll.h>    //epoll_create1()
#include <unistd.h>

#include "EpollPoller.h"


EpollPoller::EpollPoller(EventLoop *loop):m_pLoop(loop),m_iFd(epoll_create1(EPOLL_CLOEXEC)) {

}

EpollPoller::~EpollPoller() {
    close(m_iFd);
}

bool EpollPoller::hasChannel(Channel *channel) const {
    
}

void EpollPoller::updateChannel(Channel *channel) {

}


void EpollPoller::removeChannel(Channel *channel) {

}