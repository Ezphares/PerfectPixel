#pragma once

#include <graphics/LocalGL.h>
#include <graphics/SpriteComponent.h>
#include <graphics/VAO.h>
#include <graphics/BufferLayouts.h>
#include <graphics/ShaderProgram.h>

#include <worldgraph/EntityManager.h>
#include <worldgraph/PositionCallback.h>
#include <types/vectors.h>

#include <boost/function.hpp>
#include <map>

namespace perfectpixel {
namespace graphics {

	class GraphicsManager
	{
	private:
		struct Quad {
			types::PpFloat
				m_left,
				m_top,
				m_right,
				m_bottom;
		};

		struct SpriteDrawInfo {
			Quad m_worldCoord, m_texCoord;
			RenderHints m_hints;
			Texture *m_texture;
			types::PpFloat m_depth;
		};

		struct SpriteRenderState
		{
			GLuint m_texture;
			ShaderProgram *m_program;
		};

		typedef std::map<world::Entity, SpriteComponent> SpriteComponents;
		typedef std::vector<SpriteDrawInfo> SpriteDrawList;

		typedef std::vector<SpriteVertex> SpriteBuffer;

	public:
		GraphicsManager(world::EntityManager *entityManager, world::PositionCallback positionCallback);
		~GraphicsManager();

	public:
		void initialize();
		void drawAll(double deltaT);

		void registerSprite(world::Entity entity, const SpriteComponent &spriteComponent);
		bool hasSprite(world::Entity entity) const;
		SpriteComponent &getSprite(world::Entity entity);

	private:
		void drawSpriteComponent(const SpriteComponent &spriteComponent);

		void enqueueSpriteDraw(const SpriteDrawInfo &info);
		void addSpriteToBuffer(const SpriteDrawInfo &info, SpriteBuffer *buffer);
		void addSpriteVertexToBuffer(const types::Vector3 &pos, const types::Vector2 &uv, SpriteBuffer *buffer);

		void drawSpriteList(const SpriteDrawList &list, SpriteRenderState *in_out_state, bool forceCurrentRenderstate = false);
		void flushSpriteBuffer(SpriteBuffer *buffer);


		bool isStateCompatible(const SpriteDrawInfo &info, const SpriteRenderState &state);
		void setCompatibleState(const SpriteDrawInfo &info, SpriteRenderState *out_state);

		static bool compSortSoftalpha(const SpriteDrawInfo &first, const SpriteDrawInfo &second);

	private:
		world::EntityManager *m_entityManager;
		world::PositionCallback m_positionCallback;

		SpriteComponents m_spriteComponents;

		ShaderProgram *m_programSpriteHardAlpha;
		ShaderProgram *m_programSpriteSoftAlpha;

		GLuint m_shaderProgram;
		GLuint m_vao;
		GLuint m_vbo;

		VAO *m_vaoDynamicSprites;

		std::map<GLuint, SpriteDrawList> m_hardAlpha;
		SpriteDrawList m_softAlpha;
	};

}
}
