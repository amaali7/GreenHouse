#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

#define RELAY_INIT(rel, p, state) relay rel = {(int) p, (bool) state, false,Relaybegin, Control};


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
    Serial.println("Relay Strted");
    pinMode(r->pin, OUTPUT);
    if (r->state)
    {
        digitalWrite(r->pin, LOW);
    }
    else
    {
        digitalWrite(r->pin, LOW);
    }
    r->inProgres = false;
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