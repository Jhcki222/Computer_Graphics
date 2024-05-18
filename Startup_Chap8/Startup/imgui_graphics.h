#pragma once

/*! \class FrameBuffer
*	\brief OpenGL ������ ���۸� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2023-12-02
*/
class FrameBuffer
{
public:
	/*! \biref ������ ���� �ʺ� */
	int m_Width;

	/*! \biref ������ ���� ���� */
	int m_Height;

	/*! \biref ������ ���� ��ü �ڵ� */
	unsigned int m_FrameBufObj;

	/*! \biref ������ �ؽ�ó ���̵� */
	unsigned int m_Texture;

	/*! \biref ������ ���� ��ü �ڵ� */
	unsigned int m_RenderBufObj;

public:
	/*! \biref ������ */
	FrameBuffer(int width = 500, int height = 500);

	/*! \biref �Ҹ��� */
	~FrameBuffer();

	/*!
	*	\biref	������ �ؽ�ó�� ��ȯ�Ѵ�.
	*
	*	\return	������ �ؽ�ó�� ��ȯ�Ѵ�.
	*/
	unsigned int GetFrameTexture();

	/*!
	*	\biref	������ ������ ũ�⸦ �缳���Ѵ�.
	*
	*	\param	width[in]	������ �ʺ�
	*	\param	height[in]	������ ����
	*/
	void RescaleFrameBuffer(int width, int height);

	/*!
	*	\biref	������ ���۸� ���ε��Ѵ�.
	*/
	void Bind() const;

	/*!
	*	\biref	������ ���۸� ����ε��Ѵ�.
	*/
	void Unbind() const;
};

/*! \class	Camera
*	\brief	��� ������ ���� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2023-12-17
*/
class Camera
{
public:
	int m_Width;		/*! \brief Ʈ���� ȭ���� �ʺ� */
	int m_Height;		/*! \brief Ʈ���� ȭ���� ���� */	
	float m_Axis[3];	/*! \brief Ʈ���� ȸ���� */	
	float m_Angle;		/*! \brief ȸ�� ���� */	
	float m_RotMat[16];	/*! \brief ���� ȸ�� ��� */	
	float m_Zoom;		/*! \brief �� ��(�ƿ�)�� �����ϴ� ���� */
	float m_Pan[3];		/*! \brief Panning ���� */	
	int m_StartPt[2];	/*! \brief ���콺 Ŭ�� ��ġ */

public:
	Camera();			/*! \brief ������ */
	~Camera();			/*! \brief �Ҹ��� */
	void Reset();
	void SetWindowSize(int width, int height);
	void ProcessMouseEvents();
	void SetupViewFrustum();
	void SetupViewTransform();
	void GetSphereCoord(int x, int y, float* px, float* py, float* pz);
};

/*! 
*	\class	DgVertex
*	\brief	������ ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgVertex
{
public:
	double m_Pos[3];	/*! \brief ������ ��ǥ */

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
*	\brief	������ ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgNormal
{
public:
	double m_Dir[3];	/*! \brief ������ ���� */

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
*	\brief	�ﰢ���� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgFace
{
public:
	int m_VertIdxs[3];		/*! \brief �ﰢ���� �����ϴ� �� ������ �ε��� */
	int m_NormalIdxs[3];	/*! \brief �ﰢ�� �� ������ �Ҵ�� ���� ������ �ε��� */

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
*	\brief	�ﰢ������ ������ �޽� ���� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgMesh
{
public:
	std::vector<DgVertex> m_Verts;		/*! \brief �޽��� �����ϴ� ���� �迭 */
	std::vector<DgNormal> m_Normals;	/*! \brief �޽��� �����ϴ� ���� �迭 */
	std::vector<DgFace> m_Faces;		/*! \brief �޽��� �����ϴ� �ﰢ�� �迭 */
	float m_Color[3];					/*! \brief �޽��� ���� */

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

