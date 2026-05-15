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
void processInput(GLFWwindow* window);

// Ajustes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// momento
float deltaTime = 0.0f;	// tiempo entre el fotograma actual y el último fotograma
float lastFrame = 0.0f;

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
    glEnable(GL_DEPTH_TEST);  

    // construir y compilar el shader program
    // ------------------------------------
    Shader ourShader("shaders/shader_B2T3.vs", "shaders/shader_B2T3.fs");

    // configurar datos de vértice (y búfer(es)) y configurar atributos de vértice
    // -----------------------------------------------------------------
    float vertices[] = {
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,   0.3565f, 0.49f, // I 
        0.5f, -0.5f,  -0.5f,   0.1602f, 0.49f, // J
        0.5f,  0.5f,  -0.5f,   0.1602f, 0.66f, // G
        0.5f,  0.5f,  -0.5f,   0.1602f, 0.66f, // G
        -0.5f,  0.5f, -0.5f,   0.3565f, 0.66f, // H
        -0.5f, -0.5f, -0.5f,   0.3565f, 0.49f, // I 
        
        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,   0.5525f, 0.49f, // F
        0.5f, -0.5f,   0.5f,   0.7492f, 0.49f, // E
        0.5f,  0.5f,   0.5f,   0.749f,  0.66f, // D
        0.5f,  0.5f,   0.5f,   0.749f,  0.66f, // D
        -0.5f,  0.5f,  0.5f,   0.5525f, 0.66f, // C
        -0.5f, -0.5f,  0.5f,   0.5525f, 0.49f, // F
        
        //Parte lateral izquierda
        -0.5f,  0.5f,  0.5f,  0.5525f, 0.66f, // C 
        -0.5f,  0.5f, -0.5f,  0.3565f, 0.66f, // H 
        -0.5f, -0.5f, -0.5f,  0.3565f, 0.49f, // I 
        -0.5f, -0.5f, -0.5f,  0.3565f, 0.49f, // I 
        -0.5f, -0.5f,  0.5f,  0.5525f, 0.49f, // F 
        -0.5f,  0.5f,  0.5f,  0.5525f, 0.66f, // C 
        
        //Parte lateral derecha
        0.5f,  0.5f,  0.5f,  0.749f,  0.66f, // D
        0.5f,  0.5f, -0.5f,  0.946f,  0.66f, // K
        0.5f, -0.5f, -0.5f,  0.946f,  0.49f, // L
        0.5f, -0.5f, -0.5f,  0.946f,  0.49f, // L
        0.5f, -0.5f,  0.5f,  0.7492f, 0.49f, // E
        0.5f,  0.5f,  0.5f,  0.749f,  0.66f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.5525f, 0.247f, // N
        0.5f,  -0.5f, -0.5f,  0.7486f, 0.247f, // M
        0.5f,  -0.5f,  0.5f,  0.7492f, 0.49f,  // E
        0.5f,  -0.5f,  0.5f,  0.7492f, 0.49f,  // E
        -0.5f, -0.5f,  0.5f,  0.5525f, 0.49f,  // F
        -0.5f, -0.5f, -0.5f,  0.5525f, 0.247f, // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.5525f, 0.905f, // O 
        0.5f,   0.5f, -0.5f,  0.749f,  0.905f, // P
        0.5f,   0.5f,  0.5f,  0.749f,  0.66f, // D
        0.5f,   0.5f,  0.5f,  0.749f,  0.66f, // D
        -0.5f,  0.5f,  0.5f,  0.5525f, 0.66f, // C
        -0.5f,  0.5f, -0.5f,  0.5525f, 0.905f // O
    };

    float vertices1[] = {
        //COORDENADAS PARA LA TEXTURA 3
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,   0.3499f, 0.512f, // I 
        0.5f, -0.5f,  -0.5f,   0.1495f, 0.512f, // J 
        0.5f,  0.5f,  -0.5f,   0.1499f, 0.688f, // G 
        0.5f,  0.5f,  -0.5f,   0.1499f, 0.688f, // G 
        -0.5f,  0.5f, -0.5f,   0.3499f, 0.688f, // H 
        -0.5f, -0.5f, -0.5f,   0.3499f, 0.512f, // I 

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,   0.549f, 0.513f, // F
        0.5f, -0.5f,   0.5f,   0.751f, 0.513f, // E
        0.5f,  0.5f,   0.5f,   0.751f, 0.688f, // D
        0.5f,  0.5f,   0.5f,   0.751f, 0.688f, // D
        -0.5f,  0.5f,  0.5f,   0.549f, 0.688f, // C
        -0.5f, -0.5f,  0.5f,   0.549f, 0.513f, // F

        //Parte lateral derecha
        -0.5f,  0.5f,  0.5f,  0.549f, 0.688f,  // C
        -0.5f,  0.5f, -0.5f,  0.3499f, 0.688f, // H
        -0.5f, -0.5f, -0.5f,  0.3499f, 0.512f, // I
        -0.5f, -0.5f, -0.5f,  0.3499f, 0.512f, // I
        -0.5f, -0.5f,  0.5f,  0.549f, 0.513f,  // F
        -0.5f,  0.5f,  0.5f,  0.549f, 0.688f,  // C

        //Parte lateral izquierda
        0.5f,  0.5f,  0.5f,  0.751f,  0.688f, // D
        0.5f,  0.5f, -0.5f,  0.9522f, 0.688f, // K
        0.5f, -0.5f, -0.5f,  0.9522f, 0.513f, // L
        0.5f, -0.5f, -0.5f,  0.9522f, 0.513f, // L
        0.5f, -0.5f,  0.5f,  0.751f,  0.513f, // E
        0.5f,  0.5f,  0.5f,  0.751f,  0.688f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.55f,  0.26f,  // N
        0.5f,  -0.5f, -0.5f,  0.751f, 0.26f,  // M
        0.5f,  -0.5f,  0.5f,  0.751f, 0.513f, // E
        0.5f,  -0.5f,  0.5f,  0.751f, 0.513f, // E
        -0.5f, -0.5f,  0.5f,  0.549f, 0.513f, // F
        -0.5f, -0.5f, -0.5f,  0.55f,  0.26f,  // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.55f,  0.94f,  // O 
        0.5f,   0.5f, -0.5f,  0.751f, 0.94f,  // P
        0.5f,   0.5f,  0.5f,  0.751f, 0.688f, // D
        0.5f,   0.5f,  0.5f,  0.751f, 0.688f, // D
        -0.5f,  0.5f,  0.5f,  0.55f, 0.688f,  // C
        -0.5f,  0.5f, -0.5f,  0.55f,  0.94f,  // O
    };

    float vertices2[] = {
        //COORDENADAS PARA LA TEXTURA DE BAN
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,   0.356f, 0.504f, // I
        0.5f, -0.5f,  -0.5f,   0.152f, 0.504f, // J
        0.5f,  0.5f,  -0.5f,   0.152f, 0.681f, // G
        0.5f,  0.5f,  -0.5f,   0.152f, 0.681f, // G
        -0.5f,  0.5f, -0.5f,   0.356f, 0.681f, // H
        -0.5f, -0.5f, -0.5f,   0.356f, 0.504f, // I

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,   0.560f, 0.504f, // F
        0.5f, -0.5f,   0.5f,   0.766f, 0.504f, // E
        0.5f,  0.5f,   0.5f,   0.766f, 0.681f, // D
        0.5f,  0.5f,   0.5f,   0.766f, 0.681f, // D
        -0.5f,  0.5f,  0.5f,   0.560f, 0.681f, // C
        -0.5f, -0.5f,  0.5f,   0.560f, 0.504f, // F

        //Parte lateral izquierda
        -0.5f,  0.5f,  0.5f,  0.560f, 0.681f, // C
        -0.5f,  0.5f, -0.5f,  0.356f, 0.681f, // H
        -0.5f, -0.5f, -0.5f,  0.356f, 0.504f, // I
        -0.5f, -0.5f, -0.5f,  0.356f, 0.504f, // I
        -0.5f, -0.5f,  0.5f,  0.560f, 0.504f, // F
        -0.5f,  0.5f,  0.5f,  0.560f, 0.681f, // C

        //Parte lateral derecha
        0.5f,  0.5f,  0.5f,  0.765f, 0.681f, // D
        0.5f,  0.5f, -0.5f,  0.97f,  0.681f, // K
        0.5f, -0.5f, -0.5f,  0.97f,  0.504f, // L
        0.5f, -0.5f, -0.5f,  0.97f,  0.504f, // L
        0.5f, -0.5f,  0.5f,  0.765f, 0.504f, // E
        0.5f,  0.5f,  0.5f,  0.765f, 0.681f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.5604f, 0.2466f, // N
        0.5f,  -0.5f, -0.5f,  0.767f,  0.2466f, // M
        0.5f,  -0.5f,  0.5f,  0.767f,  0.505f,  // E
        0.5f,  -0.5f,  0.5f,  0.767f,  0.505f,  // E
        -0.5f, -0.5f,  0.5f,  0.560f,  0.5035f, // F
        -0.5f, -0.5f, -0.5f,  0.560f,  0.2466f, // N

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.560f, 0.941f, // O 
        0.5f,   0.5f, -0.5f,  0.766f, 0.941f, // P
        0.5f,   0.5f,  0.5f,  0.766f, 0.683f, // D
        0.5f,   0.5f,  0.5f,  0.766f, 0.683f, // D
        -0.5f,  0.5f,  0.5f,  0.560f, 0.683f, // C
        -0.5f,  0.5f, -0.5f,  0.560f, 0.941f, // O
    };

    float vertices3[] = {
        //COORDENADAS PARA LA TEXTURA DE PERSONAJES
        // posicion           // coordenadas de textura
        //Parte trasera de la cabeza
        -0.5f, -0.5f, -0.5f,   0.358f, 0.699f, // J
        0.5f, -0.5f,  -0.5f,   0.644f, 0.699f, // I
        0.5f,  0.5f,  -0.5f,   0.644f, 0.926f, // H
        0.5f,  0.5f,  -0.5f,   0.644f, 0.926f, // H
        -0.5f,  0.5f, -0.5f,   0.358f, 0.926f, // G
        -0.5f, -0.5f, -0.5f,   0.358f, 0.699f, // J

        //Parte delantera de la cabeza
        -0.5f, -0.5f,  0.5f,   0.358f, 0.246f,  // F
        0.5f, -0.5f,   0.5f,   0.644f, 0.246f,  // E
        0.5f,  0.5f,   0.5f,   0.644f, 0.4705f, // D
        0.5f,  0.5f,   0.5f,   0.644f, 0.4705f, // D
        -0.5f,  0.5f,  0.5f,   0.358f, 0.4705f, // C
        -0.5f, -0.5f,  0.5f,   0.358f, 0.246f,  // F

        //Parte lateral derecha
        -0.5f,  0.5f,  0.5f,  0.358f, 0.4705f, // C
        -0.5f,  0.5f, -0.5f,  0.074f, 0.4705f, // N
        -0.5f, -0.5f, -0.5f,  0.074f, 0.246f,  // M
        -0.5f, -0.5f, -0.5f,  0.074f, 0.246f,  // M
        -0.5f, -0.5f,  0.5f,  0.358f, 0.246f,  // F
        -0.5f,  0.5f,  0.5f,  0.358f, 0.4705f, // C

        //Parte lateral izquierda
        0.5f,  0.5f,  0.5f,  0.644f,  0.4705f, // D
        0.5f,  0.5f, -0.5f,  0.9295f, 0.4705f, // P
        0.5f, -0.5f, -0.5f,  0.9295f, 0.246f,  // O
        0.5f, -0.5f, -0.5f,  0.9295f, 0.246f,  // O
        0.5f, -0.5f,  0.5f,  0.644f,  0.246f,  // E
        0.5f,  0.5f,  0.5f,  0.644f,  0.4705f, // D

        //Parte inferior
        -0.5f, -0.5f, -0.5f,  0.358f, 0.019f, // L
        0.5f,  -0.5f, -0.5f,  0.644f, 0.019f, // K
        0.5f,  -0.5f,  0.5f,  0.644f, 0.246f, // E
        0.5f,  -0.5f,  0.5f,  0.644f, 0.246f, // E
        -0.5f, -0.5f,  0.5f,  0.358f, 0.246f, // F
        -0.5f, -0.5f, -0.5f,  0.358f, 0.019f, // L

        //Parte superior
        -0.5f,  0.5f, -0.5f,  0.358f, 0.926f, // G
        0.5f,   0.5f, -0.5f,  0.644f, 0.926f, // H
        0.5f,   0.5f,  0.5f,  0.644f, 0.699f, // I
        0.5f,   0.5f,  0.5f,  0.644f, 0.699f, // I
        -0.5f,  0.5f,  0.5f,  0.358f, 0.699f, // J
        -0.5f,  0.5f, -0.5f,  0.358f, 0.926f, // G
    };

    // Posiciones espaciales mundiales de nuestros cubos.
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -10.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),

        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-5.7f, 4.0f, -9.5f),
        glm::vec3(1.3f, -2.0f,  1.5f),

        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),

        glm::vec3(5.0f,  4.5f, 4.0f),
        glm::vec3(3.9f,  3.9f, 3.9f),
    };

    // Configuración de la primera figura
    unsigned int VBO, VAO; 
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 
    // atributo de coordinación de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1);  

    // Configuración de la segunda figura
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de coordinación de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 

    // Configuración de la tercera figura
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de coordinación de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuración de la cuarta figura
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de coordinación de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cargar y crear las texturas
    unsigned int texture1, texture2, texture3, texture4;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //  establecer los parámetros de wrapping de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // establecer parámetros de filtrado de texturas
    // establezca el filtrado de texturas en el vecino más cercano para ver claramente los texels/píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    // cargar imagen, crear textura y generar mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Dile a stb_image.h que voltee las texturas cargadas en el eje y. 
    unsigned char* data = stbi_load("textures/Texture3.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data); 

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //  establecer los parámetros de wrapping de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // establecer parámetros de filtrado de texturas
    // establezca el filtrado de texturas en el vecino más cercano para ver claramente los texels/píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // cargar imagen, crear textura y generar mipmaps
    data = stbi_load("textures/TextureLevi.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data); 

    // texture 3
    // ---------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    //  establecer los parámetros de wrapping de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // establecer parámetros de filtrado de texturas
    // establezca el filtrado de texturas en el vecino más cercano para ver claramente los texels/píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // cargar imagen, crear textura y generar mipmaps
    data = stbi_load("textures/TextureBan.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);

    // texture 4
    // ---------
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    //  establecer los parámetros de wrapping de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // establecer parámetros de filtrado de texturas
    // establezca el filtrado de texturas en el vecino más cercano para ver claramente los texels/píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // cargar imagen, crear textura y generar mipmaps
    data = stbi_load("textures/TexturePersonajes.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);

    // decirle a opengl para cada muestra a qué unidad de textura pertenece (solo debe hacerse una vez)
    // -------------------------------------------------------------------------------------------

    ourShader.use(); 
    ourShader.setInt("texture1", 0); 
    ourShader.setInt("texture2", 1);  
    ourShader.setInt("texture3", 2);
    ourShader.setInt("texture4", 3);

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // lógica de tiempo por fotograma
    // --------------------
        float currentFrame = glfwGetTime(); // Obtiene el tiempo actual en segundos desde GLFW inicializado
        deltaTime = currentFrame - lastFrame; // Calcula la diferencia de tiempo entre frames
        lastFrame = currentFrame;  // Actualiza el tiempo del último frame al actual

        // input
        // -----
        processInput(window); // Función para procesar la entrada del usuario (teclado, mouse)

        // render
        // ------
        glClearColor(0.8f, 1.0f, 0.96f, 0.0f); // Establece el color de fondo de la ventana
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia el búfer de color y profundidad

        // activar shader
        ourShader.use(); // Activa el programa de sombreador (shader) almacenado en 'ourShader'

        // pasar la matriz de proyección al sombreador
        // Calcula la matriz de proyección
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); 
        ourShader.setMat4("projection", projection); // Pasa la matriz de proyección al sombreador

        // transformación de cámara/vista
        glm::mat4 view = camera.GetViewMatrix(); // Obtiene la matriz de vista de la cámara
        ourShader.setMat4("view", view); // Pasa la matriz de vista al sombreador

        // renderizar cajas
        glActiveTexture(GL_TEXTURE0); // Activa la textura en la unidad de textura 0

        for (unsigned int i = 0; i < 11; i++)
        {
            if (i < 3) {
                glBindTexture(GL_TEXTURE_2D, texture1); // Selecciona la textura 1
                glBindVertexArray(VAO); // Activa el VAO correspondiente
                glm::mat4 model = glm::mat4(1.0f); // Inicializa la matriz de modelo como la matriz identidad
                model = glm::translate(model, cubePositions[i]); // Aplica la traslación según las posiciones de las cajas
                float angle = 20.0f * i; // Calcula el ángulo de rotación
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); // Aplica la rotación al modelo
                ourShader.setMat4("model", model); // Pasa la matriz de modelo al sombreador

                glDrawArrays(GL_TRIANGLES, 0, 36); // Dibuja las cajas utilizando triángulos
            } if (i >= 3 && i < 6) {

                glBindTexture(GL_TEXTURE_2D, texture2);
                glBindVertexArray(VAO1);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 75.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); 
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i >= 6 && i < 9) {

                glBindTexture(GL_TEXTURE_2D, texture3);
                glBindVertexArray(VAO2);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 10.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (i >= 9) {

                glBindTexture(GL_TEXTURE_2D, texture4);
                glBindVertexArray(VAO3);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 40.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(2.0f, 0.4f, 0.6f));
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
        
        }

        // glfw: intercambiar buffers y sondear eventos IO (teclas presionadas/soltadas, movimiento del mouse, etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window); 
        glfwPollEvents(); 

    }
    // optional: desasignar todos los recursos una vez que hayan superado su propósito:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);  
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO3);

    // glfw: terminar, borrando todos los recursos GLFW previamente asignados.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// procesar todas las entradas: consultar a GLFW si se presionan o sueltan teclas relevantes en este cuadro y reacciona en consecuencia
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

    //If I want to stay in ground level (xz plane)
    //camera.Position.y = 0.0f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}






