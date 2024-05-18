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
	double Pos[3];	/*! \brief ������ ��ǥ */
};

/*!
*	\class	DgTexel
*	\brief	�ؽ�ó ��ǥ�� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-05-12
*/
class DgTexel
{
public:
	double ST[2];	/*! \brief �ؽ�ó ��ǥ(s, t) */
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
	double Dir[3];	/*! \brief ������ ���� */
};

/*!
*	\class	DgFace
*	\brief	�ﰢ���� ǥ���ϴ� Ŭ����
*/
class DgFace
{
public:
	int mIdx;		/*! \brief �ﰢ���� ����ϴ� ������ �ε���(-1�̸� ���� �̻��) */
	int vIdx[3];	/*! \brief �ﰢ���� �����ϴ� �� ������ �ε��� */
	int nIdx[3];	/*! \brief �ﰢ�� �� ������ �Ҵ�� ���� ������ �ε��� */
	int tIdx[3];	/*! \brief �ﰢ�� �� ������ �Ҵ�� �ؽ�ó ������ �ε��� */
};

/*!
*	\class	DgMaterial
*	\brief	�޽��� ����ϴ� ������ ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-05-12
*/
class DgMaterial
{
public:
	std::string Name;	/*! \brief ������ �̸� */
	float Ka[3];		/*! \brief �ֺ��� �ݻ� ��� */
	float Kd[3];		/*! \brief ���ݻ籤 �ݻ� ��� */
	float Ks[3];		/*! \brief ���ݻ籤 �ݻ� ��� */
	float Ns;			/*! \brief ���ݻ� ���� */
	GLuint TexId;		/*! \brief ������ �ؽ�ó ���̵�(1���� ����) */

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
*	\brief	�ﰢ������ ������ �޽� ���� ǥ���ϴ� Ŭ����
*
*	\author ������(shyun@dongguk.edu)
*	\date	2024-04-16
*/
class DgMesh
{
public:
	std::string MeshName;			/*! \brief �޽� �̸� */	
	std::vector<DgVertex> Verts;	/*! \brief �޽��� �����ϴ� ���� �迭 */
	std::vector<DgTexel> Texels;	/*! \brief �޽��� �����ϴ� �ؽ�ó ���� �迭 */
	std::vector<DgNormal> Normals;	/*! \brief �޽��� �����ϴ� ���� �迭 */
	std::vector<DgFace> Faces;		/*! \brief �޽��� �����ϴ� �ﰢ�� �迭 */
	std::vector<DgMaterial> Mtls;	/*! \brief �޽��� ����ϴ� ���� �迭 */
	bool bRenderVertex;				/*! \brief ���� ������ ���� */
	bool bRenderPolygon;			/*! \brief �ﰢ�� ������ ���� */
	bool bRenderTexture;			/*! \brief �ؽ�ó ������ ���� */
	bool bRenderWire;				/*! \brief ���̾� ������ ���� */

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
*	\brief	OBJ ������ �ε��ϴ� �Լ�
*
*	\param	fname[in]	�ε��� OBJ ���� �̸�
*
*	\return OBJ ������ �ε��Ͽ� ������ �޽��� �����͸� ��ȯ�Ѵ�.
*/
DgMesh* import_mesh_obj(const char* fname);

// ��ƿ��Ƽ �Լ���
void sub(double c[3], double a[3], double b[3]);
void cross_product(double c[3], double a[3], double b[3]);
void normalize(double n[3]);



