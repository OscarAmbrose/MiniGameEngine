#include "Transform.h"
#include <GameObject.h>

void Transform2D::setRotation(float newRotation)
{

	while (newRotation > 360 || newRotation < 0)
	{
		if (newRotation > 360)
		{
			newRotation -= 360;
		}
		else if (newRotation < 0)
		{
			newRotation += 360;
		}
	}
	m_Rotation = newRotation;
};
