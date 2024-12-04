#pragma once
#include <GradExPlayer.h>
#include <RigidBody.h>
#include <SpriteRenderer.h>
#include <BoxCollider.h>
#include <DebugBox.h>
#include <Cursor.h>
#include <Transform.h>
#include <GradExPlayerController.h>
#include <Weapon.h>
#include <ScreenWrapper.h>
#include <HealthComponent.h>
#include<GradExPlayerHealth.h>

PlayerCharacter::PlayerCharacter() : GameObject()
{
	SetName("Player");
}

PlayerCharacter::PlayerCharacter(int playerNumber) : GameObject()
{
	m_PlayerNumber = playerNumber;
}

PlayerCharacter* PlayerCharacter::InitialiseCharacter()
{
	AddComponent<RigidBody>()->setGravityEnabled(false);
	AddComponent<SpriteRenderer>()->setUV(LLGP::Vector2i(5, 3))->setRenderLayer(1);
	AddComponent<BoxCollider>()->SetUpCollider(LLGP::Vector2f(26, 38), LLGP::Vector2f(0,0))->SetCollisionMask(0b01000111)->SetCollisionLayer(0b00001111);
	//AddComponent<DebugBox>()->SetUpDebugBox();
	AddComponent<Cursor>()->SetControllerNumber(m_PlayerNumber);
	AddComponent<PlayerController>();
	AddComponent<ScreenWrapper>();
	SetTag("Player");
	AddComponent<Weapon>();
	AddComponent<HealthComponent>();
	AddComponent<GradExPlayerHealth>();
	SetPersistent(true);
	SetActive(true);

	return this;
}
