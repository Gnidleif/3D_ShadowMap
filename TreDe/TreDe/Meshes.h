#ifndef MESHES_H
#define MESHES_H
#include "d3dUtilities.h"
#include "VertexDef.h"

// Virtual base class for meshes, the only difference between them is the type of vertex used
class VirtualMesh
{
public:
	VirtualMesh();
	virtual ~VirtualMesh();
	void Draw(ID3D11DeviceContext* devCon);
	template<typename VertexDefType>
	void SetVertexBuffer(ID3D11Device* device, const VertexDefType* vertex, UINT count);
	void SetIndexBuffer(ID3D11Device* device, const UINT* indice, UINT count);

public:
	virtual void SetVerticeSize(UINT size) = 0;
	void SetIndiceSize(UINT size) { this->mIndices.resize(size); }
	void SetIndices(std::vector<UINT> indices) { this->mIndices = indices; }
	void SetMaterialIndex(UINT materialIndex) { this->mMaterialIndex = materialIndex; }

public:
	std::vector<UINT> GetIndices() const { return this->mIndices; }
	UINT GetMaterialIndex() const { return this->mMaterialIndex; }

protected:
	UINT mOffset;

	std::vector<UINT> mIndices;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	DXGI_FORMAT mIndexBufferFormat;
	UINT mVertexStride;
	UINT mMaterialIndex;
};

template<typename VertexDefType>
void VirtualMesh::SetVertexBuffer(ID3D11Device* device, const VertexDefType* vertex, UINT count)
{
	SafeRelease(mVertexBuffer);
	mVertexStride = sizeof(VertexDefType);

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = mVertexStride * count;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA viData = {};
	viData.pSysMem = vertex;
	device->CreateBuffer(&vbd, &viData, &mVertexBuffer);
}

// Terrainmesh, used for terrain models
class TerrainMesh : public VirtualMesh
{
public:
	TerrainMesh() : VirtualMesh() {}
	~TerrainMesh() {}

public:
	std::vector<VertexDef::PosNorTex> GetVertices() const { return this->mVertices; }

public:
	void SetVerticeSize(UINT size) { this->mVertices.resize(size); }
	void SetVertices(std::vector<VertexDef::PosNorTex> vertices) { this->mVertices = vertices; }

private:
	std::vector<VertexDef::PosNorTex> mVertices;
};

// Generic mesh, used for non-moving, non-terrain models
class StaticMesh : public VirtualMesh
{
public:
	StaticMesh() : VirtualMesh() {}
	~StaticMesh() {}

public:
	std::vector<VertexDef::PosNorTexTan> GetVertices() const { return this->mVertices; }

public:
	void SetVerticeSize(UINT size) { this->mVertices.resize(size); }
	void SetVertices(std::vector<VertexDef::PosNorTexTan> vertices) { this->mVertices = vertices; }

private:
	std::vector<VertexDef::PosNorTexTan> mVertices;
};
#endif