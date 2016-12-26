#ifndef _USERINTERFACECLASS_H_
#define _USERINTERFACECLASS_H_

#include "TextClass.h"

class UserInterfaceClass
{
public:
	UserInterfaceClass();
	UserInterfaceClass(const UserInterfaceClass &);
	~UserInterfaceClass();

	bool Initialize(D3DClass *, int, int);
	void Shutdown();

	bool Frame(ID3D11DeviceContext *, int, float, float, float, float, float, float);
	bool Render(D3DClass *, ShaderManagerClass *, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool UpdateFpsString(ID3D11DeviceContext *, int);
	bool UpdatePositionStrings(ID3D11DeviceContext *, float, float, float, float, float, float);

private:
	FontClass *m_font1;
	TextClass *m_fpsString, *m_videoStrings, *m_positionStrings;
	int m_previousFps;
	int m_previousPosition[6];
};

#endif