#pragma once

struct Animation
{
	int index;
	int frames;
	int speed;
	bool loop;

	Animation() {}
	Animation(int i, int f, int s, int l)
	{
		index = i;
		frames = f;
		speed = s;
		loop = l;
	}
};
