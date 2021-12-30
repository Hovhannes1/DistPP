#include "agent.h"
#include <QDebug>
#include "world.h"

Agent::Agent(unsigned int p_id,const QPointF &p_position):id(p_id),position(p_position) {
    myColor=Qt::white;
    myText=QString::number(p_id);
};

void Agent::draw(QPainter &painter) {
    painter.setBrush(myColor);
    painter.setPen(QPen(Qt::black,1));
    painter.save();
    painter.translate(position);
    QRectF rect(-40,-40,80,80);
    painter.drawEllipse(rect);

    painter.setFont(QFont("times",14));
    painter.drawText(rect,Qt::AlignVCenter|Qt::AlignCenter,myText);
    painter.restore();
}

unsigned int Agent::sendMessageToAllNeighbors(Message *ptr, unsigned int exceptId) {
    unsigned int n=0;
    for (auto &l:p2pLinks) {
        auto a = l->getConnected(this);
        if (a->getId()!=exceptId) {
            n++;
            World::getWorld()->sendMessage(l,a->getId(),ptr->clone());
        }
    }
    return n;
}

unsigned int Agent::sendMessageToAllNeighbors(Message *ptr,QVector<unsigned int> &list, unsigned int exceptId) {
    unsigned int n=0;
    list.clear();
    for (auto &l:p2pLinks) {
        auto a = l->getConnected(this);
        if (a->getId()!=exceptId) {
            list.push_back(a->getId());
            n++;
            World::getWorld()->sendMessage(l,a->getId(),ptr->clone());
        }
    }
    return n;
}

bool Agent::sendMessageTo(unsigned int destId, Message *ptr) {
    // search the link to destId
    auto it = p2pLinks.begin();
    while (it!=p2pLinks.end() && (*it)->getConnected(this)->getId()!=destId) {
        it++;
    }
    qDebug() << getId() << ".SendMessageTo(" << destId << "):" << (it==p2pLinks.end()?"not found":"OK");
    if (it==p2pLinks.end()) return false;

    World::getWorld()->sendMessage(*it,destId,ptr);
    return true;
}

void Agent::onTap() {
    qDebug() << "onTap Event on " << getId();
}
