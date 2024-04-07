#include "imgui_manager.h"
#include "imgui_graphics.h"

void ShowTestWindow(bool* p_open);

int main(int, char**)
{
	// GLFW 윈도우를 생성하고, ImGui를 초기화 한다.
    if (!ImGuiManager::Instance().Init())
        return 0;

	// GLEW를 초기화 한다.
	glewInit();

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
					//ImGui::MenuItem("Show Test Window", NULL, &show_test_window, true);
                    
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
void DrawCube(double sx, double sy, double sz, double r, double g, double b);
void DrawSphere(double rad, double r, double g, double b);

// 테스트 윈도우를 표시하는 함수 정의
void ShowTestWindow(bool* p_open)
{
    // 윈도우 플래그 설정
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

    // 속성 윈도우 생성, collapsed된 상태라면 바로 리턴
    if (!ImGui::Begin(u8"ImGui 연습장", p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    // ImGui 창 내용
    static float Angle1 = 0.0f, Angle2 = 0.0f, Angle3 = 0.0f;
    static float Dist = 0.5f;
    ImGui::DragFloat("Angle1", &Angle1, 1.0f, -180.0f, 180.0f);
    ImGui::DragFloat("Angle2", &Angle2, 1.0f, -180.0f, 180.0f);
    ImGui::DragFloat("Angle3", &Angle3, 1.0f, -180.0f, 180.0f);
    ImGui::DragFloat("Dist", &Dist, 0.0f, 0.0f, 0.5f);

    float Width = ImGui::GetContentRegionAvail().x;
    float Height = ImGui::GetContentRegionAvail().y;

    // 카메라와 프레임 버퍼 객체 생성
    static Camera MyCamera;
    static FrameBuffer MyScene;
    MyCamera.SetWindowSize((int)Width, (int)Height);
    MyScene.RescaleFrameBuffer((int)Width, (int)Height);

    MyScene.Bind();
    {
        glViewport(0, 0, (int)Width, (int)Height);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // 마우스 입력을 받아 관측 공간과 관측 변환 적용
        MyCamera.ProcessMouseEvents();
        MyCamera.SetupViewFrustum();
        MyCamera.SetupViewTransform();

        // 좌표 평면 렌더링
        RenderFloor();

        // 큐브 및 구 렌더링
        DrawCube(10.0, 1.0, 10.0, 0.5, 0.5, 0.5);

        glTranslatef(0.0, 1.0, 0.0);
        glRotatef(Angle1, 0.0, 1.0, 0.0);
        DrawCube(1.0, 5.0, 1.0, 1.0, 1.0, 0.0);

        glTranslatef(0.0, 5.0, 0.0);
        glRotatef(Angle2, 0.0, 0.0, 1.0);
        DrawSphere(1.0, 1.0, 0.0, 0.0);
        DrawCube(1.0, 5.0, 1.0, 0.0, 1.0, 1.0);

        glTranslatef(0.0, 5.0, 0.0);
        glRotatef(Angle3, 1.0, 0.0, 0.0);
        DrawSphere(1.0, 1.0, 0.0, 0.0);
        DrawCube(1.0, 5.0, 1.0, 1.0, 0.0, 1.0);

        glPushMatrix();
        {
            glTranslatef(-Dist, 4.5, 0.0);
            DrawCube(0.1, 2.0, 1.0, 1.0, 0.0, 1.0);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-Dist / 2, 4.5, 0.0);
            DrawCube(0.1, 2.0, 1.0, 1.0, 0.0, 1.0);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(Dist / 2, 4.5, 0.0);
            DrawCube(0.1, 2.0, 1.0, 1.0, 0.0, 1.0);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(Dist, 4.5, 0.0);
            DrawCube(0.1, 2.0, 1.0, 1.0, 0.0, 1.0);
        }
        glPopMatrix();


    }
    MyScene.Unbind();

    // 프레임 버퍼에 렌더링된 이미지를 텍스처로 얻어와서 이미지 출력
    uint64_t MySceneTexId = MyScene.GetFrameTexture();
    ImGui::Image(reinterpret_cast<void*>(MySceneTexId), ImVec2(Width, Height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}


void RenderFloor()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	for (float x = -10.0; x <= 10.0; x += 1.0)
	{
		glBegin(GL_LINES);
		glVertex3f(x, 0.0f, -10.0f);
		glVertex3f(x, 0.0f, 10.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-10.0f, 0, x);
		glVertex3f(10.0f, 0, x);
		glEnd();
	}
	glLineWidth(1.0f);
}

void DrawCube(double sx, double sy, double sz, double r, double g, double b)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	{
		glScalef(sx, sy, sz);
		glColor3d(r, g, b);
		glBegin(GL_QUADS);
		{
			// Front
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3d(-0.5, 1.0, 0.5);
			glVertex3d(-0.5, 0.0, 0.5);
			glVertex3d(0.5, 0.0, 0.5);
			glVertex3d(0.5, 1.0, 0.5);

			{
				// Right
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3d(0.5, 1.0, 0.5);
				glVertex3d(0.5, 0.0, 0.5);
				glVertex3d(0.5, 0.0, -0.5);
				glVertex3d(0.5, 1.0, -0.5);

				// Back
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3d(0.5, 1.0, -0.5);
				glVertex3d(0.5, 0.0, -0.5);
				glVertex3d(-0.5, 0.0, -0.5);
				glVertex3d(-0.5, 1.0, -0.5);

				// Left
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3d(-0.5, 1.0, 0.5);
				glVertex3d(-0.5, 1.0, -0.5);
				glVertex3d(-0.5, 0.0, -0.5);
				glVertex3d(-0.5, 0.0, 0.5);

				// Top
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3d(-0.5, 1.0, 0.5);
				glVertex3d(0.5, 1.0, 0.5);
				glVertex3d(0.5, 1.0, -0.5);
				glVertex3d(-0.5, 1.0, -0.5);
			}			

			// Bottom
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3d(-0.5, 0.0, 0.5);
			glVertex3d(-0.5, 0.0, -0.5);
			glVertex3d(0.5, 0.0, -0.5);
			glVertex3d(0.5, 0.0, 0.5);
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
}

void DrawSphere(double rad, double r, double g, double b)
{
	static GLUquadric* pObj = gluNewQuadric();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColor3d(r, g, b);
	gluSphere(pObj, rad, 20, 20);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
}

