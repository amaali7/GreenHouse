#ifndef OBJECTS_H
#define OBJECTS_H

#include <Arduino.h>

#define RELAY_INIT(rel, p, state) relay rel = {(int) p, (bool) state, false,Relaybegin, Control};


typedef enum State
{
    False,
    True
} Type;

typedef struct Relay relay;
struct Relay
{
    int pin;
    bool state;
    bool inProgres;
    void (*begin)(relay *r);
    void (*Control)(relay *r, int);
};

void Relaybegin(relay *r)
{
    pinMode(r->pin, OUTPUT);
    if (r->state)
    {
        digitalWrite(r->pin, LOW);
    }
    else
    {
        digitalWrite(r->pin, LOW);
    }
}


void Control(relay *r, int interval)
{
    r->inProgres = true;
    if (r->state)
    {
        digitalWrite(r->pin, LOW);
    }
    else
    {
        digitalWrite(r->pin, HIGH);
    }
    delay(interval * 1000);
    if (r->state)
    {
        digitalWrite(r->pin, HIGH);
    }
    else
    {
        digitalWrite(r->pin, LOW);
    }
    r->inProgres = false;

}

#endif