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

#include <Resources/Resource.h>

#include <EntityComponentSystem/EntityManager.h>

#include <EntityComponentSystem/Query.h>
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
			float m_depth;
		};

		struct SpriteRenderState
		{
			GLuint m_texture;
			ShaderProgram *m_program;
		};

		typedef std::map<ecs::Entity, SpriteComponent> SpriteComponents;
		typedef std::vector<SpriteDrawInfo> SpriteDrawList;

	public:
		GraphicsManager();
		~GraphicsManager();

	public:
		void initialize();

		void drawAll(double deltaT);
		void postProcess();

		float calculateRatio(int32_t width, int32_t height);
		void setWindowRatio(float ratio);
		void setMainCamera(const CameraSettings &camera);
		void setWindowSize(bedrock::Point2 size);

		void queueDrawSingle(DrawQueueElement *element);

		IFont *getDefaultFont() {return m_font;}

		void cleanup();

	private:
		void drawSpriteComponent(ecs::Entity entity);

		void enqueueSpriteDraw(const SpriteDrawInfo &info);
		void addSpriteToBuffer(const SpriteDrawInfo &info, SpriteBuffer *buffer);
		void addSpriteVertexToBuffer(const bedrock::Vector3 &pos, const bedrock::Vector2 &uv, SpriteBuffer *buffer);

		void drawSpriteList(const SpriteDrawList &list, SpriteRenderState *in_out_state, bool forceCurrentRenderstate = false);
		void flushSpriteBuffer(SpriteBuffer *buffer);

		bool isStateCompatible(const SpriteDrawInfo &info, const SpriteRenderState &state);
		void setCompatibleState(const SpriteDrawInfo &info, SpriteRenderState *out_state);

		void updateCamera();

		Texture &getImageTexture(resources::Resource &imageResource);

		static bool compSortSoftalpha(const SpriteDrawInfo &first, const SpriteDrawInfo &second);

	private:
		ecs::Query m_spriteQuery;

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
		float m_windowRatio;
		CameraSettings m_mainCamera;

		std::vector<ecs::Entity> m_cleanup;

		std::vector<double> m_ftBuffer;
		std::size_t m_ftWrite;

		DrawQueue m_uiQueue;

		std::vector<Texture> m_managedTextures;
	};

}
}
