#version 330 core  

// Declaración de la variable de salida del fragment shader
out vec4 FragColor;

// Declaración de la variable de entrada
in vec3 ourColor;

void main()
{
    // Asigna el color de salida del fragmento
    FragColor = vec4(ourColor, 1.0f);
}
