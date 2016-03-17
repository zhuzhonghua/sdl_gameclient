#include "controltag.h"

PlaceObject2Tag::PlaceObject2Tag()
:placeFlagHasClipAction(false),
placeFlagHasClipDepth(false),
placeFlagHasName(false),
placeFlagHasRatio(false),
placeFlagHasColorTransform(false),
placeFlagHasMatrix(false),
placeFlagHasCharacter(false),
placeFlagMove(false),
depth(-1),
ch(-1),
ratio(-1),
clipDepth(-1)
{
}

void PlaceObject2Tag::read(Loader* in)
{
	placeFlagHasClipAction = in->readUInt(1);
	placeFlagHasClipDepth = in->readUInt(1);
	placeFlagHasName = in->readUInt(1);
	placeFlagHasRatio = in->readUInt(1);
	placeFlagHasColorTransform = in->readUInt(1);
	placeFlagHasMatrix = in->readUInt(1);
	placeFlagHasCharacter = in->readUInt(1);
	placeFlagMove = in->readUInt(1);

	depth = in->readU16();
	if (placeFlagHasCharacter)
	{
		ch = in->readU16();
	}
	if (placeFlagHasMatrix)
	{		
		mat.read(in);
	}
	if (placeFlagHasColorTransform)
	{
		cx.read_rgba(in);
	}
	if (placeFlagHasRatio)
	{
		ratio = in->readU16();
	}
	if (placeFlagHasName)
	{
		name = in->readString();
	}
	if (placeFlagHasClipDepth)
	{
		clipDepth = in->readU16();
	}

	if (placeFlagHasClipAction)
	{
		clipActions.read(in);
	}
}

void PlaceObject2Tag::execute()
{

}