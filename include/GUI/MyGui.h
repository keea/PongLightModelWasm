#pragma once


#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdio.h>

class MyGui
{
public:
	MyGui(GLFWwindow* window) // Setup Dear ImGui context
	{
		#ifdef __EMSCRIPTEN__
		const char* glsl_version = "#version 100";
		#else
		const char* glsl_version = "#version 330";
		#endif

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}; 

	void StartFrame() // feed inputs to dear imgui, start new frame
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}; 

	virtual void CreateWindow()
	{
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();
	};

	void Rendering() //Render dear imgui into screen
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	};

	void CleanUp() // Cleanup
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	};
};
