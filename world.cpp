#include "world.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

World *World::world = nullptr;
World::World(const QString &title) {
    if (world==nullptr) {
        world=this;
        loadConfig(title);
    }

}

World::~World() {
    clear();
}

void World::updateGraph(const QString &title) {
    clear();
    loadConfig(title);
}

void World::clear() {
    for (auto &l:links) {
        delete l;
    }
    links.clear();
    for (auto &a:agents) {
        delete a;
    }
    agents.clear();
}

void World::draw(QPainter &painter) {
    for (auto &l:links) {
        l->draw(painter);
    }
    for (auto &a:agents) {
        a->draw(painter);
    }
}

void World::scale(float sx,float sy) {
    for (auto &a:agents) {
        a->scale(sx,sy);
    }
}

bool World::loadConfig(const QString &title) {
    QFile file(title);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) return false;

    clear();
    QString JSON=file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
    QJsonArray JSONagents = doc["agents"].toArray();

    qDebug() << "Agents:" << JSONagents.size();
    for (auto &&v:JSONagents) {
        QJsonObject vector=v.toObject();
        auto strPosition = vector["position"].toString().split(',');
        QPointF pt(strPosition[0].toFloat(),strPosition[1].toFloat());
        auto intId = vector["id"].toInt();
        agents[intId]=new MyAgent(intId,pt);
    }

    QJsonArray JSONlinks = doc["links"].toArray();
    qDebug() << "Links:" << JSONlinks.size();
    for (auto &&v:JSONlinks) {
        QJsonObject vector=v.toObject();
        unsigned int A1 = vector["agent1"].toInt();
        unsigned int A2 = vector["agent2"].toInt();
        unsigned int delay = vector["delay"].toInt();
        auto link = new P2PLink(agents[A1],agents[A2],delay);
        links.push_back(link);
        agents[A1]->addLink(link);
        agents[A2]->addLink(link);
    }
    return true;
}

void World::start() {
    timer.start();
    for (auto &a:agents) {
        addEvent(new StartEvent(a));
        qDebug() << "StartEvent:" << a->getId();
    }
}

void World::addEvent(Event *e) {
    events[e->getTime()].push_back(e);
}

bool World::consume() {
    auto it=events.begin();
// consomer tous les évènements antérieurs à timer.elapsed()
    auto current = timer.elapsed();
    while (it!=events.end() && it.key()<current) {
        for (auto &l:*it) {
            l->consume();
        }
        it=events.erase(it);
    }
    return events.empty();
}

void World::sendMessage(P2PLink *link, unsigned int destId, Message *ptr) {
    Agent* destAgent = getAgent(destId);
    auto event = new MessageEvent(destAgent,timer.elapsed()+link->getDelay(),
                                  ptr,link->getConnected(destAgent)->getId());
    addEvent(event);
}

Agent* World::findAgentIn(const QPoint &pos) {
    auto itAgent = agents.begin();
    while (itAgent!=agents.end() && !itAgent.value()->isIn(pos)) {
        itAgent++;
    }
    return itAgent==agents.end()?nullptr:itAgent.value();
}
