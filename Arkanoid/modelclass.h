#pragma once

// Custom Includes
#include "common.h"
#include "materialclass.h"
#include "ioutils.h"
#include "materialfactoryclass.h"

class Model
{
public:
	struct ModelType
	{
		const char* modelFilename;
		const char* materialFilename;
	};

	struct Transform
	{
		D3DXVECTOR3 translation;
		D3DXVECTOR3 scaling;
		D3DXVECTOR3 rotation;
	};

	struct Dimensions
	{
		float width, height, depth;
	};

	struct MatrixBuffer
	{
		D3DXMATRIX finalMatrix;
		D3DXMATRIX rotMatrix;
		D3DXMATRIX worldMatrix;
	};

	Model();
	~Model();

	bool Initialize(
		const ModelType&,
		ComPtr<ID3D11Device> const,
		const void* shaderBCode,
		const size_t bcodeLength,
		const std::shared_ptr<MaterialFactory>);

	void Update(const Transform& transform);
	void Render(ComPtr<ID3D11DeviceContext> const devcon, const D3DXMATRIX&);

	bool newMaterial(
		const char* const,
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<MaterialFactory>);

	unsigned int getIndexCount() const;
	unsigned int getVertexCount() const;
	Dimensions getCurrentDimensions() const;
	Dimensions getInitialDimensions() const;
	MatrixBuffer getMatrices() const;

	void setDimensions(const float depth, const float height, const float width);
	
private:
	bool LoadModel(
		const char * const,
		ComPtr<ID3D11Device> const,
		const void*,
		const size_t);

	bool SetBuffers(
		ComPtr<ID3D11Device> const,
		const void*,
		const size_t,
		const std::vector<Vertex>&,
		const std::vector<indexType>&);
	
	void CalculateMatrices(const D3DXMATRIX&);

private:
	// COM objects
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11InputLayout> modelLayout;

	// Matrices
	D3DXMATRIX rotXMatrix;
	D3DXMATRIX rotYMatrix;
	D3DXMATRIX rotZMatrix;
	D3DXMATRIX rotMatrix;
	D3DXMATRIX transMatrix;
	D3DXMATRIX scaleMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX finalMatrix;

	std::shared_ptr<Material> material;
	Dimensions dimensions;
	Dimensions initDimensions;

	unsigned int indexCount;
	unsigned int vertexCount;
};