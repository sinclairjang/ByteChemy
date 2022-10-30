#include "fm_pch.h"
#include "CameraController.h"

#ifdef DX_MATH

// Known Issue(https://github.com/microsoft/vscode-cpptools/issues/6135) : 
// IntelliSense is incorrrectly putting the .inl code in the global namespace instead of the namespace surrounding the #include.
// So We don't use 'using namespace' here to aviod intelliSense giving false errors, 

void EditorMainCamera::SetCameraShotSize(FM_PERSPECTIVE_TRANSFORM_ARGS)
{
	m_VerticalFOV = fovY;
	m_AspectRatio = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	m_NearPlaneHeight = 2.0f * m_NearZ * tanf( 0.5f * m_VerticalFOV );
	m_FarPlaneHeight = 2.0f * m_FarZ * tanf( 0.5f * m_VerticalFOV );

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(m_VerticalFOV, m_AspectRatio, m_NearZ, m_FarZ);
	
	//DirectX::XMFLOAT4X4 proj = (DirectX::XMFLOAT4X4)m_Proj;
	//DirectX::XMStoreFloat4x4(&proj, P);
	//m_Proj = proj;
	
	// Helper function to facilitate data transfer between math libraries that does as above
	StoreXMMatAsFMMat4(m_Proj, P);

	m_DirtyFlag = true;
}

void EditorMainCamera::SetCameraMotion(CameraMotionType motionType, FM_VECTOR_MULTIPLY_ADD_ARGS)
{
	//INFO: generic argument 'd' stands for 'difference'.

	if (motionType == CameraMotionType::WALK)  // 'd' : difference in translation
	{
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_LookDir);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_CameraPos);

		StoreXMVecAsFMVec3(m_CameraPos, DirectX::XMVectorMultiplyAdd(s, l, p));
	}

	if (motionType == CameraMotionType::STRAFE)
	{
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_RightDir);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_CameraPos);

		StoreXMVecAsFMVec3(m_CameraPos, DirectX::XMVectorMultiplyAdd(s, r, p));
	}

	if (motionType == CameraMotionType::PITCH)	// 'd' : difference in angle
	{
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_RightDir), d);

		StoreXMVecAsFMVec3(m_UpDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_UpDir), R));
		
		StoreXMVecAsFMVec3(m_LookDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_LookDir), R));
	}

	if (motionType == CameraMotionType::YAW)
	{
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(d);

		StoreXMVecAsFMVec3(m_RightDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_RightDir), R));
		
		StoreXMVecAsFMVec3(m_LookDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_LookDir), R));
	}

	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_RightDir);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_UpDir);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_LookDir);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3)m_CameraPos);

	// Reothonormalize the camera's coordinates to correct numerical errors accumulated by several matrix operations (especially rotations) 
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));
	R = DirectX::XMVector3Cross(U, L);
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	StoreXMVecAsFMVec3(m_RightDir, R);
	StoreXMVecAsFMVec3(m_UpDir, U);
	StoreXMVecAsFMVec3(m_LookDir, L);

	m_View(0, 0) = m_RightDir.x;
	m_View(1, 0) = m_RightDir.y;
	m_View(2, 0) = m_RightDir.z;
	m_View(3, 0) = x;

	m_View(0, 1) = m_UpDir.x;
	m_View(1, 1) = m_UpDir.y;
	m_View(2, 1) = m_UpDir.z;
	m_View(3, 1) = y;

	m_View(0, 2) = m_LookDir.x;
	m_View(1, 2) = m_LookDir.y;
	m_View(2, 2) = m_LookDir.z;
	m_View(3, 2) = z;;

	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f;

	m_DirtyFlag = true;
}
#endif

