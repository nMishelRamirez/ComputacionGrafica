#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(const char *path);

// Ajustes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// momento
float deltaTime = 0.0f;	// tiempo entre el fotograma actual y el último fotograma
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: inicializar y configurar
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw creación de ventana
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mishel Ramirez 1752861201", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // decirle a GLFW que capture nuestro mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: cargar todos los punteros de función OpenGL
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configurar el estado global de opengl
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // construir y compilar nuestro shader zprogram
    // ------------------------------------
    Shader lightingShader("shaders/shader_B2T4_materials.vs", "shaders/shader_B2T4_materials.fs");
	Shader lightCubeShader("shaders/shader_B2T4_lightcube.vs", "shaders/shader_B2T4_lightcube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
        //COORDENADAS PARA LA TEXTURA 3
        // posiciones         // normales          // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.3564f, 0.489f, // I 
        0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,  0.160f,  0.489f, // J 
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,  0.160f,  0.661f, // G 
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,  0.160f,  0.661f, // G 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.3564f, 0.661f, // H 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.3564f, 0.489f, // I 

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.489f, // F
        0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.750f, 0.489f, // E
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.750f, 0.661f, // D
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.750f, 0.661f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.661f, // C
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.489f, // F

        //Parte lateral derecha
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 0.552f,  0.660f, // C
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.3564f, 0.660f, // H
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.3564f, 0.511f, // I 
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.3564f, 0.511f, // I 
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 0.552f,  0.489f, // F
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 0.552f,  0.660f, // C

        //Parte lateral izquierda
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.750f, 0.660f, // D
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f, 0.660f, // K
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f, 0.489f, // L
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f, 0.489f, // L
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.750f, 0.489f, // E
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.750f, 0.660f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.271f, // N
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.750f, 0.271f, // M
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.750f, 0.489f, // E
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.750f, 0.489f, // E
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.489f, // F
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.271f, // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.905f, // O 
        0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.750f, 0.905f, // P
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.750f, 0.660f, // D
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.750f, 0.660f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.660f, // C
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.905f, // O
    };

    float vertices1[] = {
        //COORDENADAS PARA LEVI
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.3565f, 0.52f, // I 
        0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.1602f, 0.52f, // J
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.1602f, 0.69f, // G
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.1602f, 0.69f, // G
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.3565f, 0.69f, // H
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.3565f, 0.52f, // I 

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.551f, 0.52f, // F
        0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.749f, 0.52f, // E
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.749f,  0.69f, // D
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.749f,  0.69f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.551f, 0.69f, // C
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.551f, 0.52f, // F

        //Parte lateral izquierda
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.551f, 0.69f, // C 
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.3565f, 0.69f, // H 
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.3565f, 0.52f, // I 
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.3565f, 0.52f, // I 
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.551f, 0.52f, // F 
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.551f, 0.69f, // C 

        //Parte lateral derecha
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.749f,  0.69f, // D
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f,  0.69f, // K
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f,  0.52f, // L
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.946f,  0.52f, // L
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.749f, 0.52f, // E
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.749f,  0.69f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.26f, // N
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.749f, 0.26f, // M
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.749f, 0.51f,  // E
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.749f, 0.51f,  // E
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.51f,  // F
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.26f, // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.551f, 0.90f, // O 
        0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.749f, 0.90f, // P
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.749f, 0.69f, // D
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.749f, 0.69f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.551f, 0.69f, // C
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.551f, 0.90f // O
    };

    float vertices2[] = {
        //COORDENADAS PARA LA TEXTURA DE BAN
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.356f, 0.504f, // I
        0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.152f, 0.504f, // J
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.152f, 0.681f, // G
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.152f, 0.681f, // G
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.356f, 0.681f, // H
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.356f, 0.504f, // I

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.560f, 0.504f, // F
        0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.766f, 0.504f, // E
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.766f, 0.681f, // D
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.766f, 0.681f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.560f, 0.681f, // C
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.560f, 0.504f, // F

        //Parte lateral izquierda
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.560f, 0.681f, // C
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.356f, 0.681f, // H
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.356f, 0.504f, // I
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.356f, 0.504f, // I
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.560f, 0.504f, // F
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.560f, 0.681f, // C

        //Parte lateral derecha
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.765f, 0.681f, // D
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.97f,  0.681f, // K
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.97f,  0.504f, // L
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.97f,  0.504f, // L
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.765f, 0.504f, // E
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.765f, 0.681f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.5604f, 0.2466f, // N
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.767f,  0.2466f, // M
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.767f,  0.505f,  // E
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.767f,  0.505f,  // E
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.560f,  0.5035f, // F
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.560f,  0.2466f, // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.560f, 0.941f, // O 
        0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.766f, 0.941f, // P
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.766f, 0.683f, // D
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.766f, 0.683f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.560f, 0.683f, // C
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.560f, 0.941f, // O
    };

    float vertices3[] = {
        //COORDENADAS PARA LA TEXTURA DE PERSONAJES
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.358f, 0.699f, // J
        0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.644f, 0.699f, // I
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.644f, 0.926f, // H
        0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f,   0.644f, 0.926f, // H
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.358f, 0.926f, // G
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.358f, 0.699f, // J

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.358f, 0.246f,  // F
        0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.644f, 0.246f,  // E
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.644f, 0.4705f, // D
        0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f,   0.644f, 0.4705f, // D
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.358f, 0.4705f, // C
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.358f, 0.246f,  // F

        //Parte lateral derecha
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.358f, 0.4705f, // C
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.074f, 0.4705f, // N
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.074f, 0.246f,  // M
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.074f, 0.246f,  // M
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.358f, 0.246f,  // F
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.358f, 0.4705f, // C

        //Parte lateral izquierda
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.644f,  0.4705f, // D
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.9295f, 0.4705f, // P
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.9295f, 0.246f,  // O
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.9295f, 0.246f,  // O
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.644f,  0.246f,  // E
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.644f,  0.4705f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.358f, 0.019f, // L
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.644f, 0.019f, // K
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.644f, 0.246f, // E
        0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.644f, 0.246f, // E
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.358f, 0.246f, // F
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.358f, 0.019f, // L

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.358f, 0.70f, // C
        0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.644f, 0.70f, // D
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.644f, 0.50f, // I
        0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.644f, 0.50f, // I 
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.358f, 0.50f, // J
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.358f, 0.70f, // C
    };

    // Posiciones espaciales mundiales de nuestros cubos.
    glm::vec3 cubePositions[] = { 
        glm::vec3(0.0f,  0.0f,  0.0f), 
        glm::vec3(1.2f,  2.2f,  -3.4f), 
        glm::vec3(-1.5f, -2.5f, -2.5f), 

        glm::vec3(2.4f, -0.4f, -1.5f), 
        glm::vec3(-1.7f, 3.0f, -0.5f), 
        glm::vec3(1.3f, -2.0f, -0.5f), 

        glm::vec3(2.5f,  2.9f, -2.5f), 
        glm::vec3(1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f, 1.0f, -1.5f), 

        glm::vec3(3.1f,  2.4f, -4.1f), 
        glm::vec3(1.9f,  3.9f, -4.5f),
        glm::vec3(0.9f,  0.7f, 0.4f),
    }; 

    // Configuración de la primera figura
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //atributo de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configuración de la segunda figura
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //atributo de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configuración de la tercera figura
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //atributo de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configuración de la cuarta figura
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //atributo de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


 // configure el VAO de la luz (VBO permanece igual; los vértices son los mismos para el objeto de luz que también es un cubo 3D)
 unsigned int lightCubeVAO;
 glGenVertexArrays(1, &lightCubeVAO);
 glBindVertexArray(lightCubeVAO);
 // tenga en cuenta que actualizamos la zancada del atributo de posición de la lámpara para reflejar 
 // los datos actualizados del búfer
 glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);

 glBindBuffer(GL_ARRAY_BUFFER, VBO1);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);

 glBindBuffer(GL_ARRAY_BUFFER, VBO2);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);

 glBindBuffer(GL_ARRAY_BUFFER, VBO3);
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);

    // cargar texturas (ahora usamos una función de utilidad para mantener el código más organizado)
    // -----------------------------------------------------------------------------
    stbi_set_flip_vertically_on_load(true); // Dile a stb_image.h que voltee las texturas cargadas en el eje y. 
    unsigned int diffuseMap = loadTexture("textures/Texture3.png");
    unsigned int diffuseMap1 = loadTexture("textures/TextureLevi.png");
    unsigned int diffuseMap2 = loadTexture("textures/TextureBan.png");
    unsigned int diffuseMap3 = loadTexture("textures/TexturePersonajes.png");
	
    // configuracion shader 
    // --------------------
    lightingShader.use(); 
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.diffuse1", 1);
    lightingShader.setInt("material.diffuse2", 2);
    lightingShader.setInt("material.diffuse3", 3);

 // render loop
 // -----------
 while (!glfwWindowShouldClose(window))
 {
     // lógica de tiempo por fotograma
     // --------------------
     float currentFrame = glfwGetTime();
     deltaTime = currentFrame - lastFrame;
     lastFrame = currentFrame;

     // entrada
     // -----
     processInput(window);

     // render
     // ------
     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // asegúrese de activar el sombreador al configurar uniformes/dibujar objetos
     lightingShader.use();
     lightingShader.setVec3("light.position", lightPos);
     lightingShader.setVec3("viewPos", camera.Position);

	// propiedades de la luz
     lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
     lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
     lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

     // propiedades de los materiales
     lightingShader.setFloat("material.shininess", 64.0f);

     // transformaciones de vista/proyección
     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
     glm::mat4 view = camera.GetViewMatrix();
     lightingShader.setMat4("projection", projection);
     lightingShader.setMat4("view", view);

     // transformación mundial
     glm::mat4 model = glm::mat4(1.0f);
     

     for (unsigned i = 0; i < 12; i++) {
         glm::mat4 model = glm::mat4(1.0f);
         glm::vec3 newPos = cubePositions[i];

         if (i % 4 == 0) {
             
             glBindTexture(GL_TEXTURE_2D, diffuseMap);
             glBindVertexArray(VAO); 
             glActiveTexture(GL_TEXTURE0);

             model = glm::translate(model, newPos);
             float angle = 50.0f * (i + 1) * currentFrame * 0.009; 
             model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 2.0f, 0.0f)); 
             model = glm::translate(model, newPos);
             lightingShader.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36); 

         
         }
         if (i % 4 == 1) {
             
             glBindTexture(GL_TEXTURE_2D, diffuseMap1); // Selecciona la textura 1
             glBindVertexArray(VAO1); // Activa el VAO correspondiente
             glActiveTexture(GL_TEXTURE0);
             model = glm::translate(model, newPos);
             float angle = 75.0f * (i + 1) * currentFrame * 0.009; 
             model = glm::rotate(model, glm::radians(angle), glm::vec3(2.5f, 0.0f, 0.0f));
             model = glm::translate(model, newPos);
             lightingShader.setMat4("model", model); 
             glDrawArrays(GL_TRIANGLES, 0, 36);

         }
         if (i % 4 == 2) {
             
             glBindTexture(GL_TEXTURE_2D, diffuseMap2); // Selecciona la textura 1
             glBindVertexArray(VAO2); // Activa el VAO correspondiente
             glActiveTexture(GL_TEXTURE0);
             model = glm::translate(model, newPos);
             float angle = 150.0f * (i + 1) * currentFrame * 0.009; 
             model = glm::rotate(model, glm::radians(angle), glm::vec3(2.5f, 0.0f, 1.0f));
             model = glm::translate(model, newPos);
             lightingShader.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);

         }
         if (i % 4 == 3) {
             
             glBindTexture(GL_TEXTURE_2D, diffuseMap3); // Selecciona la textura 1
             glBindVertexArray(VAO3); // Activa el VAO correspondiente
             glActiveTexture(GL_TEXTURE0);
             model = glm::translate(model, newPos);
             float angle = 175.0f * (i + 1) * currentFrame * 0.001; 
             model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 3.0f));
             model = glm::translate(model, newPos);
             lightingShader.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);

         }  
         
     }


     lightPos.x = 12.5f * sin(currentFrame);
     lightPos.y = 9.5f * cos(currentFrame) ;
     lightPos.z = -20.5f * sin(currentFrame);

     // dibuja también el objeto lámpara
     lightCubeShader.use();
     lightCubeShader.setMat4("projection", projection);
     lightCubeShader.setMat4("view", view);
     model = glm::mat4(1.0f);
     model = glm::translate(model, lightPos);
     model = glm::scale(model, glm::vec3(0.9f)); // a smaller cube
     lightCubeShader.setMat4("model", model);

     glBindVertexArray(lightCubeVAO);
     glDrawArrays(GL_TRIANGLES, 0, 36);


     // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
     // -------------------------------------------------------------------------------
     glfwSwapBuffers(window);
     glfwPollEvents();
 }

 // optional: de-allocate all resources once they've outlived their purpose:
 // ------------------------------------------------------------------------
 glDeleteVertexArrays(1, &lightCubeVAO);
 glDeleteVertexArrays(1, &VAO); 
 glDeleteBuffers(1, &VBO); 
 glDeleteVertexArrays(1, &VAO1); 
 glDeleteBuffers(1, &VBO1); 
 glDeleteVertexArrays(1, &VAO2); 
 glDeleteBuffers(1, &VBO2); 
 glDeleteVertexArrays(1, &VAO3);  
 glDeleteBuffers(1, &VBO3); 

 // glfw: terminate, clearing all previously allocated GLFW resources.
 // ------------------------------------------------------------------
 glfwTerminate();
 return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: cada vez que cambia el tamaño de la ventana (por el sistema operativo o el cambio de tamaño del usuario), se ejecuta esta función de devolución de llamada
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // asegúrese de que la ventana gráfica coincida con las dimensiones de la nueva ventana; Tenga en cuenta que el ancho y 
    // la altura será significativamente mayor que la especificada en las pantallas retina.
    glViewport(0, 0, width, height);
}


// glfw: cada vez que se mueve el mouse, se llama esta devolución de llamada
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // invertido ya que las coordenadas y van de abajo hacia arriba

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: cada vez que se desplaza la rueda del mouse, se llama esta devolución de llamada
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
// función de utilidad para cargar una textura 2D desde un archivo
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}