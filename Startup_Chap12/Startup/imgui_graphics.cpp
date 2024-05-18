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

	// 각각의 삼각형 마다
	int fnum = (int)Faces.size();
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < fnum; ++i)
		{
			// 정점, 텍셀, 노말, 재질의 인덱스를 구하여.
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
		// 정점, 텍셀, 노말, 재질의 인덱스를 구하여.
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

	// 각각의 삼각형 마다
	int fnum = (int)Faces.size();
	for (int i = 0; i < fnum; ++i)
	{
		// 정점, 텍셀, 노말, 재질의 인덱스를 구하여.
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

		// 삼각형의 재질이 있고, 새로 바뀌었다면
		if (mtl_idx != -1 && mtl_idx != prev_mtl_idx)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Mtls[mtl_idx].Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Mtls[mtl_idx].Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mtls[mtl_idx].Ks);			
			if (Mtls[mtl_idx].TexId != 0)
				glBindTexture(GL_TEXTURE_2D, Mtls[mtl_idx].TexId);
			prev_mtl_idx = mtl_idx;
		}

		// 텍스처를 설정하고, 삼각형을 렌더링 한다.				
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
*	\brief 파일의 디렉토리 경로명을 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 디렉토리 경로명을 반환한다.
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
*	\brief 경로명을 제외한 파일명(확장명 불포함)을 반환한다.
*
*	\param full_path 경로명을 포함한 파일의 이름
*
*	\return \a full_path에서 경로명을 제외한 파일명(확장명 불포함)을 반환한다.
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
*	\brief OBJ 파일에서 사용하는 MTL(재질)정보를 읽어 메쉬에 추가한다.
*
*	\param pMesh 재질을 추가할 메쉬의 포인터
*	\param fname 재질 파일의 이름(현재 작업 경로에서 상대적인 경로로 표현됨)
*
*	\return 성공: true, 실패: flase
*/
bool import_obj_mtl(DgMesh* pMesh, const char* fname)
{
	// 생성할 재질 변수를 정의한다.
	DgMaterial* pMtl = NULL;

	// 파일을 열고, 
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp) // 실패하면 false를 반환한다.
	{
		printf("\t재질 파일 일기 실패...\n");
		return false;
	}

	// 파일의 끝까지 한 단어씩 읽어, tag 배열에 저장한다.
	char tag[256];
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// newmtl (new material) 태그라면,
		if (!strcmp(tag, "newmtl"))
		{
			// 재질의 이름을 읽고,
			char tmp[256];
			fscanf_s(fp, "%s", tmp, 256);

			// 재질을 생성하여, 메쉬의 재질 리스트에 추가한다.
			DgMaterial mtl;
			mtl.Name = std::string(tmp);
			pMesh->Mtls.push_back(mtl);
		}

		// Ka (ambient coefficients) 태그라면,
		if (!strcmp(tag, "Ka"))
		{
			// ambient 성분을 읽어서 재질을 설정한다.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Ka[0], &pMesh->Mtls[midx].Ka[1], &pMesh->Mtls[midx].Ka[2]);
		}

		// Kd (diffuse coefficients) 태그라면,
		if (!strcmp(tag, "Kd"))
		{
			// diffuse 성분을 읽어서 재질을 설정한다.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Kd[0], &pMesh->Mtls[midx].Kd[1], &pMesh->Mtls[midx].Kd[2]);
		}

		// Ks (specular coefficients) 태그라면,
		if (!strcmp(tag, "Ks"))
		{
			// specular 성분을 읽어서 재질을 설정한다.
			int midx = (int)pMesh->Mtls.size() - 1;
			fscanf_s(fp, "%f%f%f", &pMesh->Mtls[midx].Ks[0], &pMesh->Mtls[midx].Ks[1], &pMesh->Mtls[midx].Ks[2]);
		}

		// map_Kd (diffuse texture file) 태그라면,
		if (!strcmp(tag, "map_Kd"))
		{
			// 텍스처 파일을 읽는다.
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

			// ILUT 라이브러리를 이용하여 이지미 파일을 OPENGL 텍스처 형태로 로드한다.
			pMesh->Mtls[midx].TexId = ilutGLLoadImage((wchar_t*)tex_name);
			glDisable(GL_TEXTURE_2D);

			if (pMesh->Mtls[midx].TexId < 1)
				printf("\t텍스처 로딩 실패...\n");
		}
	}

	// 파일을 닫는다.
	fclose(fp);
	return true;
}

/*!
*	\brief	OBJ 파일을 읽는다.
*
*	\param	fname[in]	로딩할 파일의 이름
*
*	\return	OBJ 파일을 읽어 메쉬 객체를 반환한다.
*/
DgMesh* import_mesh_obj(const char* fname)
{
	// 파일을 열고 실패하면 NULL을 반환한다.
	FILE* fp;
	fopen_s(&fp, fname, "r");
	if (!fp)
	{
		printf("파일 임포트 실패...\n");
		return NULL;
	}

	// 현재 작업 디렉터리를 저장한다.
	char cwd[1024] = { 0 };
	_getcwd(cwd, 1024);

	// 모델 검색 디렉터리로 이동하고, 절대 경로로 출력한다.
	std::string model_dir = get_file_path(fname);
	_chdir(model_dir.c_str());
	char msd[1024] = { 0 };
	_getcwd(msd, 1024);
	printf("\t모델 디렉터리 = %s\n", msd);

	// 파일의 이름(경로 및 확장자 제외)을 구하여 메쉬를 생성한다.
	std::string Name = get_file_name(fname);
	DgMesh* pMesh = new DgMesh();
	pMesh->MeshName = Name;
	printf("\t메쉬 이름 = %s\n", Name.c_str());

	// 로딩에 필요한 임시 변수들을 선언한다.
	char tag[256];
	bool bTexture = false;
	bool bNormal = false;
	int MtlIdx = -1;

	// 파일의 끝까지 한 단어씩 읽으면서 tag 배열에 저장한다.
	while (fscanf_s(fp, "%s", tag, 256) != EOF)
	{
		// mtllib (material library) 태그라면,
		if (!strcmp(tag, "mtllib"))
		{
			// 재질 파일명(주로 상대 경로)을 구하여
			char mtl_fname[128];
			fscanf_s(fp, "%s", mtl_fname, 128);

			// 재질 파일을 읽는다. 
			import_obj_mtl(pMesh, mtl_fname);
		}

		// v (vertex) 태그라면,
		if (!strcmp(tag, "v"))
		{
			// 좌표(x, y, z)를 읽어 정점을 생성하고 메쉬에 추가한다.
			DgVertex v;
			fscanf_s(fp, "%lf%lf%lf", &v.Pos[0], &v.Pos[1], &v.Pos[2]);
			pMesh->Verts.push_back(v);
		}

		// vt (vertex texture) 태그라면,
		if (!strcmp(tag, "vt"))
		{
			// 텍스처 좌표(u, v)를 읽어 텍셀을 생성하고 메쉬에 추가한다.
			bTexture = true;
			DgTexel t;
			fscanf_s(fp, "%lf%lf", &t.ST[0], &t.ST[1]);
			pMesh->Texels.push_back(t);
		}

		// vn (vertex normal) 태그라면,
		if (!strcmp(tag, "vn"))
		{
			// 법선의 방향(nx, ny, nz)을 읽어 법선을 생성하고 메쉬에 추가한다.
			bNormal = true;
			DgNormal n;
			fscanf_s(fp, "%lf%lf%lf", &n.Dir[0], &n.Dir[1], &n.Dir[2]);
			normalize(n.Dir);
			pMesh->Normals.push_back(n);
		}

		// usemtl (use material) 태그라면,
		if (!strcmp(tag, "usemtl"))
		{
			// 메쉬의 재질 리스트에서 해당 재질을 찾는다.
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

		// f (face) 태그라면,
		if (!strcmp(tag, "f"))
		{
			// 폴리곤을 구성하는 정점, 텍셀, 노말의 인덱스를 저장하는 변수를 선언하고,
			std::vector<int> vIdxs, tIdxs, nIdxs;

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
				int vidx, tidx, nidx;

				// 정점, 텍셀, 노말 인덱스가 모두 있는 경우 (f 1/1/1/ 2/2/2 3/3/3 ...)
				if (bTexture && bNormal)
				{
					// 모든 인덱스를 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d%c%d", &vidx, &sep, 1, &tidx, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점과 텍셀 인덱스만 있는 경우 (f 1/1 2/2 3/3 ...)
				if (bTexture && !bNormal)
				{
					// 정점과 텍셀 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%d", &vidx, &sep, 1, &tidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					tidx = (tidx < 0) ? -tidx : tidx;
					vIdxs.push_back(vidx - 1);
					tIdxs.push_back(tidx - 1);
					continue;
				}

				// 정점과 노말 인덱스만 있는 경우 (f 1//1 2//2 3//3 ...)
				if (!bTexture && bNormal)
				{
					// 정점과 노말 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d%c%c%d", &vidx, &sep, 1, &sep, 1, &nidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					nidx = (nidx < 0) ? -nidx : nidx;
					vIdxs.push_back(vidx - 1);
					nIdxs.push_back(nidx - 1);
					continue;
				}

				// 정점의 인덱스만 있는 경우 (f 1 2 3 ...)
				if (!bTexture && !bNormal)
				{
					// 정점의 인덱스만 읽어 리스트에 추가한다.
					sscanf_s(buffer, "%d", &vidx);
					vidx = (vidx < 0) ? -vidx : vidx;
					vIdxs.push_back(vidx - 1);
					continue;
				}
			}

			// 폴리곤을 삼각형으로 나누어 메쉬에 추가한다.
			int NumVert = (int)vIdxs.size(); // 삼각형(NumVert = 3), 사각형(NumVert = 4), ...
			for (int i = 0; i < NumVert - 2; ++i)
			{
				// 삼각형의 정점의 인덱스를 설정하고,
				DgFace f;
				f.vIdx[0] = vIdxs[0];
				f.vIdx[1] = vIdxs[i + 1];
				f.vIdx[2] = vIdxs[i + 2];
				f.mIdx = MtlIdx;

				// 각 정점의 텍셀 정보를 구하고,
				if (bTexture)
				{
					f.tIdx[0] = tIdxs[0];
					f.tIdx[1] = tIdxs[i + 1];
					f.tIdx[2] = tIdxs[i + 2];
				}

				// 각 정점의 법선 정보를 구하고,
				if (bNormal)
				{
					f.nIdx[0] = nIdxs[0];
					f.nIdx[1] = nIdxs[i + 1];
					f.nIdx[2] = nIdxs[i + 2];
				}
				else // 법선 정보가 없다면 삼각형 법선으로 대체한다.
				{
					// 삼각형의 노말벡터를 계산하고,
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

				// 삼각형을 생성하여 메쉬에 추가한다.
				pMesh->Faces.push_back(f);
			}
		}
	}

	// 파일을 닫는다.
	fclose(fp);

	// 텍스처 유무에 따라 쉐이딩 타입을 설정한다.
	if (bTexture)
		pMesh->bRenderTexture = true;

	// 모델 검색 디렉터리에서 원래 작업 디렉터리로 복귀한다.
	_chdir(cwd);

	// 임포트된 메쉬의 포인터를 반환한다.
	printf("OBJ 파일 임포트 성공...\n");
	return pMesh;
}


