#include "AwesomeEngine/Source/Core/App/AppBase.h"
#include "AwesomeEngine/Source/Core/EntryPoint.h"
#include "AwesomeEngine/Source/Core/FileSystem.h"
#include "AwesomeEngine/Source/Core/Logging.h"
#include "AwesomeEngine/Source/Core/Util/OBJLoader.h"
#include "AwesomeEngine/Source/Core/Util/TextureLoader.h"

#include "AwesomeEngine/Source/Graphics/Platform/Windows/WWindow.h"
#include "AwesomeEngine/Source/Graphics/VertexDescription.h"

#include "ModelViewer.hlsl" // This is a bit nasty... we are forced to include here the file so we can see the declarations.

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ModelViewer : public AppBase
{
public:
	void Init();
	void Update()override;
	void Release() override;
private:
	Graphics::BufferHandle m_VertexBuffer;
	Graphics::GraphicsPipeline m_PSO;
	Graphics::BufferHandle m_CB;
	Declarations::ViewerConstants m_Constants;
	int m_NumVertices;
	Graphics::TextureHandle m_TestTexture;
};

ModelViewer app;
ENTRY_POINT(app, "Model Viewer", false);

void ModelViewer::Init()
{
	AppBase::Init();

	{
		Core::FileDevice shadersFileSystem;
		shadersFileSystem.Identifier = "shadersrc";
		shadersFileSystem.RootPath = "../../src/shaders/";
		Core::FileSystem::GetInstance()->AddFileDevice(shadersFileSystem);
	}
	
	{
		Core::FileDevice dataFileSystem;
		dataFileSystem.Identifier = "data";
		dataFileSystem.RootPath = "../../data/";
		Core::FileSystem::GetInstance()->AddFileDevice(dataFileSystem);
	}	

	m_NumVertices = 0;

	// Load model from file and create vertex buffer
	std::string modelPath = "data:Models/suzanne.obj";
	Core::OBJ::Data modelData = {};
	if (Core::OBJ::LoadFromFile(modelPath, modelData))
	{
		std::vector<VertexData> positionsBuffer;

		for (int i = 0; i < modelData.m_Triangles.size(); ++i)
		{
			const Core::OBJ::Triangle& curTri = modelData.m_Triangles[i];
			for (int j = 0; j < 3; ++j)
			{
				const glm::vec3 curPos = modelData.m_Positions[curTri.m_Positions[j]];
				VertexData curVert;
				curVert.m_Position = glm::vec4(curPos, 1.0f);
				curVert.m_UV = modelData.m_TexCoords[curTri.m_TexCoords[j]];
				positionsBuffer.push_back(curVert);
			}
		}

		m_NumVertices = (int)positionsBuffer.size();
		m_VertexBuffer = m_GraphicsInterface->CreateBuffer(
			Graphics::BufferType::GPUBuffer, Graphics::CPUAccess::None, Graphics::GPUAccess::Read, m_NumVertices, sizeof(VertexData), positionsBuffer.data()
		);
	}

	// Load test texture
	std::string texturePath = "data:textures/test_grid.jpeg";
	Core::Texture::TextureData loadedTexture;
	if (Core::Texture::LoadFromFile(texturePath, loadedTexture))
	{		
		m_TestTexture =  m_GraphicsInterface->CreateTexture2D(
			loadedTexture.m_Width, loadedTexture.m_Height, 1, 1, 
			loadedTexture.m_Format, 
			Graphics::TextureFlags::TextureFlagNone, 
			loadedTexture.m_Data
		);
	}

	Graphics::GraphicsPipelineDescription psoDesc;
	psoDesc.ColorFormats[0] = m_GraphicsInterface->GetOutputFormat();
	
	psoDesc.VertexShader.ShaderPath = "shadersrc:ModelViewer.hlsl";
	psoDesc.VertexShader.ShaderEntryPoint = "VSMain";
	psoDesc.VertexShader.Type = Graphics::ShaderType::Vertex;
	
	psoDesc.PixelShader.ShaderPath = "shadersrc:ModelViewer.hlsl";
	psoDesc.PixelShader.ShaderEntryPoint = "PSMain";
	psoDesc.PixelShader.Type = Graphics::ShaderType::Pixel;

	m_PSO = m_GraphicsInterface->CreateGraphicsPipeline(psoDesc);

	m_CB = m_GraphicsInterface->CreateBuffer(Graphics::BufferType::ConstantBuffer, Graphics::CPUAccess::None, Graphics::GPUAccess::Read, sizeof(m_Constants));	

	// A bit nasty, need to maybe get rid of it.. On graphics creation, we leave the cmdlist open and recording, as we may submit copy commands
	// to init buffers, textures etc..
	// Probably should change the scheme and defer that copy to when we are actually in the "render frame"... Anyways :) 
	m_GraphicsInterface->FlushAndWait();
}

void ModelViewer::Update()
{
	glm::mat4x4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4x4 proj = glm::perspectiveFov(90.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.1f, 100.0f);

	m_Constants.Model = glm::rotate(glm::mat4x4(), TotalTime, glm::vec3(1.0f, 0.0f, 0.0f));
	m_Constants.Model = glm::rotate(m_Constants.Model, TotalTime, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Constants.Model = glm::rotate(m_Constants.Model, -TotalTime, glm::vec3(0.0f, 0.0f, 1.0f));

	m_Constants.ViewProjection = proj * view;

	m_GraphicsInterface->SetScissor(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

	m_GraphicsInterface->SetConstantBuffer(m_CB, Declarations::kViewerConstantsSlot, sizeof(m_Constants), &m_Constants);

	m_GraphicsInterface->SetTopology(Graphics::Topology::TriangleList);
	m_GraphicsInterface->SetGraphicsPipeline(m_PSO);
	if (m_NumVertices > 0)
	{
		m_GraphicsInterface->SetResource(m_VertexBuffer, Declarations::kg_VertexBufferSlot);
		m_GraphicsInterface->SetResource(m_TestTexture, Declarations::kg_TestTextureSlot);
		m_GraphicsInterface->Draw(m_NumVertices, 0);
	}
}

void ModelViewer::Release()
{

}