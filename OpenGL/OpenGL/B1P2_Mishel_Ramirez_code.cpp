#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow* window); 

//Configuración de dimensiones de la ventana
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSources[] = {
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.6f, 0.80f, 0.196f, 1.0f);\n" //Color Verde Claro
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.996f, 0.4f, 0.4f, 1.0f);\n" //Color Rojo
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.4f, 0.4f, 0.8f, 1.0f);\n" //Color Morado
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.8f, 0.0f, 1.0f);\n" //Color Amarillo
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.196f, 0.8f, 0.4f, 1.0f);\n" //Color Verde Obscuro
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.2f, 0.6f, 1.0f, 1.0f);\n" //Color Azul
    "}\n\0",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.996f, 0.596f, 0.2f, 1.0f);\n" //Color Naranja
    "}\n\0"
};

int main() {
    // Inicialización y configuración de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creación de la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG Exercise C", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: Carga de los punteros de las funciones de OpenGL 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Creación y compilación del vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Creación y compilación de los fragment Shaders
    unsigned int fragmentShaders[7];
    unsigned int shaderPrograms[7];

    // Iteración para crear y compilar los fragment Shaders
    for (int i = 0; i < 7; ++i) {
        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
        glCompileShader(fragmentShaders[i]);

        shaderPrograms[i] = glCreateProgram();
        glAttachShader(shaderPrograms[i], vertexShader);
        glAttachShader(shaderPrograms[i], fragmentShaders[i]);
        glLinkProgram(shaderPrograms[i]);
    }
    // Eliminación del vertex Shader después de ser vinculado
    glDeleteShader(vertexShader);

    // Definición de los datos de vértices y configuración de atributos de vértices
    float vertices[] = {
        // posiciones      
        -0.41f,  0.92f, 0.0f, // C
        -0.07f,  0.58f, 0.0f, // D
        -0.75f,  0.58f, 0.0f, // E
        -0.4f,   0.24f, 0.0f, // F
        -0.4f,   0.58f, 0.0f, // G
        -0.07f,  0.24f, 0.0f, // H
        -0.4f,  -0.09f, 0.0f, // I
        -0.73f, -0.09f, 0.0f, // J
        -0.26f, -0.56f, 0.0f, // K
        0.2f,   -0.09f, 0.0f, // L
        0.67f,  -0.56f, 0.0f, // M
        -0.26f, -0.9f,  0.0f, // N
        -0.6f,  -0.9f,  0.0f, // O
        0.47f,  -0.65f, 0.0f, // P
        0.38f,  -0.98f, 0.0f, // Q
        0.14f,  -0.56f, 0.0f, // R
    };

    // Definición de los índices de los triángulos
    unsigned int indices[] = {
        0, 1, 2, // Triángulo 1 (Verde) 
        1, 3, 4, // Triángulo 2 (Rojo)        
        1, 5, 3, // Triángulo 3 (Rojo)       
        3, 6, 7, // Triángulo 4 (Morado)       
        3, 6, 5, // Triángulo 5 (Morado)
        8, 9, 7, // Triángulo 6 (Amarillo) 
        8, 9, 10, // Triángulo 7 (Verde Obscuro)
        8, 11, 12, // Triángulo 8 (Azul)
        13, 14, 15, // Triángulo 9 (Naranja)
    };

    // Generación de los VBO, VAO y EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //Color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // Renderización de cada triángulo con su respectivo color
        // Verde
        glUseProgram(shaderPrograms[0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        // Rojo
        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        // Morado
        glUseProgram(shaderPrograms[2]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(unsigned int)));
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        // Amarillo
        glUseProgram(shaderPrograms[3]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(15 * sizeof(unsigned int)));

        // Verde Obscuro
        glUseProgram(shaderPrograms[4]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));

        // Azul
        glUseProgram(shaderPrograms[5]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(21 * sizeof(unsigned int)));

        // Naranja
        glUseProgram(shaderPrograms[6]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    for (int i = 0; i < 7; ++i) {
        glDeleteProgram(shaderPrograms[i]);
        glDeleteShader(fragmentShaders[i]);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
