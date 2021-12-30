#ifndef WORLD_H
#define WORLD_H

#include <QElapsedTimer>
#include "myagent.h"
#include "p2plink.h"
#include "message.h"
#include "event.h"
#include <QMap>

// World is a singleton
class World {
public:
    // Singleton manager
    static World *world;
    static World* getWorld() {
        assert(world != nullptr);
        return(world);
    }

    World(const QString &title);
    ~World();
    void updateGraph(const QString &title);
    bool loadConfig(const QString &title);
    void clear();
    void draw(QPainter &);
    void scale(float sx,float sy);
    bool consume();
    void addEvent(Event *e);
    void start();
    void sendMessage(P2PLink *link,unsigned int destId,Message *ptr);
    Agent *getAgent(unsigned int id) { return agents[id]; }
    int getNumberOfAgents() { return agents.size(); }
    int getNumberOfLinks() { return links.size(); }
    void changeBlinkState() {
        for (auto& a:agents) {
            a->changeBlinkState();
        }
    }
    Agent *findAgentIn(const QPoint &pos);
private:
    QMap<unsigned int,Agent*> agents;
    QVector<P2PLink*> links;
    QMap<qint64,QList<Event*>> events;
    QElapsedTimer timer;
};

#endif // WORLD_H
