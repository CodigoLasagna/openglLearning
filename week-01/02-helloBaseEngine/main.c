#include "./source/engine/engine.h"
#include <GLFW/glfw3.h>
int main()
{
	Tconfig config;
	GLFWwindow *window;
	int i;
	Tobject quad;
	Tobject cube;
	Tobject cube2;
	Tobject cube3;
	Tcamera camera;
	float delta, lastFrame, currentFrame; 
	unsigned int shader, texture, green, anotherShader;
	float step = 0.001;
	vec3 angle = {0.01f, 0.8f, 0.4f};
	window = prepareGLFW(400, 400);
	lastFrame = 0;
	
	prepareGLEW();
	createShader(&shader, "./source/shaders/perspective.vert", "./source/shaders/tex.frag");
	createShader(&green, "./source/shaders/perspective.vert", "./source/shaders/green.frag");
	createShader(&anotherShader, "./source/shaders/perspective.vert", "./source/shaders/magenta.frag");
	createTexture(&texture, "./source/images/crate_03.png");
	
	instance_create_cube(&cube, -1.0f, 0.4f, 0.0f, 100, 100, 100, 2);
	instance_create_cube(&cube2, 0.8f, 0.4f, 0.0f, 100, 100, 100, 2);
	instance_create_cube(&cube3, 0.9f, -0.5f, 0.0f, 50, 50, 50, 1);
	instance_create_quad(&quad, 0.0f, -1.0, 0, 100, 100, 2);
	
	init_camera(&camera, 1366, 768, 1);
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &(camera.width), &(camera.height));
		currentFrame = (float)glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, &config);
		
		glClearColor(0.050f, 0.046f, 0.120f, 1.0f);
		glViewport(0, 0, camera.nWidth, camera.nHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, camera.frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		run_camera(&camera, window, delta);
		
		
		angle[2] = 1.0;
		glm_rotate(cube.model, glm_rad(1), angle);
		angle[2] = 0.0;
		glm_rotate(quad.model, glm_rad(0.4), angle);
		instance_draw(cube, &green, camera, texture, 0);
		instance_draw(cube2, &shader, camera, texture, 1);
		instance_draw(quad, &green, camera, texture, 0);
		
		angle[0] = 0.0;
		angle[1] = -(sin(step))/100;
		step += 3.141516/100;
		glm_translate(cube3.model, angle);
		angle[0] = 0.01;
		angle[1] = 0.0;
		
		instance_draw(cube3, &anotherShader, camera, texture, 0);
		
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, camera.nWidth, camera.nHeight, 0, 0, camera.width, camera.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT , GL_NEAREST);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
