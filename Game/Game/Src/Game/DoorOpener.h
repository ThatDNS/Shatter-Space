// @file: DoorOpener.h
//
// @brief: Header file for DoorOpener class responsible for opening the obstacle door.

#pragma once
#ifndef _DOOR_OPENER_H_
#define _DOOR_OPENER_H_

#include "Engine/Components/Component.h"

class DoorOpener : public Component
{
	// Indication to open the door
	bool openDoorNow = false;

	// If false, it'll open right
	bool openLeft = false;

	float moveSpeed = 10.0f;

public:
	DoorOpener() { type = DoorOpenerC; }

	void SetOpenDoor(bool value);
	void SetOpensLeft(bool value) { openLeft = value; }

	void Initialize() override {}
	void Update(float) override;
	void Destroy() override {}
};

#endif // !_DOOR_OPENER_H_
