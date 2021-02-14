#include "AreaMap.h"

#include <iostream>
#include <cmath>

#include "../Game.h"

const int roomMinSize = 16;
const int roomScale = 4;
const int roomRescaled = roomMinSize * roomScale;
const int lineThickness = 2 * roomScale;

void AreaMap::drawRoom(SDL_Rect roomDim, std::vector<bool> exits)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
    SDL_Renderer* renderer = globalbilboulga->getRenderer();

	if (roomDim.w % roomMinSize != 0 || roomDim.h % roomMinSize != 0) return;

	int nbExitsMaxX = (roomDim.w / 16);
	int nbExitsMaxY = (roomDim.h / 16);
	int nbExitsMax = nbExitsMaxX * 2 + nbExitsMaxY * 2;
	int firstExit = -1;
	int lastExit = 0;

	if (exits.size() < (size_t)nbExitsMax)
	{
		size_t exitsSize = (size_t)exits.size();
		for (size_t i = 0; i < nbExitsMax - exitsSize; i++) exits.push_back(false);
	}
	else if (exits.size() > (size_t)nbExitsMax)
	{
		size_t exitsSize = exits.size();
		for (size_t i = 0; i < exitsSize - nbExitsMax; i++) exits.pop_back();
	}

	for (size_t i = 0; i < exits.size(); i++)
	{
		if (exits[i] == 1)
		{
			firstExit = i;
			break;
		}
	}

	for (size_t i = 0; i < exits.size(); i++)
	{
		if (exits[i] == 1) lastExit = i;
	}


	roomDim.x *= roomScale;
	roomDim.y *= roomScale;
	roomDim.w *= roomScale;
	roomDim.h *= roomScale;


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &roomDim);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_Rect roomInsideDim = { roomDim.x + lineThickness, roomDim.y + lineThickness, roomDim.w - 2*lineThickness, roomDim.h - 2*lineThickness };
	SDL_RenderFillRect(renderer, &roomInsideDim);


	SDL_SetRenderDrawColor(renderer, 0, 120, 255, SDL_ALPHA_OPAQUE);

	Vector2D roomPos = { roomDim.x, roomDim.y };

	if (lastExit == firstExit)
	{
		if (firstExit < nbExitsMaxX)
		{
			drawHalfLine(roomPos, Vector2D({ firstExit + 1,0 }), Vector2D({ firstExit + 1,1 }));

			SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

			line.x = roomPos.x + (firstExit + 1) * roomRescaled - 3 * roomRescaled / 4;
			line.y = roomPos.y + roomRescaled - roomRescaled / 2 - lineThickness / 2;
			line.w = roomRescaled / 2;
			line.h = lineThickness;
			SDL_RenderFillRect(renderer, &line);
		}
		else if (firstExit < nbExitsMaxX + nbExitsMaxY)
		{
			firstExit -= nbExitsMaxX - 1;

			drawHalfLine(roomPos, Vector2D({ nbExitsMaxX,firstExit }), Vector2D({ nbExitsMaxX + 1,firstExit }));

			SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

			line.x = roomPos.x + nbExitsMaxX * roomRescaled - roomRescaled / 2 - lineThickness / 2;
			line.y = roomPos.y + firstExit * roomRescaled - 3 * roomRescaled / 4;
			line.w = lineThickness;
			line.h = roomRescaled / 2;
			SDL_RenderFillRect(renderer, &line);
		}
		else if (firstExit < 2 * nbExitsMaxX + nbExitsMaxY)
		{
			firstExit = 2 * nbExitsMaxX + nbExitsMaxY - firstExit;

			drawHalfLine(roomPos, Vector2D({ firstExit,nbExitsMaxY }), Vector2D({ firstExit,nbExitsMaxY + 1 }));

			SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

			line.x = roomPos.x + (firstExit) * roomRescaled - 3 * roomRescaled / 4;
			line.y = roomPos.y + nbExitsMaxY * roomRescaled - roomRescaled / 2 - lineThickness / 2;
			line.w = roomRescaled / 2;
			line.h = lineThickness;
			SDL_RenderFillRect(renderer, &line);
		}
		else
		{
			firstExit = 2 * nbExitsMaxX +  2 * nbExitsMaxY - firstExit;

			drawHalfLine(roomPos, Vector2D({ 0,firstExit }), Vector2D({ 1,firstExit }));

			SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

			line.x = roomPos.x + roomRescaled - roomRescaled / 2 - lineThickness / 2;
			line.y = roomPos.y + firstExit * roomRescaled - 3 * roomRescaled / 4;
			line.w = lineThickness;
			line.h = roomRescaled / 2;
			SDL_RenderFillRect(renderer, &line);
		}
	}
	else if (firstExit != -1)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int start = 0; start < nbExitsMax; start++)
			{
				if (exits[start])
				{
					for (int end = start + 1; end < nbExitsMax; end++)
					{
						if (exits[end])
						{
							if (start < nbExitsMaxX)
							{
								if (end < nbExitsMaxX)
								{
									drawHalfLine(roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
									drawLine(roomPos, Vector2D({ start + 1, 1 }), Vector2D({ end + 1,1 }));
									drawHalfLine(roomPos, Vector2D({ end + 1,0 }), Vector2D({ end + 1,1 }));
								}
								else if (end < nbExitsMaxX + nbExitsMaxY)
								{
									drawHalfLine(roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
									drawLine(roomPos, Vector2D({ start + 1, 1 }), Vector2D({ start + 1, end - nbExitsMaxX + 1 }));
									drawLine(roomPos, Vector2D({ start + 1, end - nbExitsMaxX + 1 }), Vector2D({ nbExitsMaxX, end - nbExitsMaxX + 1 }));
									drawHalfLine(roomPos, Vector2D({ nbExitsMaxX, end - nbExitsMaxX + 1 }), Vector2D({ nbExitsMaxX + 1,end - nbExitsMaxX + 1 }));
								}
								else if (end < 2 * nbExitsMaxX + nbExitsMaxY)
								{
									int tempEnd = 2 * nbExitsMaxX + nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));

									if (start + 1 < tempEnd) drawLine(roomPos, Vector2D({ start + 1, 1 }), Vector2D({ tempEnd, 1 }));
									if (start + 1 > tempEnd) drawLine(roomPos, Vector2D({ 2 * nbExitsMaxX + nbExitsMaxY - end, 1 }), Vector2D({ start + 1, 1 }));

									drawLine(roomPos, Vector2D({ tempEnd,1 }), Vector2D({ tempEnd,nbExitsMaxY }));
									drawHalfLine(roomPos, Vector2D({ tempEnd,nbExitsMaxY }), Vector2D({ tempEnd,nbExitsMaxY + 1 }));
								}
								else
								{
									int tempEnd = 2 * nbExitsMaxX + 2 * nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
									drawLine(roomPos, Vector2D({ start + 1,1 }), Vector2D({ start + 1,tempEnd }));
									drawLine(roomPos, Vector2D({ 1,tempEnd }), Vector2D({ start + 1,tempEnd }));
									drawHalfLine(roomPos, Vector2D({ 0,tempEnd }), Vector2D({ 1,tempEnd }));
								}
							}
							else if (start < nbExitsMaxX + nbExitsMaxY)
							{
								int tempStart = start - nbExitsMaxX + 1;

								if (end < nbExitsMaxX + nbExitsMaxY)
								{
									drawHalfLine(roomPos, Vector2D({ nbExitsMaxX,tempStart }), Vector2D({ nbExitsMaxX + 1,tempStart }));
									drawLine(roomPos, Vector2D({ nbExitsMaxX,tempStart }), Vector2D({ nbExitsMaxX,end + 1 - nbExitsMaxX }));
									drawHalfLine(roomPos, Vector2D({ nbExitsMaxX,end + 1 - nbExitsMaxX }), Vector2D({ nbExitsMaxX + 1,end + 1 - nbExitsMaxX }));
								}
								else if (end < 2 * nbExitsMaxX + nbExitsMaxY)
								{
									int tempEnd = 2 * nbExitsMaxX + nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ nbExitsMaxX,tempStart }), Vector2D({ nbExitsMaxX + 1,tempStart }));
									drawLine(roomPos, Vector2D({ tempEnd,tempStart }), Vector2D({ nbExitsMaxX,tempStart }));
									drawLine(roomPos, Vector2D({ tempEnd,tempStart }), Vector2D({ tempEnd,nbExitsMaxY }));
									drawHalfLine(roomPos, Vector2D({ tempEnd,nbExitsMaxY }), Vector2D({ tempEnd,nbExitsMaxY + 1 }));
								}
								else
								{
									int tempEnd = 2 * nbExitsMaxX + 2 * nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ nbExitsMaxX,tempStart }), Vector2D({ nbExitsMaxX + 1,tempStart }));

									if (tempStart > tempEnd) drawLine(roomPos, Vector2D({ nbExitsMaxX,tempEnd }), Vector2D({ nbExitsMaxX,tempStart }));
									else if (tempStart < tempEnd) drawLine(roomPos, Vector2D({ nbExitsMaxX,tempStart }), Vector2D({ nbExitsMaxX,tempEnd }));

									drawLine(roomPos, Vector2D({ 1,tempEnd }), Vector2D({ nbExitsMaxX,tempEnd }));
									drawHalfLine(roomPos, Vector2D({ 0,tempEnd }), Vector2D({ nbExitsMaxX,tempEnd }));
								}
							}
							else if (start < 2 * nbExitsMaxX + nbExitsMaxY)
							{
								int tempStart = 2 * nbExitsMaxX + nbExitsMaxY - start;

								if (end < 2 * nbExitsMaxX + nbExitsMaxY)
								{
									int tempEnd = 2 * nbExitsMaxX + nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ tempStart, nbExitsMaxY }), Vector2D({ tempStart, nbExitsMaxY + 1 }));
									drawLine(roomPos, Vector2D({ tempEnd, nbExitsMaxY }), Vector2D({ tempStart, nbExitsMaxY }));
									drawHalfLine(roomPos, Vector2D({ tempEnd, nbExitsMaxY }), Vector2D({ tempEnd, nbExitsMaxY + 1 }));
								}
								else
								{
									int tempEnd = 2 * nbExitsMaxX + 2 * nbExitsMaxY - end;

									drawHalfLine(roomPos, Vector2D({ tempStart, nbExitsMaxY }), Vector2D({ tempStart, nbExitsMaxY + 1 }));
									drawLine(roomPos, Vector2D({ tempStart, tempEnd }), Vector2D({ tempStart, nbExitsMaxY }));
									drawLine(roomPos, Vector2D({ 1, tempEnd }), Vector2D({ tempStart, tempEnd }));
									drawHalfLine(roomPos, Vector2D({ 0, tempEnd }), Vector2D({ 1, tempEnd }));
								}
							}
							else
							{
								int tempStart = 2 * nbExitsMaxX + 2 * nbExitsMaxY - start;
								int tempEnd = 2 * nbExitsMaxX + 2 * nbExitsMaxY - end;

								drawHalfLine(roomPos, Vector2D({ 0, tempStart }), Vector2D({ 1, tempStart }));
								drawLine(roomPos, Vector2D({ 1, tempEnd }), Vector2D({ 1, tempStart }));
								drawHalfLine(roomPos, Vector2D({ 0, tempEnd }), Vector2D({ 1, tempEnd }));
							}

							start = end;
						}
					}
				}
			}

			for (size_t i = 0; i < exits.size(); i++)
			{
				exits[i] = 0;
			}

			exits[firstExit] = 1;
			exits[lastExit] = 1;
		}
	}
}

void AreaMap::drawLine(Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
    SDL_Renderer* renderer = globalbilboulga->getRenderer();

	SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

	if (lineStart.x != 0) line.x = roomPos.x + lineStart.x * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.y != 0) line.y = roomPos.y + lineStart.y * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.x != lineEnd.x) line.w = (lineEnd.x - lineStart.x) * roomRescaled + lineThickness / 2;
	if (lineStart.y != lineEnd.y) line.h = (lineEnd.y - lineStart.y) * roomRescaled + lineThickness / 2;
	SDL_RenderFillRect(renderer, &line);
}

void AreaMap::drawHalfLine(Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
    SDL_Renderer* renderer = globalbilboulga->getRenderer();

	SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

	if (lineStart.x != 0) line.x = roomPos.x + lineStart.x * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.y != 0) line.y = roomPos.y + lineStart.y * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.x != lineEnd.x) line.w = (lineEnd.x - lineStart.x) * roomRescaled / 2 + lineThickness / 2;
	if (lineStart.y != lineEnd.y) line.h = (lineEnd.y - lineStart.y) * roomRescaled / 2 + lineThickness / 2;
	SDL_RenderFillRect(renderer, &line);
}
