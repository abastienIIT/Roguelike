#include "AreaMap.h"
#include <iostream>

const int roomMinSize = 16;
const int roomScale = 4;
const int roomRescaled = roomMinSize * roomScale;
const int lineThickness = 8;

void AreaMap::drawRoom(SDL_Renderer* renderer, SDL_Rect roomDim, std::vector<bool> exits)
{
	if (roomDim.w % roomMinSize != 0 || roomDim.h % roomMinSize != 0) return;

	int nbExitsMaxX = (roomDim.w / 16);
	int nbExitsMaxY = (roomDim.h / 16);
	int nbExitsMax = nbExitsMaxX * 2 + nbExitsMaxY * 2;

	SDL_Rect line = { 0,0,lineThickness,lineThickness };

	if (exits.size() < nbExitsMax)
	{
		size_t exitsSize = exits.size();
		for (int i = 0; i < nbExitsMax - exitsSize; i++) exits.push_back(false);
	}
	else if (exits.size() > nbExitsMax)
	{
		size_t exitsSize = exits.size();
		for (int i = 0; i < exitsSize - nbExitsMax; i++) exits.pop_back();
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
							drawHalfLine(renderer, roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
							drawLine(renderer, roomPos, Vector2D({ start + 1, 1 }), Vector2D({ end + 1,1 }));
							drawHalfLine(renderer, roomPos, Vector2D({ end + 1,0 }), Vector2D({ end + 1,1 }));
						}
						else if (end < nbExitsMaxX + nbExitsMaxY)
						{
							drawHalfLine(renderer, roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
							drawLine(renderer, roomPos, Vector2D({ start + 1, 1 }), Vector2D({ start + 1, end - nbExitsMaxX + 1 }));
							drawLine(renderer, roomPos, Vector2D({ start + 1, end - nbExitsMaxX + 1 }), Vector2D({ nbExitsMaxX, end - nbExitsMaxX + 1 }));							
							drawHalfLine(renderer, roomPos, Vector2D({ nbExitsMaxX, end - nbExitsMaxX + 1 }), Vector2D({ nbExitsMaxX + 1,end - nbExitsMaxX + 1 }));
						}
						else if (end < 2 * nbExitsMaxX + nbExitsMaxY)
						{
							int tempEnd = 2 * nbExitsMaxX + nbExitsMaxY - end;

							drawHalfLine(renderer, roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));

							if (start + 1 < tempEnd) drawLine(renderer, roomPos, Vector2D({ start + 1, 1 }), Vector2D({ tempEnd, 1 }));
							if (start + 1 > tempEnd) drawLine(renderer, roomPos, Vector2D({ 2 * nbExitsMaxX + nbExitsMaxY - end, 1 }), Vector2D({ start + 1, 1 }));

							drawLine(renderer, roomPos, Vector2D({ tempEnd,1 }), Vector2D({ tempEnd,nbExitsMaxY }));
							drawHalfLine(renderer, roomPos, Vector2D({tempEnd,nbExitsMaxY }), Vector2D({ tempEnd,nbExitsMaxY + 1}));
						}
						else
						{
							int tempEnd = 2 * nbExitsMaxX + 2 * nbExitsMaxY - end;

							drawHalfLine(renderer, roomPos, Vector2D({ start + 1,0 }), Vector2D({ start + 1,1 }));
							drawLine(renderer, roomPos, Vector2D({ start + 1,1 }), Vector2D({ start + 1,tempEnd }));
							drawLine(renderer, roomPos, Vector2D({ 1,tempEnd }), Vector2D({ start + 1,tempEnd }));
							drawHalfLine(renderer, roomPos, Vector2D({ 0,tempEnd }), Vector2D({ 1,tempEnd }));
						}
					}

					start = end;
				}
			}
		}
	}



	
}

void AreaMap::drawLine(SDL_Renderer* renderer, Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd)
{
	SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

	if (lineStart.x != 0) line.x = roomPos.x + lineStart.x * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.y != 0) line.y = roomPos.y + lineStart.y * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.x != lineEnd.x) line.w = (lineEnd.x - lineStart.x) * roomRescaled + lineThickness / 2;
	if (lineStart.y != lineEnd.y) line.h = (lineEnd.y - lineStart.y) * roomRescaled + lineThickness / 2;
	SDL_RenderFillRect(renderer, &line);
}

void AreaMap::drawHalfLine(SDL_Renderer* renderer, Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd)
{
	SDL_Rect line = { roomPos.x,roomPos.y,lineThickness,lineThickness };

	if (lineStart.x != 0) line.x = roomPos.x + lineStart.x * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.y != 0) line.y = roomPos.y + lineStart.y * roomRescaled - roomRescaled / 2 - lineThickness / 2;
	if (lineStart.x != lineEnd.x) line.w = (lineEnd.x - lineStart.x) * roomRescaled / 2 + lineThickness / 2;
	if (lineStart.y != lineEnd.y) line.h = (lineEnd.y - lineStart.y) * roomRescaled / 2 + lineThickness / 2;
	SDL_RenderFillRect(renderer, &line);
}
