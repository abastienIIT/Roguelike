#pragma warning(disable : 26451)

#include "Map.h"
#include "../Game.h"
#include "AreaMap.h"
#include "../Common/Globalbilboulga.h"
#include "../ComponentsManagement/ActionsComponent/ActionsComponent.h"

#define STEP_BY_STEP_MAP_GENE 0
#define NB_MAX_EXITS 32

Map::Map(Vector2D areaSize, Area* area, Manager* manager, Entity* player)
{
	this->areaSize = areaSize;
	this->area = area;
	this->manager = manager;
	this->player = player;
	this->playerPosition = &player->getComponent<TransformComponent>().truePosition;

	currentRoom = nullptr;

	availableMaps = area->getAvailableMaps();

	roomMap.clear();

	std::vector<Room*> column(areaSize.y, nullptr);

	for (int x = 0; x < areaSize.x; x++)
	{
		roomMap.emplace_back(column);
	}


	exitMap.clear();
	std::vector<bool> roomExits(4, false);
	std::vector<std::vector<bool>> columnExit(areaSize.y, roomExits);
	for (int x = 0; x < areaSize.x; x++)
	{
		exitMap.emplace_back(columnExit);
	}
}

void Map::checkPlayerPosition()
{
	if (!(playerPosition->y < 0 || playerPosition->x > Globalbilboulga::getInstance()->getCurrentRoomSize().x || playerPosition->y > Globalbilboulga::getInstance()->getCurrentRoomSize().y || playerPosition->x < 0))
		return;

	Vector2D playerPosOnMapGrid = { currentRoom->getRoomPosition().x + (int)playerPosition->x / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize()),
									currentRoom->getRoomPosition().y + (int)playerPosition->y / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize()) };

	Room* nextRoom = nullptr;
	Vector2D spawnCoordInNextRoom;

	if (playerPosition->y < 0)
	{
		nextRoom = roomMap.at(currentRoom->getRoomPosition().x + (int)playerPosition->x / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize())).at(currentRoom->getRoomPosition().y - 1);

		spawnCoordInNextRoom.x = (ROOM_BASE_SIZE / 2 + ROOM_BASE_SIZE * (playerPosOnMapGrid.x - nextRoom->getRoomPosition().x)) * Globalbilboulga::getInstance()->getCurrentTileSize();
		spawnCoordInNextRoom.y = ROOM_BASE_SIZE * (nextRoom->getRoomSize().y) * Globalbilboulga::getInstance()->getCurrentTileSize() - 1;

		player->getComponent<ActionsComponent>().resetJumpAtRoomChange(spawnCoordInNextRoom);
	}
	else if (playerPosition->x > Globalbilboulga::getInstance()->getCurrentRoomSize().x)
	{
		nextRoom = roomMap.at(currentRoom->getRoomPosition().x + currentRoom->getRoomSize().x).at(currentRoom->getRoomPosition().y + (int)playerPosition->y / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize()));
		
		spawnCoordInNextRoom.x = 1;
		spawnCoordInNextRoom.y = (ROOM_BASE_SIZE / 2 + ROOM_BASE_SIZE * (playerPosOnMapGrid.y - nextRoom->getRoomPosition().y)) * Globalbilboulga::getInstance()->getCurrentTileSize() + 16;
	}
	else if (playerPosition->y > Globalbilboulga::getInstance()->getCurrentRoomSize().y)
	{
		nextRoom = roomMap.at(currentRoom->getRoomPosition().x + (int)playerPosition->x / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize())).at(currentRoom->getRoomPosition().y + currentRoom->getRoomSize().y);

		spawnCoordInNextRoom.x = (ROOM_BASE_SIZE / 2 + ROOM_BASE_SIZE * (playerPosOnMapGrid.x - nextRoom->getRoomPosition().x)) * Globalbilboulga::getInstance()->getCurrentTileSize();
		spawnCoordInNextRoom.y = 1;
	}
	else if (playerPosition->x < 0)
	{
		nextRoom = roomMap.at(currentRoom->getRoomPosition().x - 1).at(currentRoom->getRoomPosition().y + (int)playerPosition->y / (ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize()));

		spawnCoordInNextRoom.x = ROOM_BASE_SIZE * (nextRoom->getRoomSize().x) * Globalbilboulga::getInstance()->getCurrentTileSize() - 1;
		spawnCoordInNextRoom.y = (ROOM_BASE_SIZE / 2 + ROOM_BASE_SIZE * (playerPosOnMapGrid.y - nextRoom->getRoomPosition().y)) * Globalbilboulga::getInstance()->getCurrentTileSize() + 16;
	}

	deactivateCurrentRoom();
	activateRoom(nextRoom);
	playerPosition->x = (double)spawnCoordInNextRoom.x;
	playerPosition->y = (double)spawnCoordInNextRoom.y;
}

bool Map::generateMap(std::string startName, Vector2D startPos, std::string endName, Vector2D endPos)
{
	Room* startRoom = new Room(startName, "Specials/Start", { 1, 1 }, startPos);
	startRoom->setExit(1, true);
	addRoom(startRoom);
	specialsRoomsList.emplace_back(startRoom);
	

	Room* endRoom = new Room(endName, "Specials/End", { 1, 1 }, endPos);
	endRoom->setExit(3, true);
	addRoom(endRoom);
	specialsRoomsList.emplace_back(endRoom);

	Vector2D midRoomPos = { 1,1 };
	Room* midRoom = new Room("Mid", "Specials/Mid", {1, 1}, midRoomPos);
	midRoom->setExit(3, true);
	midRoom->setExit(1, true);
	addRoom(midRoom);
	specialsRoomsList.emplace_back(midRoom);

#if STEP_BY_STEP_MAP_GENE
	displayMap();
#endif

	generateNextRoom({ startPos.x + 1, startPos.y });
	
	for (Room* specialRoom : specialsRoomsList) //Pour chaque pièce spéciale
	{
		while (checkEveryExitIsOk(specialRoom) == false) //Tant qu'une des sorties de la pièce n'est pas reliée
		{
			if (errorInMapGeneration)
			{
				std::cout << "Error in map generation" << std::endl;
				return false;
			}

			regenerateClosestRoom(specialRoom->getRoomPosition());
		}
	}


	for (Room* room : roomList)
	{
		area->loadRoom(room);
	}

	activateRoom(startRoom);

	return true;
}

void Map::generateNextRoom(Vector2D pos)
{
	Vector2D topLeftMaxSize = pos;
	Vector2D maxSize = { 1,1 }; //Taille maximale que la nouvelle pièce peut faire
	bool hasGrown = true;
	bool canGrow = false;
	while (hasGrown) //Tant que la pièce arrive à grandir suivant au moins un axe, la pièce continue d'essayer de grandir
	{
		hasGrown = false;

		//La pièce essaie de grandir vers le haut
		for (int x = 0; x < maxSize.x; x++) //Pour toute sa largeur
		{
			canGrow = true;
			if (topLeftMaxSize.y == 0 || roomMap.at(topLeftMaxSize.x + x).at(topLeftMaxSize.y - 1) != nullptr) //Si un bord de map ou une autre pièce se trouve au dessus d'un des morceaux de la pièce
			{
				canGrow = false; //Elle ne peut pas grandir dans cette direction
				break;
			}
		}
		if (canGrow) //Si elle peut grandir
		{
			maxSize.y += 1; //Sa taille maximale en hauteur augmente
			topLeftMaxSize.y -= 1; //Le coin supérieur gauche est donc remonté
			hasGrown = true; //La pièce a grandi
		}

		//La pièce essaie de grandir vers la droite
		for (int y = 0; y < maxSize.y; y++)
		{	
			canGrow = true;
			if (topLeftMaxSize.x + maxSize.x >= areaSize.x || roomMap.at(topLeftMaxSize.x + maxSize.x).at(topLeftMaxSize.y + y) != nullptr)
			{
				canGrow = false;
				break;
			}
		}
		if (canGrow)
		{
			maxSize.x += 1;
			hasGrown = true;
		}
		
		//La pièce essaie de grandir vers la bas
		for (int x = 0; x < maxSize.x; x++)
		{
			canGrow = true;
			if (topLeftMaxSize.y + maxSize.y >= areaSize.y || roomMap.at(topLeftMaxSize.x + x).at(topLeftMaxSize.y + maxSize.y) != nullptr)
			{
				canGrow = false;
				break;
			}
		}
		if (canGrow)
		{
			maxSize.y += 1;
			hasGrown = true;
		}

		//La pièce essaie de grandir vers la gauche
		for (int y = 0; y < maxSize.y; y++)
		{
			canGrow = true;
			if (topLeftMaxSize.x == 0 || roomMap.at(topLeftMaxSize.x - 1).at(topLeftMaxSize.y + y) != nullptr)
			{
				canGrow = false;
				break;
			}
		}
		if (canGrow)
		{
			maxSize.x += 1;
			topLeftMaxSize.x -= 1;
			hasGrown = true;
		}

	}

	std::vector<int> indexAvailableMapsWithOkSize; //Vecteur stockant les indexes des tailles de maps OK venant de availableMaps

	for (int i = 0; i < availableMaps->size(); i++)
	{
		if (availableMaps->at(i).first.x <= maxSize.x && availableMaps->at(i).first.y <= maxSize.y) //Si la taille des maps à l'index i est inférieur à la taille disponible
			indexAvailableMapsWithOkSize.emplace_back(i); //L'index du vecteur contenant toutes les maps de cette taille est ajouté aux indexes OK
	}

	int chosenSizeIndex = 0;
	std::vector<int> goodRoomIndexes;
	Vector2D chosenSize;
	int nbExits = 0;
	Vector2D roomPos;

	while (goodRoomIndexes.size() == 0)
	{
		chosenSizeIndex = indexAvailableMapsWithOkSize.at(rand() % indexAvailableMapsWithOkSize.size()); //Une taille de map parmi les tailles qui rentrent est choisie au hasard
		chosenSize = availableMaps->at(chosenSizeIndex).first;
		nbExits = chosenSize.x * 2 + chosenSize.y * 2;

		
		roomPos = pos;
		bool goodPosFound = false;
		for (int y = 0; y < chosenSize.y; y++, roomPos.y--) //Recherche de la position qui permet de faire rentrer la pièce où elle doit aller. La pièce est décalée de droite à gauche et de bas en haut jusqu'à rentrer.
		{
			roomPos.x = pos.x;

			for (int x = 0; x < chosenSize.x; x++, roomPos.x--)
			{
				if (canBeAdded(roomPos, chosenSize))
				{
					goodPosFound = true;
					break;
				}
			}

			if (goodPosFound)
				break;
		}


		for (int i = 0; i < availableMaps->at(chosenSizeIndex).second.size(); i++) //Pour toute les formes de cette taille
		{
			bool roomIsGood = true;

			for (int j = 0; j < nbExits; j++) //Pour chaque sortie possible pour cette taille
			{
				bool exitAtJ = availableMaps->at(chosenSizeIndex).second.at(i).first >> (nbExits - j - 1) & 1; //Sortie présente ou absente

				if (j < chosenSize.x) //Si la sortie est vers le haut
				{
					if (roomPos.y == 0) //Si la pièce est en haut de la map
					{
						if (exitAtJ) //Et qu'elle à une sortie vers le haut
						{
							roomIsGood = false; //La formes est invalide
							break;
						}
						else
							continue;
					}
					else if (roomMap.at(roomPos.x + j).at(roomPos.y - 1) == nullptr) //Si il n'y a pas de pièce au dessus, la sortie est possible
					{
						continue;
					}
					else if (exitMap.at(roomPos.x + j).at(roomPos.y - 1).at(2) != exitAtJ) //Si il y a une pièce au dessus et que la présence/abscence de sortie ne match pas avec la pièce au dessus
					{
						roomIsGood = false; //La formes est invalide
						break;
					}
				}
				else if (j < chosenSize.x + chosenSize.y) //Si la sortie est vers la droite
				{
					if (roomPos.x + chosenSize.x - 1 == areaSize.x - 1)
					{
						if (exitAtJ)
						{
							roomIsGood = false;
							break;
						}
						else
							continue;
					}
					else if (roomMap.at(roomPos.x + chosenSize.x).at(roomPos.y + j - chosenSize.x) == nullptr)
					{
						continue;
					}
					else if (exitMap.at(roomPos.x + chosenSize.x).at(roomPos.y + j - chosenSize.x).at(3) != exitAtJ)
					{
						roomIsGood = false;
						break;
					}
				}
				else if (j < chosenSize.x * 2 + chosenSize.y) //Si la sortie est vers le bas
				{
					if (roomPos.y + chosenSize.y - 1 == areaSize.y - 1)
					{
						if (exitAtJ)
						{
							roomIsGood = false;
							break;
						}
						else
							continue;
					}
					else if (roomMap.at(roomPos.x + (chosenSize.x * 2 + chosenSize.y - (j + 1))).at(roomPos.y + chosenSize.y) == nullptr)
					{
						continue;
					}
					else if (exitMap.at(roomPos.x + (chosenSize.x * 2 + chosenSize.y - (j + 1))).at(roomPos.y + chosenSize.y).at(0) != exitAtJ)
					{
						roomIsGood = false;
						break;
					}
				}
				else //Si la sortie est vers la gauche
				{
					if (roomPos.x == 0)
					{
						if (exitAtJ)
						{
							roomIsGood = false;
							break;
						}
						else
							continue;
					}
					else if (roomMap.at(roomPos.x - 1).at(roomPos.y + (chosenSize.x * 2 + chosenSize.y * 2 - (j + 1))) == nullptr)
					{
						continue;
					}
					else if (exitMap.at(roomPos.x - 1).at(roomPos.y + (chosenSize.x * 2 + chosenSize.y * 2 - (j + 1))).at(1) != exitAtJ)
					{
						roomIsGood = false;
						break;
					}
				}
			}

			if (roomIsGood) //Si après que chaque sortie ait étée testée la forme est toujours valide, l'index de la forme est ajouté aux indexes OK pour cette taille et la taille est lockée
			{
				goodRoomIndexes.emplace_back(i);
			}
		}

		if (goodRoomIndexes.size() == 0) //Si il n'y a aucune forme OK dans cette taille, son index est retiré de la liste des indexes de taille et une autre taille sera tirée au hasard au prochain tour de boucle
		{
			indexAvailableMapsWithOkSize.erase(std::remove(indexAvailableMapsWithOkSize.begin(), indexAvailableMapsWithOkSize.end(), chosenSizeIndex), indexAvailableMapsWithOkSize.end());
		}
	}

	int chosenShapeIndex = goodRoomIndexes.at(rand() % goodRoomIndexes.size()); //Choix de la forme parmis les formes OK
	int chosenShape = availableMaps->at(chosenSizeIndex).second.at(chosenShapeIndex).first;
	size_t nbRoomAvailableForShape = availableMaps->at(chosenSizeIndex).second.at(chosenShapeIndex).second.size();
	int chosenRoomNumber = rand() % nbRoomAvailableForShape; //Choix de la pièce parmis toutes les pièces de cette forme

	std::string roomFolder = std::to_string(chosenSize.x * ROOM_BASE_SIZE) + "x" + std::to_string(chosenSize.y * ROOM_BASE_SIZE) + "/" + std::bitset<NB_MAX_EXITS>(chosenShape).to_string().substr(NB_MAX_EXITS - nbExits);
	std::string roomName = availableMaps->at(chosenSizeIndex).second.at(chosenShapeIndex).second.at(chosenRoomNumber);

	Room* newRoom = new Room(roomName, roomFolder, chosenSize, roomPos); //Création de la nouvelle pièce
	newRoom->setExitsFromInt(chosenShape); //Set des sorties
	addRoom(newRoom); //Ajout de la pièce sur la map des pièces

#if STEP_BY_STEP_MAP_GENE
	displayMap();
#endif

	std::vector<bool>* newRoomExits = newRoom->getExits();
	for (int i = 0; i < newRoomExits->size(); i++) //Pour chaque sortie possible
	{
		if (newRoomExits->at(i)) //Si il y a une sortie
		{
			Vector2D nextRoomPos; //Position sur laquelle la sortie donne

			if (i < chosenSize.x)
			{
				nextRoomPos.x = roomPos.x + i;
				nextRoomPos.y = roomPos.y - 1;
			}
			else if (i < chosenSize.x + chosenSize.y)
			{
				nextRoomPos.x = roomPos.x + chosenSize.x;
				nextRoomPos.y = roomPos.y + i - chosenSize.x;
			}
			else if (i < chosenSize.x * 2 + chosenSize.y)
			{
				nextRoomPos.x = roomPos.x + (chosenSize.x * 2 + chosenSize.y - (i + 1));
				nextRoomPos.y = roomPos.y + chosenSize.y;
			}
			else 
			{
				nextRoomPos.x = roomPos.x - 1;
				nextRoomPos.y = roomPos.y + (chosenSize.x * 2 + chosenSize.y * 2 - (i + 1));
			}

			if (roomMap.at(nextRoomPos.x).at(nextRoomPos.y) == nullptr) //Si il n'y a pas de pièce
				generateNextRoom(nextRoomPos); //Générer une pièce
		}
	}
}

void Map::regenerateClosestRoom(Vector2D pos)
{
	std::vector<Vector2D> emptyRoomsOK;
	Vector2D currentRoom;
	Room* roomToCheck;

	for (int y = 0; y < areaSize.y; y++) //Pour chaque case de la grille
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			if (roomMap.at(x).at(y) == nullptr) //Si la case est vide
			{
				if (y != 0 && roomMap.at(x).at(y - 1) != nullptr) //Si la case au dessus n'est pas vide
				{
					roomToCheck = roomMap.at(x).at(y - 1);
					if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
					{ //Et si la case au dessus n'est pas une pièce spéciale
						currentRoom = { x,y };
						emptyRoomsOK.emplace_back(currentRoom); //La case vide peut être la source du reset
						continue;
					}
				}
				if (x < areaSize.x - 1 && roomMap.at(x + 1).at(y) != nullptr) //Si la case à droite n'est pas vide
				{
					roomToCheck = roomMap.at(x + 1).at(y);
					if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
					{
						currentRoom = { x,y };
						emptyRoomsOK.emplace_back(currentRoom);
						continue;
					}
				}
				if (y < areaSize.y - 1 && roomMap.at(x).at(y + 1) != nullptr) //Si la case en dessous n'est pas vide
				{
					roomToCheck = roomMap.at(x).at(y + 1);
					if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
					{
						currentRoom = { x,y };
						emptyRoomsOK.emplace_back(currentRoom);
						continue;
					}
				}
				if (x != 0 && roomMap.at(x - 1).at(y) != nullptr) //Si la case à gauche n'est pas vide
				{
					roomToCheck = roomMap.at(x - 1).at(y);
					if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
					{
						currentRoom = { x,y };
						emptyRoomsOK.emplace_back(currentRoom);
						continue;
					}
				}
			}
		}
	}

	// Recherche de la case à reset la plus proche de la case spéciale
	Vector2D closestPosition;
	double shortestDistance = INT_MAX;
	for (Vector2D roomOK : emptyRoomsOK)
	{
		double currentDistance = roomOK.distanceTo(pos);
		if (currentDistance < shortestDistance)
		{
			closestPosition = roomOK;
			shortestDistance = currentDistance;
		}
	}

	//Détérmination de la case à supprimer autour de la case vide. La case à supprimer ne doit ni être vide ni être une pièce spéciale
	std::vector<Room*> deletionCandidates;
	if (closestPosition.y != 0 && roomMap.at(closestPosition.x).at(closestPosition.y - 1) != nullptr)
	{
		roomToCheck = roomMap.at(closestPosition.x).at(closestPosition.y - 1);
		if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
			deletionCandidates.emplace_back(roomToCheck); //Si la case au dessus n'est ni vide ni une pièce spéciale, elle peut être supprimée
	}
	if (closestPosition.x < areaSize.x - 1 && roomMap.at(closestPosition.x + 1).at(closestPosition.y) != nullptr)
	{
		roomToCheck = roomMap.at(closestPosition.x + 1).at(closestPosition.y);
		if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
			deletionCandidates.emplace_back(roomToCheck); //Case à droite
	}
	if (closestPosition.y < areaSize.y - 1 && roomMap.at(closestPosition.x).at(closestPosition.y + 1) != nullptr)
	{
		roomToCheck = roomMap.at(closestPosition.x).at(closestPosition.y + 1);
		if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
			deletionCandidates.emplace_back(roomToCheck); //Case en dessous
	}
	if (closestPosition.x != 0 && roomMap.at(closestPosition.x - 1).at(closestPosition.y) != nullptr)
	{
		roomToCheck = roomMap.at(closestPosition.x - 1).at(closestPosition.y);
		if (std::find_if(specialsRoomsList.begin(), specialsRoomsList.end(), [&roomToCheck](const Room* sr) { return sr == roomToCheck; }) == specialsRoomsList.end())
			deletionCandidates.emplace_back(roomToCheck); //Case à gauche
	}

	Room* roomToDelete = deletionCandidates.at(rand() % deletionCandidates.size()); //Une des cases pouvant être supprimée est choisie au hasard
	std::vector<Vector2D> deletedPositions;

	//Recherche et suppression de toutes les cases faisant partie de la pièce
	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			if (roomMap.at(x).at(y) == roomToDelete)
			{
				Vector2D delPos = { x,y };
				roomMap.at(x).at(y) = nullptr;

				exitMap.at(x).at(y).assign(4, false);

				deletedPositions.emplace_back(delPos);
			}
		}
	}
	roomList.erase(std::remove_if(roomList.begin(), roomList.end(), [&roomToDelete](const Room* r) {return r == roomToDelete; }));
	delete roomToDelete;

#if STEP_BY_STEP_MAP_GENE
	displayMap();
#endif

	//Reprendre la génération sur les cases supprimées
	for (Vector2D roomToRegenerate : deletedPositions)
	{
		if (roomMap.at(roomToRegenerate.x).at(roomToRegenerate.y) == nullptr)
			generateNextRoom(roomToRegenerate);
	}
}

bool Map::checkEveryExitIsOk(Room* roomToCheck)
{
	Vector2D roomSize = roomToCheck->getRoomSize();
	Vector2D roomPos = roomToCheck->getRoomPosition();
	std::vector<bool>* roomExits = roomToCheck->getExits();

	for (int i = 0; i < roomExits->size(); i++) //Pour chaque exit possible
	{
		if (roomExits->at(i)) //Si il y a une exit
		{
			if (i < roomSize.x) //Si l'exit est au dessus de la case
			{
				if (roomPos.y == 0) //Si la pièce est au bord en haut de la map
				{
					std::cout << "Special room " + roomToCheck->getName() + " mal placée" << std::endl; //Il ne devrait pas y avoir d'exit qui pointe vers le bord de map, erreur dans le placement de la pièce.
					errorInMapGeneration = true;
					return false;
				}
				else if (roomMap.at(roomPos.x + i).at(roomPos.y - 1) == nullptr) //Si la case sur laquelle donne l'exit est vide
				{
					return false; //La génération n'est pas terminée	
				}
				else if (exitMap.at(roomPos.x + i).at(roomPos.y - 1).at(2) == false) //Si la case n'est pas vide mais qu'aucune sortie de cette case ne donne sur la pièce courante
				{
					std::cout << "Génération incorrecte" << std::endl; //Il y a eu une erreur dans la génération
					errorInMapGeneration = true;
					return false;
				}
			}
			else if (i < roomSize.x + roomSize.y) //Si l'exit est à gauche de la map
			{
				if (roomPos.x + roomSize.x == areaSize.x)
				{
					std::cout << "Special room " + roomToCheck->getName() + " mal placée" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
				else if (roomMap.at(roomPos.x + roomSize.x).at(roomPos.y + i - roomSize.x) == nullptr)
				{
					return false;
				}
				else if (exitMap.at(roomPos.x + roomSize.x).at(roomPos.y + i - roomSize.x).at(3) == false)
				{
					std::cout << "Génération incorrecte" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
			}
			else if (i < roomSize.x * 2 + roomSize.y) //Si l'exit est en dessous de la map
			{
				if (roomPos.y + roomSize.y == areaSize.y)
				{
					std::cout << "Special room " + roomToCheck->getName() + " mal placée" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
				else if (roomMap.at(roomPos.x + (roomSize.x * 2 + roomSize.y - (i + 1))).at(roomPos.y + roomSize.y) == nullptr)
				{
					return false;
				}
				else if (exitMap.at(roomPos.x + (roomSize.x * 2 + roomSize.y - (i + 1))).at(roomPos.y + roomSize.y).at(0) == false)
				{
					std::cout << "Génération incorrecte" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
			}
			else //Si l'exit est à droite de la map
			{
				if (roomPos.x == 0)
				{
					std::cout << "Special room " + roomToCheck->getName() + " mal placée" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
				else if (roomMap.at(roomPos.x - 1).at(roomPos.y + (roomSize.x * 2 + roomSize.y * 2 - (i + 1))) == nullptr)
				{
					return false;
				}
				else if (exitMap.at(roomPos.x - 1).at(roomPos.y + (roomSize.x * 2 + roomSize.y * 2 - (i + 1))).at(1) == false)
				{
					std::cout << "Génération incorrecte" << std::endl;
					errorInMapGeneration = true;
					return false;
				}
			}
		}
	}

	return true;
}

bool Map::addRoom(Room* room)
{
	Vector2D roomSize = room->getRoomSize();
	Vector2D roomPos = room->getRoomPosition();

	//Check room can be added
	bool canBeAdded = true;

	for (int x = roomPos.x; x < roomPos.x + roomSize.x; x++)
	{
		if (roomMap.size() - 1 < x)
		{
			canBeAdded = false;
			break;
		}
			
		for (int y = roomPos.y; y < roomPos.y + roomSize.y; y++)
		{
			if (roomMap.size() - 1 < y)
			{
				canBeAdded = false;
				break;
			}

			if (roomMap.at(x).at(y) != nullptr)
			{
				canBeAdded = false;
				break;
			}
		}
	}

	if (!canBeAdded)
	{
		std::cout << "Failed to add room " << room->getName() << " in map at pos " << roomPos << std::endl;
		return false;
	}

	//Add room
	for (int x = roomPos.x; x < roomPos.x + roomSize.x; x++)
	{
		for (int y = roomPos.y; y < roomPos.y + roomSize.y; y++)
		{
			roomMap.at(x).at(y) = room;
		}
	}
	roomList.emplace_back(room);

	int exitNumber = 0;
	for (bool exit : *room->getExits())
	{
		if (exitNumber < roomSize.x)
		{
			exitMap.at(roomPos.x + exitNumber).at(roomPos.y).at(0) = exit;
		}
		else if (exitNumber < roomSize.x + roomSize.y)
		{
			exitMap.at(roomPos.x + roomSize.x - 1).at(roomPos.y + exitNumber - roomSize.x).at(1) = exit;
		}
		else if (exitNumber < roomSize.x * 2 + roomSize.y)
		{
			exitMap.at(roomPos.x + (roomSize.x * 2 + roomSize.y - (exitNumber + 1))).at(roomPos.y + roomSize.y - 1).at(2) = exit;
		}
		else
		{
			exitMap.at(roomPos.x).at(roomPos.y + (roomSize.x * 2 + roomSize.y * 2 - (exitNumber + 1))).at(3) = exit;
		}
		exitNumber++;
	}

	return true;
}

void Map::deactivateCurrentRoom()
{
	manager->clearGroup(Game::Maps);
	manager->clearGroup(Game::TerrainColliders);
	manager->clearGroup(Game::Traps);
	manager->clearGroup(Game::Enemies);
	manager->clearGroup(Game::Projectiles);

	manager->clearActivatedEntities();

	Globalbilboulga::getInstance()->setCurrentMapCollider(nullptr);

	manager->activateManyEntities(&manager->getGroup(Game::Players));
}

void Map::activateRoom(Room* room)
{
	manager->setGroup(room->getTiles(), Game::Maps);
	manager->setGroup(room->getTilesCollider(), Game::TerrainColliders);
	manager->setGroup(room->getTraps(), Game::Traps);
	manager->setGroup(room->getEnemies(), Game::Enemies);

	manager->activateManyEntities(room->getTiles());
	manager->activateManyEntities(room->getTraps());
	manager->activateManyEntities(room->getEnemies());

	Globalbilboulga::getInstance()->setCurrentMapCollider(room->getMapColliders());
	Globalbilboulga::getInstance()->setCurrentRoomSize(room->getRoomSize() * ROOM_BASE_SIZE * Globalbilboulga::getInstance()->getCurrentTileSize());

	currentRoom = room;
}

bool Map::canBeAdded(Vector2D roomPos, Vector2D roomSize)
{
	bool canBeAdded = true;

	for (int x = roomPos.x; x < roomPos.x + roomSize.x; x++)
	{
		if (roomMap.size() - 1 < x)
		{
			canBeAdded = false;
			break;
		}

		for (int y = roomPos.y; y < roomPos.y + roomSize.y; y++)
		{
			if (roomMap.size() - 1 < y)
			{
				canBeAdded = false;
				break;
			}

			if (roomMap.at(x).at(y) != nullptr)
			{
				canBeAdded = false;
				break;
			}
		}
	}

	return canBeAdded;
	
}

void Map::displayMap()
{
	/*
	std::cout << "Map of the rooms :" << std::endl;
	Room* currentRoom = nullptr;

	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			currentRoom = roomMap.at(x).at(y);
			if (currentRoom != nullptr)
				std::cout << currentRoom->getName() << " ; ";
			else
				std::cout << "nullptr0 ; ";
		}

		std::cout << std::endl;
	}

	std::cout << "\nMap of the exits :" << std::endl;
	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			std::cout << exitMap.at(x).at(y)[0] << exitMap.at(x).at(y)[1] << exitMap.at(x).at(y)[2] << exitMap.at(x).at(y)[3] << " ; ";
		}

		std::cout << std::endl;
	}
	*/
	SDL_SetRenderDrawColor(Globalbilboulga::getInstance()->getRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globalbilboulga::getInstance()->getRenderer());

	for (Room* roomToDraw : roomList)
	{
		Vector2D roomPos = roomToDraw->getRoomPosition();
		AreaMap::drawRoom({ roomPos.x * ROOM_BASE_SIZE,roomPos.y * ROOM_BASE_SIZE,roomToDraw->getRoomSize().x * ROOM_BASE_SIZE,roomToDraw->getRoomSize().y * ROOM_BASE_SIZE }, *roomToDraw->getExits());
	}

	SDL_RenderPresent(Globalbilboulga::getInstance()->getRenderer());


	SDL_Event event;

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
	}
}
