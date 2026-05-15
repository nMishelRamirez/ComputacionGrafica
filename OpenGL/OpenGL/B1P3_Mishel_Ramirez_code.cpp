#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <learnopengl/shader_s.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Configuración de dimensiones de la ventana
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // Inicialización y configuración de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    // Creación de la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B1P3 Ramirez Mishel", NULL, NULL);
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

    // Cargar y compilar los shaders
    Shader ourShader("shaders/shader_B1P3.vs", "shaders/shader_B1P3.fs");

    // Definición de los datos de vértices y configuración de atributos de vértices
    float vertices[] = {
        // Triangulo 1   
        // Color verde claro
        -0.41f,  0.92f, 0.0f, 0.6f, 0.80f, 0.196f,// C
        -0.07f,  0.58f, 0.0f, 0.6f, 0.80f, 0.196f,// D
        -0.75f,  0.58f, 0.0f, 0.6f, 0.80f, 0.196f,// E

        //Triangulo 2
        //Color Rojo
        -0.4f,   0.58f, 0.0f, 0.996f, 0.4f, 0.4f,// G
        -0.07f,  0.58f, 0.0f, 0.996f, 0.4f, 0.4f,// D
        -0.4f,   0.24f, 0.0f, 0.996f, 0.4f, 0.4f,// F

        // Triangulo 3
        //Color Rojo
        -0.07f,  0.58f, 0.0f, 0.996f, 0.4f, 0.4f,// D
        -0.07f,  0.24f, 0.0f, 0.996f, 0.4f, 0.4f,// H
        -0.4f,   0.24f, 0.0f, 0.996f, 0.4f, 0.4f,// F

        // Triangulo 4
        //Color Morado
        -0.07f,  0.24f, 0.0f, 0.4f, 0.4f, 0.8f,   // H
        -0.4f,   0.24f, 0.0f, 0.4f, 0.4f, 0.8f,  // F
        -0.4f,  -0.09f, 0.0f, 0.4f, 0.4f, 0.8f,  // I

        // Triangulo 5
        //Color Morado
        -0.4f,   0.24f, 0.0f, 0.4f, 0.4f, 0.8f,  // F
        -0.4f,  -0.09f, 0.0f, 0.4f, 0.4f, 0.8f,  // I
        -0.73f, -0.09f, 0.0f, 0.4f, 0.4f, 0.8f,  // J

        // Triangulo 6
        //Color Amarillo
        -0.73f, -0.09f, 0.0f,  1.0f, 0.8f, 0.0f, // J
        -0.26f, -0.56f, 0.0f, 1.0f, 0.8f, 0.0f,  // K
        0.2f,   -0.09f, 0.0f, 1.0f, 0.8f, 0.0f,  // L

        // Triangulo 7
        //Color Verde Obscuro
        -0.26f, -0.56f, 0.0f, 0.196f, 0.8f, 0.4f,// K
        0.2f,   -0.09f, 0.0f, 0.196f, 0.8f, 0.4f,// L
        0.67f,  -0.56f, 0.0f, 0.196f, 0.8f, 0.4f,// M

        // Triangulo 8
        //Color Azul
        -0.26f, -0.56f, 0.0f, 0.2f, 0.6f, 1.0f,// K
        -0.26f, -0.9f,  0.0f, 0.2f, 0.6f, 1.0f,// N
        -0.6f,  -0.9f,  0.0f, 0.2f, 0.6f, 1.0f,// O

        // Triangulo 9
        //Color Naranja
        0.47f,  -0.65f, 0.0f, 0.996f, 0.596f, 0.2f,// P
        0.38f,  -0.98f, 0.0f, 0.996f, 0.596f, 0.2f,// Q
        0.14f,  -0.56f, 0.0f, 0.996f, 0.596f, 0.2f,// R
    };

    // Generación de los VBO, VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO); 

    glBindVertexArray(VAO); // Vincular el VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vincular el VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copiar los datos de los vértices en el buffer

    // Configuración de los atributos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Atributo posición
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Atributo color
    glEnableVertexAttribArray(1);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window)) {
        processInput(window); 

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Color de fondo
        glClear(GL_COLOR_BUFFER_BIT); // Limpiar el buffer de color

        // Calculo de tiempo
        float calculoTiempo = glfwGetTime();
        ourShader.use(); // Usar el shader program
        int almacenamientoTiempo = glGetUniformLocation(ourShader.ID, "tiempo"); // Obtener la ubicación del 'tiempo'
        glUniform1f(almacenamientoTiempo, calculoTiempo); // Configurar el valor del 'tiempo'

        glBindVertexArray(VAO); // Vincular el VAO
        glDrawArrays(GL_TRIANGLES, 0, 27); // Dibujar los triángulos
        glfwSwapBuffers(window); // Intercambiar los buffers
        glfwPollEvents(); // Procesar eventos
    }

    // Limpiar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate(); // Terminar GLFW
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Cerrar la ventana si se presiona la tecla ESC
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Ajustar el viewport al nuevo tamaño de la ventana
}

