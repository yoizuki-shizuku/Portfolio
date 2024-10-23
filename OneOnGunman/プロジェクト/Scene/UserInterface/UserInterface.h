/*/
 *  �t�@�C����		�F	UserInterface.h
 *  �T�v			�F	UI��\�����邽�߂̃N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/02
/*/
#pragma once

//	�A���J�[�|�C���g�̗񋓑�
enum class Anchor : int
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,

	MIDDLE_LEFT,
	MIDDLE_CENTER,
	MIDDLE_RIGHT,

	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT
};

//	�Q�[�W�̕���
enum class Direction
{
	Down,
	Up,
	Right,
	Left,
};

class UserInterface
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		float windowWidth;						//	��ʂ̉���
		float windowHeight;						//	��ʂ̏c��
		float ratio;							//	�Q�[�W�Ƃ��Ďg���ۂ̕\�����銄��
		int direction;							//	�Q�[�W�Ƃ��Ďg���ۂ̕���
		DirectX::SimpleMath::Vector4 color;		//	�F
	};

public:
	UserInterface();
	~UserInterface() = default;

	//	�摜�f�[�^��ǂݍ���
	void LoadTexture(const wchar_t* path);

	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetRadian(const float& radian)                            { m_radian = radian; }
	void SetScale(const DirectX::SimpleMath::Vector2& scale)       { m_scale = scale; }
	void SetScale(const float& scale)                              { m_scale = DirectX::SimpleMath::Vector2(scale); }
	void SetAnchor(const Anchor anchor)                            { m_anchor = anchor; }
	void SetAlpha(const float& alpha)                              { m_alpha = alpha; }
	void SetRatio(const float& ratio)                              { m_ratio = ratio; }
	void SetDirection(const Direction dir)                         { m_direction = dir; }
	void SetColor(const DirectX::SimpleMath::Vector3& color)	   { m_color = color; }

	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	float GetDegree()                          const { return DirectX::XMConvertToDegrees(m_radian); }
	DirectX::SimpleMath::Vector2 GetScale()    const { return m_scale; }
	Anchor GetAnchor()                         const { return m_anchor; }
	float GetAlpha()                           const { return m_alpha; }
	DirectX::SimpleMath::Vector2 GetSize() const 
	{ return DirectX::SimpleMath::Vector2(static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight)); }

private:
	//	�V�F�[�_�[�̍쐬
	void CreateShader();

private:
	//	�V�F�[�_�[�ɏ���n�����߂̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	�ʒu
	DirectX::SimpleMath::Vector2 m_position;
	//	�p�x
	float m_radian;
	//	�傫��
	DirectX::SimpleMath::Vector2 m_scale;
	//	�A���J�[�|�C���g
	Anchor m_anchor;
	//	�����x
	float m_alpha;
	//	��ʂ̑傫��
	int m_windowWidth, m_windowHeight;
	//	�e�N�X�`���̌��̑傫��
	int m_textureWidth, m_textureHeight;
	//	�Q�[�W�Ƃ��Ă̊���
	float m_ratio;
	//	�Q�[�W�Ƃ��Ă̕���
	Direction m_direction;
	//	�F
	DirectX::SimpleMath::Vector3 m_color;

};