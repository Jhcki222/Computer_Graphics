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
}

Camera::~Camera()
{
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
	// 현재 윈도우의 좌측 상단을 기준(0, 0)으로 마우스 좌표(x, y)를 구한다.
	ImVec2 pos = ImGui::GetMousePos() - ImGui::GetCursorScreenPos();
	int x = (int)pos.x;	int y = (int)pos.y;
	if (x < 0 || y < 0 || x > m_Width || y > m_Height)	return;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))			// 왼쪽 버튼을 클릭한 경우
	{
		m_StartPt[0] = x; m_StartPt[1] = y;		//  시작점 기록
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))		// 왼쪽 버튼으로 드래깅하는 경우
	{
		// 단위 구 위의 좌표 계산
		float px, py, pz, qx, qy, qz;
		GetSphereCoord(m_StartPt[0], m_StartPt[1], &px, &py, &pz);
		GetSphereCoord(x, y, &qx, &qy, &qz);

		// 회전 축과 각도 계산
		m_Axis[0] = py * qz - pz * qy;	m_Axis[1] = pz * qx - px * qz;	m_Axis[2] = px * qy - py * qx;
		m_Angle = 0.0;
		float len = m_Axis[0] * m_Axis[0] + m_Axis[1] * m_Axis[1] + m_Axis[2] * m_Axis[2];
		if (len > 0.0001) // 일정 변위 이상만 처리
			m_Angle = (float)acos(px * qx + py * qy + pz * qz) * 180.0f / 3.141592f;

		// 시작점을 현재점으로 갱신
		m_StartPt[0] = x; m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))		// 클릭했던 왼쪽 버튼을 놓는 경우
	{
		m_StartPt[0] = m_StartPt[1] = 0; 
		m_Angle = 0.0;
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))		// 오른쪽 버튼을 클릭한 경우
	{
		m_StartPt[0] = x; m_StartPt[1] = y;		//  시작점 기록
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))	// 오른쪽 버튼으로 드래깅하는 경우 
	{
		float dx = (float)(x - m_StartPt[0]) * 0.01f;
		float dy = (float)(m_StartPt[1] - y) * 0.01f;
		// 장면 이동 변위 = 현재 회전 행렬의 역행렬 * (dx, dy, 0)
		m_Pan[0] += m_RotMat[0] * dx + m_RotMat[1] * dy;
		m_Pan[1] += m_RotMat[4] * dx + m_RotMat[5] * dy;
		m_Pan[2] += m_RotMat[8] * dx + m_RotMat[9] * dy;
		m_StartPt[0] = x;	m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))	// 클릭했던 오른쪽 버튼을 놓은 경우
	{
		m_StartPt[0] = m_StartPt[1] = 0;
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

void DgMesh::Render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColor3fv(m_Color);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_Faces.size(); ++i)
	{
		int* vidx = m_Faces[i].m_VertIdxs;
		int* nidx = m_Faces[i].m_NormalIdxs;

		glNormal3dv(m_Normals[nidx[0]].m_Dir);
		glVertex3dv(m_Verts[vidx[0]].m_Pos);

		glNormal3dv(m_Normals[nidx[1]].m_Dir);
		glVertex3dv(m_Verts[vidx[1]].m_Pos);

		glNormal3dv(m_Normals[nidx[2]].m_Dir);
		glVertex3dv(m_Verts[vidx[2]].m_Pos);
	}
	glEnd();
	glDisable(GL_LIGHTING);
}

void sub(double c[3], double a[3], double b[3])
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

void cross_product(double c[3], double a[3], double b[3])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double n[3])
{
	double len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0] /= len;
	n[1] /= len;
	n[2] /= len;
}

DgMesh* import_mesh_obj(const char* fname)
{
	// 파일을 열고 실패하면 false를 반환한다.
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp) return NULL;

	// 파일의 이름(경로 및 확장자 제외)을 구하여 메쉬를 생성한다.
	DgMesh* pMesh = new DgMesh();

	// 로딩에 필요한 임시 변수들을 선언한다.
	char tag[256];
	bool bNormal = false;

	// 파일의 끝까지 한 단어씩 읽으면서 tag 배열에 저장한다.
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// v (vertex) 태그라면,
		if (!strcmp(tag, "v"))
		{
			// 좌표(x, y, z)를 읽어 정점을 생성하고 메쉬에 추가한다.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			DgVertex vtx(x, y, z);
			pMesh->m_Verts.push_back(vtx);
		}

		// vn (vertex normal) 태그라면,
		if (!strcmp(tag, "vn"))
		{
			// 법선의 방향(nx, ny, nz)을 읽어 법선을 생성하고 메쉬에 추가한다.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			DgNormal normal(nx, ny, nz);
			pMesh->m_Normals.push_back(normal);
		}

		// f (face) 태그라면,
		if (!strcmp(tag, "f"))
		{
			// 폴리곤을 구성하는 정점, 노말의 인덱스를 저장하는 변수를 선언하고,
			std::vector<int> vIdxs, nIdxs;

			// 한 줄을 읽어서,
			char line[256];
			fgets(line, 256, fp);
			char* pData = line;

			// 한 줄을 모두 읽을 때까지
			while (true)
			{
				// 불필요한 공백과 줄빠꿈을 건너띄고,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// 한 줄을 모두 읽었다면 반복문을 빠져 나온다.
				if (!strcmp(pData, ""))
					break;

				// 인덱스 정보를 buffer로 읽은 후,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// 인덱스 정보를 저장할 변수를 선언하고
				char sep;
				int vidx, nidx;

				// 정점과 노말 인덱스가 있는 경우 (f 1//1 2//2 3//3 ...)
				if (bNormal)
				{
					// 정점과 노말 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
				}
				else // 정점만 있는 경우 (f 1 2 3 ...)
				{
					// 정점의 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d", &vidx);
					vIdxs.push_back(vidx - 1);
				}
			}

			if (!bNormal) // 법선 정보가 없는 경우
			{
				DgVertex v0 = pMesh->m_Verts[vIdxs[0]];
				DgVertex v1 = pMesh->m_Verts[vIdxs[1]];
				DgVertex v2 = pMesh->m_Verts[vIdxs[2]];
				double e0[3], e1[3], n[3];
				sub(e0, v1.m_Pos, v0.m_Pos);
				sub(e1, v2.m_Pos, v0.m_Pos);
				cross_product(n, e0, e1);
				normalize(n);
				DgNormal normal(n[0], n[1], n[2]);
				pMesh->m_Normals.push_back(normal);
				nIdxs.push_back(pMesh->m_Normals.size() - 1);
				nIdxs.push_back(pMesh->m_Normals.size() - 1);
				nIdxs.push_back(pMesh->m_Normals.size() - 1);
			}

			// 삼각형을 생성하여 메쉬에 추가한다.
			DgFace f(vIdxs[0], vIdxs[1], vIdxs[2], nIdxs[0], nIdxs[1], nIdxs[2]);
			pMesh->m_Faces.push_back(f);
		}
	}
	// 파일을 닫고, 생성된 메쉬 포인터를 반환한다.
	fclose(fp);
	return pMesh;
}

