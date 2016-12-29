#include "UserInterfaceClass.h"

UserInterfaceClass::UserInterfaceClass()
{
	m_font1 = 0;
	m_fpsString = 0;
	m_videoStrings = 0;
	m_positionStrings = 0;
}

UserInterfaceClass::UserInterfaceClass(const UserInterfaceClass &other)
{
}

UserInterfaceClass::~UserInterfaceClass()
{
}

bool UserInterfaceClass::Initialize(D3DClass *Direct3D, int screenHeight, int screenWidth)
{
	bool result;
	char videoCard[128];
	int videoMemory;
	char videoString[144];
	char memoryString[32];
	char tempString[16];
	int i;

	m_font1 = new FontClass;
	if (!m_font1)
		return false;

	result = m_font1->Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		"data/font/font01.txt",
		"data/font/font01.tga",
		32.0f,
		3);

	if (!result)
		return false;

	m_fpsString = new TextClass;
	if (!m_fpsString)
		return false;

	result = m_fpsString->Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"Fps: 0",
		10,
		-50,
		0.0f,
		1.0f,
		0.0f);

	if (!result)
		return false;

	m_previousFps = -1;

	Direct3D->GetVideoCardInfo(videoCard, videoMemory);
	strcpy_s(videoString, "Video Card: ");
	strcat_s(videoString, videoCard);

	_itoa_s(videoMemory, tempString, 10);

	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	m_videoStrings = new TextClass[2];
	if (!m_videoStrings)
		return false;

	result = m_videoStrings[0].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		256,
		false,
		m_font1,
		videoString,
		10,
		-10,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_videoStrings[1].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		32,
		false,
		m_font1,
		memoryString,
		10,
		-30,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	m_positionStrings = new TextClass[6];
	if (!m_positionStrings)
		return false;

	result = m_positionStrings[0].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"X: 0",
		10,
		-310,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_positionStrings[1].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"Y: 0",
		10,
		-330,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_positionStrings[2].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"Z: 0",
		10,
		-350,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_positionStrings[3].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"rX: 0",
		10,
		-370,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_positionStrings[4].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"rY: 0",
		10,
		-390,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	result = m_positionStrings[5].Initialize(Direct3D->GetDevice(),
		Direct3D->GetDeviceContext(),
		screenWidth,
		screenHeight,
		16,
		false,
		m_font1,
		"rZ: 0",
		10,
		-410,
		1.0f,
		1.0f,
		1.0f);

	if (!result)
		return false;

	for (i = 0; i < 6; ++i)
		m_previousPosition[i] = -1;

	return true;
}

void UserInterfaceClass::Shutdown()
{
	if (m_positionStrings)
	{
		m_positionStrings[0].Shutdown();
		m_positionStrings[1].Shutdown();
		m_positionStrings[2].Shutdown();
		m_positionStrings[3].Shutdown();
		m_positionStrings[4].Shutdown();
		m_positionStrings[5].Shutdown();

		delete[] m_positionStrings;
		m_positionStrings = NULL;
	}

	if (m_videoStrings)
	{
		m_videoStrings[0].Shutdown();
		m_videoStrings[1].Shutdown();

		delete[] m_videoStrings;
		m_videoStrings = NULL;
	}

	if (m_fpsString)
	{
		m_fpsString->Shutdown();
		delete m_fpsString;
		m_fpsString = NULL;
	}

	if (m_font1)
	{
		m_font1->Shutdown();
		delete m_font1;
		m_font1 = NULL;
	}

	return;
}

bool UserInterfaceClass::Frame(ID3D11DeviceContext *deviceContext,
	int fps,
	float posX, 
	float posY,
	float posZ,
	float rotX,
	float rotY,
	float rotZ)
{
	bool result;

	result = UpdateFpsString(deviceContext, fps);
	if (!result)
		return false;

	result = UpdatePositionStrings(deviceContext,
		posX,
		posY,
		posZ,
		rotX,
		rotY,
		rotZ);

	if (!result)
		return false;

	return true;
}

bool UserInterfaceClass::Render(D3DClass *Direct3D,
	ShaderManagerClass *shaderManager,
	XMMATRIX worldMatrix,
	XMMATRIX viewMatrix,
	XMMATRIX orthoMatrix)
{
	int i;

	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaBlending();

	m_fpsString->Render(Direct3D->GetDeviceContext(),
		shaderManager,
		worldMatrix,
		viewMatrix,
		orthoMatrix,
		m_font1->GetTexture());

	m_videoStrings[0].Render(Direct3D->GetDeviceContext(),
		shaderManager,
		worldMatrix,
		viewMatrix,
		orthoMatrix,
		m_font1->GetTexture());
	
	m_videoStrings[1].Render(Direct3D->GetDeviceContext(),
		shaderManager,
		worldMatrix,
		viewMatrix,
		orthoMatrix,
		m_font1->GetTexture());

	for (i = 0; i < 6; ++i)
		m_positionStrings[i].Render(Direct3D->GetDeviceContext(),
			shaderManager,
			worldMatrix,
			viewMatrix,
			orthoMatrix,
			m_font1->GetTexture());

	Direct3D->DisableAlphaBlending();
	Direct3D->TurnZBufferOn();

	return true;
}

bool UserInterfaceClass::UpdateFpsString(ID3D11DeviceContext *deviceContext, int fps)
{
	char tempString[16];
	char finalString[16];
	float red, green, blue;
	bool result;

	if (m_previousFps == fps)
		return true;

	m_previousFps = fps;

	if (fps > 99999)
		fps = 99999;

	_itoa_s(fps, tempString, 10);

	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0;
	}
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0;
	}
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	result = m_fpsString->UpdateSentence(deviceContext,
		m_font1,
		finalString,
		10,
		-50,
		red,
		green,
		blue);
	
	if (!result)
		return false;

	return true;
}

bool UserInterfaceClass::UpdatePositionStrings(ID3D11DeviceContext *deviceContext,
	float posX,
	float posY,
	float posZ,
	float rotX,
	float rotY,
	float rotZ)
{
	int positionX, positionY, positionZ, rotationX, rotationY, rotationZ;
	char tempString[16];
	char finalString[16];
	bool result;

	positionX = (int)posX;
	positionY = (int)posY;
	positionZ = (int)posZ;
	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	if (positionX != m_previousPosition[0])
	{
		m_previousPosition[0] = positionX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[0].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-100,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}
	if (positionY != m_previousPosition[1])
	{
		m_previousPosition[1] = positionY;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[1].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-120,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}
	if (positionZ != m_previousPosition[2])
	{
		m_previousPosition[2] = positionZ;
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[2].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-140,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}

	if (rotationX != m_previousPosition[3])
	{
		m_previousPosition[3] = rotationX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "rX: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[3].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-180,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}
	if (rotationY != m_previousPosition[4])
	{
		m_previousPosition[4] = rotationY;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "rY: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[4].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-200,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}
	if (rotationZ != m_previousPosition[5])
	{
		m_previousPosition[5] = rotationX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "rZ: ");
		strcat_s(finalString, tempString);
		result = m_positionStrings[5].UpdateSentence(deviceContext,
			m_font1,
			finalString,
			10,
			-220,
			1.0f,
			1.0f,
			1.0f);

		if (!result)
			return false;
	}

	return true;
}
