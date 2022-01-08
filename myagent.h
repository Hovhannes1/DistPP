#ifndef MYAGENT_H
#define MYAGENT_H
#include "agent.h"

class MyAgent: public Agent {
public:
    MyAgent(unsigned int p_id,const QPointF &p_position);
    void start();
    void messageCallback(MessagePtr ptr,unsigned int senderId);
    void distanceMsgCallback(MessagePtr ptr,unsigned int senderId);
    void backMsgCallback(MessagePtr ptr,unsigned int senderId);
    void electBMsgCallback(MessagePtr ptr,unsigned int senderId);
    virtual void draw(QPainter &painter);
protected:
    // my local variable
    unsigned int myDistance;
    QVector<unsigned int> myWaitingFrom;
    unsigned int myParent;
    unsigned int myBestValue;
    unsigned int myBestChild;
    unsigned int myStage;
    bool wasLeader=false;
};

#endif // MYAGENT_H
