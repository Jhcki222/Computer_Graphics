#include "imgui_manager.h"
#include "imgui_graphics.h"

void ShowTestWindow(bool* p_open);

int main(int, char**)
{
	// GLFW �����츦 �����ϰ�, ImGui�� �ʱ�ȭ �Ѵ�.
    if (!ImGuiManager::Instance().Init())
        return 0;

	// GLEW�� �ʱ�ȭ �Ѵ�.
	glewInit();

    bool show_demo_window = true;	// ���� ������ ����ȭ ����
    bool show_test_window = true;	// �׽�Ʈ ������ ����ȭ ����

    // ���� ������ �����Ѵ�.
    while (!glfwWindowShouldClose(ImGuiManager::Instance().m_Window))
    {
        glfwPollEvents();
        ImGuiManager::Instance().Begin();
        {
			if (ImGui::BeginMainMenuBar()) // ���� �޴��� �����Ѵ�.
			{
				if (ImGui::BeginMenu("Test"))
				{
					if (ImGui::MenuItem("Show Demo Window", 0, show_demo_window, true))
						show_demo_window = !show_demo_window;
					
                    if (ImGui::MenuItem("Show Test Window", NULL, show_test_window, true))
                        show_test_window = !show_test_window;
					//ImGui::MenuItem("Show Test Window", NULL, &show_test_window, true);
                    
                    if (ImGui::MenuItem("Exit")) // ���α׷��� ���� �޴�
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
void DrawObject(double r, double g, double b);

void ShowTestWindow(bool* p_open)
{
	// ������ �÷���(window flag)�� �����Ѵ�.
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

	// �Ӽ� �����츦 �����ϰ�, collapsed�� ���¶�� �ٷ� �����Ѵ�.
	if (!ImGui::Begin(u8"ImGui ������", p_open, window_flags))
	{
		ImGui::End();
		return;
	}

	// ���⿡ ImGui�� �����ϼ���.
	ImGui::Text("Hello World~");
	ImGui::Text(u8"���⿡ ImGui �� �����ϼ���.");

	float Width = ImGui::GetContentRegionAvail().x;
	float Height = ImGui::GetContentRegionAvail().y;

	// ī�޶�� ������ ���� ��ü�� �����Ѵ�.
	static Camera MyCamera;
	static FrameBuffer MyScene;
	MyCamera.SetWindowSize((int)Width, (int)Height);
	MyScene.RescaleFrameBuffer((int)Width, (int)Height);

	int exp = 0;
	if (exp == 0)
	{
		// ������ ���ۿ� ��ü�� ������ �Ѵ�.
		MyScene.Bind();
		{
			glViewport(0, 0, (int)Width, (int)Height);
			glClearColor(0.5, 0.5, 0.5, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glEnable(GL_DEPTH_TEST);

			// ���콺 �Է��� �޾� ���� ������ ���� ��ȯ�� �����Ѵ�.
			MyCamera.ProcessMouseEvents();
			MyCamera.SetupViewFrustum();
			MyCamera.SetupViewTransform();

			// ��ǥ ����� ������ �Ѵ�.
			RenderFloor();

			DrawObject(1.0, 1.0, 0.0); // {0}-��ǥ��
			glPushMatrix(); // {0}-��ǥ�� 
			{
				glTranslatef(2.0f, 3.0f, 0.0f);
				DrawObject(1.0, 0.0, 0.0); // {1}-��ǥ��

				glPushMatrix(); // {1}-��ǥ��
				{
					glTranslatef(-4.0, -2.0, 0.0);
					glRotatef(45.0, 0.0, 0.0, 1.0);
					DrawObject(0.0, 1.0, 0.0);  // {2}-��ǥ��
				}
				glPopMatrix();  // {1}-��ǥ�� ����

				glTranslatef(1.0, -4.0, 0.0);
				glRotatef(-90.0, 0.0, 0.0, 1.0);
				DrawObject(0.0, 1.0, 1.0); // {3}-��ǥ��
			}
			glPopMatrix(); // {0}-��ǥ�� ����
		}
		MyScene.Unbind();
	}
	if (exp == 1)
	{
		// ������ ���ۿ� ��ü�� ������ �Ѵ�.
		MyScene.Bind();
		{
			glViewport(0, 0, (int)Width, (int)Height);
			glClearColor(0.2, 0.2, 0.2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glEnable(GL_DEPTH_TEST);

			// ���콺 �Է��� �޾� ���� ������ ���� ��ȯ�� �����Ѵ�.
			MyCamera.ProcessMouseEvents();
			MyCamera.SetupViewFrustum();
			MyCamera.SetupViewTransform();

			// ��ǥ ����� ������ �Ѵ�.
			RenderFloor();

			static float angle1 = 0.0, timer1 = 0.0;
			static float angle2 = 0.0, timer2 = 0.0;

			glPushMatrix();
			{
				glTranslated(-2.0, 0.0, 0.0);
				glRotated(angle1, 0.0, 0.0, 1.0);
				DrawObject(0.0, 1.0, 0.0);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslated(2.0, 0.0, 0.0);
				glRotated(angle2, 0.0, 0.0, 1.0);
				DrawObject(0.0, 1.0, 1.0);
			}
			glPopMatrix();
						
			if (timer1 > 1.0)
			{
				angle1 += 5.0;
				timer1 = 0.0f;
			}
			if (timer2 > 1.0)
			{
				angle2 -= 5.0;
				timer2 = 0.0f;
			}
			timer1 += ImGui::GetIO().DeltaTime; // ��� �ð��� ����
			timer2 += ImGui::GetIO().DeltaTime;	// ��� �ð��� ����
		}
		MyScene.Unbind();
	}		

	// ������ ���ۿ� �������� �̹����� �ؽ�ó�� ���ͼ� �̹����� ����Ѵ�.
	uint64_t MySceneTexId = MyScene.GetFrameTexture();
	ImGui::Image(reinterpret_cast<void*>(MySceneTexId), ImVec2(Width, Height), ImVec2(0, 1), ImVec2(1, 0));
	
	ImGui::End();
}

void RenderFloor()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	for (float x = -10.0; x <= 10.0; x += 1.0)
	{
		glBegin(GL_LINES);
		glVertex3f(x, -10.0f, 0.0f);
		glVertex3f(x, 10.0f, 0.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-10.0f, x, 0);
		glVertex3f(10.0f, x, 0);
		glEnd();
	}

	glLineWidth(5.0f);
	// x ��
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(-10.0, 0.0, 0.0);
	glVertex3d(10.0, 0.0, 0.0);
	glEnd();

	// y ��
	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, -10.0, 0.0);
	glVertex3d(0.0, 10.0, 0.0);
	glEnd();

	glLineWidth(1.0f);
}

void DrawObject(double r, double g, double b)
{
	glLineWidth(5.0f);
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	{
		glVertex3d(0.0, 1.0, 0.0);
		glVertex3d(-1.0, 0.0, 0.0);

		glVertex3d(-1.0, 0.0, 0.0);
		glVertex3d(0.5, 0.0, 0.0);

		glVertex3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, -0.7, 0.0);
	}
	glEnd();
	glLineWidth(1.0f);
}

