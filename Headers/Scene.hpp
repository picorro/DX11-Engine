#pragma once
#include <memory>
#include <Object3D.hpp>
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();


	void AddObject(std::unique_ptr<Object3D> object);
	void RemoveObject(Object3D* object);

	const std::vector<std::unique_ptr<Object3D>>& GetObjects() const;
	const std::vector<Object3D*> GetAllObjects() const;

	void Update(float _deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, ID3D11Buffer* perObjectConstantBuffer);

private:
	std::vector<std::unique_ptr<Object3D>> _objects;
};

