#include "errorCheck.h"
#include <iostream>
using namespace std;

GLenum errorCheck()
{
	GLenum code = glGetError();
	const GLubyte* str;
	if (code != GL_NO_ERROR) {
		str = gluErrorString(code);
		cout << "OpenGL Error Code: " << str << endl;
	}
	return code;
}
