#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_terrainFilename = 0;
	m_colorMapFilename = 0;
	m_heightMap = 0;
	m_terrainModel = 0;
}

TerrainClass::TerrainClass(const TerrainClass &other)
{
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device *device, char *setupFilename)
{
	bool result;

	result = LoadSetupFile(setupFilename);
	if (!result)
		return false;

	result = LoadBitmapHeightMap();
	if (!result)
		return false;

	SetTerrainCoordinates();

	result = CalculateNormals();
	if (!result)
		return false;

	result = LoadColorMap();
	if (!result)
		return false;

	result = BuildTerrainModel();
	if (!result)
		return false;

	result = InitializeBuffers(device);
	if (!result)
		return false;

	ShutdownTerrainModel();

	return true;
}

void TerrainClass::Shutdown()
{
	ShutdownBuffers();
	ShutdownTerrainModel();
	ShutdownHeightMap();

	return;
}

bool TerrainClass::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);

	return true;
}

int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}

bool TerrainClass::LoadSetupFile(char *filename)
{
	int stringLength;
	ifstream fin;
	char input;

	stringLength = 256;
	
	m_terrainFilename = new char[stringLength];
	if (!m_terrainFilename)
		return false;

	m_colorMapFilename = new char[stringLength];
	if (!m_colorMapFilename)
		return false;

	fin.open(filename);
	if (fin.fail())
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> m_terrainFilename;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> m_terrainHeight;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> m_terrainWidth;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> m_heightScale;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> m_colorMapFilename;

	fin.close();

	return true;
}

bool TerrainClass::LoadBitmapHeightMap()
{
	int error, imageSize, i, j, k, index;
	FILE *filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char *bitmapImage;
	unsigned char height;

	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
		return false;

	error = fopen_s(&filePtr, m_terrainFilename, "rb");
	if (error != 0)
		return false;

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
		return false;

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
		return false;

	if (bitmapInfoHeader.biHeight != m_terrainHeight ||
		bitmapInfoHeader.biWidth != m_terrainWidth)
		return false;

	imageSize = m_terrainHeight * (m_terrainWidth * 3 + 1);

	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
		return false;

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	k = 0;

	for (j = 0; j < m_terrainHeight; ++j)
	{
		for (i = 0; i < m_terrainWidth; ++i)
		{
			// Bitmaps are upside down so load bottom to top into the height map array.
			index = m_terrainWidth * (m_terrainHeight - 1 - j) + i;

			height = bitmapImage[k];

			m_heightMap[index].y = (float)height;

			k += 3;
		}

		++k;
	}

	delete[] bitmapImage;
	bitmapImage = NULL;

	delete[] m_terrainFilename;
	m_terrainFilename = NULL;

	return true;
}

void TerrainClass::ShutdownHeightMap()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = NULL;
	}

	return;
}

void TerrainClass::SetTerrainCoordinates()
{
	int i, j, index;

	for (j = 0; j < m_terrainHeight; ++j)
	{
		for (i = 0; i < m_terrainWidth; ++i)
		{
			index = m_terrainWidth * j + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].z = -(float)j;

			m_heightMap[index].z += (float)(m_terrainHeight - 1);

			m_heightMap[index].y /= m_heightScale;
		}
	}

	return;
}

bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType *normals;

	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
		return false;

	for (j = 0; j < m_terrainHeight - 1; ++j)
	{
		for (i = 0; i < m_terrainWidth - 1; ++i)
		{
			index1 = (j + 1) * m_terrainWidth + i;			// Bottom left vertex.
			index2 = (j + 1) * m_terrainWidth + (i + 1);	// Bottom right vertex.
			index3 = j * m_terrainWidth + i;				// Upper left vertex.

			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[1] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[1] = vertex3[2] - vertex2[2];

			index = j * (m_terrainWidth - 1) + i;

			normals[index].x = vector1[1] * vector2[2] - vector1[2] * vector2[1];
			normals[index].y = vector1[2] * vector2[0] - vector1[0] * vector2[2];
			normals[index].z = vector1[0] * vector2[1] - vector1[1] * vector2[0];

			length = (float)sqrt(normals[index].x * normals[index].x +
				normals[index].y * normals[index].y +
				normals[index].z * normals[index].z);

			normals[index].x = normals[index].x / length;
			normals[index].y = normals[index].y / length;
			normals[index].z = normals[index].z / length;
		}
	}

	for (j = 0; j < m_terrainHeight; ++j)
	{
		for (i = 0; i < m_terrainWidth; ++i)
		{
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Bottom left face.
			if (i - 1 >= 0 && j - 1 >= 0)
			{
				index = (j - 1) * (m_terrainWidth - 1) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Bottom right face.
			if (i < m_terrainWidth - 1 && j - 1 >= 0)
			{
				index = (j - 1) * (m_terrainWidth - 1) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper left face.
			if (i - 1 >= 0 && j < m_terrainHeight - 1)
			{
				index = j * (m_terrainWidth - 1) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			// Upper right face.
			if (i < m_terrainWidth - 1 && j < m_terrainHeight - 1)
			{
				index = j * (m_terrainWidth - 1) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
			}

			length = (float)sqrt(sum[0] * sum[0] + sum[1] * sum[1] + sum[2] * sum[2]);
			index = j * m_terrainWidth + i;

			m_heightMap[index].nx = sum[0] / length;
			m_heightMap[index].ny = sum[1] / length;
			m_heightMap[index].nz = sum[2] / length;
		}
	}

	delete[] normals;
	normals = NULL;

	return true;
}

bool TerrainClass::LoadColorMap()
{
	int error, imageSize, i, j, k, index;
	FILE *filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char *bitmapImage;

	error = fopen_s(&filePtr, m_colorMapFilename, "rb");
	if (error != 0)
		return false;

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
		return false;

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
		return false;

	if (bitmapInfoHeader.biWidth != m_terrainWidth ||
		bitmapInfoHeader.biHeight != m_terrainHeight)
		return false;

	imageSize = m_terrainHeight * (m_terrainWidth * 3 + 1);

	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
		return false;

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	k = 0;
	for (j = 0; j < m_terrainHeight; ++j)
	{
		for (i = 0; i < m_terrainWidth; ++i)
		{
			// Bitmaps are upside down so load bottom to top into the array.
			index = m_terrainWidth * (m_terrainHeight - 1 - j) + i;

			m_heightMap[index].b = (float)bitmapImage[k] / 255.0f;
			m_heightMap[index].g = (float)bitmapImage[k + 1] / 255.0f;
			m_heightMap[index].r = (float)bitmapImage[k + 2] / 255.0f;

			k += 3;
		}

		++k;
	}

	delete[] bitmapImage;
	bitmapImage = NULL;

	delete[] m_colorMapFilename;
	m_colorMapFilename = NULL;

	return true;
}

bool TerrainClass::BuildTerrainModel()
{
	int i, j, index, index1, index2, index3, index4;

	m_vertexCount = (m_terrainHeight - 1) * (m_terrainWidth - 1) * 6;

	m_terrainModel = new ModelType[m_vertexCount];
	if (!m_terrainModel)
		return false;

	index = 0;

	for (j = 0; j < m_terrainHeight - 1; ++j)
	{
		for (i = 0; i < m_terrainWidth - 1; ++i)
		{
			// Get the indexes to the four points of the quad.
			index1 = m_terrainWidth * j + i;				// Upper left.
			index2 = m_terrainWidth * j + (i + 1);			// Upper right.
			index3 = m_terrainWidth * (j + 1) + i;			// Bottom left.
			index4 = m_terrainWidth * (j + 1) + (i + 1);	// Bottom right.

			// Now create two triangles for that quad.
			// Triangle 1 - Upper left.
			m_terrainModel[index].x = m_heightMap[index1].x;
			m_terrainModel[index].y = m_heightMap[index1].y;
			m_terrainModel[index].z = m_heightMap[index1].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index1].nx;
			m_terrainModel[index].ny = m_heightMap[index1].ny;
			m_terrainModel[index].nz = m_heightMap[index1].nz;
			m_terrainModel[index].r = m_heightMap[index1].r;
			m_terrainModel[index].g = m_heightMap[index1].g;
			m_terrainModel[index].b = m_heightMap[index1].b;
			++index;

			// Triangle 1 - Upper right.
			m_terrainModel[index].x = m_heightMap[index2].x;
			m_terrainModel[index].y = m_heightMap[index2].y;
			m_terrainModel[index].z = m_heightMap[index2].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index2].nx;
			m_terrainModel[index].ny = m_heightMap[index2].ny;
			m_terrainModel[index].nz = m_heightMap[index2].nz;
			m_terrainModel[index].r = m_heightMap[index2].r;
			m_terrainModel[index].g = m_heightMap[index2].g;
			m_terrainModel[index].b = m_heightMap[index2].b;
			++index;

			// Triangle 1 - Bottom left.
			m_terrainModel[index].x = m_heightMap[index3].x;
			m_terrainModel[index].y = m_heightMap[index3].y;
			m_terrainModel[index].z = m_heightMap[index3].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].nx = m_heightMap[index3].nx;
			m_terrainModel[index].ny = m_heightMap[index3].ny;
			m_terrainModel[index].nz = m_heightMap[index3].nz;
			m_terrainModel[index].r = m_heightMap[index3].r;
			m_terrainModel[index].g = m_heightMap[index3].g;
			m_terrainModel[index].b = m_heightMap[index3].b;
			++index;

			// Triangle 2 - Bottom left.
			m_terrainModel[index].x = m_heightMap[index3].x;
			m_terrainModel[index].y = m_heightMap[index3].y;
			m_terrainModel[index].z = m_heightMap[index3].z;
			m_terrainModel[index].tu = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index3].nx;
			m_terrainModel[index].ny = m_heightMap[index3].ny;
			m_terrainModel[index].nz = m_heightMap[index3].nz;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].r = m_heightMap[index3].r;
			m_terrainModel[index].g = m_heightMap[index3].g;
			m_terrainModel[index].b = m_heightMap[index3].b;
			++index;

			// Triangle 2 - Upper right.
			m_terrainModel[index].x = m_heightMap[index2].x;
			m_terrainModel[index].y = m_heightMap[index2].y;
			m_terrainModel[index].z = m_heightMap[index2].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 0.0f;
			m_terrainModel[index].nx = m_heightMap[index2].nx;
			m_terrainModel[index].ny = m_heightMap[index2].ny;
			m_terrainModel[index].nz = m_heightMap[index2].nz;
			m_terrainModel[index].r = m_heightMap[index2].r;
			m_terrainModel[index].g = m_heightMap[index2].g;
			m_terrainModel[index].b = m_heightMap[index2].b;
			++index;

			// Triangle 2 - Upper left.
			m_terrainModel[index].x = m_heightMap[index4].x;
			m_terrainModel[index].y = m_heightMap[index4].y;
			m_terrainModel[index].z = m_heightMap[index4].z;
			m_terrainModel[index].tu = 1.0f;
			m_terrainModel[index].tv = 1.0f;
			m_terrainModel[index].nx = m_heightMap[index4].nx;
			m_terrainModel[index].ny = m_heightMap[index4].ny;
			m_terrainModel[index].nz = m_heightMap[index4].nz;
			m_terrainModel[index].r = m_heightMap[index4].r;
			m_terrainModel[index].g = m_heightMap[index4].g;
			m_terrainModel[index].b = m_heightMap[index4].b;
			++index;
		}
	}

	return true;
}

void TerrainClass::ShutdownTerrainModel()
{
	if (m_terrainModel)
	{
		delete[] m_terrainModel;
		m_terrainModel = NULL;
	}

	return;
}

bool TerrainClass::InitializeBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	XMFLOAT4 color;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	for (i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = XMFLOAT3(m_terrainModel[i].x,
			m_terrainModel[i].y,
			m_terrainModel[i].z);
		
		vertices[i].texture = XMFLOAT2(m_terrainModel[i].tu, 
			m_terrainModel[i].tv);

		vertices[i].normal = XMFLOAT3(m_terrainModel[i].nx,
			m_terrainModel[i].ny,
			m_terrainModel[i].nz);

		vertices[i].color = XMFLOAT3(m_terrainModel[i].r,
			m_terrainModel[i].g,
			m_terrainModel[i].b);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = NULL;

	delete[] indices;
	indices = NULL;

	return true;
}

void TerrainClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
}

void TerrainClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
