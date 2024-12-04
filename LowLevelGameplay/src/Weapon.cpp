#include <Weapon.h>
#include <GlobalEvents.h>
#include <GameObject.h>
#include <Cursor.h>
#include <Timer.h>
#include <ObjectPool.h>
#include <Transform.h>
#include <numbers>

#define M_PI		3.14159265358979323846

Weapon::Weapon(GameObject* owner) : Component(owner)
{
	currentWeapon = new CurrentWeaponInfo();
	m_CurrentAmmoCount = currentWeapon->s_MaxAmmo;
	m_FireDelayTimer = std::make_unique<Timer>(1.f/ 1.f);
	m_ReloadTimer = std::make_unique<Timer>(currentWeapon->s_ReloadTime);
	m_FireDelayTimer->TimerFinished.AddListener(this, std::bind(&Weapon::SetCanFire, this, std::placeholders::_1, std::placeholders::_2));
	m_ReloadTimer->TimerFinished.AddListener(this, std::bind(&Weapon::RefillAmmo, this, std::placeholders::_1));
	g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
	currentWeapon->SetWeapon(1);
}

Weapon::~Weapon()
{
	g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
	g_OnUpdate.RemoveListener(this, std::bind(&Component::Update, this, std::placeholders::_1));
	delete currentWeapon;
}

void Weapon::FixedUpdate(float deltaTime)
{
	m_AimDirection = m_GameObject->GetComponent<Cursor>()->GetLookAtCursorDirection();
}

bool Weapon::Fire()
{
	if (!GetCanFire() || !HasAmmo()) { return false; }

	m_CanFire = false;

	if (currentWeapon->s_WeaponIndex == 3) { FireShotgun(); return true; }
	//Fire rate timer (I want this to be a different thread)
	m_FireDelayTimer->StartTimer((1 / (currentWeapon->s_FireRate)));

	//Firing Logic (spawn bullet with direction and velocity.)
	LLGP::Vector2f fireLocation = m_GameObject->GetTransform()->ReturnPosition() + (m_AimDirection * m_AimOffset);

	ObjectPooler::FindRemainingObject()->EnableProjectile(m_AimDirection, fireLocation, currentWeapon->s_ProjectileSpeed, currentWeapon->s_ProjectileBounceAmount, currentWeapon->s_Damage);

	SetAmmoCount(m_CurrentAmmoCount - 1);

	return true;
}

bool Weapon::FireShotgun()
{
	//Fire rate timer (I want this to be a different thread)
	m_FireDelayTimer->StartTimer((1 / (currentWeapon->s_FireRate)));

	//Firing Logic (spawn bullet with direction and velocity.)
	LLGP::Vector2f fireLocation = m_GameObject->GetTransform()->ReturnPosition() + (m_AimDirection * m_AimOffset);

	LLGP::Vector2f newAimDirection;
	float aimOffset = 1 * (std::numbers::pi/180);
	for (int projectileCount = 0; projectileCount < currentWeapon->s_NumberOfProjectiles; projectileCount++)
	{
		int numberOfOffsets = projectileCount - (currentWeapon->s_NumberOfProjectiles / 2);
		float angle = aimOffset * numberOfOffsets;
		newAimDirection = RotateVector(m_AimDirection, angle).Normalised();
		ObjectPooler::FindRemainingObject()->EnableProjectile(newAimDirection, fireLocation, currentWeapon->s_ProjectileSpeed, currentWeapon->s_ProjectileBounceAmount, currentWeapon->s_Damage);
	}

	SetAmmoCount(m_CurrentAmmoCount - 1);

	return true;
}

void Weapon::Reload()
{
	if (m_CurrentAmmoCount == currentWeapon->s_MaxAmmo || m_Reloading) { return; }
	m_FireDelayTimer->ClearAndInvalidateTimer();
	m_ReloadTimer->StartTimer(currentWeapon->s_ReloadTime);
	m_Reloading = true;
}

void Weapon::RefillAmmo(bool DoesFill)
{
	if (DoesFill)
	{
		m_CurrentAmmoCount = currentWeapon->s_MaxAmmo;
	}
	m_Reloading = false;
	m_CanFire = true;
}

bool Weapon::HasAmmo()
{
	if (m_CurrentAmmoCount > 0) { return true; }
	if(!m_Reloading) { Reload(); }
	return false;
}

void Weapon::Update(float deltaTime)
{
	Component::Update(deltaTime);
	if (m_Firing)
	{
		Fire();
	}
}

void Weapon::SetMaxAmmoCount(int newMaxAmmo, bool IsFilling)
{
	if (IsFilling) { m_CurrentAmmoCount = newMaxAmmo; }

	currentWeapon->s_MaxAmmo = newMaxAmmo;
}

void Weapon::SetAmmoCount(int newAmmoCount)
{
	if (newAmmoCount > currentWeapon->s_MaxAmmo) { m_CurrentAmmoCount = currentWeapon->s_MaxAmmo; }
	else { m_CurrentAmmoCount = newAmmoCount; }
}

/// <summary>
/// This will be remade to a function which gets data from a static function and sets the instanced versions data. 
/// </summary>
/// <param name="weaponType">: 1 = Pistol, 2 = SMG, 3 = Shotgun, 4 = Sniper, 5 = Golden Gun. Fallthrough = Pistol. </param>
void Weapon::CurrentWeaponInfo::SetWeapon(int weaponType)
{
	s_WeaponIndex = weaponType;

	switch (weaponType)
	{
	case 1:
		s_WeaponName = "Pistol";
		s_RenderUV = LLGP::Vector2i(3, 5);
		s_FireRate = 5.f;
		s_ProjectileSpeed = 1000.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 0.7f;
		s_MaxAmmo = 7;
		s_Damage = 34.f;
		s_NumberOfProjectiles = 1;
		break;
	case 2:
		s_WeaponName = "SMG";
		s_RenderUV = LLGP::Vector2i(4, 5);
		s_FireRate = 15.f;
		s_ProjectileSpeed = 1000.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 1.6f;
		s_MaxAmmo = 20;
		s_Damage = 15.f;
		s_NumberOfProjectiles = 1;

		break;
	case 3:
		s_WeaponName = "Shotgun";
		s_RenderUV = LLGP::Vector2i(5, 5);
		s_FireRate = 0.7f;
		s_ProjectileSpeed = 1000.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 1.4f;
		s_MaxAmmo = 6;
		s_Damage = 14.f;
		s_NumberOfProjectiles = 8;
		break;

	case 4:
		s_WeaponName = "Sniper";
		s_RenderUV = LLGP::Vector2i(2, 5);
		s_FireRate = 0.7f;
		s_ProjectileSpeed = 1400.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 1.1f;
		s_MaxAmmo = 5;
		s_NumberOfProjectiles = 1;
		s_Damage = 100.f;
		break;

	case 5:
		s_WeaponName = "GoldenGun";
		s_RenderUV = LLGP::Vector2i(1, 5);
		s_FireRate = 10.f;
		s_ProjectileSpeed = 1000.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 1.3f;
		s_MaxAmmo = 100;
		s_NumberOfProjectiles = 1;
		s_Damage = 9999.f;
		break;

	default:
		s_WeaponName = "Pistol";
		s_RenderUV = LLGP::Vector2i(3, 5);
		s_FireRate = 3.5f;
		s_ProjectileSpeed = 600.f;
		s_ProjectileBounceAmount = 4;
		s_ReloadTime = 0.7f;
		s_MaxAmmo = 7;
		s_NumberOfProjectiles = 1;
		s_Damage = 34.f;
		break;
	}
}

bool Weapon::CurrentWeaponInfo::operator==(CurrentWeaponInfo* other)
{
	if (this->s_WeaponName == other->s_WeaponName)
	{
		return true;
	}
	return false;
}
