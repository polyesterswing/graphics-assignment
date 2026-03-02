#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>
#include <vector>

#include <shader.h>
#include <camera.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <mesh.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f), 5.0f);
bool panning = false;
bool dragging = false;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ikeloa", NULL, NULL);
	if (window == NULL) {
		std::println("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::println("Failed to initialize GLAD");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

	std::vector<float> vertices = loadOBJ("res/wrench.obj");

	unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

	ourShader.use();

	bool show_wireframe = false;
	bool show_isoline = false;
	float isoline_value = 0.0f;
	float min_value = 0.0f;
	float max_value = 1.0f;
	float disp_value = 0.0f;
	int selected_colormap = 0;

	// https://www.shadertoy.com/view/WlfXRN
	const char* colormaps[] = {
		"Viridis",
		"Plasma",
		"Magma",
		"Inferno",
	};

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		glm::mat4 view = camera.getViewMatrix();
		ourShader.setMat4("view", view);
		ourShader.setBool("uShowWireframe", show_wireframe);
		ourShader.setInt("uColormap", selected_colormap);
		ourShader.setFloat("uMinVal", min_value);
		ourShader.setFloat("uMaxVal", max_value);
		ourShader.setFloat("uShowIsoline", show_isoline);
		ourShader.setFloat("uIsoline", isoline_value);
		ourShader.setFloat("uDisp", disp_value);

		glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate( model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		{

			ImGui::SetNextWindowSize(ImVec2(320, 173), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
			ImGui::Begin("Rendering Options", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::Checkbox("Show Wireframe", &show_wireframe);
			ImGui::Checkbox("Show Isoline", &show_isoline);
			ImGui::Combo("Colormap", &selected_colormap, colormaps, IM_ARRAYSIZE(colormaps));

			ImGui::DragFloatRange2(
					"Scalar Range",
					&min_value,
					&max_value,
					0.01f,
					0.0f,
					1.0f,
					"Min: %.2f",
					"Max: %.2f"
				);

			ImGui::SliderFloat("Isoline", &isoline_value, 0.0f, 1.0f);
			ImGui::SliderFloat("Displacement", &disp_value, 0.0f, 1.0f);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float dx = xpos - lastX;
	float dy = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	if (dragging) camera.processMouseMovement(dx, dy);
	if (panning) camera.processMousePan(dx, dy);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;

	camera.processMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		dragging = (action == GLFW_PRESS);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		panning = (action == GLFW_PRESS);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		firstMouse = true;
	}
}
