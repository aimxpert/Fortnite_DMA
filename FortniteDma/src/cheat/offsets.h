#pragma once
#include <cstdint>

namespace offsets {
	uintptr_t temp1 = 0x0;
	uintptr_t temp2 = 0x0;

	uintptr_t Uworld = 0x169D6108;
	uintptr_t CameraLocationPointer = 0x128;
	uintptr_t CameraRotationPointer = 0x138;
	uintptr_t GameInstance = 0x1F0;
	uintptr_t GameState = 0x178;
	uintptr_t PersistentLevel = 0x40;
	uintptr_t LocalPlayers = 0x38;
	uintptr_t FieldOfView = 0x4AC; // point::LocalPlayer + 0x4AC
	uintptr_t PlayerController = 0x30;
	uintptr_t LocalPawn = 0x350;
	uintptr_t Cameramanager = 0x348;
	uintptr_t PlayerState = 0x2c8;
	uintptr_t PawnPrivate = 0x320;
	uintptr_t PlayerArray = 0x2c0;
	uintptr_t Mesh = 0x328;
	uintptr_t ComponentToWorld = 0x1E0;
	uintptr_t Seconds = 0x148; // UWorld + 0x148
	uintptr_t LastRenderTime = 0x30C;
	uintptr_t BoneArray = 0x5A8;
	uintptr_t Velocity = 0x180;
	uintptr_t IsDBNO = 0x962;
	uintptr_t isDying = 0x718;
	uintptr_t bIsABot = 0x2B2;
	uintptr_t TeamId = 0x1261;
	uintptr_t RootComponent = 0x1b0;
	uintptr_t LocationUnderReticle = 0x2748;
	uintptr_t WeaponData = 0x690;
	uintptr_t CurrentWeapon = 0xDC8;
	uintptr_t WeaponProjectileSpeed = 0x1D20;
	uintptr_t WeaponProjectileGravity = 0x1D88;

	uintptr_t FNamePool = 0x12BA9140;
	uintptr_t FNameKey = 0x18;
	uintptr_t ItemDefinition = 0x368;
	uintptr_t OwningWorld = 0xC8;
	uintptr_t ULevelArray = 0x190;
	uintptr_t ULevelCount = ULevelArray + 8;
	uintptr_t AActorArray = 0x128;
	uintptr_t AActorCount = AActorArray + 8;
	uintptr_t Kills = 0x1274; //AFortPlayerStateAthena.KillScore: 0x1274
	uintptr_t TextPtr = 0x38;
	uintptr_t NameOffset = 0xAF0;
}