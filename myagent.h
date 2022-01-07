#ifndef MYAGENT_H
#define MYAGENT_H
#include "agent.h"

class MyAgent: public Agent
{
public:
    MyAgent(unsigned int p_id,const QPointF &p_position);
    ~MyAgent() {}
    void start();
    void messageCallback(MessagePtr ptr,unsigned int senderId);
    void distanceMsgCallback(MessagePtr ptr,unsigned int senderId);
    void  backMsgCallback(MessagePtr ptr,unsigned int senderId);
    void  electBValueMsgCallback(MessagePtr ptr,unsigned int senderId);
    void  electCValueMsgCallback(MessagePtr ptr,unsigned int senderId);

    virtual void draw(QPainter &painter);
protected:
//my local variable
    unsigned int myDistance;
    QVector<unsigned int> myWaitingFrom;
    unsigned int myParent;
    unsigned int myBestValue;
    unsigned int myBestChild;
    unsigned int myStage;
    QPair<unsigned int,unsigned int> staDist;
    bool wasLeader=false;
};

#endif // MYAGENT_H
