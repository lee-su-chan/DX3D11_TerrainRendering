#include "ZoneClass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Position = 0;
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

	m_Position = new PositionClass;
	if (!m_Position)
		return false;

	m_Position->SetPosition(128.0f, 10.0f, -10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

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

	if (m_Position)
	{
		delete m_Position;
		m_Position = NULL;
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
	float frameTime,
	int fps)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	HandleMovementInput(input, frameTime);

	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	result = m_UserInterface->Frame(direct3D->GetDeviceContext(),
		fps,
		posX,
		posY,
		posZ,
		rotX,
		rotY,
		rotZ);
	
	if (!result)
		return false;

	result = Render(direct3D, shaderManager);
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

	keyDown = input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = input->IsZPressed();
	m_Position->MoveDownward(keyDown);

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

	return;
}

bool ZoneClass::Render(D3DClass *direct3D, ShaderManagerClass *shaderManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	m_Camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_wireFrame)
		direct3D->EnableWireframe();

	m_Terrain->Render(direct3D->GetDeviceContext());

	result = shaderManager->RenderColorShader(direct3D->GetDeviceContext(),
		m_Terrain->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix);

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
