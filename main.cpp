#include <iostream>
#include <filesystem>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "OBJFile.h"


//Marco Antonio Quiña Mamani
//ejecucion
// cmake
// make
// ./nombreDelEjecutable

glm::vec3 functionF(glm::vec3 x, float t);

int main()
{

    if(glfwInit() == 0)
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }

    GLFWwindow* window;
    window = glfwCreateWindow(1000, 800, "Hello OpenGL", nullptr, nullptr);

    if(window == nullptr)
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
// Quiña Mamani Marco Antonio

    if(glewInit() != GLEW_OK)
    {
        std::cerr<<"GLEW failed\n";
        glfwTerminate();
        return -1;

    }




    /*std::vector<glm::vec3> vertices = {glm::vec3(0.0f, 0.5f, 0.0f),
                                       glm::vec3(0.5f, -0.5f, 0.0f),
                                       glm::vec3(-0.5f, -0.5f, 0.0f)}; //positions*/
    /*std::vector<glm::vec3> vertices = {glm::vec3(0.5f, 0.5f, 0.0f),
                                           glm::vec3(0.5f, -0.5f, 0.0f),
                                           glm::vec3(-0.5f, -0.5f, 0.0f),
                                       glm::vec3(0.5f, 0.5f, 0.0f),
                                            glm::vec3(-0.5f, -0.5f, 0.0f),
                                        glm::vec3(-0.5f, 0.5f, 0.0f)};*/
    OBJFile objConejo;
    //Ruta actual
    std::cout<<std::filesystem::current_path().string()<<std::endl;
    objConejo.readFileOBJ("conejo2.obj");
    std::vector<glm::vec3>& vertices = objConejo.GetVertices();
    //creacion de la carpeta frame donde se guardaran los frames
    std::filesystem::create_directory("frames") ?
                std::cout << "Se creo la carpeta frames" << std::endl :
                    std::cout << "Error al crear la carpeta frames, talvez ya existe" << std::endl;

    //std::cout<<objConejo.GetIndicesVertices()<<std::endl;
    /*for(unsigned i=0;i<objConejo.GetIndicesVertices().size();++i)
    {
        std::cout << objConejo.GetIndicesVertices()[i] << std::endl;
    }*/



    /*std::vector<glm::vec3> vertices2;
    float tam = 10.f;
    for(auto pt = vertices.begin(); pt != vertices.end(); ++pt)
    {
        vertices2.push_back(*pt * tam);
    }*/
    /*
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

    */

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() *3 * sizeof(float)  , &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);



    const char* vertexShaderSrc =
            "#version 400\n"
            "in vec4 position;"
            "void main()"
            "{"
            "gl_Position = position;"
            "}";

    const char* fragmentShaderSrc =
            "#version 400\n"
            "out vec4 color;"
            "void main()"
            "{"
            "color = vec4(0.0, 0.5, 0.0, 1.0);"
            "}";


    GLuint vertexShader  = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);


    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);


    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    std::vector<glm::vec3> velocities(vertices.size(), glm::vec3(0,0,0));

    std::vector<float> mass(vertices.size(), 1);

    //glm::vec3 gravity(0, -0.98, 0);

    float h = 0.0002;
    float time=0;
    unsigned numFrames = 1;
    unsigned limFrames = 400;
    glm::vec3 k1, k2;

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(0.0, 0.0, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if(numFrames <= limFrames)
        {
            for(unsigned i = 0; i < vertices.size(); ++i)
            {
                k1 = h *functionF(velocities[i], time);
                k2 = h *functionF(velocities[i] + k1/2.f, time + h/2.f);
                velocities[i]  = velocities[i] + k2;
                vertices[i] = vertices[i] + velocities[i]*time;

                /*velocities[i]  = velocities[i] + h*(gravity/mass[i]);
                vertices[i] = vertices[i] + velocities[i]*time;*/
            }
            time+=h;
            objConejo.writeFramesInVTK("frames",numFrames);
            numFrames++;
        }


        glBufferData(GL_ARRAY_BUFFER, vertices.size() *3 * sizeof(float)  , &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);



        glDrawArrays(GL_POINTS, 0, vertices.size());
        glfwSwapBuffers(window);

        glfwPollEvents();

    }


    glfwTerminate();

    return 0;
}

glm::vec3 functionF(glm::vec3 x, float t) {
    glm::vec3 gravity(0, -0.98, 0);
    return gravity;
}
