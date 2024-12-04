#include <GlobalEvents.h>

LLGP::Event<float> g_OnUpdate;
LLGP::Event<float> g_OnStart;
LLGP::Event<float> g_OnFixedUpdate;

LLGP::Event<float> g_OnCollectGarbage;

LLGP::Event<float> g_OnPhysicsUpdate;
LLGP::Event<sf::Event> g_OnPollInputs;
//LLGP::Event<float> g_OnRender;
LLGP::Event<sf::RenderWindow*, int> g_OnRender;

