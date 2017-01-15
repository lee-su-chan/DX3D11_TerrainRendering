#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <fstream>
#include <stdio.h>

#include "TerrainCellClass.h"

using namespace std;

class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
		XMFLOAT3 color;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
		float r, g, b;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	
public:
	TerrainClass();
	TerrainClass(const TerrainClass &);
	~TerrainClass();

	bool Initialize(ID3D11Device *, char *);
	void Shutdown();
	
	bool RenderCell(ID3D11DeviceContext *, int);
	void RenderCellLines(ID3D11DeviceContext *, int);

	int GetCellIndexCount(int);
	int GetCellLinesIndexCount(int);
	int GetCellCount();

private:
	bool LoadSetupFile(char *);
	bool LoadBitmapHeightMap();
	bool LoadRawHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();
	bool LoadColorMap();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();
	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType &, VectorType &);

	bool LoadTerrainCells(ID3D11Device *);
	void ShutdownTerrainCells();

private:
	int m_terrainHeight, m_terrainWidth, m_vertexCount;
	float m_heightScale;
	char *m_terrainFilename, *m_colorMapFilename;
	HeightMapType *m_heightMap;
	ModelType *m_terrainModel;

	TerrainCellClass *m_TerrainCells;
	int m_cellCount;
};

#endif