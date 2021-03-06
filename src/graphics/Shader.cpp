#include "Shader.h"
#include "../Editor.h"

#include "imgui/imgui.h"


Shader::Shader(const std::string& vshader, const std::string& fshader) :
	model(new UniformMat4("model")),
	projection(new UniformMat4("projection")),
	view(new UniformMat4("view")),
	viewPosition(new Uniform3f("viewPosition"))
{
	this->vShader = vshader; this->fShader = fshader;
	shaderProgram = new ShaderProgram();

	projection->set(EditorData::getProjectionMatrix());
	view->set(EditorData::getViewMatrix());
	viewPosition->set(EditorData::getViewPosition());

	shaderProgram->addUniform(model);
	shaderProgram->addUniform(view);
	shaderProgram->addUniform(projection);
	shaderProgram->addUniform(viewPosition);

	shaderProgram->createFromFile(vshader, fshader);

	createMaterialNamesUi();
}

Shader::~Shader()
{
	delete shaderProgram;
	for (Material* material : materials)
	{
		delete material;
	}
}

void Shader::addMaterial(const std::string& name)
{
	auto material = new Material(shaderProgram, name);
	materials.push_back(material);
}

Material& Shader::getMaterial(const std::string& name)
{
	for (Material* m : materials)
	{
		if (m->getName() == name)
		{
			return *m;
		}
	}
	for (Material* m : materialsToRenderUi)
	{
		if (m->getName() == name)
		{
			return *m;
		}
	}
	std::cout << "could not find material with name: " << name << "\n";
	return *materials[0];
}

void Shader::addUniform(Uniform* uniform)
{
	shaderProgram->addUniform(uniform);
}

void Shader::bind()
{
	shaderProgram->bind();
	projection->set(EditorData::getProjectionMatrix());
	view->set(EditorData::getViewMatrix());
	viewPosition->set(EditorData::getViewPosition());
}

void Shader::unBind()
{
	shaderProgram->unBind();
}

void Shader::compile()
{
	this->shaderProgram->compile();
}

void Shader::renderMaterialsUi()
{
	for (Material* m : materialsToRenderUi)
	{
		m->renderUi();
	}
}

void Shader::createMaterialNamesUi()
{
	std::vector<std::string> strings = shaderProgram->getAllMaterialNames();
	for (const std::string& s : strings)
	{
		if (std::find(
			Material::RESERVED_MATERIAL_NAMES.begin(),
			Material::RESERVED_MATERIAL_NAMES.end(),
			s)
			== Material::RESERVED_MATERIAL_NAMES.end())
		{
			auto material = new Material(shaderProgram, s);
			materialsToRenderUi.push_back(material);
		}
	}
}
