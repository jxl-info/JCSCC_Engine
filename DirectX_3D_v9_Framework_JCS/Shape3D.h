/*******************************************************************
*                   JCSCC_Framework Version 1.0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   See LICENSE.txt for modification and distribution information
*		        Copyright (c) 2016 by Shen, Jen-Chieh
******************************************************************/

#ifndef __SHAPE3D_H__
#define __SHAPE3D_H__

#include "GameObject3D.h"


namespace JCS_D3DX_v9_Engine
{
	class Shape3D 
        : public GameObject3D
	{
	public:
		Shape3D();
		Shape3D(float32 x, float32 y);
		~Shape3D();
	};

}

#endif // __SHAPE3D_H__

