#pragma once
#include <vector>
#include <utility>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Shader.h"
#include "basic/Mesh.h"
#include "basic/Texture.h"


class Model
{
public:
	Model(const std::string& name, const std::string& modelFileName);
	Model(const std::string& name, Mesh* m, const std::string& texturePath);
	~Model();
	void render(Shader* shader);

	void setPosition(const glm::vec3& position){ this->position = position; }
	glm::vec3 getPosition() const { return position; }
	void setRotation(const glm::vec3& rotation) { this->rotation = rotation; }
	glm::vec3 getRotation() const { return rotation; }
	void setScale(const glm::vec3& scale) { this->scale = scale; }
	glm::vec3 getScale() const { return scale; }
	std::string getName() const { return name; }
	float getShine() const { return shine; }
	void setShine(float shine) { this->shine = shine; }
	std::vector<Texture*> getTextures() { return allTextures; }
	static const std::string AMBIENT_NAME;
	static const std::string DIFFUSE_NAME;
	static const std::string SPECULAR_NAME;
	static const std::string SHINE_NAME;

private:
	std::string name;
	std::string filePath;
	std::vector<std::pair<Mesh*, std::vector<Texture*>>> meshes;
	std::vector<Texture*> allTextures;
	Assimp::Importer importer;

	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

	float shine = 32;

	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);

	std::string getFilePathWithoutFileName()
	{
		size_t slashPos = filePath.find_last_of('/');
		if (slashPos == std::string::npos)
		{
			slashPos = filePath.find_last_of('\\');
			if (slashPos == std::string::npos)
				return filePath;
		}
		return filePath.substr(0, slashPos + 1);
	}

	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

