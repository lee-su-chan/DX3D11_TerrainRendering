#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"

#ifdef _COLOR_SHADER_MODE_
#include "ColorshaderClass.h"
#endif

#ifdef _TEXTURE_SHADER_MODE_
#include "TextureShaderClass.h"
#endif

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass *m_Direct3D;
	CameraClass *m_Camera;
	ModelClass *m_Model;

#ifdef _COLOR_SHADER_MODE_
	ColorShaderClass *m_ColorShader;
#endif

#ifdef _TEXTURE_SHADER_MODE_
	TextureShaderClass *m_TextureShader;
#endif
};

#endif