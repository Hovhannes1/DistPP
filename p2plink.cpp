#include "p2plink.h"
#include "agent.h"
#include <QtMath>

P2PLink::P2PLink(Agent *p_A1,Agent *p_A2,qint64 p_delay):
    agent1(p_A1),agent2(p_A2),delay(p_delay) {
}

void P2PLink::draw(QPainter &painter) {
    painter.setPen(QPen(Qt::black,5));
    QPointF pos1=agent1->getPosition(),
            pos2=agent2->getPosition();
    painter.drawLine(pos1,pos2);

    painter.save();
    painter.translate(0.5*(pos1+pos2));
    int sizeR=10+int(qLn(delay)/qLn(10.0))*8;
    QRectF rect(-sizeR,-20,2*sizeR,40);

    painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);
    painter.setPen(QPen(Qt::black,1));
    painter.setFont(QFont("times",12));

    painter.drawText(rect,Qt::AlignVCenter|Qt::AlignCenter,QString::number(delay));
    painter.restore();

}
