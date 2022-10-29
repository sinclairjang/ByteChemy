#include "fm_pch.h"
#include "CameraController.h"

#ifdef DX_MATH

using namespace DirectX;

void EditorMainCamera::SetCameraShotSize(FM_PERSPECTIVE_TRANSFORM_ARGS)
{
	m_VerticalFOV = fovY;
	m_AspectRatio = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	m_NearPlaneHeight = 2.0f * m_NearZ * tanf( 0.5f * m_VerticalFOV );
	m_FarPlaneHeight = 2.0f * m_FarZ * tanf( 0.5f * m_VerticalFOV );

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_VerticalFOV, m_AspectRatio, m_NearZ, m_FarZ);
	
	DirectX::XMStoreFloat4x4(&(XMFLOAT4X4)m_Proj, P);				// Known Issue(https://github.com/microsoft/vscode-cpptools/issues/6135) : 
																	// IntelliSense is incorrrectly putting the .inl code in the global namespace instead of the namesoace surrounding the #include.
	
}

void EditorMainCamera::SetCameraMotion(CameraMotionType motionType, FM_VECTOR_MULTIPLY_ADD_ARGS)
{
	if (motionType == CameraMotionType::WALK)
	{
		XMVECTOR s = XMVectorReplicate(d);
		XMVECTOR l = DirectX::XMLoadFloat3(&(XMFLOAT3)m_RightDir);	// Same as above
		XMVECTOR p = DirectX::XMLoadFloat3(&(XMFLOAT3)m_CameraPos);	// Same as above
		DirectX::XMStoreFloat3(&(XMFLOAT3)m_CameraPos, XMVectorMultiplyAdd(s, l, p));
	}
}
#endif






void EditorMainCamera::Update()
{
}
