#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


// Vertex shader source code
const GLchar* vertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;

	out vec3 vertexColor;	

    void main()
    {
        gl_Position = vec4(position.x, position.y, 0.0, 1.0);
		vertexColor = color;
    }
)";

// Fragment shader source code
const GLchar* fragmentShaderSource = R"(
    #version 450 core
	in vec3 vertexColor;
	out vec4 fragColor;
    void main()
    {
        fragColor = vec4(vertexColor, 1.0f);
    }
)";

// Vertex array object (VAO) is an object that stores the configuration of vertex data for rendering
GLuint VAO;

// Shader program. A shader program is an object capable of combining shaders
GLuint shaderProgram;

GLuint screenWidth, screenHeight, windowWidth, windowHeight;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}

// Initialize OpenGL state
void init()
{
    // Create vertex array object (VAO)
    glGenVertexArrays(1, &VAO);															//Generates 1 vertex array to the direction of the stored values, in this case VAO
    glBindVertexArray(VAO);																//Is used to bind a VAO in the current state of rendering

    // Triangle vertex data
	GLfloat vertices[] = {

		-1.0f,  0.0f,
		 1.0f,  0.0f,

		 0.0f,  1.0f,
		 0.0f, -1.0f

	};


	GLfloat colors[] = {
	
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

    // Create vertex buffer object (VBO)
    GLuint VBO;																			//VBO stores vertex data such as (coordinates, color, texture...)
    glGenBuffers(1, &VBO);																//Generates 1 buffer pointing to the direction of the stored values, in this case VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);													//Bind a buffer to an specific binding point
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);			//Used to transfer data to the buffer

	// Specify vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);	//Specify the layout and format of vertex data stored in VBO
    glEnableVertexAttribArray(0);														//Enable attributes for use during rendering

	GLuint colorVBO;
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

   	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	
    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);													//Unbinds a buffer object from the current binding point
    glBindVertexArray(0);																//Unbinds a vertex array object from the current binding point
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Render the scene
void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);												
    glClear(GL_COLOR_BUFFER_BIT);														//Clear the screen

    // Use the shader program
    glUseProgram(shaderProgram);														//Activates the program for use
    glBindVertexArray(VAO);																//Binds a VAO to the current state of rendering
    glDrawArrays(GL_LINES, 0, 4);													//Draw the array of data
	glDrawArrays(GL_LINES, 4, 4);
    glBindVertexArray(0);																//Unbind the vertex array object from the current binding point
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Triangle", nullptr, nullptr);		//Creates a GLFW window
    if (!window)																					//Checks for error in window initialization
    {
        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the created window the current context
    glfwMakeContextCurrent(window);														//OpenGL commands are directed towars a Current

    // Initialize GLEW
    glewExperimental = GL_TRUE;															
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Set viewport dimensions
	 glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);								//Creates a shader of the specified type, in this case: is 'vertex shader'
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);						//Specifies the shader source code to be compiled
    glCompileShader(vertexShader);														//Compiles the shader(vertex shader) code

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);							//Creates a shader of the specified type, in this case: is 'fragment shader'
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);					//Specifies the shader source code to be compiled
    glCompileShader(fragmentShader);													//Compiles the shader(fragment shader) code

    // Create shader program and link shaders
    shaderProgram = glCreateProgram();													//Creates a shader program objects, capable of combining shader
    glAttachShader(shaderProgram, vertexShader);										//Attachs the vertex shader into the program
    glAttachShader(shaderProgram, fragmentShader);										//Attachs the fragment shader into the program
    glLinkProgram(shaderProgram);														//Makes sure that every links between attachments are correct

    // Clean up shader objects
    glDeleteShader(vertexShader);														//Delete vortex shader as it is already into the program and is no longer needed
    glDeleteShader(fragmentShader);														//Delete fragment shader as it is already into the program and is no longer needed

    // Call initialization function
    init();

    // Main loop
    while (!glfwWindowShouldClose(window))												//Determinates if the window shloud be closed or not
    {
        // Poll events
        glfwPollEvents();																//Checks for incoming messages from the operative system

        // Call display function
        display();																		//Calls display function

        // Swap the buffers
        glfwSwapBuffers(window);														//Redraws the contents to the user
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();																	//Shut down

    return 0;
}
