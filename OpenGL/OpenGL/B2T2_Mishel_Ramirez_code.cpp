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

// Ajustes de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Factor de visibilidad
//float visibilyFactor = 0.5f;
float visibilidad1  = 0.0f; 
float visibilidad2 = 1.0f; 

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T2 Mishel Ramirez 175286120-1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: cargar todos los punteros de función OpenGL
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // construir y compilar el shader program
    // ------------------------------------
    Shader ourShader("shaders/shader_B2T2.vs", "shaders/shader_B2T2.fs");

    // configurar datos de vértice (y búfer(es)) y configurar atributos de vértice
    // -----------------------------------------------------------------
    float vertices1[] = {
        // posicion          // coordenadas de textura
        -0.41f, 0.92f, 0.0f,  0.27f, 0.97f,   // C -- 0
        -0.07f, 0.58f, 0.0f,  0.44f, 0.8f,    // D -- 1
        -0.75f, 0.58f, 0.0f,  0.1f,  0.8f,    // E -- 2
        -0.4f,  0.24f, 0.0f,  0.28f, 0.63f,   // F -- 3
        -0.4f,  0.58f, 0.0f,  0.28f, 0.8f,    // G -- 4		
        -0.07f, 0.24f, 0.0f,  0.44f, 0.63f,   // H -- 5
        -0.4f, -0.09f, 0.0f,  0.28f, 0.46f,   // I -- 6
        -0.73f,-0.09f, 0.0f,  0.11f, 0.46f,   // J -- 7
    };

    unsigned int indices1[] = {
        0, 1, 2,    // Triángulo 1 
        1, 3, 4,    // Triángulo 2        
        1, 5, 3,    // Triángulo 3      
        3, 6, 7,    // Triángulo 4       
        3, 6, 5,    // Triángulo 5 
    };

    float vertices2[] = {
        // posicion           // coordenadas de textura
        -0.73f,-0.09f, 0.0f,  0.01f, 0.76f,   // J -- 0
        -0.26f,-0.56f, 0.0f,  0.25f, 0.53f,   // K -- 1 
        0.2f,  -0.09f, 0.0f,  0.48f, 0.76f,   // L -- 2 
        0.67f, -0.56f, 0.0f,  0.71f, 0.53f,   // M -- 3
        -0.26f,-0.9f,  0.0f,  0.24f, 0.36f,   // N -- 4  
        -0.6f, -0.9f,  0.0f,  0.08f, 0.36f,   // O -- 5 
        0.47f, -0.65f, 0.0f,  0.61f, 0.48f,   // P -- 6
        0.38f, -0.98f, 0.0f,  0.56f, 0.32f,   // Q -- 7
        0.14f, -0.56f, 0.0f,  0.45f, 0.53f,   // R -- 8
    };

    unsigned int indices2[] = {
        0, 1, 2,    // Triángulo 6 
        1, 2, 3,    // Triángulo 7 
        1, 4, 5,    // Triángulo 8 
        6, 7, 8,    // Triángulo 9 
    };

    // Configuración de la primera figura
    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de coordinación de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuración de la segunda figura
    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    // atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cargar y crear las texturas
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //  establecer los parámetros de wrapping de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // establecer parámetros de filtrado de texturas
    // establezca el filtrado de texturas en el vecino más cercano para ver claramente los texels/píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // cargar imagen, crear textura y generar mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Dile a stb_image.h que voltee las texturas cargadas en el eje y. 
    unsigned char* data = stbi_load("textures/santuario_rosado.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    data = stbi_load("textures/naturaleza.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.8f, 1.0f, 0.96f, 0.0f); //Color de fondo
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


        // get matrix's uniform location and set matrix
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // Figura 1 con la primera textura
        ourShader.setFloat("visibilidad", visibilidad1);
        glBindVertexArray(VAO1);

        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

        // Renderizar el segundo conjunto de vértices
        ourShader.setFloat("visibilidad", visibilidad2);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


        transform = glm::mat4(1.0f); //RESTABLECER LA MATRIZ DE TRANSFORMACIÓN 
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scale = 0.2f + 0.2f * sin(glfwGetTime());
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));  

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); 

        // Figura 1 con la primera textura
        ourShader.setFloat("visibilidad", visibilidad1); 
        glBindVertexArray(VAO1);   

        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0); 

        // Renderizar el segundo conjunto de vértices
        ourShader.setFloat("visibilidad", visibilidad2);  
        glBindVertexArray(VAO2);  
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glm::mat4 transform3 = glm::mat4(1.0f); 
        transform3 = glm::translate(transform3, glm::vec3(0.5f, 0.7f, 0.0f)); // Trasladar al centro 
        transform3 = glm::rotate(transform3, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar alrededor del eje Y 
        transform3 = glm::scale(transform3, glm::vec3(0.3f, 0.3f, 0.3f)); // Escalar al 50% 
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform3)); 

        // Figura 1 con la primera textura
        ourShader.setFloat("visibilidad", visibilidad1);
        glBindVertexArray(VAO1);

        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

        // Renderizar el segundo conjunto de vértices
        ourShader.setFloat("visibilidad", visibilidad2);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // glfw: intercambiar buffers y sondear eventos IO (teclas presionadas/soltadas, movimiento del mouse, etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window); 
        glfwPollEvents(); 

    }
    // optional: desasignar todos los recursos una vez que hayan superado su propósito:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

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
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        visibilidad1 = 0.0f;
        visibilidad2 = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        visibilidad1 = 1.0f;
        visibilidad2 = 0.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}







