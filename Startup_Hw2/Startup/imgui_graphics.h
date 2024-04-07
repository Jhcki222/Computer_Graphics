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
*	\brief	화면 관측을 위한 클래스
*
*	\author 윤승현(shyun@dongguk.edu)
*	\date	2023-12-17
*/
class Camera
{
public:
	/*! \brief 트랙볼 화면의 너비 */
	int m_Width;

	/*! \brief 트랙볼 화면의 높이 */
	int m_Height;

	/*! \brief 트랙볼 회전축 */
	float m_Axis[3];

	/*! \brief 회전 각도 */
	float m_Angle;

	/*! \brief 현재 회전행렬 */
	float m_RotMat[16];

	/*! \brief 줌인(아웃)을 제어하는 변수 */
	float m_Zoom;

	/*! \brief Panning 정보 */
	float m_Pan[3];

	/*! \brief 마우스 클릭 위치 */
	int m_StartPt[2];

	/*! \brief GL 2차원 객체 포인터 */
	GLUquadric* m_pObj;

public:
	/*! \brief 생성자 */
	Camera();

	/*! \brief 소멸자 */
	~Camera();

	void Reset();
	void SetWindowSize(int width, int height);
	void ProcessMouseEvents();
	void SetupViewFrustum();
	void SetupViewTransform();
	void GetSphereCoord(int x, int y, float* px, float* py, float* pz);
};
