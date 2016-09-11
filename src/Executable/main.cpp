#include "../Core/Engine.h"
#include "../Core/HTTPEventStreamBuf.h"
#include "../SDL/EventHandler.h"
#include "../SDL/Window.h"
#include "../SDL/Keyboard.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Debug.h"
#include "../Renderer/internal/Buffer.h"
#include "../Renderer/internal/ProgramPipeline.h"
#include "../Renderer/internal/VertexFormat.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <experimental/array>
#include <glbinding/gl45ext/gl.h>

using namespace std::literals;
using namespace ge::Core::literals;

struct ExitOnEscape
{
	void cycle([[maybe_unused]] ge::Core::EntityBase &e, [[maybe_unused]] float dt)
	{
		if(ge::SDL::keyboard[SDL_SCANCODE_ESCAPE])
		{
			std::exit(EXIT_SUCCESS);
		}
	}
};

struct SomeEntity : public ge::Core::EntityBase
{
	std::function<void()> drawHook;

	void update([[maybe_unused]] float dt) override
	{

	}

	void onMessage(uint64_t code, void *) override
	{
		switch(code)
		{
			case "draw"_msg: drawHook();
		}
	}
};


auto main() -> int
{
	auto root = new SomeEntity{};
	auto window = ge::SDL::Window{"Hello World!", {1280, 720}, ge::SDL::VSync::OFF};
	auto engine = ge::Core::Engine
	<
		ge::gl4::Renderer<decltype(window)>,
		ExitOnEscape,
		ge::SDL::EventHandler
	>
	{
		std::move(root),
		{window},
		{},
		{}
	};

	boost::asio::io_service ioService{};
	ge::Core::HTTPEventStreamBuf ev{ioService, ge::Core::HTTPEventStreamBuf::Port{10000}};
	std::ostream oev{&ev};
	auto netThread = std::thread{[&]
	{
		while(true)
		{
			try
			{
				ioService.run();
				break;
			}
			catch (std::exception &e)
			{
				std::cout << "asio exception: " << e.what() << '\n';
			}
		}
	}};

	ge::gl4::setGLDebugOutputStream(std::cout);
	ge::gl4::enableGLDebugOuput();

	ge::gl4::glEnable(gl::GL_CONSERVATIVE_RASTERIZATION_NV);
	auto err = ge::gl4::glGetError();
	std::cout << (err == 0) << "\n"; 

	auto tri = std::experimental::make_array
	(
		lmi::vec2{-0.5, -0.5},
		lmi::vec2{0.5, -0.5},
		lmi::vec2{0.0, 0.5}
	);
	auto idx = std::experimental::make_array<uint16_t>
	(
		0, 1, 2
	);
	ge::gl4::Buffer buf{tri.size() * sizeof(*tri.data()), tri.data()};
	ge::gl4::Buffer idxBuf{idx.size() * sizeof(*idx.data()), idx.data()};

	ge::gl4::ProgramPipeline pipeline;
	try
	{
		auto simpleVShader = std::make_shared<ge::gl4::Shader>(
			ge::gl4::Shader::Type::Vertex,
			R"(
				#version 450 core

				out gl_PerVertex
				{
					vec4 gl_Position;
					//float gl_PointSize;
					//float gl_ClipDistance[];
				};

				in vec2 inPos;
				out vec2 pos;

				void main(void)
				{
					pos = inPos;
					gl_Position = vec4(inPos, 0, 1);
				}
		)"
		);

		auto simpleFShader = std::make_shared<ge::gl4::Shader>(
			ge::gl4::Shader::Type::Fragment,
			R"(
				#version 450 core

				in vec2 pos;
				out vec4 color;

				void main(void)
				{
					color = vec4(1);
				}
		)"
		);

		pipeline.attach(simpleVShader);
		pipeline.attach(simpleFShader);
	}
	catch(std::exception &e)
	{
		std::cout << "--- Shader Linking Error ---\n" << e.what() << "\n";
		std::exit(-1);
	}

	root->drawHook = [&]
	{
		pipeline.use();
		ge::gl4::glDrawElements(gl::GL_LINE_LOOP, 3, gl::GL_UNSIGNED_SHORT, nullptr);
	};

	auto vFormat = ge::gl4::VertexFormat{};
	vFormat.setAttributeFormat(0, 2, gl::GL_FLOAT, false, 0);
	vFormat.setAttributeBinding(0, 0);
	vFormat.bindBufferAsAttribute(buf, 0, 0, sizeof(lmi::vec2));
	vFormat.setAttributeEnabled(0, true);
	vFormat.bindIndexBuffer(idxBuf);
	vFormat.use();

	auto res = engine.run();
	ioService.stop();
	return res;
}
