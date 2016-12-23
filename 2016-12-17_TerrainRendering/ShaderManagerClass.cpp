#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_ColorShader = 0;
	m_FontShader = 0;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass &other)
{
}

ShaderManagerClass::~ShaderManagerClass()
{
}

bool ShaderManagerClass::Initialize(ID3D11Device *device, HWND hwnd)
{
	bool result;

	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
		return false;

	result = m_ColorShader->Initialize(device, hwnd);
	if (!result)
		return false;

	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
		return false;

	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
		return false;

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = NULL;
	}

	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = NULL;
	}

	return;
}

bool ShaderManagerClass::RenderColorShader(ID3D11DeviceContext *deviceContext,
	int indexCount,
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatix,
	XMMATRIX projectionMatrix)
{
	return m_ColorShader->Render(deviceContext,
		indexCount,
		worldMatrix,
		viewMatix,
		projectionMatrix);
}

bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext *deviceContext,
	int indexCount,
	XMMATRIX worldMatrix,
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView *texture, 
	XMFLOAT4 color)
{
	return m_FontShader->Render(deviceContext,
		indexCount,
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		texture,
		color);
}
