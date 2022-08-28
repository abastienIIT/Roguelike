#pragma once

struct Animation
{
	int index = 0;
	int frames = 0;
	int speed = 0;
	bool loop = 0;

	Animation() {}
	Animation(int i, int f, int s, int l)
	{
		index = i;
		frames = f;
		speed = s;
		loop = l;
	}
};
