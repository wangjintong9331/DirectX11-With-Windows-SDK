#ifndef SKYRENDER_H
#define SKYRENDER_H

#include <vector>
#include <string>
#include "Effects.h"
#include "Camera.h"

class SkyRender
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	// ��Ҫ�ṩ��������պ���ͼ ���� �Ѿ������õ���պ�����.dds�ļ�
	SkyRender(ComPtr<ID3D11Device> device, 
		ComPtr<ID3D11DeviceContext> deviceContext, 
		const std::wstring& cubemapFilename, 
		float skySphereRadius,		// �����뾶
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps


	// ��Ҫ�ṩ��պе�������������ͼ
	SkyRender(ComPtr<ID3D11Device> device, 
		ComPtr<ID3D11DeviceContext> deviceContext, 
		const std::vector<std::wstring>& cubemapFilenames, 
		float skySphereRadius,		// �����뾶
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps


	ComPtr<ID3D11ShaderResourceView> GetTextureCube();

	virtual void Draw(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect& skyEffect, const Camera& camera);

protected:
	void InitResource(ComPtr<ID3D11Device> device, float skySphereRadius);

protected:
	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;

	UINT mIndexCount;

	ComPtr<ID3D11ShaderResourceView> mTextureCubeSRV;
};

class DynamicSkyRender : public SkyRender
{
public:
	DynamicSkyRender(ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		const std::wstring& cubemapFilename,
		float skySphereRadius,		// �����뾶
		int dynamicCubeSize,		// �������ⳤ
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps
									// ��̬��պб�Ȼ����mipmaps

	DynamicSkyRender(ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> deviceContext,
		const std::vector<std::wstring>& cubemapFilenames,
		float skySphereRadius,		// �����뾶
		int dynamicCubeSize,		// �������ⳤ
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps
									// ��̬��պб�Ȼ����mipmaps


	// ���浱ǰ��ȾĿ����ͼ
	void Cache(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect);

	// ָ����պ�ĳһ�濪ʼ���ƣ���Ҫ�ȵ���Cache����
	void BeginCapture(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect, const DirectX::XMFLOAT3& pos,
		D3D11_TEXTURECUBE_FACE face, float nearZ = 1e-3f, float farZ = 1e3f);

	// �ָ���ȾĿ����ͼ������������󶨵�ǰ��̬��պ�
	void Restore(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect, const Camera& camera);

	// ��ȡ��̬��պ�
	// ע�⣺�÷���ֻ����Restore���ٵ���
	ComPtr<ID3D11ShaderResourceView> GetDynamicTextureCube();

	// ��ȡ��ǰ���ڲ������պ�
	const Camera& GetCamera() const;

private:
	void InitResource(ComPtr<ID3D11Device> device, int dynamicCubeSize);

private:
	ComPtr<ID3D11RenderTargetView>		mCacheRTV;		// ��ʱ����ĺ󱸻�����
	ComPtr<ID3D11DepthStencilView>		mCacheDSV;		// ��ʱ��������/ģ�建����
	
	FirstPersonCamera					mCamera;				// ����ǰ��պ�����һ��������
	ComPtr<ID3D11DepthStencilView>		mDynamicCubeMapDSV;		// ��̬��պ���Ⱦ��Ӧ�����/ģ����ͼ
	ComPtr<ID3D11ShaderResourceView>	mDynamicCubeMapSRV;		// ��̬��պж�Ӧ����ɫ����Դ��ͼ
	ComPtr<ID3D11RenderTargetView>		mDynamicCubeMapRTVs[6];	// ��̬��պ�ÿ�����Ӧ����ȾĿ����ͼ
	
};

#endif