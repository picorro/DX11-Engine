#include <Cylinder.hpp>

Cylinder::Cylinder(bool caps): _caps(caps)
{
    _vertexBuffer = nullptr;
    _indexBuffer = nullptr;
}

Cylinder::Cylinder(DirectX::XMFLOAT3 position, bool caps): _caps(caps)
{
    _vertexBuffer = nullptr;
    _indexBuffer = nullptr;

    transform.position = position;
}

Cylinder::~Cylinder()
{
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = nullptr;
    }
    if (_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = nullptr;
    }
}

void Cylinder::GenerateCylinderVertices(float radius, float height, int numSlices, std::vector<VertexPositionNormalColorUv>& vertices)
{
    const float PI = 3.14159265358979323846;
    vertices.clear();

    // Generate cap vertices and side vertices
    for (int i = 0; i <= numSlices; ++i) {
        float phi = i * 2.0f * PI / numSlices;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        if (_caps)
        {
            vertices.push_back({
                {radius * cosPhi, height / 2, radius * sinPhi},  // Position
                {0, 1, 0},                                       // Normal
                {1.0f, 1.0f, 1.0f},                              // Color
                {cosPhi, sinPhi}                                  // Uv
                });

            // Bottom cap
            vertices.push_back({
                {radius * cosPhi, -height / 2, radius * sinPhi}, // Position
                {0, -1, 0},                                      // Normal
                {1.0f, 1.0f, 1.0f},                              // Color
                {cosPhi, sinPhi}                                  // Uv
                });
        }

        // Side top
        vertices.push_back({
            {radius * cosPhi, height / 2, radius * sinPhi},  // Position
            {cosPhi, 0, sinPhi},                             // Normal
            {1.0f, 1.0f, 1.0f},                              // Color
            {(float)i / numSlices, 1.0f}                      // Uv
            });

        // Side bottom
        vertices.push_back({
            {radius * cosPhi, -height / 2, radius * sinPhi}, // Position
            {cosPhi, 0, sinPhi},                             // Normal
            {1.0f, 1.0f, 1.0f},                              // Color
            {(float)i / numSlices, 0.0f}                      // Uv
            });
    }
}

void Cylinder::GenerateCylinderIndices(int numSlices, std::vector<uint32_t>& indices)
{
    indices.clear();

    int vertexOffset = (_caps) ? 4 : 2;  // offset 4 if caps are enabled, else 2

    for (int i = 0; i < numSlices; ++i) {
        // Side
        indices.push_back(i * vertexOffset);
        indices.push_back(((i + 1) % numSlices) * vertexOffset);
        indices.push_back(i * vertexOffset + 1);

        indices.push_back(i * vertexOffset + 1);
        indices.push_back(((i + 1) % numSlices) * vertexOffset);
        indices.push_back(((i + 1) % numSlices) * vertexOffset + 1);

        if (_caps)
        {
            // Top cap
            indices.push_back(i * 4);
            indices.push_back(((i + 1) % numSlices) * 4);
            indices.push_back(numSlices * 4);  // Center vertex

            // Bottom cap
            indices.push_back(i * 4 + 1);
            indices.push_back(numSlices * 4 + 1);  // Center vertex
            indices.push_back(((i + 1) % numSlices) * 4 + 1);
        }
    }
}

bool Cylinder::Initialize(ID3D11Device* device)
{

    std::vector<VertexPositionNormalColorUv> vertices;
    GenerateCylinderVertices(0.5f, 1, 30, vertices);

    std::vector<uint32_t> indices;
    GenerateCylinderIndices(30, indices);

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexPositionNormalColorUv) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices.data();
    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    if (FAILED(hr)) return false;

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(uint32_t) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices.data();
    hr = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    if (FAILED(hr)) return false;

    return true;
}

void Cylinder::Update()
{

}

void Cylinder::Render(ID3D11DeviceContext* deviceContext)
{
    UINT stride = sizeof(VertexPositionNormalColorUv);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    int numSlices = 30;

    int numIndices = numSlices * 6; // for the side indices

    if (_caps) {
        numIndices += numSlices * 6; // add indices for top and bottom caps
    }

    deviceContext->DrawIndexed(numIndices, 0, 0);
}
