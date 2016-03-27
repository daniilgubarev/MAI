#include "CCubeRenderer.h"

// Массив из 3 векторов, которые являются вершинами треугольника
static const GLfloat vertexBufferData[] = {
	// front
	-0.5, -0.5,  0.5,
	0.5, -0.5,  0.5,
	0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	// back
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0
};

static const GLfloat uvBufferData[] = {
	0.000059f, 1.0f-0.000004f,
	0.000103f, 1.0f-0.336048f,
	0.335973f, 1.0f-0.335903f,
	1.000023f, 1.0f-0.000013f,
	0.667979f, 1.0f-0.335851f,
	0.999958f, 1.0f-0.336064f,
	0.667979f, 1.0f-0.335851f,
	0.336024f, 1.0f-0.671877f
};

static const uint32_t cubeIndexes[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 0, 3,
	3, 7, 4,
	// left
	4, 5, 1,
	1, 0, 4,
	// right
	3, 2, 6,
	6, 7, 3,
};

CCubeRenderer::CCubeRenderer(CScene* scene, const CTerrain* terrain)
{
	Scene = scene;

	Shader.LoadShader("fx/pos_uv_TO_uv.vert", CShaderProgram::ST_VERTEX);
	Shader.LoadShader("fx/uv_TO_color.frag", CShaderProgram::ST_FRAGMENT);

	Shader.LinkShaders();

	VertexBuffer = new CVertexAttribArray(sizeof(glm::vec3) * 8, 3, CVertexAttribArray::AT_FLOAT, false);
	UVBuffer = new CVertexAttribArray(sizeof(glm::vec2) * 8, 2, CVertexAttribArray::AT_FLOAT, false);

	Indexes = new CIndexBuffer(6 * 2 * 3, CIndexBuffer::IT_UINT);

	// fill vertices
	char* bufferData = (char*)VertexBuffer->Lock();
	{
		memcpy(bufferData, vertexBufferData, sizeof(vertexBufferData));
	}
	VertexBuffer->Unlock();

	bufferData = (char*)UVBuffer->Lock();
	{
		memcpy(bufferData, uvBufferData, sizeof(uvBufferData));
	}
	UVBuffer->Unlock();

	bufferData = (char*)Indexes->Lock();
	{
		memcpy(bufferData, cubeIndexes, sizeof(cubeIndexes));
	}
	Indexes->Unlock();

	if (!Texture.Load("img/image.bmp"))
		std::cerr << "fail load img/image.bmp" << std::endl;

	for (size_t i = 0; i < 100; i++)
	{
		SRenderable* cube = scene->NewRenderableObject();

		float x = float(i * 10);
		float y = 0.0f;
		float z = x;

		if (terrain)
			terrain->GetHeight(glm::vec3(x, y, z), y);

		cube->Transform.SetPosition(glm::vec3(x, y, z));
		cube->Transform.SetRotation(glm::vec3(x, y, z), float(i));

		cube->AttribArrays.push_back(VertexBuffer);
		cube->AttribArrays.push_back(UVBuffer);

		cube->IndexBuffer = Indexes;
		cube->Shader = &Shader;
		
		cube->Textures.push_back(std::make_pair(&Texture, "textureSampler"));

		SRenderable::SUniform* matMVP = new SRenderable::SUniform;

		cube->Uniforms.push_back(matMVP);

		matMVP->Type = SRenderable::UT_MAT4;
		matMVP->Name = "MVP";

		Cubes.push_back(cube);
	}
}

CCubeRenderer::~CCubeRenderer()
{
	for (size_t i = 0; i < Cubes.size(); i++)
	{
		delete Cubes[i];
		Scene->DeleteRenderableObject(Cubes[i]);
	}
}

bool CCubeRenderer::Update(float dt)
{
	return true;
}

void CCubeRenderer::Render(float dt)
{
	for (size_t i = 0; i < Cubes.size(); i++)
	{
		Cubes[i]->Uniforms[0]->Mat4 = Scene->GetActiveCamera()->GetViewProjMatrix() *
			Cubes[i]->Transform.GetTransformMatrix();
	}
}
