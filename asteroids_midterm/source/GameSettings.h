/*
* Global settings for the game
* Created by Michael Furst 10/2017
*/

#ifndef _GameSettings_h
#define _GameSettings_h
#include <map>
enum BoundsMode {
	_BOUNDINGBOX,_FULL_OBJECT,_BMEND
};
enum DrawMode {
	_OUTLINE,_FILL,_DMEND
};
enum BulletAmounts {
	_MINIMUM=10,
	_LOW=15,
	_MEDIUM=30,
	_HIGH=50,
	_BULLETHELL=100
};
struct _GameSettings {
public:
	DrawMode dMode;
	BoundsMode bMode;
	int _MAX_BULLETS;
	bool limitExplosions;
	bool explosionsColide;
	bool fullScreen;
	vec2 size;
	std::map<int,std::string> BulletValues;
	std::map<int,std::string> DrawModeValues;
	std::map<int,std::string> BoundsModeValues;
	void init() {
		limitExplosions=true;
		explosionsColide=true;
		fullScreen=true;
		size = vec2(0,0);
		dMode=_OUTLINE;
		bMode=_FULL_OBJECT;
		_MAX_BULLETS=BulletAmounts::_LOW;
		BulletValues[_MINIMUM]="MINIMUM";
		BulletValues[_LOW]="LOW";
		BulletValues[_MEDIUM]="MEDIUM";
		BulletValues[_HIGH]="HIGH";
		BulletValues[_BULLETHELL]="BULLET HELL";
		DrawModeValues[_OUTLINE]="OUTLINE";
		DrawModeValues[_FILL]="FILL";
		BoundsModeValues[_BOUNDINGBOX]="BOUNDING BOX";
		BoundsModeValues[_FULL_OBJECT]="FULL OBJECT";
	};
};
static _GameSettings GameSettings;
#endif /* defined(__Asteroids__Ship__) */
