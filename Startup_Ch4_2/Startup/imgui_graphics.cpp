#include "imgui_manager.h"

FrameBuffer::FrameBuffer(int width, int height)
{
	m_Width = width;
	m_Height = height;

	// 프레임 버퍼의 핸들을 생성하고 바인딩한다.
	glGenFramebuffers(1, &m_FrameBufObj);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufObj);

	// 텍스처 핸들을 생성하여 바인딩한다.
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	// 렌더링 버퍼의 핸들을 생성하고 바인딩한다.
	glGenRenderbuffers(1, &m_RenderBufObj);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufObj);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufObj);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// 버퍼를 해제한다.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	// 생성된 버퍼를 지운다.
	glDeleteFramebuffers(1, &m_FrameBufObj);
	glDeleteTextures(1, &m_Texture);
	glDeleteRenderbuffers(1, &m_RenderBufObj);
}

unsigned int FrameBuffer::GetFrameTexture()
{
	return m_Texture;
}

void FrameBuffer::RescaleFrameBuffer(int width, int height)
{
	m_Width = width;
	m_Height = height;
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufObj);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufObj);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufObj);
}


void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_Zoom = -20.0;
	m_Angle = 0.0;
	m_Axis[0] = 0.0f; m_Axis[1] = 0.0f; m_Axis[2] = 0.0f;
	m_Pan[0] = m_Pan[1] = m_Pan[2] = 0.0f;
	m_Height = m_Width = 0;
	m_StartPt[0] = m_StartPt[1] = 0;
	m_RotMat[0] = 1.0;	m_RotMat[4] = 0.0;	m_RotMat[8] = 0.0;	m_RotMat[12] = 0.0;
	m_RotMat[1] = 0.0;	m_RotMat[5] = 1.0;	m_RotMat[9] = 0.0;	m_RotMat[13] = 0.0;
	m_RotMat[2] = 0.0;	m_RotMat[6] = 0.0;	m_RotMat[10] = 1.0;	m_RotMat[14] = 0.0;
	m_RotMat[3] = 0.0;	m_RotMat[7] = 0.0;	m_RotMat[11] = 0.0; m_RotMat[15] = 1.0;
	m_pObj = gluNewQuadric();
}


Camera::~Camera()
{
	gluDeleteQuadric(m_pObj);
}


void Camera::Reset()
{
	m_Zoom = -20.0;
	m_Angle = 0.0;
	m_Axis[0] = 0.0f; m_Axis[1] = 0.0f; m_Axis[2] = 0.0f;
	m_StartPt[0] = m_StartPt[1] = 0;
	m_RotMat[0] = 1.0;	m_RotMat[4] = 0.0;	m_RotMat[8] = 0.0;	m_RotMat[12] = 0.0;
	m_RotMat[1] = 0.0;	m_RotMat[5] = 1.0;	m_RotMat[9] = 0.0;	m_RotMat[13] = 0.0;
	m_RotMat[2] = 0.0;	m_RotMat[6] = 0.0;	m_RotMat[10] = 1.0;	m_RotMat[14] = 0.0;
	m_RotMat[3] = 0.0;	m_RotMat[7] = 0.0;	m_RotMat[11] = 0.0; m_RotMat[15] = 1.0;
}


void Camera::SetWindowSize(int width, int height)
{
	m_Width = width;
	m_Height = height;
}


void Camera::ProcessMouseEvents()
{
	// 현재 윈도우의 좌측 상단을 기준(0, 0)으로 마우스 좌표를 구한다.
	ImVec2 pos = ImGui::GetMousePos() - ImGui::GetCursorScreenPos();
	int x = (int)pos.x;
	int y = (int)pos.y;
	//printf("(x, y) = (%d, %d)\n", x, y);
	if (x < 0.0 || y < 0.0 || x > m_Width || y > m_Height)
		return;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))			// 왼쪽 버튼을 클릭한 경우
	{
		m_StartPt[0] = x;
		m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))		// 왼쪽 버튼으로 드래깅하는 경우
	{
		// 단위 구 위의 좌표 계산
		float px, py, pz, qx, qy, qz;
		GetSphereCoord(m_StartPt[0], m_StartPt[1], &px, &py, &pz);
		GetSphereCoord(x, y, &qx, &qy, &qz);

		// 회전 축과 각도 계산
		m_Axis[0] = py * qz - pz * qy;
		m_Axis[1] = pz * qx - px * qz;
		m_Axis[2] = px * qy - py * qx;
		m_Angle = 0.0;
		float len = m_Axis[0] * m_Axis[0] + m_Axis[1] * m_Axis[1] + m_Axis[2] * m_Axis[2];
		if (len > 0.0001) // 일정 변위 이상만 처리
			m_Angle = (float)acos(px * qx + py * qy + pz * qz) * 180.0f / 3.141592f;

		m_StartPt[0] = x;	// Update startpt as current position
		m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))		// 클릭했던 왼쪽 버튼을 놓는 경우
	{
		m_StartPt[0] = m_StartPt[1] = 0;
		m_Angle = 0.0;
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))	// 중간 버튼을 클릭한 경우
	{
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))	// 중간 버튼으로 드래깅하는 경우
	{
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))	// 클릭했던 중간 버튼을 놓은 경우
	{
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))		// 오른쪽 버튼을 클릭한 경우
	{
		m_StartPt[0] = x;
		m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))	// 오른쪽 버튼으로 드래깅하는 경우 
	{
		float dx = (float)(x - m_StartPt[0]) * 0.01f;
		float dy = (float)(m_StartPt[1] - y) * 0.01f;
		// 회전 행렬 및 역행렬
		// R = 0 4 8   invR = 0 1 2
		//     1 5 9          4 5 6    
		//     2 6 10         8 9 10
		// invR * (dx, dy, 0)
		m_Pan[0] += m_RotMat[0] * dx + m_RotMat[1] * dy;
		m_Pan[1] += m_RotMat[4] * dx + m_RotMat[5] * dy;
		m_Pan[2] += m_RotMat[8] * dx + m_RotMat[9] * dy;
		m_StartPt[0] = x;	// Update startpt as current position
		m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))	// 클릭했던 오른쪽 버튼을 놓은 경우
	{
		m_StartPt[0] = m_StartPt[1] = 0;
	}
	else														// 그냥 움직이는 경우
	{
		//printf("Mouse moves...\n");
	}

	// 장면의 줌인/아웃을 수행한다.
	if (ImGui::GetIO().MouseWheel != 0.0f)
	{
		int dir = (ImGui::GetIO().MouseWheel > 0.0) ? 1 : -1;
		m_Zoom += dir;
	}
}


void Camera::SetupViewFrustum()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)m_Width / (double)m_Height, 1.0, 10000.0);
}


void Camera::SetupViewTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 줌 인/아웃을 위한 변환, M = I * T_zoom
	glTranslatef(0.0, 0.0, m_Zoom);

	// 새로운 회전을 적용, M = I * T_zoom * R_new
	glRotated(m_Angle, m_Axis[0], m_Axis[1], m_Axis[2]);

	// 기존 회전을 적용, M = I * T_zoom * R_new * R_old	//   R_n .... * R3 * R2 * R1
	glMultMatrixf(m_RotMat);

	// 회전 행렬 추출, R_old = R_new * R_old
	glGetFloatv(GL_MODELVIEW_MATRIX, m_RotMat);
	m_RotMat[12] = m_RotMat[13] = m_RotMat[14] = 0.0;

	// 이동 변환을 적용, M = I * T_zoom * R_new * R_old * T_pan
	glTranslatef(m_Pan[0], m_Pan[1], m_Pan[2]);
}


void Camera::GetSphereCoord(int x, int y, float* px, float* py, float* pz)
{
	*px = (2.0f * x - m_Width) / m_Width;
	*py = (-2.0f * y + m_Height) / m_Height;

	float r = (*px) * (*px) + (*py) * (*py); // 원점으로부터의 거리 계산
	if (r >= 1.0f)
	{
		*px = *px / sqrt(r);
		*py = *py / sqrt(r);
		*pz = 0.0f;
	}
	else
		*pz = sqrt(1.0f - r);  // 일반적인 경우
}

