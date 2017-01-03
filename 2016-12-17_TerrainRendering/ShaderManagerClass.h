#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_

#include "D3DClass.h"
#include "ColorshaderClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "FontShaderClass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass &);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device *, HWND);
	void Shutdown();

	bool RenderColorShader(ID3D11DeviceContext *, int, XMMATRIX, XMMATRIX, XMMATRIX);
	bool RenderTextureShader(ID3D11DeviceContext *, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView *);
	bool RenderLightShader(ID3D11DeviceContext *, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView *, XMFLOAT3, XMFLOAT4);
	bool RenderFontShader(ID3D11DeviceContext *, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView *, XMFLOAT4);

private:
	ColorShaderClass *m_ColorShader;
	TextureShaderClass *m_TextureShader;
	LightShaderClass *m_LightShader;
	FontShaderClass *m_FontShader;
};

#endif