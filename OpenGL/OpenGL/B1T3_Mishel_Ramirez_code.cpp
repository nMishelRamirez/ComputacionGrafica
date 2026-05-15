// Nombre: Mishel Ramirez
// Cedula: 1752861201

// Encabezados de las bibliotecas GLAD y GLFW. 
// GLAD se utiliza para cargar las funciones de OpenGL
// GLFW se utiliza para crear y gestionar ventanas.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Declaraciones anticipadas de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Ancho y alto de la ventana.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Toma las posiciones de los vértices como entrada y establece la posición del vértice en el espacio de pantalla
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Establece un color de salida constante para todos los fragmentos procesados
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.10f,0.55f,1.0f, 0.0f);\n"
"}\n\0";

int main()
{

    glfwInit(); // Inicializa GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Establece la versión principal del contexto de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Establece el valor de la versión secundaria del contexto de OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Utiliza el perfil central de OpenGL

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Descomenta esta línea para solucionar problemas de compilación en macOS
    #endif

    //Inicializa GLFW y configura algunas opciones de la ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Triangle", NULL, NULL); //Crea una nueva ventana GLFW
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl; //Imprime un mensaje de error si falla la creación de la ventana
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //Establece la ventana GLFW recién creada como el contexto de renderizado actual 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Configura la función de devolución de llamada para cambiar el tamaño del marco

    //Carga todas las funciones de OpenGL utilizando la biblioteca GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl; //Imprime un mensaje de error si falla la inicialización de GLAD
        return -1;
    }


    int vertexShader = glCreateShader(GL_VERTEX_SHADER); //Crea un objeto de shader de vértices
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Asigna el código fuente al shader de vértices
    glCompileShader(vertexShader); //Compila el shader de vértices

    //Se verifica si hay errores en la compilación del shader de vértices
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //Obtiene el estado de compilación del "vertexShader"
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); //Obtiene los mensajes de error del shader de vértices
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; //Imprime los mensajes de error
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Crea un objeto de shader de fragmentos
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Asigna el código fuente al shader de fragmentos
    glCompileShader(fragmentShader);// Compila el shader de fragmentos

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); //Obtiene el estado de compilación del "fragmentShader"
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); //Obtiene los mensajes de error del shader de fragmentos
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;//Imprime los mensajes de error
    }

    //Se crea un programa de shader y se adjuntan los shaders compilados
    int shaderProgram = glCreateProgram(); //crea un programa de shader 
    glAttachShader(shaderProgram, vertexShader); //Adjunta el "vertexShader" al "shaderProgram"
    glAttachShader(shaderProgram, fragmentShader); //Adjunta el "fragmentShader" al "shaderProgram"
    glLinkProgram(shaderProgram); //Se enlazan los shaders para formar un programa ejecutable

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //Obtiene el estado de enlace del "shaderProgram"
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); //Obtiene los mensajes de error generados durante el enlace del programa
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl; //Imprime los mensajes de error
    }
    // Se eliminan los shaders compilados, ya que no son necesarios después de enlazarlos
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);


    // Definición de los vértices del triángulo
    float vertices[] = {
        //Primer Triangulo
        0.0f, 0.7f, 0.0f,
        0.0f,0.0f,0.0f,
        0.55f, 0.44f,0.0f,

        //Segundo triangulo
        0.55f, 0.44f,0.0f,
        0.0f,0.0f,0.0f,
        0.68f,-0.16f, 0.0f,

        //Tercer triangulo
        0.68f,-0.16f, 0.0f,
        0.0f,0.0f,0.0f,
        0.3f,-0.63f, 0.0f,

        //Cuarto triangulo
        0.3f,-0.63f, 0.0f,
        0.0f,0.0f,0.0f,
        -0.3f,-0.63f, 0.0f,

        //Quinto triangulo
        -0.3f,-0.63f, 0.0f,
        0.0f,0.0f,0.0f,
        -0.68f,-0.16f, 0.0f,

        //Sexto triangulo
        -0.68f,-0.16f, 0.0f,
        0.0f,0.0f,0.0f,
        -0.55f, 0.44f,0.0f,

        //Septimo triangulo
        -0.55f, 0.44f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f, 0.7f, 0.0f,
    };

    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO); //Genera un o más arreglos de vértices y devuelve sus identificadores.

    glGenBuffers(1, &VBO); //Genera un búfer de vértices y devuelve su identificador
    
    glBindVertexArray(VAO); //Enlaza el arreglo de vértices (VAO) como el arreglo de vértices actual

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Enlaza el búfer de vértices(VBO) como el búfer de vértices actual.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copia los datos de los vértices al búfer de vértices actual (VBO).

    //Especifica cómo OpenGL debe interpretar los datos de los vértices cuando se utilizan en un shader.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Habilita el atributo de vértice especificado por su índice (en este caso, el índice 0)
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Desenlaza el búfer de vértices actual. 

    glBindVertexArray(0); //Desenlaza el arreglo de vértices actual.

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Procesa la entrada del usuario
        processInput(window);

        // Establece el color de fondo
        glClearColor(0.53f, 0.00f, 0.80f, 0.0f); 
        glClear(GL_COLOR_BUFFER_BIT); // Limpia el buffer de color con el color de fondo establecido

        // Usa el programa de shaders
        glUseProgram(shaderProgram); // Activa el programa de shaders que se usará para el renderizado
        glBindVertexArray(VAO); // Enlaza el VAO que contiene la configuración de los atributos de los vértices
        glDrawArrays(GL_TRIANGLES, 0, 21); // Dibuja los triángulos usando los vértices definidos en el VAO

        // Intercambia los buffers y procesa eventos
        glfwSwapBuffers(window); // Intercambia los buffers del marco actual (doble buffer)
        glfwPollEvents(); // Procesa los eventos pendientes (entrada del teclado, mouse, etc.)
    }


    // Elimina los objetos de arreglo de vértices y búferes de datos
    glDeleteVertexArrays(1, &VAO); // Elimina el VAO 
    glDeleteBuffers(1, &VBO); // Elimina el VBO

    glfwTerminate(); // Termina GLFW y libera los recursos asignados por esta librería
    return 0; // Finaliza el programa
}

// Función para procesar la entrada del usuario
void processInput(GLFWwindow* window)
{
    // Si la tecla ESCAPE está presionada
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Marca la ventana para cerrarse
}

// Función de callback para cambiar el tamaño del framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // Ajusta el área de dibujo al nuevo tamaño de la ventana
}

