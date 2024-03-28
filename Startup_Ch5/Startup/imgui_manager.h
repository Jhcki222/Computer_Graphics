#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "..\usr\include\imgui\imgui.h"
#include "..\usr\include\imgui\imgui_impl_glfw.h"
#include "..\usr\include\imgui\imgui_impl_opengl3.h"
#include "..\usr\include\GL\glew.h"
#include "..\usr\include\glfw\glfw3.h" // Will drag system OpenGL headers

#include "imgui_graphics.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// 외부 라이브러리 링크
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "..\\usr\\lib\\glfw3.lib")
#pragma comment(lib, "..\\usr\\lib\\glew32.lib")

/*! 
*	\class	ImGuiManager
*	\brief	GLFW + OpenGL3.0 기반의 ImGui 응용프로그램 개발을 위한 매니저 싱글톤(singleton) 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-03-04
*/
class ImGuiManager
{
public:
	/*! \biref	메인 윈도우 포인터 */
	GLFWwindow* m_Window;

private:
	/*! \biref 생성자 */
	ImGuiManager();

	/*! \biref 소멸자 */
	~ImGuiManager();

public:
	/*! 
	*	\biref	클래스 객체를 반환한다.
	* 
	*	\return 클래스 객체를 반환한다.
	*/
	static ImGuiManager& Instance()
	{
		static ImGuiManager _inst;
		return _inst;
	}

	/*! \biref	GLFW 윈도우를 생성하고 ImGui를 초기화 한다.	*/
	bool Init();

	/*! \biref	새로운 프레임을 시작한다.	*/
	void Begin();

	/*! \biref	프레임의 내용을 렌더링한다. */
	void End();

	/*! \biref	생성된 윈도우와 ImGui를 종료한다. */
	void CleanUp();
};
