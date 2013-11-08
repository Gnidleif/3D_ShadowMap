#include "StaticEntity.h"

StaticEntity::StaticEntity(std::string key, std::string texPath)
	: VirtualEntity()
{
	mModelInstance.mModel = Model->GetStaticModel(key, texPath);
}

StaticEntity::~StaticEntity(void)
{
}

void StaticEntity::Initialize(XMFLOAT3 position, float scale)
{
	VirtualEntity::Initialize(position, scale);
}

void StaticEntity::Draw(ID3D11DeviceContext* devCon, ID3DX11EffectTechnique* activeTech, Camera* camera)
{
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devCon->IAGetInputLayout(&InputLayouts::mPosNorTexTan);

	D3DX11_TECHNIQUE_DESC techDesc = {};
	activeTech->GetDesc(&techDesc);

	Effects::NormalFX->SetView(&camera->GetViewMatrix());
	Effects::NormalFX->SetProj(&camera->GetProjMatrix());

	for(UINT i(0); i != techDesc.Passes; ++i)
	{
		activeTech->GetPassByIndex(i)->Apply(0, devCon);
		for(UINT j(0); j != mModelInstance.mModel->GetMeshes().size(); ++j)
		{
			mModelInstance.mModel->GetMesh(j)->Draw(devCon);
		}
	}
}

void StaticEntity::CalcWorld()
{
	XMStoreFloat4x4(
		&mModelInstance.mWorld,
		XMLoadFloat4x4(&this->mModelScale) *
		XMLoadFloat4x4(&this->mModelRot) *
		XMLoadFloat4x4(&this->mModelOffset));
	Effects::NormalFX->SetWorld(&XMLoadFloat4x4(&mModelInstance.mWorld));
	Effects::NormalFX->SetWorldInvTranspose(&MathHelper::InverseTranspose(XMLoadFloat4x4(&mModelInstance.mWorld)));
}
