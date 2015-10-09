#include "transitionclass.h"

Transition::Transition()
{

}

Transition::~Transition()
{
	LOG("Deleting Transition...\n");
}

bool Transition::Initialize(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager)
{
	HRESULT result;
	alphaCounter = 0.0f;

	Vertex vertices[] =
	{
		{D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
		{D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
		{D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
		{D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
		{D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
		{D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
	};

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = &vertices;

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = ARRAYSIZE(vertices) * sizeof(Vertex);

	result = device->CreateBuffer(&vbDesc, &srd, vertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		MessageBox(NULL, "Transition Vertex Buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC pied = {};
	pied.SemanticName = "POSITION";
	pied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pied.AlignedByteOffset = 0;
	
	D3D11_INPUT_ELEMENT_DESC cied = {};
	cied.SemanticName = "COLOR";
	cied.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	cied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	cied.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	D3D11_INPUT_ELEMENT_DESC ieds[] = {pied, cied};

	result = device->CreateInputLayout(
		ieds,
		ARRAYSIZE(ieds),
		shaderManager->getTransitionShader()->getShaderBCode(),
		shaderManager->getTransitionShader()->getByteCodeLength(),
		&transLayout);

	if (FAILED(result))
	{
		MessageBox(NULL, "Input Layout creation for transition failed", FATALERROR, FATALLAYOUT);
		return false;
	}


	active = false;
	return true;
}

void Transition::Update()
{
	if (!active) return;
	alphaCounter += 0.02f;
	LOG(std::to_string(alphaCounter).c_str());
	LOG("\n");
	if (alphaCounter >= 2.0f) alphaCounter = 2.0f;
}

void Transition::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const std::shared_ptr<ShaderManager> shaderManager)
{
	if (!active) return;

	devcon->IASetInputLayout(transLayout.Get());
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	TransitionShader::TransBuffer transBuffer = {};
	transBuffer.alphaCounter = alphaCounter;

	shaderManager->getTransitionShader()->Render(devcon, transBuffer);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(6, 0);
}

bool Transition::getFinished() const
{
	return alphaCounter == 2.0f;
}

bool Transition::getActive() const
{
	return active;
}

void Transition::setActive(const bool active)
{
	this->active = active;
}
