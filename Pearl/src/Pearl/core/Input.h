#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "imgui.h"    // Inputs Utilities: Keyboard, Gamepad, Mouse

namespace Pearl 
{
	// Simple delegator to ImGui platform backend to achieve portability
	class Input 
	{
	public: 
		// Keyboard and Gamepad
		inline static bool IsKeyDown(KeyCode key)															{ return ImGui::IsKeyDown(key); }
		inline static bool IsKeyPressed(KeyCode key, bool repeat = true)									{ return ImGui::IsKeyPressed(key, repeat); }
		inline static bool IsKeyReleased(KeyCode key)														{ return ImGui::IsKeyReleased(key); }
		inline static int GetKeyPressedAmount(KeyCode key, float repeat_delay, float rate)					{ return ImGui::GetKeyPressedAmount(key, repeat_delay, rate); }
		inline static const char* GetKeyName(KeyCode key)													{ return ImGui::GetKeyName(key); }
		inline static void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard)						{ return ImGui::SetNextFrameWantCaptureKeyboard(want_capture_keyboard); }

		// Mouse
		inline static bool IsMouseDown(MouseCode button)													{ return ImGui::IsMouseDown(button); }
		inline static bool IsMouseClicked(MouseCode button, bool repeat = false)							{ return ImGui::IsMouseClicked(button, repeat); }
		inline static bool IsMouseReleased(MouseCode button)												{ return ImGui::IsMouseReleased(button); }
		inline static bool IsMouseDoubleClicked(MouseCode button)											{ return ImGui::IsMouseDoubleClicked(button); }
		inline static int  GetMouseClickedCount(MouseCode button)											{ return ImGui::GetMouseClickedCount(button); }
		inline static bool IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true)	{ return ImGui::IsMouseHoveringRect(r_min, r_max, clip); }
		inline static bool IsMousePosValid(const ImVec2* mouse_pos = NULL)									{ return ImGui::IsMousePosValid(mouse_pos); }
		inline static bool IsAnyMouseDown()																	{ return ImGui::IsAnyMouseDown(); }
		inline static ImVec2 GetMousePos()																	{ return ImGui::GetMousePos(); }
		inline static ImVec2 GetMousePosOnOpeningCurrentPopup()												{ return ImGui::GetMousePosOnOpeningCurrentPopup(); }
		inline static bool IsMouseDragging(MouseCode button, float lock_threshold = -1.0f)					{ return ImGui::IsMouseDragging(button, lock_threshold); }
		inline static ImVec2 GetMouseDragDelta(MouseCode button = 0, float lock_threshold = -1.0f)			{ return ImGui::GetMouseDragDelta(button, lock_threshold); }
		inline static void ResetMouseDragDelta(MouseCode button = 0)										{ return ImGui::ResetMouseDragDelta(button); }
		inline static void ResetMouseDragDelta(MouseCode button = 0)										{ return ImGui::ResetMouseDragDelta(button); }
		inline static ImGuiMouseCursor GetMouseCursor()														{ return ImGui::GetMouseCursor(); }
		inline static void SetMouseCursor(MouseCursor cursor_type)											{ return ImGui::SetMouseCursor(cursor_type); }
		inline static void SetNextFrameWantCaptureMouse(bool want_capture_mouse)							{ return ImGui::SetNextFrameWantCaptureMouse(want_capture_mouse); }
	};
}
