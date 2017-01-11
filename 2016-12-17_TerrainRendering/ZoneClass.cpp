#include "ZoneClass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Light = 0;
	m_Position = 0;
	m_SkyDome = 0;
	m_Terrain = 0;
}

ZoneClass::ZoneClass(const ZoneClass &other)
{
}

ZoneClass::~ZoneClass()
{
}

bool ZoneClass::Initialize(D3DClass *direct3D, 
	HWND hwnd, 
	int screenWidth, 
	int screenHeight, 
	float screenDepth)
{
	bool result;

	m_UserInterface = new UserInterfaceClass;
	if (!m_UserInterface)
		return false;

	result = m_UserInterface->Initialize(direct3D, screenHeight, screenWidth);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
		return false;

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	m_Light = new LightClass;
	if (!m_Light)
		return false;

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, -0.5f);

	m_Position = new PositionClass;
	if (!m_Position)
		return false;

	//m_Position->SetPosition(128.0f, 100.0f, -150.0f);
	//m_Position->SetRotation(19.0f, 0.0f, 0.0f);
	m_Position->SetPosition(128.0f, 1.0f, 0.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
		return false;

	result = m_SkyDome->Initialize(direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);

		return false;
	}

	m_Terrain = new TerrainClass;
	if (!m_Terrain)
		return false;

	result = m_Terrain->Initialize(direct3D->GetDevice(), "data/Setup.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);

		return false;
	}

	m_displayUI = true;
	m_wireFrame = true;
	m_play = false;

	return true;
}

void ZoneClass::Shutdown()
{
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = NULL;
	}

	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = NULL;
	}

	if (m_Position)
	{
		delete m_Position;
		m_Position = NULL;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = NULL;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

	if (m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = NULL;
	}

	return;
}

bool ZoneClass::Frame(D3DClass *direct3D, 
	InputClass *input,
	ShaderManagerClass *shaderManager, 
	TextureManagerClass *textureManager,
	float frameTime,
	int fps)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	HandleMovementInput(input, frameTime);

	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	if(m_play)
		PushedF3Button(frameTime);
	
	result = m_UserInterface->Frame(direct3D->GetDeviceContext(),
		fps,
		posX,
		posY,
		posZ,
		rotX,
		rotY,
		rotZ,
		m_SkyDome->GetApexColor(),
		m_SkyDome->GetCenterColor());
	
	if (!result)
		return false;

	result = Render(direct3D, shaderManager, textureManager);
	if (!result)
		return false;

	return true;
}

void ZoneClass::HandleMovementInput(InputClass *input, float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	m_Position->SetFrameTime(frameTime);

	keyDown = input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = input->IsWPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = input->IsXPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = input->IsAPressed();
	m_Position->MoveLeftward(keyDown);

	keyDown = input->IsDPressed();
	m_Position->MoveRightward(keyDown);

	keyDown = input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	if (input->IsF1Toggled())
		m_displayUI = !m_displayUI;

	if (input->IsF2Toggled())
		m_wireFrame = !m_wireFrame;

	if (input->IsF3Toggled())
		m_play = !m_play;

	return;
}

bool ZoneClass::Render(D3DClass *direct3D, 
	ShaderManagerClass *shaderManager,
	TextureManagerClass *textureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;
	XMFLOAT3 cameraPosition;

	m_Camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	cameraPosition = m_Camera->GetPosition();

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	direct3D->TurnOffCulling();
	direct3D->TurnZBufferOff();

	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	m_SkyDome->Render(direct3D->GetDeviceContext());
	
	result = shaderManager->RenderSkyDomeShader(direct3D->GetDeviceContext(),
		m_SkyDome->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_SkyDome->GetApexColor(),
		m_SkyDome->GetCenterColor());

	if (!result)
		return false;

	direct3D->GetWorldMatrix(worldMatrix);
	
	direct3D->TurnZBufferOn();
	direct3D->TurnOnCulling();

	if (m_wireFrame)
		direct3D->EnableWireframe();

	m_Terrain->Render(direct3D->GetDeviceContext());

	result = shaderManager->RenderTerrainShader(direct3D->GetDeviceContext(),
		m_Terrain->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		textureManager->GetTexture(1),
		textureManager->GetTexture(2),
		m_Light->GetDirection(),
		m_Light->GetDiffuseColor());

	if (!result)
		return false;

	if (m_wireFrame)
		direct3D->DisableWireframe();

	if (m_displayUI)
	{
		result = m_UserInterface->Render(direct3D,
			shaderManager,
			worldMatrix,
			baseViewMatrix,
			orthoMatrix);	

		if (!result)
			return false;
	}

	direct3D->EndScene();

	return true;
}

// 0 - 257 : 129
void ZoneClass::PushedF3Button(float frameTime)
{
	//XMFLOAT3 dir;
	//
	//dir = m_Light->GetDirection();
	//
	//if (dir.x >= 360) dir.x -= 360;
	//if (dir.y >= 360) dir.y -= 360;
	//if (dir.z >= 360) dir.z -= 360;
	//
	//dir.x += frameTime / 5;
	//
	//m_Light->SetDirection(dir.x, dir.y, dir.z);
	
	XMFLOAT4 tempColor;

	tempColor = m_SkyDome->GetApexColor();

	tempColor.x -= frameTime / 10; // R
	tempColor.y -= frameTime / 10; // G
	tempColor.z -= frameTime / 10; // B

	m_SkyDome->SetApexColor(tempColor);

	return;
}
