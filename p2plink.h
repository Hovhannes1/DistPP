#ifndef P2PLINK_H
#define P2PLINK_H
#include "message.h"
#include <QPainter>

class Agent;

class P2PLink {
public:
    P2PLink(Agent *p_A1,Agent *p_A2,qint64 p_delay);

    void sendMessageFrom(Agent*,Message*);
    void draw(QPainter &);
    Agent *getConnected(Agent *a) {
        return (agent1==a?agent2:(agent2==a?agent1:nullptr));
    }
    qint64 getDelay() { return delay; }
private:
    Agent *agent1,*agent2;
    qint64 delay;
};

#endif // P2PLINK_H
