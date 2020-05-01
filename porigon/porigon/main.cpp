#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
 
using namespace DirectX;

struct VERTEX {
	XMFLOAT3 V;
	XMFLOAT4 C;
};

//���b�Z�[�W�v���[�W���[
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY://�I����
		PostQuitMessage(0);//�����ɂ͂��̃A�v�����I�����鎞�̏I���R�[�h���w�肷��
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR pCmdLine, int nCmdShow)
{
	const LPCSTR WindowName = "GameWindow";//�E�B���h�E��
	const int WindowWidth = 640;//�E�B���h�E�̕�
	const int WindowHegint = 480;//�E�B���h�E�̍���

	WNDCLASS wc{};//WNDCLASS�\���̂̒�`

	wc.lpfnWndProc = WndProc;//�E�B���h�E�v���[�W���[�̖��O
	wc.hInstance = hInstance;//�C���X�^���X�n���h��
	wc.lpszClassName = WindowName;//�N���X�o�^��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&wc);
	RECT rect{ 0, 0, WindowWidth, WindowHegint };//�l�p�`�̕��A�������L�q
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(WindowName, WindowName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Dx11 init
	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferCount = 1;                                                                //�o�b�N�o�b�t�@�̐�
	scd.BufferDesc.Width = WindowWidth;                                                 //�o�b�t�@�̕�
	scd.BufferDesc.Height = WindowHegint;                                               //�o�b�t�@�̍���
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                                 //�o�b�t�@�[�t�H�[�}�b�g
	scd.BufferDesc.RefreshRate.Numerator = 60;                                          //���t���b�V�����[�g
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1,
		D3D11_SDK_VERSION,
		&scd, &pSwapChain, &pDevice, NULL, &pDeviceContext);


	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11RenderTargetView* pBackBufferRenderTargetView;
	ID3D11Texture2D *pbbTex;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbbTex);
	pDevice->CreateRenderTargetView(pbbTex, NULL, &pBackBufferRenderTargetView);
	pbbTex->Release();

	ID3D11Texture2D* pDepthStencil;
	D3D11_TEXTURE2D_DESC dtd{};
	dtd.Width = WindowWidth;
	dtd.Height = WindowHegint;
	dtd.MipLevels = 1;
	dtd.ArraySize = 1;
	dtd.Format = DXGI_FORMAT_D32_FLOAT;
	dtd.SampleDesc.Count = 1;
	dtd.SampleDesc.Quality = 0;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtd.CPUAccessFlags = 0;
	dtd.MiscFlags = 0;
	pDevice->CreateTexture2D(&dtd, NULL, &pDepthStencil);
	ID3D11DepthStencilView* pDepthStencilView{};

	pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

	pDeviceContext->OMSetRenderTargets(1, &pBackBufferRenderTargetView, pDepthStencilView);

	const float vertices[]{
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	ID3D11Buffer* pVertexBuffer;

	D3D11_BUFFER_DESC vbd{};
	vbd.ByteWidth = sizeof(vertices);
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA dataVertex{};
	dataVertex.pSysMem = vertices;

	pDevice->CreateBuffer(
		&vbd,
		&dataVertex,
		&pVertexBuffer
	);

	const unsigned int indices[]{ 0, 1, 2 };

	ID3D11Buffer* pIndexBuffer;

	D3D11_BUFFER_DESC ibd{};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA dataIndex{};
	dataIndex.pSysMem = indices;

	pDevice->CreateBuffer(
		&ibd,
		&dataIndex,
		&pIndexBuffer
	);

	ID3D11VertexShader* pVertexShader;
	ID3DBlob* pCompileVS{};
	D3DCompileFromFile(L"simple_shader.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, &pCompileVS, NULL);
	pDevice->CreateVertexShader(
		pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(),
		NULL,
		&pVertexShader
	);

	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pCompilePS{};
	D3DCompileFromFile(L"simple_shader.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, &pCompilePS, NULL);
	pDevice->CreatePixelShader(
		pCompilePS->GetBufferPointer(),
		pCompilePS->GetBufferSize(),
		NULL,
		&pPixelShader
	);

	pCompilePS->Release();

	ID3D11InputLayout* pVertexLayout;
	D3D11_INPUT_ELEMENT_DESC layout[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, 2,
		pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	pCompileVS->Release();


	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)WindowWidth;
	vp.Height = (FLOAT)WindowHegint;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1, &vp);

	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float clearColor[4] = { 0.5f, 0.5f, 1.0f, 1.0f };
			pDeviceContext->ClearRenderTargetView(pBackBufferRenderTargetView, clearColor);
			pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
			pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
			pDeviceContext->IASetInputLayout(pVertexLayout);

			UINT stride = sizeof(float) * 3;
			UINT offset = 0;
			pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
			pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pDeviceContext->DrawIndexed(3, 0, 0);

			pSwapChain->Present(1, 0);
		}
	}
	pVertexLayout->Release();
	pPixelShader->Release();
	pVertexShader->Release();
	pVertexBuffer->Release();
	pIndexBuffer->Release();
	pDepthStencilView->Release();
	pDepthStencil->Release();
	pBackBufferRenderTargetView->Release();
	pDeviceContext->Release();
	pDevice->Release();
	pSwapChain->Release();
	return 0;
}