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
	if (bRenderTexture)
		RenderTexture();
	else if (bRenderPolygon)
		RenderPolygon();

	if (bRenderWire)
		RenderWire();

	if (bRenderVertex)
		RenderVertex();

}

void DgMesh::RenderVertex()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	int fnum = (int)Faces.size();
	glPointSize(3.0);	
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < fnum; ++i)
	{
		int vidx0 = Faces[i].vIdx[0];
		int vidx1 = Faces[i].vIdx[1];
		int vidx2 = Faces[i].vIdx[2];
		glVertex3dv(Verts[vidx0].Pos);
		glVertex3dv(Verts[vidx1].Pos);
		glVertex3dv(Verts[vidx2].Pos);
	}
	glEnd();
	glPointSize(1.0);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
}

void DgMesh::RenderPolygon()
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// ������ �ﰢ�� ����
	int fnum = (int)Faces.size();
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < fnum; ++i)
		{
			// ����, �ؼ�, �븻, ������ �ε����� ���Ͽ�.
			int vidx0 = Faces[i].vIdx[0];
			int vidx1 = Faces[i].vIdx[1];
			int vidx2 = Faces[i].vIdx[2];
			int nidx0 = Faces[i].nIdx[0];
			int nidx1 = Faces[i].nIdx[1];
			int nidx2 = Faces[i].nIdx[2];

			glNormal3dv(Normals[nidx0].Dir);
			glVertex3dv(Verts[vidx0].Pos);

			glNormal3dv(Normals[nidx1].Dir);
			glVertex3dv(Verts[vidx1].Pos);

			glNormal3dv(Normals[nidx2].Dir);
			glVertex3dv(Verts[vidx2].Pos);
		}
	}
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void DgMesh::RenderWire()
{
	glDisable(GL_LIGHTING);
	glColor3d(0.0, 0.0, 0.0);
	int fnum = (int)Faces.size();
	for (int i = 0; i < fnum; ++i)
	{
		// ����, �ؼ�, �븻, ������ �ε����� ���Ͽ�.
		int vidx0 = Faces[i].vIdx[0];
		int vidx1 = Faces[i].vIdx[1];
		int vidx2 = Faces[i].vIdx[2];
		glBegin(GL_LINE_LOOP);
		{
			glVertex3dv(Verts[vidx0].Pos);
			glVertex3dv(Verts[vidx1].Pos);
			glVertex3dv(Verts[vidx2].Pos);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void DgMesh::RenderTexture()
{
	if (Texels.empty())
	{
		RenderPolygon();
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	int prev_mtl_idx = -1;

	// ������ �ﰢ�� ����
	int fnum = (int)Faces.size();
	for (int i = 0; i < fnum; ++i)
	{
		// ����, �ؼ�, �븻, ������ �ε����� ���Ͽ�.
		int vidx0 = Faces[i].vIdx[0];
		int vidx1 = Faces[i].vIdx[1];
		int vidx2 = Faces[i].vIdx[2];
		int tidx0 = Faces[i].tIdx[0];
		int tidx1 = Faces[i].tIdx[1];
		int tidx2 = Faces[i].tIdx[2];
		int nidx0 = Faces[i].nIdx[0];
		int nidx1 = Faces[i].nIdx[1];
		int nidx2 = Faces[i].nIdx[2];
		int mtl_idx = Faces[i].mIdx;		

		// �ﰢ���� ������ �ְ�, ���� �ٲ���ٸ�
		if (mtl_idx != -1 && mtl_idx != prev_mtl_idx)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Mtls[mtl_idx].Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Mtls[mtl_idx].Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mtls[mtl_idx].Ks);			
			if (Mtls[mtl_idx].TexId != 0)
				glBindTexture(GL_TEXTURE_2D, Mtls[mtl_idx].TexId);
			prev_mtl_idx = mtl_idx;
		}

		// �ؽ�ó�� �����ϰ�, �ﰢ���� ������ �Ѵ�.				
		glBegin(GL_TRIANGLES);
		{
			glTexCoord2d(Texels[tidx0].ST[0], Texels[tidx0].ST[1]);
			glNormal3dv(Normals[nidx0].Dir);
			glVertex3dv(Verts[vidx0].Pos);

			glTexCoord2d(Texels[tidx1].ST[0], Texels[tidx1].ST[1]);
			glNormal3dv(Normals[nidx1].Dir);
			glVertex3dv(Verts[vidx1].Pos);

			glTexCoord2d(Texels[tidx2].ST[0], Texels[tidx2].ST[1]);
			glNormal3dv(Normals[nidx2].Dir);
			glVertex3dv(Verts[vidx2].Pos);
		}
		glEnd();
	}

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
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

/*!
*	\brief ������ ���丮 ��θ��� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ���丮 ��θ��� ��ȯ�Ѵ�.
*/
std::string get_file_path(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx = (int)full_path.rfind("\\");
	if (idx == -1)
		idx = (int)full_path.rfind("/");
	ret = full_path.substr(0, idx);

	return ret;
}

/*!
*	\brief ��θ��� ������ ���ϸ�(Ȯ��� ������)�� ��ȯ�Ѵ�.
*
*	\param full_path ��θ��� ������ ������ �̸�
*
*	\return \a full_path���� ��θ��� ������ ���ϸ�(Ȯ��� ������)�� ��ȯ�Ѵ�.
*/
std::string get_file_name(std::string full_path)
{
	std::string ret;
	if (full_path.empty())
		return ret;

	int idx0 = (int)full_path.rfind("\\");
	if (idx0 == -1)
		idx0 = (int)full_path.rfind("/");
	int idx1 = (int)full_path.rfind(".");
	ret = full_path.substr(idx0 + 1, idx1 - idx0 - 1);

	return ret;
}

/*!
*	\brief OBJ ���Ͽ��� ����ϴ� MTL(����)������ �о� �޽��� �߰��Ѵ�.
*
*	\param pMesh ������ �߰��� �޽��� ������
*	\param fname ���� ������ �̸�(���� �۾� ��ο��� ������� ��η� ǥ����)
*
*	\return ����: true, ����: flase
*/
bool import_obj_mtl(DgMesh* pMesh, const char* fname)
{
	// ������ ���� ������ �����Ѵ�.
	DgMaterial* pMtl = NULL;

	// ������ ����, 
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // �����ϸ� false�� ��ȯ�Ѵ�.
	{
		printf("\t���� ���� �ϱ� ����...\n");
		return false;
	}

	// ������ ������ �� �ܾ �о�, tag �迭�� �����Ѵ�.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) �±׶��,
		if (!strcmp(tag, "newmtl"))
		{
			// ������ �̸��� �а�,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// ������ �����Ͽ�, �޽��� ���� ����Ʈ�� �߰��Ѵ�.
			DgMaterial mtl;
			mtl.Name = std::string(tmp);
			pMesh->Mtls.push_back(mtl);
		}

		// Ka (ambient coefficients) �±׶��,
		if (!strcmp(tag, "Ka"))
		{
			// ambient ������ �о ������ �����Ѵ�.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Ka[0], &pMesh->Mtls[midx].Ka[1], &pMesh->Mtls[midx].Ka[2]);
		}

		// Kd (diffuse coefficients) �±׶��,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse ������ �о ������ �����Ѵ�.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Kd[0], &pMesh->Mtls[midx].Kd[1], &pMesh->Mtls[midx].Kd[2]);
		}

		// Ks (specular coefficients) �±׶��,
		if (!strcmp(tag, "Ks"))
		{
			// specular ������ �о ������ �����Ѵ�.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Ks[0], &pMesh->Mtls[midx].Ks[1], &pMesh->Mtls[midx].Ks[2]);
		}

		// map_Kd (diffuse texture file) �±׶��,
		if (!strcmp(tag, "map_Kd"))
		{
			// �ؽ�ó ������ �д´�.
			char tex_name[512];
			fscanf_s(fp, "%s", tex_name, 512);
			int midx = (int)pMesh->Mtls.size() - 1;

			ilInit();
			iluInit();
			ilutRenderer(ILUT_OPENGL);
			glEnable(GL_TEXTURE_2D);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// GL_CLAMP, GL_REPEAT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// GL_CLAMP, GL_REPEAT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_LINEAR, GL_NEAREST
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// GL_LINEAR, GL_NEAREST
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		// GL_MODULATE, GL_REPLACE

			// ILUT ���̺귯���� �̿��Ͽ� ������ ������ OPENGL �ؽ�ó ���·� �ε��Ѵ�.
			pMesh->Mtls[midx].TexId = ilutGLLoadImage((wchar_t*)tex_name);
			glDisable(GL_TEXTURE_2D);

			if (pMesh->Mtls[midx].TexId < 1)
				printf("\t�ؽ�ó �ε� ����...\n");
		}
	}

	// ������ �ݴ´�.
	fclose(fp);
	return true;
}

/*!
*	\brief	OBJ ������ �д´�.
*
*	\param	fname[in]	�ε��� ������ �̸�
*
*	\return	OBJ ������ �о� �޽� ��ü�� ��ȯ�Ѵ�.
*/
DgMesh* import_mesh_obj(const char* fname)
{
	// ������ ���� �����ϸ� NULL�� ��ȯ�Ѵ�.
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp)
	{
		printf("���� ����Ʈ ����...\n");
		return NULL;
	}

	// ���� �۾� ���͸��� �����Ѵ�.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);

	// �� �˻� ���͸��� �̵��ϰ�, ���� ��η� ����Ѵ�.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\t�� ���͸� = %s\n", msd);

	// ������ �̸�(��� �� Ȯ���� ����)�� ���Ͽ� �޽��� �����Ѵ�.
	std::string Name = get_file_name(fname);
	DgMesh* pMesh = new DgMesh();
	pMesh->MeshName = Name;
	printf("\t�޽� �̸� = %s\n", Name.c_str());

	// �ε��� �ʿ��� �ӽ� �������� �����Ѵ�.
	char tag[256];
	bool bTexture = false;
	bool bNormal = false;
	int MtlIdx = -1;

	// ������ ������ �� �ܾ �����鼭 tag �迭�� �����Ѵ�.
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// mtllib (material library) �±׶��,
		if (!strcmp(tag, "mtllib"))
		{
			// ���� ���ϸ�(�ַ� ��� ���)�� ���Ͽ�
			char mtl_fname[128];
			fscanf_s(fp, "%s", mtl_fname, 128);

			// ���� ������ �д´�. 
			import_obj_mtl(pMesh, mtl_fname);
		}

		// v (vertex) �±׶��,
		if (!strcmp(tag, "v"))
		{
			// ��ǥ(x, y, z)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			DgVertex v;
			fscanf_s(fp, "%lf%lf%lf", &v.Pos[0], &v.Pos[1], &v.Pos[2]);
			pMesh->Verts.push_back(v);
		}

		// vt (vertex texture) �±׶��,
		if (!strcmp(tag, "vt"))
		{
			// �ؽ�ó ��ǥ(u, v)�� �о� �ؼ��� �����ϰ� �޽��� �߰��Ѵ�.
			bTexture = true;
			DgTexel t;
			fscanf_s(fp, "%lf%lf", &t.ST[0], &t.ST[1]);
			pMesh->Texels.push_back(t);
		}

		// vn (vertex normal) �±׶��,
		if (!strcmp(tag, "vn"))
		{
			// ������ ����(nx, ny, nz)�� �о� ������ �����ϰ� �޽��� �߰��Ѵ�.
			bNormal = true;
			DgNormal n;
			fscanf_s(fp, "%lf%lf%lf", &n.Dir[0], &n.Dir[1], &n.Dir[2]);
			normalize(n.Dir);
			pMesh->Normals.push_back(n);
		}

		// usemtl (use material) �±׶��,
		if (!strcmp(tag, "usemtl"))
		{
			// �޽��� ���� ����Ʈ���� �ش� ������ ã�´�.
			char mtl_name[256];
			fscanf_s(fp, "%s", mtl_name, 256);
			MtlIdx = -1;
			for (int i = 0; i < (int)pMesh->Mtls.size(); ++i)
			{
				if (pMesh->Mtls[i].Name == std::string(mtl_name))
				{
					MtlIdx = i;
					break;
				}
			}
		}

		// f (face) �±׶��,
		if (!strcmp(tag, "f"))
		{
			// �������� �����ϴ� ����, �ؼ�, �븻�� �ε����� �����ϴ� ������ �����ϰ�,
			std::vector<int> vIdxs, tIdxs, nIdxs;

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
				int vidx, tidx, nidx;

				// ����, �ؼ�, �븻 �ε����� ��� �ִ� ��� (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// ��� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ؼ� �ε����� �ִ� ��� (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// ������ �ؼ� �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// ������ �븻 �ε����� �ִ� ��� (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// ������ �븻 �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// ������ �ε����� �ִ� ��� (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// ������ �ε����� �о� ����Ʈ�� �߰��Ѵ�.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// �������� �ﰢ������ ������ �޽��� �߰��Ѵ�.
			int NumVert = (int)vIdxs.size(); // �ﰢ��(NumVert = 3), �簢��(NumVert = 4), ...
			for (int i = 0; i < NumVert - 2; ++i)
			{
				// �ﰢ���� ������ �ε����� �����ϰ�,
				DgFace f;
				f.vIdx[0] = vIdxs[0];
				f.vIdx[1] = vIdxs[i + 1];
				f.vIdx[2] = vIdxs[i + 2];
				f.mIdx = MtlIdx;

				// �� ������ �ؼ� ������ ���ϰ�,
				if (bTexture)
				{
					f.tIdx[0] = tIdxs[0];
					f.tIdx[1] = tIdxs[i + 1];
					f.tIdx[2] = tIdxs[i + 2];
				}

				// �� ������ ���� ������ ���ϰ�,
				if (bNormal)
				{
					f.nIdx[0] = nIdxs[0];
					f.nIdx[1] = nIdxs[i + 1];
					f.nIdx[2] = nIdxs[i + 2];
				}
				else // ���� ������ ���ٸ� �ﰢ�� �������� ��ü�Ѵ�.
				{
					// �ﰢ���� �븻���͸� ����ϰ�,
					double e1[3], e2[3];
					double* p0 = pMesh->Verts[vIdxs[0]].Pos;
					double* p1 = pMesh->Verts[vIdxs[i + 1]].Pos;
					double* p2 = pMesh->Verts[vIdxs[i + 2]].Pos;
					sub(e1, p1, p0);
					sub(e2, p2, p0);

					DgNormal n;
					cross_product(n.Dir, e1, e2);
					normalize(n.Dir);
					pMesh->Normals.push_back(n);

					f.nIdx[0] = (int)pMesh->Normals.size() - 1;
					f.nIdx[1] = (int)pMesh->Normals.size() - 1;
					f.nIdx[2] = (int)pMesh->Normals.size() - 1;
				}

				// �ﰢ���� �����Ͽ� �޽��� �߰��Ѵ�.
				pMesh->Faces.push_back(f);
			}
		}
	}

	// ������ �ݴ´�.
	fclose(fp);

	// �ؽ�ó ������ ���� ���̵� Ÿ���� �����Ѵ�.
	if (bTexture)
		pMesh->bRenderTexture = true;

	// �� �˻� ���͸����� ���� �۾� ���͸��� �����Ѵ�.
	_chdir(cwd);

	// ����Ʈ�� �޽��� �����͸� ��ȯ�Ѵ�.
	printf("OBJ ���� ����Ʈ ����...\n");
	return pMesh;
}


