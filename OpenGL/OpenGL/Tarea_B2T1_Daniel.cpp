#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_s.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800; //Ancho ventana
const unsigned int SCR_HEIGHT = 800; //Alto ventana

bool swapTexture0 = true; // Variable para intercambiar textura 0
bool swapTexture1 = false; // Variable para intercambiar textura 1

int main()
{
    /* Aquí se crea la ventana vacía(es el código comentado de antes) */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Crear y compilar los shader programs
    Shader ourShader("shaders/shader_B2T1_Daniel.vs", "shaders/shader_B2T1_Daniel.fs");

    // Vértices que se van a utilizar para: posición, color, textura e ID de la textura; en ese orden
    float vertices[] = {
        // x,      y,    z,        r,      g,      b,          s,       t,      texID
        //Triángulos del VBO
        -0.35f, -0.58f, 0.0f,   0.392f, 0.208f, 0.988f,     0.2414f, 0.1868f,   1.0f,
        -0.7f,  -0.58f, 0.0f,   0.992f, 0.608f, 0.688f,     0.0f,    0.1868f,   1.0f,
        -0.52f, -0.75f, 0.0f,   0.692f, 0.808f, 0.188f,     0.1241f, 0.0934f,   1.0f,

        0.25f,  -0.58f, 0.0f,   0.292f, 0.112f, 0.388f,     0.6552f, 0.1868f,   1.0f,
        0.48f,  -0.71f, 0.0f,   0.192f, 0.612f, 0.988f,     0.8138f, 0.1154f,   1.0f,
        0.36f,  -0.92f, 0.0f,   0.792f, 0.212f, 0.988f,     0.7310f, 0.0f,      1.0f,

        // Triángulos del EBO
        -0.06f, -0.58f, 0.0f,   0.696f, 0.804f, 0.404f,     0.4414f, 0.1868f,   1.0f,
        0.46f,  -0.58f, 0.0f,   0.496f, 0.804f, 0.204f,     0.8f, 0.1868f,      1.0f,
        -0.06f, -0.05f, 0.0f,   0.196f, 0.804f, 0.604f,     0.4414f, 0.478f,    1.0f,

        -0.45f, -0.58f, 0.0f,   0.596f, 0.800f, 0.192f,     0.1724f, 0.1868f,   1.0f,
        -0.06f, -0.19f, 0.0f,   0.596f, 0.800f, 0.192f,     0.4414f, 0.401f,    1.0f,

        0.06f,  0.56f, 0.0f,    1.000f, 0.796f, 0.020f,     0.5241f, 0.813f,    0.0f,
        -0.3f,  0.19f, 0.0f,    1.000f, 0.796f, 0.020f,     0.2759f, 0.61f,     0.0f,
        0.06f, -0.17f, 0.0f,    1.000f, 0.796f, 0.820f,     0.5241f, 0.4121f,   0.0f,

        -0.18f, 0.69f, 0.0f,    0.996f, 0.427f, 0.396f,     0.3655f, 0.8846f,   0.0f,
        0.19f,  0.77f, 0.0f,    0.496f, 0.727f, 0.696f,     0.6345f, 0.9285f,   0.0f,
        -0.05f, 0.9f, 0.0f,     0.996f, 0.427f, 0.396f,     0.4483f, 1.0f,      0.0f,

        0.4f,  0.9f, 0.0f,      0.408f, 0.400f, 0.796f,     0.7931f, 1.0f,      0.0f,
        0.53f, 0.68f, 0.0f,     0.408f, 0.400f, 0.796f,     0.8828f, 0.8791f,   0.0f,
        0.75f, 0.81f, 0.0f,     0.608f, 0.600f, 0.796f,     1.0f,    0.9505f,   0.0f
    };

    // Obtener posiciones para el EBO
    unsigned int indices[] = {
        6, 7, 8,      // tercero
        6, 9, 10,     // cuarto
        11, 12, 13,   // quinto
        11, 14, 15,   // sexto
        14, 15, 16,   // séptimo
        15, 17, 18,   // octavo
        17, 18, 19    // noveno
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Agregar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Agregar el EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar el atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configurar el atributo de color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configurar el atributo de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configurar el atributo del ID para la textura
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load and create the textures
    unsigned int texture1, texture2;
    // Texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/santuario_rosado.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);

    // Texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("textures/naturaleza.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // unir texturas en las unidades de textura correspondientes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // crear transformaciones
        glm::mat4 transform = glm::mat4(1.0f); // asegúrese de inicializar primero la matriz con la matriz de identidad
        transform = glm::translate(transform, glm::vec3(0.0f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        float timeValue = glfwGetTime();

        ourShader.use();
        glUniform1f(glGetUniformLocation(ourShader.ID, "time"), timeValue);
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // Unir las texturas
        /*glActiveTexture(GL_TEXTURE0);
        if (swapTexture0)   //Si swaptTexture0 = true se pasa la textura 2, caso contrario se pasa la 1
            glBindTexture(GL_TEXTURE_2D, texture2);
        else
            glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        if (swapTexture1)   //Si swaptTexture1 = true se pasa la textura 2, caso contrario se pasa la 1
            glBindTexture(GL_TEXTURE_2D, texture2);
        else
            glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);*/

        //Dibujar los triángulos
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);   //VBO
        glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0);   //EBO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    // Si se presiona la tecla ESC, se debe cerrar la ventana
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Variables estáticas para detectar el estado de las teclas J y K
    static bool jKeyPressed = false;
    static bool kKeyPressed = false;

    // Si se presiona la tecla J
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (!jKeyPressed) // Solo alterna en la primera pulsación
        {
            // Invierte el valor de swapTexture0
            swapTexture0 = !swapTexture0;
            // Marca que la tecla J está presionada
            jKeyPressed = true;
        }
    }
    else
    {
        // Si la tecla J no está presionada, resetea el estado de jKeyPressed
        jKeyPressed = false;
    }

    // Si se presiona la tecla K
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        if (!kKeyPressed) // Solo alterna en la primera pulsación
        {
            // Invierte el valor de swapTexture1
            swapTexture1 = !swapTexture1;
            // Marca que la tecla K está presionada
            kKeyPressed = true;
        }
    }
    else
    {
        // Si la tecla K no está presionada, resetea el estado de kKeyPressed
        kKeyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
