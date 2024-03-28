#include "imgui_manager.h"
#include "imgui_graphics.h"
#include <cmath>
#include <math.h>
#define M_PI 3.14159265358979323846
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

		switch (1)
		{
		case 1:
		{

			// �� �׸���
			glColor3f(0.0f, 0.0f, 1.0f);
			const int numSegments = 100;
			glBegin(GL_TRIANGLE_FAN);
			{
				glVertex2f(-6.0f, 0.0f); // ���� �߽�
				for (int i = 0; i <= numSegments; ++i) {
					float angle = i * 2.0f * M_PI / numSegments;
					float x = -6.0f + cosf(angle) * 2.0f; // ������ = 2.0
					float y = sinf(angle) * 2.0f;
					glVertex2f(x, y);
				}
			}
			glEnd();

			// Ÿ�� �׸���
			glColor3f(1.0f, 1.0f, 0.0f);
			const float ellipse_radius_x = 2.0f;
			const float ellipse_radius_y = 1.0f;
			glBegin(GL_TRIANGLE_FAN);
			{
				glVertex2f(6.0f, 0.0f); // Ÿ���� �߽�
				for (int i = 0; i <= numSegments; ++i) {
					float angle = i * 2.0f * M_PI / numSegments;
					float x = 6.0f + cosf(angle) * ellipse_radius_x;
					float y = sinf(angle) * ellipse_radius_y;
					glVertex2f(x, y);
				}
			}
			glEnd();
			// ��ä�� �׸���
			glColor3f(0.0f, 1.0f, 1.0f);
			glBegin(GL_TRIANGLE_FAN);
			{
				// ��ä���� �߽��� (0, 3)
				glVertex2f(0.0f, 3.0f);
				const float radius = 3.0f; // ��ä���� ������
				const int numSegments = 100;
				float startAngle = 0.0f; // ���� ����
				float endAngle = M_PI / 3.0f; // 60���� �ش��ϴ� ����

				// ���� �������� �� �������� ��ä���� �� ���� ����
				for (float angle = startAngle; angle <= endAngle; angle += (endAngle - startAngle) / numSegments) {
					float x = radius * cosf(angle); // x ��ǥ ���
					float y = radius * sinf(angle) + 3.0f; // y ��ǥ ��� (���� �߽��� (0, 3)�̹Ƿ� y ��ǥ�� 3�� ������)
					glVertex2f(x, y);
				}
			}
			glEnd();



			break;
			}
		case 2: 
		{
			// �� �׸���
			glColor3f(0.0f, 1.0f, 1.0f);
			glPointSize(10.0f); // ���� ũ�� ����
			glBegin(GL_POINTS);
			glVertex2f(-6.0f, 0.0f); // (x, y)
			glEnd();

			// �� �׸���
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex2f(-8.0f, 4.0f); 
			glVertex2f(8.0f, 4.0f);  
			glEnd();

			// �簢�� �׸���
			glColor3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_QUADS);
			glVertex2f(3.0f, -3.0f); 
			glVertex2f(6.0f, -3.0f);  
			glVertex2f(6.0f, 0.0f);   
			glVertex2f(3.0f, 0.0f);  
			glEnd();

			// �ﰢ�� �׸���
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLES);
			{
				glVertex2f(-3.0f, -3.0f);
				glVertex2f(3.0f, -3.0f);
				glVertex2f(0.0f, 2.0f);
			}
			glEnd();
			break;


		}
	}
}


	MyScene.Unbind();

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
		glVertex3f(x, 0.0, -10.0f);
		glVertex3f(x, 0.0, 10.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-10.0f, 0.0, x);
		glVertex3f(10.0f, 0.0, x);
		glEnd();
	}

	glLineWidth(5.0f);
	// x ��
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0);
	glEnd();

	// y ��
	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 1.0, 0.0);
	glEnd();

	// z ��
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 1.0);
	glEnd();
	glLineWidth(1.0f);
}

