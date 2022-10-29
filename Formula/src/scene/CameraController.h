#pragma once

enum class CameraMotionType : UINT8
{
	STRAFE,
	WALK,
	PITCH,
	YAW,
	ROLL,
};


class EditorMainCamera
{
public:
	// Adjust camera field of view  (equivalent to setting zoom level)
	void SetCameraShotSize(FM_PERSPECTIVE_TRANSFORM_ARGS);	

	// Adjust camera coordinates
	void SetCameraMotion(CameraMotionType motionType, FM_VECTOR_MULTIPLY_ADD_ARGS);
	
	// On dirty flag, update view and projection matrix
	void Update();

private:
	// Camera coordinate with respect to the world space
	FMVec3 m_CameraPos	= { 0.0f, 0.0f, 0.0f };
	FMVec3 m_RightDir	= { 1.0f, 0.0f, 0.0f };
	FMVec3 m_UpDir		= { 0.0f, 1.0f, 0.0f };
	FMVec3 m_LookDir	= { 0.0f, 0.0f, 1.0f };

	// Frustum data
	float m_NearZ			= 0.0f;
	float m_FarZ			= 0.0f;
	float m_AspectRatio		= 0.0f;
	float m_VerticalFOV		= 0.0f;
	float m_NearPlaneHeight = 0.0f;
	float m_FarPlaneHeight	= 0.0f;

	// Checked on update loop to determine whether the game editor loop should update the associated shader data, in this case MainPassProperty 
	bool m_DirtyFlag = true;

	// Main engine camera data staged in system memory before it affects its associated shader data, in this case MainPassProperty
	FMMat4 m_View = FMMat4(1);
	FMMat4 m_Proj = FMMat4(1);
};
