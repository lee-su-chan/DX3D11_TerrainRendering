#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	
public:
	TerrainClass();
	TerrainClass(const TerrainClass &);
	~TerrainClass();

	bool Initialize(ID3D11Device *);
	void Shutdown();
	bool Render(ID3D11DeviceContext *);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif