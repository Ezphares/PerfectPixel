#pragma once

#include <graphics/LocalGL.h>
#include <graphics/SpriteComponent.h>
#include <graphics/VAO.h>
#include <graphics/BufferLayouts.h>
#include <graphics/ShaderProgram.h>
#include <graphics/CameraSettings.h>
#include <graphics/FrameBuffer.h>
#include <graphics/Quad.h>
#include <graphics/IFont.h>
#include <graphics/DrawQueue.h>

#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/PositionCallback.h>
#include <Bedrock/vectors.h>

#include <map>

namespace perfectpixel {
namespace graphics {
	class GraphicsManager
	{
	private:
		struct SpriteDrawInfo {
			Quad m_worldCoord, m_texCoord;
			RenderHints m_hints;
			Texture *m_texture;
			bedrock::PpFloat m_depth;
		};

		struct SpriteRenderState
		{
			GLuint m_texture;
			ShaderProgram *m_program;
		};

		typedef std::map<ecs::Entity, SpriteComponent> SpriteComponents;
		typedef std::vector<SpriteDrawInfo> SpriteDrawList;

	public:
		GraphicsManager(ecs::PositionCallback positionCallback);
		~GraphicsManager();

	public:
		void initialize();

		void drawAll(double deltaT);
		void postProcess();

		bedrock::PpFloat calculateRatio(bedrock::PpInt width, bedrock::PpInt height);
		void setWindowRatio(bedrock::PpFloat ratio);
		void setMainCamera(const CameraSettings &camera);
		void setWindowSize(bedrock::Point2 size);

		void registerSprite(ecs::Entity entity, const SpriteComponent &spriteComponent);
		bool hasSprite(ecs::Entity entity) const;
		SpriteComponent &getSprite(ecs::Entity entity);

		void queueDrawSingle(DrawQueueElement *element);

		IFont *getDefaultFont() {return m_font;}

		void cleanup();

	private:
		void drawSpriteComponent(const SpriteComponent &spriteComponent);

		void enqueueSpriteDraw(const SpriteDrawInfo &info);
		void addSpriteToBuffer(const SpriteDrawInfo &info, SpriteBuffer *buffer);
		void addSpriteVertexToBuffer(const bedrock::Vector3 &pos, const bedrock::Vector2 &uv, SpriteBuffer *buffer);

		void drawSpriteList(const SpriteDrawList &list, SpriteRenderState *in_out_state, bool forceCurrentRenderstate = false);
		void flushSpriteBuffer(SpriteBuffer *buffer);

		bool isStateCompatible(const SpriteDrawInfo &info, const SpriteRenderState &state);
		void setCompatibleState(const SpriteDrawInfo &info, SpriteRenderState *out_state);

		void updateCamera();

		static bool compSortSoftalpha(const SpriteDrawInfo &first, const SpriteDrawInfo &second);

	private:
		ecs::PositionCallback m_positionCallback;

		SpriteComponents m_spriteComponents;

		ShaderProgram *m_programSpriteHardAlpha;
		ShaderProgram *m_programSpriteSoftAlpha;
		ShaderProgram *m_programPostProcess;
		ShaderProgram *m_programUiText;

		FrameBuffer *m_frameBuffer;

		GLuint m_ppbuffer;

		VAO *m_vaoDynamicSprites;
		VAO *m_vaoPostProcess;

		IFont *m_font;

		std::map<GLuint, SpriteDrawList> m_hardAlpha;
		SpriteDrawList m_softAlpha;

		bedrock::Point2 m_mainWindowSize;
		bedrock::PpFloat m_windowRatio;
		CameraSettings m_mainCamera;

		std::vector<ecs::Entity> m_cleanup;

		std::vector<double> m_ftBuffer;
		std::size_t m_ftWrite;

		DrawQueue m_uiQueue;
	};

}
}
