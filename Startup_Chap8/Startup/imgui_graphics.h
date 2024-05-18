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
	double m_Pos[3];	/*! \brief 정점의 좌표 */

public:
	DgVertex(double x, double y, double z){
		m_Pos[0] = x;
		m_Pos[1] = y;
		m_Pos[2] = z;
	}
	~DgVertex()	{}
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
	double m_Dir[3];	/*! \brief 법선의 방향 */

public:
	DgNormal(double x, double y, double z){
		m_Dir[0] = x;
		m_Dir[1] = y;
		m_Dir[2] = z;
	}
	~DgNormal()	{}
};

/*!
*	\class	DgFace
*	\brief	삼각형을 표현하는 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgFace
{
public:
	int m_VertIdxs[3];		/*! \brief 삼각형을 구성하는 세 정점의 인덱스 */
	int m_NormalIdxs[3];	/*! \brief 삼각형 세 정점에 할당된 법선 벡터의 인덱스 */

public:
	DgFace(int vidx0, int vidx1, int vidx2, int nidx0, int nidx1, int nidx2){
		m_VertIdxs[0] = vidx0;
		m_VertIdxs[1] = vidx1;
		m_VertIdxs[2] = vidx2;
		m_NormalIdxs[0] = nidx0;
		m_NormalIdxs[1] = nidx1;
		m_NormalIdxs[2] = nidx2;
	}
	~DgFace() {}
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
	std::vector<DgVertex> m_Verts;		/*! \brief 메쉬를 구성하는 정점 배열 */
	std::vector<DgNormal> m_Normals;	/*! \brief 메쉬를 구성하는 법선 배열 */
	std::vector<DgFace> m_Faces;		/*! \brief 메쉬를 구성하는 삼각형 배열 */
	float m_Color[3];					/*! \brief 메쉬의 색상 */

public:
	DgMesh() { 
		m_Color[0] = 0.5f;
		m_Color[1] = 0.5f; 
		m_Color[2] = 0.5f; 
	}
	~DgMesh() {};
	void Render();	
};


DgMesh* import_mesh_obj(const char* fname);
void sub(double c[3], double a[3], double b[3]);
void cross_product(double c[3], double a[3], double b[3]);
void normalize(double n[3]);

