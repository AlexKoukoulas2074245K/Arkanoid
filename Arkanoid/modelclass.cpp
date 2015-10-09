#include "modelclass.h"

Model::Model()
{

}

Model::~Model()
{
	LOG("Deleting Model...\n");
}

bool Model::Initialize(
	const ModelType& modelDesc,
	ComPtr<ID3D11Device> const device,
	const void* shaderBCode, 
	const size_t bcodeLength,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	dimensions = {};
	initDimensions = {};

	if (!LoadModel(modelDesc.modelFilename, device, shaderBCode, bcodeLength))
	{
		return false;
	}

	material = materialFactory->getMaterial(device, modelDesc.materialFilename);
	if (!material) return false;

	return true;
}

void Model::Update(const Transform& transform)
{	
	D3DXMatrixTranslation(&transMatrix, transform.translation.x, transform.translation.y, transform.translation.z);
	D3DXMatrixScaling(&scaleMatrix, transform.scaling.x, transform.scaling.y, transform.scaling.z);
	D3DXMatrixRotationX(&rotXMatrix, transform.rotation.x);
	D3DXMatrixRotationY(&rotYMatrix, transform.rotation.y);
	D3DXMatrixRotationZ(&rotZMatrix, transform.rotation.z);
}

void Model::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const D3DXMATRIX& viewProjectionMatrix)
{
	// Set the model's input layout as active
	devcon->IASetInputLayout(modelLayout.Get());
	
	// Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer.Get(), INDEX_FORMAT, 0);

	// Render the model's texture
	material->Render(devcon);

	// Calculate the transformation matrices
	CalculateMatrices(viewProjectionMatrix);
}

bool Model::newMaterial(
	const char* const filename,
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	material = matFactory->getMaterial(device, filename);
	if (!material)
	{
		return false;
	}

	return true;
}

// Accessors
Model::Dimensions Model::getCurrentDimensions() const
{
	return dimensions;
}

Model::Dimensions Model::getInitialDimensions() const
{
	return initDimensions;
}

Model::MatrixBuffer Model::getMatrices() const
{
	MatrixBuffer mb;
	mb.finalMatrix = finalMatrix;
	mb.worldMatrix = worldMatrix;
	mb.rotMatrix = rotMatrix;

	return mb;
}

unsigned int Model::getIndexCount() const
{
	return indexCount;
}

unsigned int Model::getVertexCount() const
{
	return vertexCount;
}

void Model::setDimensions(const float maxDepth, const float maxHeight, const float maxWidth)
{
	dimensions.depth = maxDepth;
	dimensions.height = maxHeight;
	dimensions.width = maxWidth;
}

// Private methods
bool Model::SetBuffers(
	ComPtr<ID3D11Device> const device,
	const void* shaderBCode,
	const size_t bcodeLength,
	const std::vector<Vertex>& vertices,
	const std::vector<indexType>& indices)
{
	HRESULT result;

	vertexCount = vertices.size();

	// Vertex Buffer creation
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = vertexCount * sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sbd = {};
	sbd.pSysMem = &vertices[0];
	sbd.SysMemPitch = 0;
	sbd.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vbDesc, &sbd, &vertexBuffer);

	if (FAILED(result))
	{
		MessageBox(NULL, "Vertex buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	// Model Element Layout creation
	D3D11_INPUT_ELEMENT_DESC vDesc = {};
	vDesc.SemanticName = "POSITION";
	vDesc.Format = POS_FORMAT;
	vDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vDesc.AlignedByteOffset = 0;

	D3D11_INPUT_ELEMENT_DESC tDesc = {};
	tDesc.SemanticName = "TEXCOORD";
	tDesc.Format = TEXCOORD_FORMAT;
	tDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	D3D11_INPUT_ELEMENT_DESC nDesc = {};
	nDesc.SemanticName = "NORMAL";
	nDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	nDesc.Format = NORMAL_FORMAT;
	nDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	D3D11_INPUT_ELEMENT_DESC finalDesc[] = {vDesc, tDesc, nDesc};

	result = device->CreateInputLayout(
		finalDesc,
		ARRAYSIZE(finalDesc),
		shaderBCode,
		bcodeLength,
		&modelLayout);

	if (FAILED(result))
	{
		MessageBox(NULL, "Input layout creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	indexCount = indices.size();
	
	// Index Buffer creation
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(indexType) * indexCount;

	D3D11_SUBRESOURCE_DATA isrd;
	isrd.pSysMem = &indices[0];
	isrd.SysMemPitch = 0;
	isrd.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&ibd, &isrd, &indexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Index buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	return true;
}

bool Model::LoadModel(
	const char* const filename,
	ComPtr<ID3D11Device> const device,
	const void* shaderBCode,
	const size_t bcodeLength)
{
	IndexedModel model;
	if (!LoadOBJFromFile(GetAssetPath(filename).c_str(), model))
	{
		return false;
	}
	
	initDimensions.depth = model.maxDepth;
	initDimensions.height = model.maxHeight;
	initDimensions.width = model.maxWidth;
	dimensions = initDimensions;

	return SetBuffers(device, shaderBCode, bcodeLength, model.vertices, model.indices);
}

void Model::CalculateMatrices(const D3DXMATRIX& viewProjection)
{
	rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;
	worldMatrix = scaleMatrix * rotMatrix * transMatrix;
	finalMatrix = worldMatrix * viewProjection;
}