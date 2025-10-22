#include <stdio.h>
#include "AwesomeEngine/Source/Core/EntryPoint.h"
#include "AwesomeEngine/Source/Core/App/AppBase.h"
#include "AwesomeEngine/Source/Core/Logging.h"
#include "AwesomeEngine/Source/Graphics/GraphicsInterface.h"
#include "AwesomeEngine/Source/Graphics/Platform/BaseWindow.h"
#include "AwesomeEngine/Source/Graphics/VertexDescription.h"
#include "AwesomeEngine/Source/Core/FileSystem.h"

using namespace Graphics;
struct Vertex
{
	float x, y, z;
	float r, g, b;
};

class TriangleApp : public AppBase
{
public:
	TriangleApp(){}
	~TriangleApp(){}
	void Init();
	void Update();
	void Release();

private:
	BufferHandle m_VertexBuffer;
	GraphicsPipeline m_Pipeline;
};

void TriangleApp::Init()
{
	AppBase::Init();
	
	Core::FileDevice exampleFileSystem;
	exampleFileSystem.Identifier = "shadersrc";
	exampleFileSystem.RootPath = "../../src/shaders/";
	Core::FileSystem::GetInstance()->AddFileDevice(exampleFileSystem);

	Vertex arr[3] =
	{
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f
	};
	m_VertexBuffer = m_GraphicsInterface->CreateBuffer(BufferType::VertexBuffer, CPUAccess::None, GPUAccess::None, sizeof(Vertex) * 3, 0, &arr[0]);
	{
		GraphicsPipelineDescription pdesc = {};
		pdesc.PixelShader.ShaderEntryPoint = "PSSimple";
		pdesc.PixelShader.ShaderPath = "shadersrc:TriangleSample.hlsl";
		pdesc.PixelShader.Type = ShaderType::Pixel;

		pdesc.VertexShader.ShaderEntryPoint = "VSSimple";
		pdesc.VertexShader.ShaderPath = "shadersrc:TriangleSample.hlsl";
		pdesc.VertexShader.Type = ShaderType::Vertex;
		pdesc.VertexDescription = PosColorVertexDescription::GetDescription();

		pdesc.ColorFormats[0] = m_GraphicsInterface->GetOutputFormat();
		m_Pipeline = m_GraphicsInterface->CreateGraphicsPipeline(pdesc);
	}

	m_GraphicsInterface->FlushAndWait();
}

void TriangleApp::Update()
{
	AppBase::Update();

	m_GraphicsInterface->SetScissor(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_GraphicsInterface->SetTopology(Topology::TriangleList);
	m_GraphicsInterface->SetGraphicsPipeline(m_Pipeline);
	m_GraphicsInterface->SetVertexBuffer(m_VertexBuffer, 3, sizeof(Vertex));
	m_GraphicsInterface->Draw(3, 0);
}

void TriangleApp::Release()
{
	m_GraphicsInterface->ReleaseGraphicsPipeline(m_Pipeline);
	AppBase::Release();
}

TriangleApp app;
ENTRY_POINT(app, "Triangle App", false);