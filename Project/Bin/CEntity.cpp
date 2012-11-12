//==============================================================================
#include "CEntity.h"
#include "Define.h"

#include "functions.h"
#include <iostream>

#include <cmath>

#define Maximum(a, b) ((a > b) ? a : b)
#define Minimum(a, b) ((a < b) ? a : b)

//==============================================================================
std::vector<CEntity*> 	CEntity::EntityList;
 
//==============================================================================
CEntity::CEntity() {
	Surf_Entity = NULL;

	X = 0;
	Y = 0;

	Width 	= 0;
	Height 	= 0;

	MoveLeft  = false;
	MoveRight = false;
	MoveUp	  = false;
	MoveDown  = false;

	TargetAngle = 999;

	Type	= ENTITY_TYPE_GENERIC;
	SubType = ENTITY_SUBTYPE_NONE;

	Dead = false;
	Flags = ENTITY_FLAG_NONE;

	HP = 1;

	SpeedX = 0;
	SpeedY = 0;

	AccelX = 0;
	AccelY = 0;

	MaxSpeedX = 10;
	MaxSpeedY = 10;

	CurrentFrameCol = 0;
	CurrentFrameRow = 0;
}

//------------------------------------------------------------------------------
CEntity::~CEntity() {
}

//==============================================================================
bool CEntity::OnLoad(char* File, int Width, int Height, int MaxFrames) {
	if((Surf_Entity = CSurface::OnLoad(File)) == NULL) {
		return false;
	}

	CSurface::Transparent(Surf_Entity, 255, 0, 255);

	this->Width = Width;
	this->Height = Height;

	Anim_Control.MaxFrames = MaxFrames;

    return true;
}

//------------------------------------------------------------------------------
void CEntity::OnLoop() {
	if( Dead ) return;
	
	// Check HP
	if( HP <= 0 ) {
		Dead = true;
		return;
	}

	// Kill entities that move too far out of screen
	if( X < CCamera::CameraControl.GetX() - ENTITY_KILLDISTANCE ) {
		Dead = true;
	}
	else if( X > CCamera::CameraControl.GetX() + WWIDTH + ENTITY_KILLDISTANCE && Type != ENTITY_TYPE_GENERIC ) {
		Dead = true;
	}

	
	if( TargetAngle == 999 ) {
		//We're not Moving
		if(MoveLeft == false && MoveRight == false && MoveUp == false && MoveDown == false) {
			StopMove();
		}

		if (MoveLeft) {
			AccelX = -0.5;
		}
		else if (MoveRight) {
			AccelX = 0.5;
		}

		if (MoveUp) {
			AccelY = -0.5;
		}
		else if (MoveDown) {
			AccelY = 0.5;
		}
	}
	else{
		// Need to turn the moving direction of object to TargetAngle
		if( Angle >= 0 && Angle < 90 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
		}
		else if( Angle >= 90 && Angle <= 180 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
		}
		else if( Angle < 0 && Angle >= -90 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
		}
		else if( Angle < -90 && Angle >= -180 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
		}
	}

	SpeedX += AccelX * CFPS::FPSControl.GetSpeedFactor();
	SpeedY += AccelY * CFPS::FPSControl.GetSpeedFactor();

	if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
	if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
	if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
	if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;

	OnAnimate();
	OnMove(SpeedX, SpeedY);
}

//------------------------------------------------------------------------------
void CEntity::OnRender(SDL_Surface* Surf_Display) {
    if(Surf_Entity == NULL || Surf_Display == NULL) return;

	if (Anim_Control.KeepAnimating) {
		CSurface::OnDraw(Surf_Display, Surf_Entity, X - CCamera::CameraControl.GetX(), Y - CCamera::CameraControl.GetY(), 0, Anim_Control.GetCurrentFrame() * Height, Width, Height);
	}
}

//------------------------------------------------------------------------------
void CEntity::OnCleanup() {
    if(Surf_Entity) {
        SDL_FreeSurface(Surf_Entity);
    }

    Surf_Entity = NULL;
}

//------------------------------------------------------------------------------
void CEntity::OnAnimate() {
	Anim_Control.OnAnimate();
}

//==============================================================================
void CEntity::OnCollision(CEntity* Entity) {
    return;
}

//------------------------------------------------------------------------------
void CEntity::OnCollision(CTile* Tile) {
    return;
}
//==============================================================================
bool CEntity::IsActive() {
	if( Dead || HP <= 0 ) {
		return false;
	}
	return true;
}

//==============================================================================
void CEntity::OnMove(float MoveX, float MoveY) {
	if(MoveX == 0 && MoveY == 0) return;
	
	int oldAngle = Angle;
	Angle = static_cast<int>(atan2(MoveY, MoveX) * 180.0 / 3.141592);

	if( oldAngle == -180 && Angle == 180 ){
		Angle = -180;
	}

	double NewX = 0;
	double NewY = 0;

	MoveX *= CFPS::FPSControl.GetSpeedFactor();
	MoveY *= CFPS::FPSControl.GetSpeedFactor();

	if(MoveX != 0) {
		if(MoveX >= 0) 	NewX =  CFPS::FPSControl.GetSpeedFactor();
		else 			NewX = -CFPS::FPSControl.GetSpeedFactor();
	}

	if(MoveY != 0) {
		if(MoveY >= 0) 	NewY =  CFPS::FPSControl.GetSpeedFactor();
		else 			NewY = -CFPS::FPSControl.GetSpeedFactor();
	}

	while(true) {
		if (Flags & ENTITY_FLAG_GHOST) {
			CheckCollisions((int)(X + NewX), (int)(Y + NewY)); //We don't care about collisions, but we need to send events to other entities

			X += static_cast<float>(NewX);
			Y += static_cast<float>(NewY);
		}
		else {
			int newLocX = (int)(X + NewX);
			int newLocY = (int)(Y + NewY);

			CheckCollisions(newLocX, (int)Y);
			CheckCollisions((int)(X), newLocY);

			X += static_cast<float>(NewX);

			// If entity's new location is inside top and bottom 
			// edge of the screen. Bullets are allowed to exit screen vertically
			if ( (newLocY > GUI_HEIGHT && 
				(newLocY+Surf_Entity->h) < WHEIGHT) ||
				Type & ENTITY_TYPE_BULLET) {
				Y += static_cast<float>(NewY);
			}
			else {
				SpeedY = 0;
			}

		}

		MoveX += static_cast<float>(-NewX);
		MoveY += static_cast<float>(-NewY);

		if(NewX > 0 && MoveX <= 0) NewX = 0;
		if(NewX < 0 && MoveX >= 0) NewX = 0;

		if(NewY > 0 && MoveY <= 0) NewY = 0;
		if(NewY < 0 && MoveY >= 0) NewY = 0;

		if(MoveX == 0) NewX = 0;
		if(MoveY == 0) NewY = 0;

		if(MoveX == 0 && MoveY 	== 0) 	break;
		if(NewX  == 0 && NewY 	== 0) 	break;
	}
}

//==============================================================================
bool CEntity::Collides(int oX, int oY, int oW, int oH) {
    int left1, left2;
    int right1, right2;
    int top1, top2;
    int bottom1, bottom2;

	int tX = (int)X;
	int tY = (int)Y;

    left1 = tX;
    left2 = oX;

	right1 = left1 + Width - 1;
    right2 = oX + oW - 1;

    top1 = tY;
    top2 = oY;

	bottom1 = top1 + Height - 1;
    bottom2 = oY + oH - 1;


    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;

    if (right1 < left2) return false;
    if (left1 > right2) return false;

    return true;
}

//==============================================================================
void CEntity::CheckCollisions(int NewX, int NewY) {

	// Check if entity collides with any tile
	CheckPossibleTileCollision(NewX, NewY);

	// Queue all possible collisions with other entities
	for(unsigned int i = 0;i < EntityList.size();i++) {
		QueuePossibleEntityCollision(EntityList[i], NewX, NewY);
	}

	return;
}

//------------------------------------------------------------------------------
void CEntity::QueuePossibleEntityCollision(CEntity* Entity, int NewX, int NewY) {
	if(this != Entity && Entity != NULL && !Entity->Dead &&
		Entity->Collides(NewX, NewY, Width - 1, Height - 1) == true) {

		CEntityCol EntityCol;

		EntityCol.EntityA = this;
		EntityCol.EntityB = Entity;

		CEntityCol::EntityColList.push_back(EntityCol);

		return;
	}

	return;
}

//==============================================================================

void CEntity::StopMove() {
	if(SpeedX > 0) {
		AccelX = -1;
	}

	if(SpeedX < 0) {
		AccelX =  1;
	}

	if(SpeedX < 2.0f && SpeedX > -2.0f) {
		AccelX = 0;
		SpeedX = 0;
	}
}

//------------------------------------------------------------------------------
SDL_Rect CEntity::GetBounds()
{
    SDL_Rect bounds;
    bounds.x = (Sint16)(X);
    bounds.y = (Sint16)(Y);
    bounds.w = (Sint16)(Width);
    bounds.h = (Sint16)(Height);
 
    return bounds;
}

//------------------------------------------------------------------------------
SDL_Rect CEntity::GetFrameBounds()
{
    SDL_Rect frameBounds;
    frameBounds.x = 0;
    frameBounds.y = 0;
    frameBounds.w = (Sint16)Width;
    frameBounds.h = (Sint16)Height;
     
    return frameBounds;
}

//------------------------------------------------------------------------------
SDL_Rect CEntity::NormalizeBounds(const SDL_Rect& rect)
{
    SDL_Rect normalized;
    normalized.x = rect.x - (Sint16)X + GetFrameBounds().x;
    normalized.y = rect.y - (Sint16)Y + GetFrameBounds().y;
    normalized.w = rect.w;
    normalized.h = rect.h;
     
    return normalized;
}

//------------------------------------------------------------------------------
SDL_Rect CEntity::Intersection(const SDL_Rect& boundsA, const SDL_Rect& boundsB)
{
    int x1 = Maximum(boundsA.x, boundsB.x);
    int y1 = Maximum(boundsA.y, boundsB.y);
    int x2 = Minimum(boundsA.x + boundsA.w, boundsB.x + boundsB.w);
    int y2 = Minimum(boundsA.y + boundsA.h, boundsB.y + boundsB.h);
     
    int width = x2 - x1;
    int height = y2 - y1;
     
    if(width > 0 && height > 0)
    {
        SDL_Rect intersect = {x1, y1, width, height};
        return intersect;
    }
    else
    {
        SDL_Rect intersect = {0, 0, 0, 0};
        return intersect;
    }
}

//------------------------------------------------------------------------------
bool CEntity::CheckCollision(CEntity* entityA, CEntity* entityB)
{
    SDL_Rect collisionRect = Intersection(entityA->GetBounds(), entityB->GetBounds());
 
    if(collisionRect.w == 0 && collisionRect.h == 0)
        return false;
 
    SDL_Rect normalA = entityA->NormalizeBounds(collisionRect);
    SDL_Rect normalB = entityB->NormalizeBounds(collisionRect);
 
    for(int y = 0; y < collisionRect.h; y++)
        for(int x = 0; x < collisionRect.w; x++)
            if(GetAlphaXY(entityA, normalA.x + x, normalA.y + y) && GetAlphaXY(entityB, normalB.x + x, normalB.y + y))
                return true;
 
    return false;
}

//------------------------------------------------------------------------------
bool CEntity::GetAlphaXY(CEntity* entity, int x, int y)
{
    int bpp = entity->Surf_Entity->format->BytesPerPixel;
    Uint8* p = (Uint8*)entity->Surf_Entity->pixels + y * entity->Surf_Entity->pitch + x * bpp;
    Uint32 pixelColor;
     
    switch(bpp)
    {
        case(1):
            pixelColor = *p;
            break;
        case(2):
            pixelColor = *(Uint16*)p;
            break;
        case(3):
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixelColor = p[0] << 16 | p[1] << 8 | p[2];
            else
                pixelColor = p[0] | p[1] << 8 | p[2] << 16;
            break;
        case(4):
            pixelColor = *(Uint32*)p;
            break;
    }
     
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixelColor, entity->Surf_Entity->format, &red, &green, &blue, &alpha);
 
    return alpha > 10;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void CEntity::CheckPossibleTileCollision(int NewX, int NewY) {
	int StartX 	= NewX / TILE_SIZE;
	int StartY 	= NewY / TILE_SIZE;

	int EndX	= (NewX + Width - 1) / TILE_SIZE;
	int EndY	= (NewY + Height - 1) / TILE_SIZE;
	
	// Check every tile around the entity that it might be touching
	for(int iY = StartY;iY <= EndY;iY++) {
		for(int iX = StartX;iX <= EndX;iX++) {
			CTile* tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
			
			// If the tile is blocking type, check pixel precisely
			// if the entity is colliding it
			if(tile != NULL && (tile->TypeID == TILE_TYPE_BLOCK
				|| tile->TypeID == TILE_TYPE_BLOCK_BREAKABLE)) {
				
 				if (CheckTileCollision(tile, iX, iY)) {
					OnCollision(tile); // Call on collision
				}
			}
		}
	}

	return;
}

bool CEntity::CheckTileCollision(CTile* tile, int tileX, int tileY)
{
	// Create rectangle of the tile's area in the space
	SDL_Rect tileBounds;
	tileBounds.x = (Sint16)(tileX*TILE_SIZE);
    tileBounds.y = (Sint16)(tileY*TILE_SIZE);
    tileBounds.w = (Sint16)(TILE_SIZE);
    tileBounds.h = (Sint16)(TILE_SIZE);

    SDL_Rect collisionRect = Intersection(this->GetBounds(), tileBounds);
 
    if(collisionRect.w == 0 && collisionRect.h == 0) return false;

	CMap* map = CArea::AreaControl.GetMap((int)X, (int)Y);
	SDL_Surface* tileset = map->Surf_Tileset;

	int TilesetWidth  = tileset->w / TILE_SIZE;
	int TilesetHeight = tileset->h / TILE_SIZE;
	int TilesetX = (tile->TileID % TilesetWidth) * TILE_SIZE;
	int TilesetY = (tile->TileID / TilesetWidth) * TILE_SIZE;

    SDL_Rect entityRect = this->NormalizeBounds(collisionRect);

	// Set the correct area from the tileset to compare
	SDL_Rect tileRect;
	tileRect.x = TilesetX;
	tileRect.y = TilesetY;

	if (collisionRect.h > TILE_SIZE || collisionRect.w > TILE_SIZE) {
		return false;
	}

    for(int y = 0; y < collisionRect.h; y++) {
        for(int x = 0; x < collisionRect.w; x++) {

			// Here we check if both are on top of each other pixel precisely
            if(GetAlphaXY(this, entityRect.x + x, entityRect.y + y) && GetAlphaXYTile(tileset, tileRect.x + x, tileRect.y + y)) {
                return true;
			}
		}
	}

    return false;
}

//------------------------------------------------------------------------------

bool CEntity::GetAlphaXYTile(SDL_Surface* tileset, int x, int y)
{

    int bpp = tileset->format->BytesPerPixel;
    Uint8* p = (Uint8*)tileset->pixels + y * tileset->pitch + x * bpp;
    Uint32 pixelColor;
     
    switch(bpp)
    {
        case(1):
            pixelColor = *p;
            break;
        case(2):
            pixelColor = *(Uint16*)p;
            break;
        case(3):
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixelColor = p[0] << 16 | p[1] << 8 | p[2];
            else
                pixelColor = p[0] | p[1] << 8 | p[2] << 16;
            break;
        case(4):
            pixelColor = *(Uint32*)p;
            break;
    }
     
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixelColor, tileset->format, &red, &green, &blue, &alpha);
 
    return alpha > 10;
}
//------------------------------------------------------------------------------

void CEntity::Die() {
	Dead = true;
}

bool CEntity::IsDead() {
	return Dead;
}
//------------------------------------------------------------------------------