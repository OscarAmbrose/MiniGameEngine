#pragma once
#include <Event.h>
#include <SFML/Graphics.hpp>

extern LLGP::Event<float> g_OnUpdate;
extern LLGP::Event<float> g_OnStart;
extern LLGP::Event<float> g_OnFixedUpdate;

extern LLGP::Event<float> g_OnCollectGarbage;

extern LLGP::Event<float> g_OnPhysicsUpdate;

extern LLGP::Event<sf::Event> g_OnPollInputs;
//extern LLGP::Event<float> g_OnRender;
extern LLGP::Event<sf::RenderWindow*, int> g_OnRender;
