#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "toucharea.h"
#include "gizmo.h"

namespace Zengine{
class Button :public TouchArea
{
public:
	Button();
	~Button();
};
}
#endif