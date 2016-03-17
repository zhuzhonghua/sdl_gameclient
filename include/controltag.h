#ifndef __CONTROL_TAG_H__
#define __CONTROL_TAG_H__

#include "loader.h"
#include "swftypes.h"

class ControlTag
{
public:
	virtual void execute() = 0;
};

class PlaceObject2Tag: public ControlTag
{
public:
	PlaceObject2Tag();
	void read(Loader* in);
	virtual void execute();
public:
	bool placeFlagHasClipAction;
	bool placeFlagHasClipDepth;
	bool placeFlagHasName;
	bool placeFlagHasRatio;
	bool placeFlagHasColorTransform;
	bool placeFlagHasMatrix;
	bool placeFlagHasCharacter;
	bool placeFlagMove;

	int depth;
	int ch;
	swftypes::matrix mat;
	swftypes::cxform cx;
	int ratio;
	std::string name;
	int clipDepth;

	swftypes::ClipActions	clipActions;
};
#endif