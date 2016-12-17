#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

// Mode select define variable -> | _COLOR_SHADER_MODE_ | _TEXTURE_SHADER_MODE_ |
#define _COLOR_SHADER_MODE_

#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"

using namespace DirectX;

class ModelClass
{
#ifdef _COLOR_SHADER_MODE_ 
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
#endif

#ifdef _TEXTURE_SHADER_MODE_ 
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
#endif


public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Initialize(ID3D11Device *, ID3D11DeviceContext *, char *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

#ifdef _TEXTURE_SHADER_MODE_
	ID3D11ShaderResourceView *GetTexture();
#endif

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *);

#ifdef _TEXTURE_SHADER_MODE_
	bool LoadTexture(ID3D11Device *, ID3D11DeviceContext *, char *);
	void ReleaseTexture();
#endif

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

#ifdef _TEXTURE_SHADER_MODE_ 
	TextureClass *m_Texture;
#endif
};

#endif