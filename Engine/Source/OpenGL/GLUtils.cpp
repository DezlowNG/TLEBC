/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "GLUtils.h"

#include <fstream>

namespace rb
{
	namespace glu
	{
		void ClientToGL(int x, int y, double* ox, double* oy, double* oz)
		{
			/*int vp[4];

			double pM[16], mM[16];
			float z;

			glGetIntegerv(GL_VIEWPORT, vp);
			y = vp[3] - y - 1;

			glGetDoublev(GL_MODELVIEW_MATRIX, mM);
			glGetDoublev(GL_PROJECTION_MATRIX, pM);			

			glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
			gluUnProject(x, y, z, mM, pM, vp, ox, oy, oz);*/
		}
	}
}