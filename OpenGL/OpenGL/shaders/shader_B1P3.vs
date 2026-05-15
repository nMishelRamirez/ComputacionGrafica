#version 330 core  

// Atributos de entrada
layout (location = 0) in vec3 aPos;   // Posición del vértice
layout (location = 1) in vec3 aColor; // Color del vértice

// Variable de salida para pasar el color al fragment shader
out vec3 ourColor;

// Uniforme que recibe el tiempo desde la aplicación de OpenGL
uniform float tiempo;

void main()
{
    // Calcula un desplazamiento basado en la función seno del tiempo
    float movimiento = sin(tiempo)/8;
    
    // Calcula la posición del vértice con el desplazamiento aplicado
    gl_Position = vec4(aPos.x + (movimiento * 3), aPos.y / (movimiento/0.08) , aPos.z, 1.0);
    
    // Pasa el color del vértice al fragment shader
    ourColor = aColor;
}
