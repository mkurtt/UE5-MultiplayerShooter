﻿#pragma once

UENUM()
enum class ECombatState : uint8
{
	ECS_UnOccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGrenade UMETA(DisplayName = "ThrowingGrenade"),
	
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};