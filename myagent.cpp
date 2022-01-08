#include "myagent.h"

enum MyMsgType{DISTANCE_MSG=1, BACK_MSG, ELECTB_MSG};

MyAgent::MyAgent(unsigned int p_id,const QPointF &p_position):Agent(p_id,p_position) {
    myDistance=0;
    myStage=0;
};

void MyAgent::draw(QPainter &painter) {
    Agent::draw(painter);
    painter.save();
    painter.translate(position);
    QRectF rect(-80,60,180,30);
    painter.setFont(QFont("times",12));
    QString str="<"+QString::number(myDistance)+","+QString::number(myStage)+"><"+QString::number(myBestChild)+","+QString::number(myBestValue)+">";
    painter.drawText(rect,Qt::AlignVCenter|Qt::AlignCenter,str);
    str="<";
    for (auto &l:myWaitingFrom) {
        str+=QString::number(l)+" ";
    }
    str+=">";
    rect=QRect(-80,90,180,30);
    painter.drawText(rect,Qt::AlignVCenter|Qt::AlignCenter,str);
    painter.restore();
}

void MyAgent::start() {
    myParent=0;
    myBestValue=0;
    myBestChild=0;
    myStage=0;
    setText(QString::number(getId()));
    if (getId()==1) {
        setBlink(0,10); // 0 is RED, 10 is WHITE
        setText("A");
        myDistance = 0;
        myStage=1;
        wasLeader= true;
        // sendMessageToAllNeighbors
        // send a copy of the message to all neighbors
        // clear the myWaitingFrom list
        // AND add the id of this neighbor into myWaitingFrom list
        // return the number of neighbors that have received a message (the size of myWaitingFrom)
        sendMessageToAllNeighbors(
                    new MessageOf<QPair<unsigned int,unsigned int>>(DISTANCE_MSG,{1,myStage}),
                    myWaitingFrom);
    } else {
        setColor(9); // 9 is pink
        myDistance = std::numeric_limits<unsigned int>::max();
    }
}

void MyAgent::messageCallback(MessagePtr ptr, unsigned int senderId) {
    switch(ptr->getId()) {
        case DISTANCE_MSG : distanceMsgCallback(ptr,senderId); break;
        case BACK_MSG : backMsgCallback(ptr,senderId); break;
        case ELECTB_MSG : electBMsgCallback(ptr,senderId); break;
    }
}

void MyAgent::distanceMsgCallback(MessagePtr ptr, unsigned int senderId) {
    MessageOf<QPair<unsigned int,unsigned int>>*msg = (MessageOf<QPair<unsigned int,unsigned int>>*)ptr.get();
    unsigned int distMsg = msg->getData().first;
    unsigned int stageMsg = msg->getData().second;

    if (stageMsg>myStage) { // re-init the variables used for distances
        myParent=0;
        myBestValue=0;
        myBestChild=0;
        myWaitingFrom.clear();
        myDistance=std::numeric_limits<unsigned int>::max();
        myStage=stageMsg;
        qDebug() << "Node#" << getId() << ": new Stage=" << myStage;
    }
    if (distMsg<myDistance) {
        myDistance = distMsg;
        myParent = senderId;
        sendMessageToAllNeighbors(
                    new MessageOf<QPair<unsigned int,unsigned int>>(DISTANCE_MSG,{myDistance+1,myStage}),
                                  myWaitingFrom,senderId);
        if(!wasLeader){
            setColor(myDistance);
        }
        if (myWaitingFrom.empty()) {
            sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
        }
    } else {
        if (myDistance+1==distMsg) {
            myWaitingFrom.removeAll(senderId);
            if (myWaitingFrom.empty() && myParent!=0) {
                sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
            }
            QString str;
            for (auto &w:myWaitingFrom) {
                str += QString::number(w)+" ";
            }
            qDebug() << getId() << ":" << str;
        }
        sendMessageTo(senderId,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
    }
}

void MyAgent::backMsgCallback(MessagePtr ptr, unsigned int senderId) {
    MessageOf<int>*msg = (MessageOf<int>*)ptr.get();
    unsigned int bestValueMsg = msg->getData();

    if (bestValueMsg>myBestValue) {
        myBestValue=bestValueMsg;
        myBestChild=senderId;
        /*setColor(6);
        setText(QString::number(myBestChild));*/
    }
    if (myWaitingFrom.removeAll(senderId)!=0 ) {
        if (myWaitingFrom.empty()) {
            if (myParent!=0) {
                sendMessageTo(myParent,new MessageOf<unsigned int>(BACK_MSG,qMax(myBestValue,myDistance)));
            } else {
                // I'm the leader
                setBlink(0,10);
                //setColor(0);
                if (myBestChild!=0) sendMessageTo(myBestChild,new Message(ELECTB_MSG));
            }
        }
    }
}

void MyAgent::electBMsgCallback(MessagePtr ptr, unsigned int senderId) {
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
          myWaitingFrom.clear();
          myStage=3;
          myDistance=0;
          myBestChild=0;
          myParent=0;
          myBestValue=0;
          wasLeader=true;
      }
      else{
           sendMessageTo(myBestChild,new Message(ELECTB_MSG));
      }

}
