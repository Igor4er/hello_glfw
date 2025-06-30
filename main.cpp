#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello GLFW", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", reinterpret_cast<const char *>(glewGetErrorString(err)));
      }
    fprintf(stdout, "Status: Using GLEW %s\n", reinterpret_cast<const char *>(glewGetString(GLEW_VERSION)));

    static constexpr GLfloat g_vertex_buffer_data[] = {
    	// POS   --------------   COLOR
        -0.4f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.6f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.6f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
     };

	static constexpr GLfloat g_vertex_buffer_data2[] = {
		-0.8f, -0.7f, 0.0f,
		0.6f,  0.7f, 0.0f,
		-0.8f, 0.7f, 0.0f
	 };

	static constexpr GLint indices[] = {
		0, 1, 2,
		0, 2, 3
	 };

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   6 * sizeof(float),                  // stride
	   (void*)0           // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(
	   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   6 * sizeof(float),                  // stride
	   (void*)(3* sizeof(float))           // array buffer offset
	);
	glEnableVertexAttribArray(1);


	GLuint VertexArray2;
	glGenVertexArrays(1, &VertexArray2);
	glBindVertexArray(VertexArray2);

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   nullptr           // array buffer offset
	);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint programID = LoadShaders( "shader.glsl", "shader.glslf" );
	GLuint program2 = LoadShaders( "shader2.glsl", "shader2.glslf" );


	GLint tr_offset = glGetUniformLocation(programID, "offset");
	// GLint colorLocation2 = glGetUniformLocation(program2, "inputColor");
	GLint offs_xy = glGetUniformLocation(program2, "offs_xy");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // int state = glfwGetKey(window, GLFW_KEY_E);
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        // if (state == GLFW_PRESS){
        //     glClearColor(0.0f, 0.6f, 0.2f, 1.0f);
        // }
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    	glBindVertexArray(VertexArrayID);
    	glUseProgram(programID);
    	float timeValue = glfwGetTime();
    	float offset_value = (sin((timeValue*2) + 25) / 2.0f) + (cos(timeValue*3) / 1.3f);
    	glUniform1f(tr_offset, offset_value);
    	glDrawArrays(GL_TRIANGLES, 0, 3);


    	glBindVertexArray(VertexArray2);
    	glUseProgram(program2);
    	// float greenValue = (sin(timeValue*2) / 2.0f) + 0.5f;
    	float offs_x = (sin(timeValue) / 3.0f) + 0.5;
    	float offs_y = (cos(timeValue*7) / 1.5f) - 0.2f;

    	glUniform2f(offs_xy, offs_x, offs_y);

    	// if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    	// 	glUniform3f(colorLocation2, 6.0f, greenValue, 0.0f);
    	// } else {
    	// 	glUniform3f(colorLocation2, 0.0f, greenValue, 0.4f);
    	// }
    	glDrawArrays(GL_TRIANGLES, 0, 3);


        // Draw the triangle !
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle
        // glDisableVertexAttribArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}