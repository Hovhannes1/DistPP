#include "myagent.h"

enum MyMsgType{DISTANCE_MSG=1, BACK_MSG, ELECTB_VALUE};
MyAgent::MyAgent(unsigned int p_id, const QPointF &p_position): Agent(p_id,p_position)
{

}

void MyAgent::start()
{
    myBestChild=0;
    myParent=0;
    myBestValue=0;
    myStage=0;
    if(getId()==1){
        setColor(0);
        setBlink(0,10);
        setText("A");
        myDistance=0;
        myStage=1;
        wasLeader=true;
        sendMessageToAllNeighbors(new MessageOf<QPair<unsigned int,unsigned int>>(DISTANCE_MSG,{1,1}),myWaitingFrom);

    } else {
        setColor(9);
        myDistance=std::numeric_limits<unsigned int>::max();
    }

}

void MyAgent::messageCallback(MessagePtr ptr, unsigned int senderId) {
    switch(ptr->getId()){
        case DISTANCE_MSG : distanceMsgCallback(ptr,senderId); break;
        case BACK_MSG : backMsgCallback(ptr,senderId); break;
    case ELECTB_VALUE : electBValueMsgCallback(ptr,senderId);break;

    }
}

void MyAgent::distanceMsgCallback(MessagePtr ptr, unsigned int senderId)
{
    MessageOf<QPair<unsigned int, unsigned int>> *msg=(MessageOf<QPair<unsigned int, unsigned int>>*)ptr.get();
    unsigned int distMsg=msg->getData().first;
    unsigned int stage=msg->getData().second;
    if (stage>myStage) { // re-init the variables used for distances
            myParent=0;
            myBestValue=0;
            myBestChild=0;
            myWaitingFrom.clear();
            myDistance=std::numeric_limits<unsigned int>::max();
            myStage=stage;
            qDebug() << "Node#" << getId() << ": new Stage=" << myStage;
        }
    if(distMsg<myDistance){
        myDistance = distMsg;
        myParent = senderId;
        sendMessageToAllNeighbors(new MessageOf<QPair<unsigned int,unsigned int>>(DISTANCE_MSG,{myDistance+1,myStage}), myWaitingFrom,senderId);
        if(!wasLeader){
             setColor(myDistance);
         }
        if (myWaitingFrom.empty()) {
                  sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
           }
    } else {
        if (myDistance+1==distMsg) { //[1]
            myWaitingFrom.removeAll(senderId);
            if (myWaitingFrom.empty() && myParent!=0) {
                sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
            }
               }
        sendMessageTo(senderId,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
    }
}

void MyAgent::backMsgCallback(MessagePtr ptr, unsigned int senderId)
{
    MessageOf<int> *msg=(MessageOf<int>*)ptr.get();
    unsigned int bestValueMsg=msg->getData();
    if(bestValueMsg >myBestValue){
        myBestValue=bestValueMsg;
        myBestChild=senderId;
    }
    myWaitingFrom.removeAll(senderId);
    if(myWaitingFrom.empty()){
        if(myParent!=0){
            sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));

        } else {
            //i'm the leader
            if(myBestChild!=0){
                sendMessageTo(myBestChild,new Message(ELECTB_VALUE));

            }
        }
    }
}

void MyAgent::electBValueMsgCallback(MessagePtr ptr, unsigned int senderId)
{
    if(myBestChild==0 && myStage==1){
        setText("B");
        setBlink(3,10);
        //stage is what will tell me that now B is the new leader
        //QPair<int,int> will let me send my stage and distance
        //if new stage detected renislize the myDistance and something
        myWaitingFrom.clear();
        myStage=2;
        myDistance=0;
        myBestChild=0;
        myParent=0;
        myBestValue=0;
        wasLeader=true;
        sendMessageToAllNeighbors(new MessageOf<QPair<unsigned int,unsigned int>>(DISTANCE_MSG,{1,myStage}),myWaitingFrom);
    }else if(myBestChild==0 && myStage==2){
        setText("C");
        setBlink(4,10);
        //stage is what will tell me that now B is the new leader
        //QPair<int,int> will let me send my stage and distance
        //if new stage detected renislize the myDistance and something
        myWaitingFrom.clear();
        myStage=3;
        myDistance=0;
        myBestChild=0;
        myParent=0;
        myBestValue=0;
        wasLeader=true;
    }
    else{
         sendMessageTo(myBestChild,new Message(ELECTB_VALUE));
    }
}


void MyAgent::draw(QPainter &painter) {
    Agent::draw(painter);

    painter.save();
    painter.translate(position);
    QRectF rect(-80,+20,160,30);
    QString str="(" + QString::number(myBestChild) + ")";
    painter.setFont(QFont("times",12));
    painter.drawText(rect,Qt::AlignVCenter|Qt::AlignCenter,str);
    painter.restore();
}
