#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

#include "Actor.h"

class PlayerControls
{
	Ref<Actor> Player;
	float walkSpeed;
	float runSpeed;

	float jumpForce;
	float currentTime;
	float frameRate = 0.1f;
	int animationIndex = 0;

	Vec3 ZaxisVector = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 XaxisVector = Vec3(1.0f, 0.0f, 0.0f);

	bool isJumping = false;
	bool isGrounded = false;
};

#endif // !PLAYER_CONTROLS_H