#include "imgui_manager.h"

FrameBuffer::FrameBuffer(int width, int height)
{
	m_Width = width;
	m_Height = height;

	// ������ ������ �ڵ��� �����ϰ� ���ε��Ѵ�.
	glGenFramebuffers(1, &m_FrameBufObj);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufObj);

	// �ؽ�ó �ڵ��� �����Ͽ� ���ε��Ѵ�.
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	// ������ ������ �ڵ��� �����ϰ� ���ε��Ѵ�.
	glGenRenderbuffers(1, &m_RenderBufObj);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufObj);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufObj);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// ���۸� �����Ѵ�.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	// ������ ���۸� �����.
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
	// ���� �������� ���� ����� ����(0, 0)���� ���콺 ��ǥ(x, y)�� ���Ѵ�.
	ImVec2 pos = ImGui::GetMousePos() - ImGui::GetCursorScreenPos();
	int x = (int)pos.x;	int y = (int)pos.y;
	if (x < 0 || y < 0 || x > m_Width || y > m_Height)	return;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))			// ���� ��ư�� Ŭ���� ���
	{
		m_StartPt[0] = x; m_StartPt[1] = y;		//  ������ ���
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))		// ���� ��ư���� �巡���ϴ� ���
	{
		// ���� �� ���� ��ǥ ���
		float px, py, pz, qx, qy, qz;
		GetSphereCoord(m_StartPt[0], m_StartPt[1], &px, &py, &pz);
		GetSphereCoord(x, y, &qx, &qy, &qz);

		// ȸ�� ��� ���� ���
		m_Axis[0] = py * qz - pz * qy;	m_Axis[1] = pz * qx - px * qz;	m_Axis[2] = px * qy - py * qx;
		m_Angle = 0.0;
		float len = m_Axis[0] * m_Axis[0] + m_Axis[1] * m_Axis[1] + m_Axis[2] * m_Axis[2];
		if (len > 0.0001) // ���� ���� �̻� ó��
			m_Angle = (float)acos(px * qx + py * qy + pz * qz) * 180.0f / 3.141592f;

		// �������� ���������� ����
		m_StartPt[0] = x; m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))		// Ŭ���ߴ� ���� ��ư�� ���� ���
	{
		m_StartPt[0] = m_StartPt[1] = 0; 
		m_Angle = 0.0;
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))		// ������ ��ư�� Ŭ���� ���
	{
		m_StartPt[0] = x; m_StartPt[1] = y;		//  ������ ���
	}
	else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))	// ������ ��ư���� �巡���ϴ� ��� 
	{
		float dx = (float)(x - m_StartPt[0]) * 0.01f;
		float dy = (float)(m_StartPt[1] - y) * 0.01f;
		// ��� �̵� ���� = ���� ȸ�� ����� ����� * (dx, dy, 0)
		m_Pan[0] += m_RotMat[0] * dx + m_RotMat[1] * dy;
		m_Pan[1] += m_RotMat[4] * dx + m_RotMat[5] * dy;
		m_Pan[2] += m_RotMat[8] * dx + m_RotMat[9] * dy;
		m_StartPt[0] = x;	m_StartPt[1] = y;
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))	// Ŭ���ߴ� ������ ��ư�� ���� ���
	{
		m_StartPt[0] = m_StartPt[1] = 0;
	}
	// ����� ����/�ƿ��� �����Ѵ�.
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

	// �� ��/�ƿ��� ���� ��ȯ, M = I * T_zoom
	glTranslatef(0.0, 0.0, m_Zoom);

	// ���ο� ȸ���� ����, M = I * T_zoom * R_new
	glRotated(m_Angle, m_Axis[0], m_Axis[1], m_Axis[2]);

	// ���� ȸ���� ����, M = I * T_zoom * R_new * R_old	//   R_n .... * R3 * R2 * R1
	glMultMatrixf(m_RotMat);

	// ȸ�� ��� ����, R_old = R_new * R_old
	glGetFloatv(GL_MODELVIEW_MATRIX, m_RotMat);
	m_RotMat[12] = m_RotMat[13] = m_RotMat[14] = 0.0;

	// �̵� ��ȯ�� ����, M = I * T_zoom * R_new * R_old * T_pan
	glTranslatef(m_Pan[0], m_Pan[1], m_Pan[2]);
}

void Camera::GetSphereCoord(int x, int y, float* px, float* py, float* pz)
{
	*px = (2.0f * x - m_Width) / m_Width;
	*py = (-2.0f * y + m_Height) / m_Height;

	float r = (*px) * (*px) + (*py) * (*py); // �������κ����� �Ÿ� ���
	if (r >= 1.0f)
	{
		*px = *px / sqrt(r);
		*py = *py / sqrt(r);
		*pz = 0.0f;
	}
	else
		*pz = sqrt(1.0f - r);  // �Ϲ����� ���
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
	// ������ ���� �����ϸ� false�� ��ȯ�Ѵ�.
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp) return NULL;

	// ������ �̸�(��� �� Ȯ���� ����)�� ���Ͽ� �޽��� �����Ѵ�.
	DgMesh* pMesh = new DgMesh();

	// �ε��� �ʿ��� �ӽ� �������� �����Ѵ�.
	char tag[256];
	bool bNormal = false;

	// ������ ������ �� �ܾ �����鼭 tag �迭�� �����Ѵ�.
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// v (vertex) �±׶��,
		if (!strcmp(tag, "v"))
		{
			// ��ǥ(x, y, z)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			double x, y, z;
			fscanf_s(fp, "%lf%lf%lf", &x, &y, &z);
			DgVertex vtx(x, y, z);
			pMesh->m_Verts.push_back(vtx);
		}

		// vn (vertex normal) �±׶��,
		if (!strcmp(tag, "vn"))
		{
			// ������ ����(nx, ny, nz)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			bNormal = true;
			double nx, ny, nz;
			fscanf_s(fp, "%lf%lf%lf", &nx, &ny, &nz);
			DgNormal normal(nx, ny, nz);
			pMesh->m_Normals.push_back(normal);
		}

		// f (face) �±׶��,
		if (!strcmp(tag, "f"))
		{
			// �������� �����ϴ� ����, �븻�� �ε����� �����ϴ� ������ �����ϰ�,
			std::vector<int> vIdxs, nIdxs;

			// �� ���� �о,
			char line[256];
			fgets(line, 256, fp);
			char* pData = line;

			// �� ���� ��� ���� ������
			while (true)
			{
				// ���ʿ��� ����� �ٺ����� �ǳʶ��,
				while (*pData == ' ' || *pData == '\n')
					pData++;

				// �� ���� ��� �о��ٸ� �ݺ����� ���� ���´�.
				if (!strcmp(pData, ""))
					break;

				// �ε��� ������ buffer�� ���� ��,
				char buffer[256];	// buffer[] = "vidx/tidx/nidx" 
				sscanf_s(pData, "%s", buffer, 256);
				pData += strlen(buffer);

				// �ε��� ������ ������ ������ �����ϰ�
				char sep;
				int vidx, nidx;

				// ������ �븻 �ε����� �ִ� ��� (f 1//1 2//2 3//3 ...)
				if (bNormal)
				{
					// ������ �븻 �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
				}
				else // ������ �ִ� ��� (f 1 2 3 ...)
				{
					// ������ �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d", &vidx);
					vIdxs.push_back(vidx - 1);
				}
			}

			if (!bNormal) // ���� ������ ���� ���
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

			// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
			DgFace f(vIdxs[0], vIdxs[1], vIdxs[2], nIdxs[0], nIdxs[1], nIdxs[2]);
			pMesh->m_Faces.push_back(f);
		}
	}
	// ������ �ݰ�, ������ �޽� �����͸� ��ȯ�Ѵ�.
	fclose(fp);
	return pMesh;
}

