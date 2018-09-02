#include "graphics/GraphicsManager.h"

#include <graphics/LocalGL.h>


namespace perfectpixel {
namespace graphics {

	namespace {
		const types::PpInt SPRITE_VERTEX_ELEMENTS = 5;
	}

GraphicsManager::GraphicsManager(world::EntityManager *entityManager, boost::function<types::Vector3(world::Entity)> positionCallback)
	: m_entityManager(entityManager)
	, m_positionCallback(positionCallback)
{

}


GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::initialize()
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glewInit();

	glGenBuffers(1, &m_vbo);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	GLsizei
		indexXyz{ 0 },
		indexUv{ 3 };

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_ELEMENTS*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_ELEMENTS*sizeof(float), (GLvoid*)(sizeof(float)*3));

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 xyz;"
		"layout(location = 1) in vec2 uv;"
		"out vec2 tex_uv;"
		"void main() {"
		"  gl_Position = vec4(xyz, 1.0);"
		"  tex_uv = uv;"
		"}";
// 	const char* vertex_shader =
// 		"#version 400\n"
// 		"in vec3 xyz;"
// 		"void main() {"
// 		"  gl_Position = vec4(xyz, 1.0);"
// 		"}";

	const char* fragment_shader =
		"#version 400\n"
		"in vec2 tex_uv;"
		"uniform sampler2D u_texture;"
		"out vec4 frag_colour;"
		"void main() {"
		"  vec4 colour = texture2D(u_texture, tex_uv);"
		"  if (colour.a < 0.5)"
		"  {"
		"    discard;"
		"  }"
		"  colour.a = 1;"
		"  frag_colour = colour;"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	m_programSpriteHardalpha = glCreateProgram();
	glAttachShader(m_programSpriteHardalpha, fs);
	glAttachShader(m_programSpriteHardalpha, vs);
	glLinkProgram(m_programSpriteHardalpha);

	glUniform1i(glGetUniformLocation(m_programSpriteHardalpha, "u_texture"), 1);
}

void GraphicsManager::drawAll(double deltaT)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_softAlpha.clear();
	m_hardAlpha.clear();

	for (auto it : m_spriteComponents)
	{
		drawSpriteComponent(it.second);
	}

	std::sort(m_softAlpha.begin(), m_softAlpha.end(), &GraphicsManager::compSortSoftalpha);

	SpriteRenderState renderState{ 0, 0 };
	for (auto it : m_hardAlpha)
	{
		setCompatibleState(it.second[0], &renderState);
		drawSpriteList(it.second, &renderState, true);
	}

	drawSpriteList(m_softAlpha, &renderState);

	glEnd();
}

void GraphicsManager::registerSprite(world::Entity entity, const SpriteComponent &spriteComponent)
{
	m_spriteComponents.insert(std::pair<world::Entity, SpriteComponent>(entity, spriteComponent));
}

void GraphicsManager::drawSpriteComponent(const SpriteComponent &spriteComponent)
{
	types::Vector3 actualPosition{ m_positionCallback(spriteComponent.getEntity()) + types::Vector3(spriteComponent.getOffset()) };
	types::Vector2 worldSize{ spriteComponent.getSize() };

	SpriteDrawInfo drawInfo;

	drawInfo.m_worldCoord = {
		actualPosition.m_x,
		actualPosition.m_y,
		actualPosition.m_x + worldSize.m_x,
		actualPosition.m_y + worldSize.m_y
	};

	Sprite *sprite = spriteComponent.getSprite();
	types::Vector2 texturePosition = sprite->getTexCoord(spriteComponent.getFrame());
	types::Vector2 textureSize = sprite->getSize();

	drawInfo.m_texCoord = {
		texturePosition.m_x,
		texturePosition.m_y,
		texturePosition.m_x + textureSize.m_x,
		texturePosition.m_y + textureSize.m_y
	};

	drawInfo.m_hints = spriteComponent.getHints();
	drawInfo.m_texture = sprite->getTexture();
	drawInfo.m_depth = actualPosition.m_z;

	enqueueSpriteDraw(drawInfo);
}

void GraphicsManager::enqueueSpriteDraw(const SpriteDrawInfo &info)
{
	if (info.m_hints & RenderHints::RH_SOFTALPHA)
	{
		m_softAlpha.push_back(info);
	}
	else
	{
		m_hardAlpha[info.m_texture->getId()].push_back(info);
	}
}

void GraphicsManager::addSpriteToBuffer(const SpriteDrawInfo &info, std::vector<types::PpFloat> *buffer)
{
	const Quad 
		&pos { info.m_worldCoord },
		&tex { info.m_texCoord };

	types::PpFloat z{ info.m_depth };

	types::Vector3
		posTopLeft{ pos.m_left, pos.m_top, z },
		posTopRight{ pos.m_right, pos.m_top, z },
		posBottomLeft{ pos.m_left, pos.m_bottom, z },
		posBottomRight{ pos.m_right, pos.m_bottom, z };

	types::Vector2
		texTopLeft{ tex.m_left, tex.m_top },
		texTopRight{ tex.m_right, tex.m_top },
		texBottomLeft{ tex.m_left, tex.m_bottom },
		texBottomRight{ tex.m_right, tex.m_bottom };

	// First tri
	addSpriteVertexToBuffer(posTopLeft, texTopLeft, buffer);
	addSpriteVertexToBuffer(posTopRight, texTopRight, buffer);
	addSpriteVertexToBuffer(posBottomLeft, texBottomLeft, buffer);

	// Second tri
	addSpriteVertexToBuffer(posBottomLeft, texBottomLeft, buffer);
	addSpriteVertexToBuffer(posTopRight, texTopRight, buffer);
	addSpriteVertexToBuffer(posBottomRight, texBottomRight, buffer);

}

void GraphicsManager::addSpriteVertexToBuffer(const types::Vector3 &pos, const types::Vector2 &uv, std::vector<types::PpFloat> *buffer)
{
	buffer->push_back(pos.m_x);
	buffer->push_back(pos.m_y);
	buffer->push_back(pos.m_z);

  	buffer->push_back(uv.m_x);
  	buffer->push_back(uv.m_y);
}

void GraphicsManager::drawSpriteList(const SpriteDrawList &list, SpriteRenderState *in_out_state, bool forceCurrentState)
{
	std::vector<types::PpFloat> buffer;
	for (auto &info : list)
	{
		if (!forceCurrentState && !isStateCompatible(info, *in_out_state))
		{
			flushSpriteBuffer(&buffer);

			setCompatibleState(info, in_out_state);
		}
		
		addSpriteToBuffer(info, &buffer);
	}

	flushSpriteBuffer(&buffer);
}

void GraphicsManager::flushSpriteBuffer(std::vector<types::PpFloat> *buffer)
{
	if (!buffer->empty())
	{
		glBindBuffer(GL_VERTEX_ARRAY, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer->size() * sizeof(types::PpFloat), buffer->data(), GL_STREAM_DRAW);

		glBindVertexArray(m_vao);

		//glDrawArrays(GL_TRIANGLES, 0, buffer->size() / SPRITE_VERTEX_ELEMENTS);
		glDrawArrays(GL_TRIANGLES, 0, buffer->size() / 3);
	}

	buffer->clear();
}

bool GraphicsManager::isStateCompatible(const SpriteDrawInfo &info, const SpriteRenderState &state)
{
	if (state.m_texture != info.m_texture->getId())
	{
		return false;
	}

	GLuint targetProgram = info.m_hints & RH_SOFTALPHA ? m_programSpriteSoftAlpha : m_programSpriteHardalpha;
	return targetProgram == state.m_program;
}

void GraphicsManager::setCompatibleState(const SpriteDrawInfo &info, SpriteRenderState *out_state)
{
	// Figure out program depending on alpha settings
	GLuint targetProgram = info.m_hints & RH_SOFTALPHA ? m_programSpriteSoftAlpha : m_programSpriteHardalpha;
	if (out_state->m_program != targetProgram)
	{
		glUseProgram(targetProgram);
		out_state->m_program = targetProgram;
	}

	// Bind desired texture
	GLuint targetTexture = info.m_texture->getId();
	if (out_state->m_texture != targetTexture)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		info.m_texture->bind();
		out_state->m_texture = targetTexture;
	}
}

bool GraphicsManager::compSortSoftalpha(const SpriteDrawInfo &first, const SpriteDrawInfo &second)
{
	if (first.m_depth > second.m_depth)
	{
		return true;
	}
	else if (first.m_depth < second.m_depth)
	{
		return false;
	}

	return first.m_texture < second.m_texture; // Desperately try to fix performance on softalpha textures
}

}
}