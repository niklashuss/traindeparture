#pragma once

#include <functional>

typedef void (*EventFunc_t)(int, void*);

void em_setup();

void em_add_listener(int id, EventFunc_t func);
void em_add_listener(int id, std::function<void(void)> func);

void em_post_event(int id, void* data);
