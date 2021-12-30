#ifndef EVENT_H
#define EVENT_H

#include <QTime>
#include "agent.h"

class Event {
public:
    Event(qint64 t,Agent *p_agent):time(t),agent(p_agent) {};
    virtual void consume()=0;
    unsigned int getTime() { return time; }
    Agent *getAgent() { return agent; }
protected:
    qint64 time;
    Agent *agent;
};

class StartEvent:public Event {
public:
    StartEvent(Agent *p_agent):Event(0,p_agent) {};
    void consume() override;
};

class MessageEvent:public Event {
public:
    MessageEvent(Agent*p_agent,qint64 p_time,Message* p_message,unsigned int p_sender):
        Event(p_time,p_agent),message(MessagePtr(p_message)),senderId(p_sender) {};
    void consume() override;
protected:
    MessagePtr message;
    unsigned int senderId;
};

class TapEvent:public Event {
public:
    TapEvent(Agent*p_agent):Event(0,p_agent) {};
    void consume() override;
};

#endif // EVENT_H
