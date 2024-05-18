#include "imgui_manager.h"
#include "imgui_graphics.h"

DgMesh* gMesh = NULL;

void ShowTestWindow(bool* p_open);

void InitOpenGL()
{
	// 깊이 테스트 활성화
	glEnable(GL_DEPTH_TEST);

	// 조명 모델 설정
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int, char**)
{
	// GLFW 윈도우를 생성하고, ImGui를 초기화 한다.
    if (!ImGuiManager::Instance().Init())
        return 0;

	// Glew 및 Ilut 초기화
	glewInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	// OpenGL을 설정을 초기화 한다.
	InitOpenGL();

	// 테스트 메쉬를 로드한다.
	//gMesh = import_mesh_obj("..\\obj\\camel.obj");
	//gMesh = import_mesh_obj("..\\obj\\dice\\dice.obj");
	gMesh = import_mesh_obj("..\\obj\\pear\\pear_export.obj");

    bool show_demo_window = true;	// 데모 윈도우 가시화 변수
    bool show_test_window = true;	// 테스트 윈도우 가시화 변수

	// 메인 루프에 진입한다.
    while (!glfwWindowShouldClose(ImGuiManager::Instance().m_Window))
    {
        glfwPollEvents();
        ImGuiManager::Instance().Begin();
        {
			if (ImGui::BeginMainMenuBar()) // 메인 메뉴를 생성한다.
			{
				if (ImGui::BeginMenu("Test"))
				{
					if (ImGui::MenuItem("Show Demo Window", 0, show_demo_window, true))
						show_demo_window = !show_demo_window;
					
                    if (ImGui::MenuItem("Show Test Window", NULL, show_test_window, true))
                        show_test_window = !show_test_window;
					ImGui::MenuItem("Show Test Window", NULL, &show_test_window, true);
                    
                    if (ImGui::MenuItem("Exit")) // 프로그램을 종료 메뉴
                    {
                        ImGui_ImplOpenGL3_Shutdown();
                        ImGui_ImplGlfw_Shutdown();
                        ImGui::DestroyContext();
                        glfwDestroyWindow(ImGuiManager::Instance().m_Window);
                        glfwTerminate();
                        exit(-1);
                    }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
            if (show_demo_window)	
				ImGui::ShowDemoWindow(&show_demo_window);     
            if (show_test_window)	
				ShowTestWindow(&show_test_window);
        }
        ImGuiManager::Instance().End();        
    }

    ImGuiManager::Instance().CleanUp();
    return 0;
}

void RenderFloor();
void DrawCube(double sx, double sy, double sz);

void ShowTestWindow(bool* p_open)
{
	// 윈도우 플래그(window flag)를 설정한다.
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = true;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = true;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool no_docking = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

	// 속성 윈도우를 생성하고, collapsed된 상태라면 바로 리턴한다.
	if (!ImGui::Begin(u8"ImGui 연습장", p_open, window_flags))
	{
		ImGui::End();
		return;
	}	

	ImGui::Checkbox(u8"정점", &gMesh->bRenderVertex);		ImGui::SameLine();
	ImGui::Checkbox(u8"삼각형", &gMesh->bRenderPolygon);	ImGui::SameLine();
	ImGui::Checkbox(u8"와이어", &gMesh->bRenderWire);		ImGui::SameLine();
	ImGui::Checkbox(u8"텍스처", &gMesh->bRenderTexture);

	// 카메라와 프레임 버퍼 객체를 생성한다.
	float Width = ImGui::GetContentRegionAvail().x;
	float Height = ImGui::GetContentRegionAvail().y;
	static Camera MyCamera;
	static FrameBuffer MyScene;
	MyCamera.SetWindowSize((int)Width, (int)Height);
	MyScene.RescaleFrameBuffer((int)Width, (int)Height);
	MyScene.Bind();
	{
		glViewport(0, 0, (int)Width, (int)Height);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 마우스 입력을 받아 관측 공간과 관측 변환을 적용한다.
		MyCamera.ProcessMouseEvents();
		MyCamera.SetupViewFrustum();
		MyCamera.SetupViewTransform();		

		// 바닥 평면을 렌더링 한다.
		RenderFloor();
		gMesh->Render();
	}
	MyScene.Unbind();

	// 프레임 버퍼에 렌더링된 이미지를 텍스처로 얻어와서 이미지로 출력한다.
	uint64_t MySceneTexId = MyScene.GetFrameTexture();
	ImGui::Image(reinterpret_cast<void*>(MySceneTexId), ImVec2(Width, Height), ImVec2(0, 1), ImVec2(1, 0));
	
	ImGui::End();
}

void RenderFloor()
{
	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int x = -10; x <= 10; x++)
	{
		if (x == 0) continue;
		glBegin(GL_LINES);
		glVertex3f((float)x, 0.0f, -10.0f);
		glVertex3f((float)x, 0.0f, 10.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-10.0f, 0, (float)x);
		glVertex3f(10.0f, 0, (float)x);
		glEnd();
	}

	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -10.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glEnd();

	glEnable(GL_LIGHTING);
	glLineWidth(1.0f);
}

void DrawCube(double sx, double sy, double sz)
{
	glPushMatrix();
	{
		sx *= 0.5;
		sy *= 0.5;
		sz *= 0.5;
		glBegin(GL_QUADS);
		{
			// Front
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3d(-sx, sy, sz);
			glVertex3d(-sx, -sy, sz);
			glVertex3d(sx, -sy, sz);
			glVertex3d(sx, sy, sz);

			// Right
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3d(sx, sy, sz);
			glVertex3d(sx, -sy, sz);
			glVertex3d(sx, -sy, -sz);
			glVertex3d(sx, sy, -sz);

			// Back
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3d(sx, sy, -sz);
			glVertex3d(sx, -sy, -sz);
			glVertex3d(-sx, -sy, -sz);
			glVertex3d(-sx, sy, -sz);

			// Left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3d(-sx, sy, sz);
			glVertex3d(-sx, sy, -sz);
			glVertex3d(-sx, -sy, -sz);
			glVertex3d(-sx, -sy, sz);

			// Top
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3d(-sx, sy, sz);
			glVertex3d(sx, sy, sz);
			glVertex3d(sx, sy, -sz);
			glVertex3d(-sx, sy, -sz);

			// Bottom
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3d(-sx, -sy, sz);
			glVertex3d(-sx, -sy, -sz);
			glVertex3d(sx, -sy, -sz);
			glVertex3d(sx, -sy, sz);
		}
		glEnd();
	}
	glPopMatrix();
}
