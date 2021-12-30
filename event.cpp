#include "event.h"

void StartEvent::consume() {
    agent->start();
}

void MessageEvent::consume() {
    agent->messageCallback(message,senderId);
}

void TapEvent::consume() {
    agent->onTap();
}
