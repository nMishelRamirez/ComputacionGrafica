#include <GLFW/glfw3.h>  // Biblioteca GLFW para manejo de ventanas y contexto OpenGL

int main(void)
{
    GLFWwindow* window;  // puntero a GLFWwindow para manejar la ventana

    /* Inicializa la biblioteca GLFW */
    if (!glfwInit())  // Si la inicialización de GLFW falla
        return -1;    // Sale del programa 

    /* Crea una ventana en modo ventana y su contexto OpenGL */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);  // Crea una ventana de 640x480 píxeles con título "Hello World"
    if (!window)  // Si la creación de la ventana falla
    {
        glfwTerminate();  // Termina GLFW y limpia los recursos
        return -1;        // Sale del programa 
    }

    // Establece el contexto OpenGL de la ventana como el contexto actual
    glfwMakeContextCurrent(window);  

    // Bucle principal: sigue ejecutándose mientras la ventana no deba cerrarse
    while (!glfwWindowShouldClose(window))  
    {
        // Limpia el buffer de color para preparar el frame para el dibujo (Renderiza)
        glClear(GL_COLOR_BUFFER_BIT);  

        // Intercambia el buffer trasero con el buffer frontal para mostrar lo dibujado
        glfwSwapBuffers(window);  

        // Procesa eventos pendientes (teclado, ratón, etc.)
        glfwPollEvents();  
    }

    glfwTerminate();  // Termina GLFW y limpia todos los recursos
    return 0;  // Sale del programa
}
