#pragma once
#include <Component.h>
#include <Vector2.h>

class Timer;

class Weapon : public Component
{
public:

	struct CurrentWeaponInfo
	{
		CurrentWeaponInfo() {};

		//Weapon 1 = Pistol, Weapon 2 = SMG, Weapon 3 = Shotgun, Weapon 4 = Sniper, Weapon 5 = GoldenGun
		void SetWeapon(int weaponType);

		std::string s_WeaponName = "DefaultWeapon";
		LLGP::Vector2i s_RenderUV = LLGP::Vector2i(2, 5);
		float s_FireRate = 10.f;
		float s_ProjectileSpeed = 800.f;
		int s_ProjectileBounceAmount = 0;
		float s_ReloadTime = 1.2f;
		int s_MaxAmmo = 15;
		float s_Damage = 34.f;
		int s_WeaponIndex = 1;
		int s_NumberOfProjectiles = 1;

		bool operator==(CurrentWeaponInfo* other);
	};


	Weapon(GameObject* owner);
	~Weapon();

	void FixedUpdate(float deltaTime);

	bool Fire();
	bool FireShotgun();

	void SetFiring(bool isFiring) { m_Firing = isFiring; }

	inline bool GetCanFire() const { return m_CanFire; }
	inline void SetCanFire(Timer* timer, bool newCanFire) { m_CanFire = newCanFire; }

	void Reload();

	void RefillAmmo(bool DoesFill);

	void SetWeaponType(int newType) { currentWeapon->SetWeapon(newType); RefillAmmo(true); }
	int GetWeaponType() { return currentWeapon->s_WeaponIndex; }

	bool HasAmmo();

	void Update(float deltaTime);

	void SetMaxAmmoCount(int newMaxAmmo, bool IsFilling);

	void SetAmmoCount(int newAmmoCount);

	inline void SetAimDirection(LLGP::Vector2f newDirection) { m_AimDirection = newDirection; }
	LLGP::Vector2f GetAimDirection() const { return m_AimDirection; }

protected:
	bool m_CanFire = true;

	bool m_Reloading = false;

	bool m_Firing = false;

	CurrentWeaponInfo* currentWeapon;

	LLGP::Vector2f m_AimDirection = LLGP::Vector2f::zero;
	float m_AimOffset = 50.f;

	std::unique_ptr<Timer> m_FireDelayTimer;
	std::unique_ptr<Timer> m_ReloadTimer;

	int m_CurrentAmmoCount;
};
