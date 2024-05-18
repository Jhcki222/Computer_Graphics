#pragma once

/*! \class FrameBuffer
*	\brief OpenGL 프레임 버퍼를 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2023-12-02
*/
class FrameBuffer
{
public:
	/*! \biref 프레임 버퍼 너비 */
	int m_Width;

	/*! \biref 프레임 버퍼 높이 */
	int m_Height;

	/*! \biref 프레임 버퍼 객체 핸들 */
	unsigned int m_FrameBufObj;

	/*! \biref 생성된 텍스처 아이디 */
	unsigned int m_Texture;

	/*! \biref 렌더링 버퍼 객체 핸들 */
	unsigned int m_RenderBufObj;

public:
	/*! \biref 생성자 */
	FrameBuffer(int width = 500, int height = 500);

	/*! \biref 소멸자 */
	~FrameBuffer();

	/*!
	*	\biref	생성된 텍스처를 반환한다.
	*
	*	\return	생성된 텍스처를 반환한다.
	*/
	unsigned int GetFrameTexture();

	/*!
	*	\biref	프레임 버퍼의 크기를 재설정한다.
	*
	*	\param	width[in]	버퍼의 너비
	*	\param	height[in]	버퍼의 높이
	*/
	void RescaleFrameBuffer(int width, int height);

	/*!
	*	\biref	프레임 버퍼를 바인딩한다.
	*/
	void Bind() const;

	/*!
	*	\biref	프레임 버퍼를 언바인딩한다.
	*/
	void Unbind() const;
};

/*! \class	Camera
*	\brief	장면 관측을 위한 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2023-12-17
*/
class Camera
{
public:
	int m_Width;		/*! \brief 트랙볼 화면의 너비 */
	int m_Height;		/*! \brief 트랙볼 화면의 높이 */	
	float m_Axis[3];	/*! \brief 트랙볼 회전축 */	
	float m_Angle;		/*! \brief 회전 각도 */	
	float m_RotMat[16];	/*! \brief 현재 회전 행렬 */	
	float m_Zoom;		/*! \brief 줌 인(아웃)을 제어하는 변수 */
	float m_Pan[3];		/*! \brief Panning 정보 */	
	int m_StartPt[2];	/*! \brief 마우스 클릭 위치 */

public:
	Camera();			/*! \brief 생성자 */
	~Camera();			/*! \brief 소멸자 */
	void Reset();
	void SetWindowSize(int width, int height);
	void ProcessMouseEvents();
	void SetupViewFrustum();
	void SetupViewTransform();
	void GetSphereCoord(int x, int y, float* px, float* py, float* pz);
};

/*! 
*	\class	DgVertex
*	\brief	정점을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgVertex
{
public:
	double Pos[3];	/*! \brief 정점의 좌표 */
};

/*!
*	\class	DgTexel
*	\brief	텍스처 좌표를 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-05-12
*/
class DgTexel
{
public:
	double ST[2];	/*! \brief 텍스처 좌표(s, t) */
};

/*!
*	\class	DgNormal
*	\brief	법선을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgNormal
{
public:
	double Dir[3];	/*! \brief 법선의 방향 */
};

/*!
*	\class	DgFace
*	\brief	삼각형을 표현하는 클래스
*/
class DgFace
{
public:
	int mIdx;		/*! \brief 삼각형이 사용하는 재질의 인덱스(-1이면 재질 미사용) */
	int vIdx[3];	/*! \brief 삼각형을 구성하는 세 정점의 인덱스 */
	int nIdx[3];	/*! \brief 삼각형 세 정점에 할당된 법선 벡터의 인덱스 */
	int tIdx[3];	/*! \brief 삼각형 세 정점에 할당된 텍스처 정점의 인덱스 */
};

/*!
*	\class	DgMaterial
*	\brief	메쉬가 사용하는 재질을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-05-12
*/
class DgMaterial
{
public:
	std::string Name;	/*! \brief 재질의 이름 */
	float Ka[3];		/*! \brief 주변광 반사 계수 */
	float Kd[3];		/*! \brief 난반사광 반사 계수 */
	float Ks[3];		/*! \brief 전반사광 반사 계수 */
	float Ns;			/*! \brief 전반사 지수 */
	GLuint TexId;		/*! \brief 재질의 텍스처 아이디(1부터 시작) */

public:
	DgMaterial() {
		Ka[0] = 0.1f;	Ka[1] = 0.1f;	Ka[2] = 0.1f;
		Kd[0] = 0.7f;	Kd[1] = 0.7f;	Kd[2] = 0.7f;
		Ks[0] = 0.9f;	Ks[1] = 0.9f;	Ks[2] = 0.9f;
		Ns = 32.0f;
		TexId = 0;
	}
};

/*!
*	\class	DgMesh
*	\brief	삼각형으로 구성된 메쉬 모델을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgMesh
{
public:
	std::string MeshName;			/*! \brief 메쉬 이름 */	
	std::vector<DgVertex> Verts;	/*! \brief 메쉬를 구성하는 정점 배열 */
	std::vector<DgTexel> Texels;	/*! \brief 메쉬를 구성하는 텍스처 정점 배열 */
	std::vector<DgNormal> Normals;	/*! \brief 메쉬를 구성하는 법선 배열 */
	std::vector<DgFace> Faces;		/*! \brief 메쉬를 구성하는 삼각형 배열 */
	std::vector<DgMaterial> Mtls;	/*! \brief 메쉬가 사용하는 재질 배열 */
	bool bRenderVertex;				/*! \brief 정점 렌더링 여부 */
	bool bRenderPolygon;			/*! \brief 삼각형 렌더링 여부 */
	bool bRenderTexture;			/*! \brief 텍스처 렌더링 여부 */
	bool bRenderWire;				/*! \brief 와이어 렌더링 여부 */

public:
	DgMesh() {
		bRenderPolygon = true;
		bRenderVertex = false;
		bRenderWire = false;
		bRenderTexture = false;
	}
	~DgMesh() {};
	void Render();
	void RenderVertex();
	void RenderPolygon();
	void RenderWire();
	void RenderTexture();
};

/*!
*	\brief	OBJ 파일을 로드하는 함수
*
*	\param	fname[in]	로드할 OBJ 파일 이름
*
*	\return OBJ 파일을 로드하여 생성한 메쉬의 포인터를 반환한다.
*/
DgMesh* import_mesh_obj(const char* fname);

// 유틸리티 함수들
void sub(double c[3], double a[3], double b[3]);
void cross_product(double c[3], double a[3], double b[3]);
void normalize(double n[3]);



