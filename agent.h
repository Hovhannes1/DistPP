#ifndef AGENT_H
#define AGENT_H
#include <QColor>
#include <QString>
#include <QPainter>
#include <QDebug>
#include "p2plink.h"



class Agent {
    static const short nbColors=11;
    const QColor tabColors[nbColors]={QColor(0xff0000),QColor(0xff7f00),QColor(0xffff00),QColor(0x00ff00),
                             QColor(0x0060ff),QColor(0x4b0082),QColor(0x7f00ff),QColor(0x777777),
                             QColor(0x582900),QColor(0xf3c4cf),QColor(0xffffff)};
public:
    Agent(unsigned int p_id,const QPointF &p_position);
    virtual ~Agent() {};

    void addLink(P2PLink *link) {
        p2pLinks.push_back(link);
    }
    void draw(QPainter &painter);

    virtual void start()=0;
    virtual void messageCallback(MessagePtr ptr,unsigned int senderId)=0;

    QPointF getPosition() { return position; }
    void scale(float sx,float sy) { position=QPointF(position.x()*sx,position.y()*sy); }
    unsigned int getId() { return id; }

    void setColor(short color) { myColor=tabColors[color%nbColors]; }
    void setBlink() { blinkMode=false; }
    void setBlink(short c1,short c2) {
        blinkMode=true;
        myColor=tabColors[c1];
        myBlinkColor=tabColors[c2];
    }
    void changeBlinkState() {
        if (blinkMode) {
            QColor c=myColor;
            myColor=myBlinkColor;
            myBlinkColor=c;
        }
    }
    /* Send a message to all neighbors but the one with exceptId */
    unsigned int sendMessageToAllNeighbors(Message* ptr,unsigned int exceptId=0);
    unsigned int sendMessageToAllNeighbors(Message* ptr,QVector<unsigned int> &list,unsigned int exceptId=0);
    /* Send a message to a neighbor */
    bool sendMessageTo(unsigned int destId,Message* ptr);
    virtual void onTap();
    bool isIn(const QPoint &pos) {
        float x = pos.x()-position.x();
        float y = pos.y()-position.y();
        return (x*x+y*y<40*40);
    }
    inline void setText(const QString &str) {
        myText=str;
    }
private:
    unsigned int id;
    QVector<P2PLink*> p2pLinks;
    QPointF position;
protected:
    bool blinkMode=false;
    QColor myBlinkColor;
    QColor myColor;
    QString myText;
};

#endif // AGENT_H
