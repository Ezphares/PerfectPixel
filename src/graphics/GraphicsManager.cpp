#include "graphics/GraphicsManager.h"

#include <graphics/LocalGL.h>

#include <types/File.h>
#include <types/PpException.h>

namespace perfectpixel {
namespace graphics {

GraphicsManager::GraphicsManager(world::EntityManager *entityManager, world::PositionCallback positionCallback)
	: m_entityManager(entityManager)
	, m_positionCallback(positionCallback)
	, m_programSpriteHardAlpha(NULL)
	, m_programSpriteSoftAlpha(NULL)
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
	
	VAO::BufferLayout spriteLayout;
	spriteLayout.push_back(VAO::BE_VEC_3); // Xyz
	spriteLayout.push_back(VAO::BE_VEC_2); // Uv

	m_vaoDynamicSprites = new VAO(spriteLayout);

	std::string
		vertex_shader = types::File("Sprite_vertex.glsl").str(),
		fragment_shader_hard = types::File("SpriteHard_fragment.glsl").str(),
		fragment_shader_soft = types::File("SpriteSoft_fragment.glsl").str();

	m_programSpriteHardAlpha = new ShaderProgram();
	m_programSpriteHardAlpha->addShader(GL_VERTEX_SHADER, vertex_shader);
	m_programSpriteHardAlpha->addShader(GL_FRAGMENT_SHADER, fragment_shader_hard);
	m_programSpriteHardAlpha->link();
	glUniform1i(m_programSpriteHardAlpha->getUniformLocation("u_texture"), 1);

	m_programSpriteSoftAlpha = new ShaderProgram();
	m_programSpriteSoftAlpha->addShader(GL_VERTEX_SHADER, vertex_shader);
	m_programSpriteSoftAlpha->addShader(GL_FRAGMENT_SHADER, fragment_shader_soft);
	m_programSpriteSoftAlpha->link();
	glUniform1i(m_programSpriteSoftAlpha->getUniformLocation("u_texture"), 1);
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

bool GraphicsManager::hasSprite(world::Entity entity) const
{
	return m_spriteComponents.find(entity) != m_spriteComponents.end();
}

perfectpixel::graphics::SpriteComponent & GraphicsManager::getSprite(world::Entity entity)
{
	auto it = m_spriteComponents.find(entity);
	if (it == m_spriteComponents.end())
	{
		throw types::PpException("Attempted to get SpriteComponent for entity without one attached");
	}
	return it->second;
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

void GraphicsManager::addSpriteToBuffer(const SpriteDrawInfo &info, SpriteBuffer *buffer)
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

void GraphicsManager::addSpriteVertexToBuffer(const types::Vector3 &pos, const types::Vector2 &uv, SpriteBuffer *buffer)
{
	SpriteVertex vertex{
		static_cast<GLfloat>(pos.m_x),
		static_cast<GLfloat>(pos.m_y),
		static_cast<GLfloat>(pos.m_z),

		static_cast<GLfloat>(uv.m_x),
		static_cast<GLfloat>(uv.m_y)
	};

	buffer->push_back(vertex);
}

void GraphicsManager::drawSpriteList(const SpriteDrawList &list, SpriteRenderState *in_out_state, bool forceCurrentState)
{
	SpriteBuffer buffer;
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

void GraphicsManager::flushSpriteBuffer(SpriteBuffer *buffer)
{
	if (!buffer->empty())
	{
		m_vaoDynamicSprites->bindBuffer();
		glBufferData(GL_ARRAY_BUFFER, buffer->size() * sizeof(SpriteVertex), buffer->data(), GL_STREAM_DRAW);

		m_vaoDynamicSprites->bindVAO();

		//glDrawArrays(GL_TRIANGLES, 0, buffer->size() / SPRITE_VERTEX_ELEMENTS);
		glDrawArrays(GL_TRIANGLES, 0, buffer->size());
	}

	buffer->clear();
}

bool GraphicsManager::isStateCompatible(const SpriteDrawInfo &info, const SpriteRenderState &state)
{
	if (state.m_texture != info.m_texture->getId())
	{
		return false;
	}

	ShaderProgram *targetProgram = info.m_hints & RH_SOFTALPHA ? m_programSpriteSoftAlpha : m_programSpriteHardAlpha;
	return targetProgram == state.m_program;
}

void GraphicsManager::setCompatibleState(const SpriteDrawInfo &info, SpriteRenderState *out_state)
{
	// Figure out program depending on alpha settings
	ShaderProgram *targetProgram = info.m_hints & RH_SOFTALPHA ? m_programSpriteSoftAlpha : m_programSpriteHardAlpha;
	if (out_state->m_program != targetProgram)
	{
		targetProgram->use();
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