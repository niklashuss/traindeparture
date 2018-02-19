#include "eventmanager.h"
#include <memory.h>
#include <stdio.h>

static const int MAX_EVENT_LISTENER_COUNT = 100;

struct Event {
    int id;
    void* data;
};

struct EventListener {
    int id;
    EventFunc_t callback;
};


static EventListener eventListeners[MAX_EVENT_LISTENER_COUNT];

void em_setup() {
    memset(&eventListeners[0], 0, sizeof(eventListeners));
}

void em_add_listener(int id, EventFunc_t func) {
    if (id == 0) {
        printf("Error: Illegal event id. Must not be zero.\n");
        return;
    }

    for (int i = 0; i < MAX_EVENT_LISTENER_COUNT; i++) {
        if (eventListeners[i].id != 0) {
            continue;
        }
        eventListeners[i].id = id;
        eventListeners[i].callback = func;
        break;
    }
}


void em_post_event(int id, void* data) {
    if (id == 0) {
        printf("Error: Illegal event id. Must not be zero.\n");
        return;
    }

    for (int i = 0; i < MAX_EVENT_LISTENER_COUNT; i++) {
        if (eventListeners[i].id == id) {
            eventListeners[i].callback(id, data);
        }
    }
}

