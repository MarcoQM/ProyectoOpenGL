#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OBJFile.h"
#include "Geometry.h"
#include "Topology.h"

glm::vec3 functionF(glm::vec3 x, float t);
void updatePrimitives(std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices, std::vector<glm::vec3> &primitives);

int main()
{

    GLFWwindow *window;

    if (glfwInit() == 0) // si todo esta ok con el SO
    {
        std::cerr << "GLFW failed\n";
        return -1;
    }

    // dar memoria
    window = glfwCreateWindow(1000, 1000, "Hello triangle", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "GLFW failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW failed\n";
        glfwTerminate(); // delete
        return -1;
    }

    // Se realiza la lectura de los archivos obj
    OBJFile objBunny("conejo1.obj");
    Geometry geometryBunny(objBunny);
    Topology topologyBunny(geometryBunny.GetIndices());

    /*for (unsigned i = 0; i < topologyBunny.GetEdges().size(); ++i)
    {
        std::cout << topologyBunny.GetEdges()[i] << std::endl;
    }
    std::cout << "tamanio topologia " << topologyBunny.GetEdges().size() << std::endl;*/

    OBJFile objDragon("dragon1.obj");
    Geometry geometryDragon(objDragon);
    Topology topologyDragon(geometryDragon.GetIndices());

    //std::vector<glm::vec3> bunny{geometryBunny.GetVertices()};
    //std::vector<glm::vec3> dragon{geometryDragon.GetVertices()};
    std::vector<glm::vec3> verticesBunny{geometryBunny.GetVertices()};
    std::vector<glm::vec3> verticesDragon{geometryDragon.GetVertices()};

    std::vector<unsigned short> indicesBunny{geometryBunny.GetIndices()};
    std::vector<unsigned short> indicesDragon{geometryDragon.GetIndices()};

    std::vector<glm::vec3> bunny(indicesBunny.size());
    std::vector<glm::vec3> dragon(indicesDragon.size());

    // Se formas las primitivas para enviarlas a opengl
    updatePrimitives(verticesBunny, indicesBunny, bunny);
    updatePrimitives(verticesDragon, indicesDragon, dragon);

    unsigned bunnyNVertices = bunny.size();
    unsigned dragonNVertices = dragon.size();

    // Vector de velocidad
    std::vector<glm::vec3> velocityBunny(bunnyNVertices, glm::vec3(0, 0, 0));
    std::vector<glm::vec3> velocityDragon(dragonNVertices, glm::vec3(0, 0, 0));

    GLuint vbo[2]; // vertex buffer object para cada objeto

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, bunnyNVertices * 3 * sizeof(float), &bunny[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, dragonNVertices * 3 * sizeof(float), &dragon[0], GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const char *verter_shader =
        "#version 400\n"
        "in vec3 vp;"
        "uniform mat4 mvMatrix;"   // model-view
        "uniform mat4 projMatrix;" // projection
        "void main(){"
        "gl_Position = projMatrix * mvMatrix * vec4(vp, 1.0);"
        "}";

    const char *fragment_shader =
        "#version 400\n"
        "out vec4 frag_color;"
        "uniform mat4 mvMatrix;"   // model-view
        "uniform mat4 projMatrix;" // projection
        "void main(){"
        "frag_color = vec4(1.0, 1.0, 1.0, 0.0);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &verter_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // all objects
    float cameraX = 0.f;
    float cameraY = 0.f;
    float cameraZ = 7.f;

    // bunny
    float lockX = 2.0;
    float lockY = -1.5;
    float lockZ = 0.0; // offset

    // dragon
    float pyLockX = -2.0;
    float pyLockY = -2.0;
    float pyLockZ = 0.0;

    float h = 0.0001;
    float timeBunny = 0;
    float timeDragon = 0;
    glm::vec3 k1, k2;

    int width, height;
    glm::mat4 mMat, vMat, mvMat, pMat, sMat;

    float angle = 0;
    while (glfwWindowShouldClose(window) == 0) // animation
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 0.0); // RGB/255

        // get matrix locations
        GLuint mvLoc = glGetUniformLocation(shaderProgram, "mvMatrix");
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projMatrix");

        // perstective
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

        // matrix model - view
        vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));

        glm::mat4 vRot = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0));
        // Giro de Camara
        // vMat =  vMat*vRot;

        // aminate here
        // std::cout<<bunnyNVertices<<std::endl;
        // std::cout<<bunny.size()<<std::endl;
        for (unsigned int i = 0; i < verticesBunny.size(); ++i)
        {
            //velocityBunny[i] = velocityBunny[i] + (-0.98f / 1.f);
            //bunny[i] = bunny[i] + h * velocityBunny[i];

            k1 = h * functionF(velocityBunny[i], timeBunny);
            k2 = h * functionF(velocityBunny[i] + k1 / 2.f, timeBunny + h / 2.f);
            velocityBunny[i] = velocityBunny[i] + k2;
            verticesBunny[i] = verticesBunny[i] + velocityBunny[i] * timeBunny;
        }
        timeBunny+=h;

        for (unsigned int i = 0; i < verticesDragon.size(); ++i)
        {
            //velocityDragon[i] = velocityDragon[i] + (-0.98f / 1.f);
            //dragon[i] = dragon[i] + h * velocityDragon[i];

            k1 = h * functionF(velocityDragon[i], timeDragon);
            k2 = h * functionF(velocityDragon[i] + k1 / 2.f, timeDragon + h / 2.f);
            velocityDragon[i] = velocityDragon[i] + k2;
            verticesDragon[i] = verticesDragon[i] + velocityDragon[i] * timeDragon;
        }
        timeDragon+=h;

        updatePrimitives(verticesBunny, indicesBunny, bunny);
        updatePrimitives(verticesDragon, indicesDragon, dragon);


        glm::mat4 mOrg = glm::translate(glm::mat4(1.f), glm::vec3(1.0f, -1.0f, 1.0f));   // offset
        glm::mat4 mBack = glm::translate(glm::mat4(1.f), glm::vec3(-1.0f, 1.0f, -1.0f)); // offset

        glm::mat4 mRot = mBack * glm::rotate(mOrg, angle, glm::vec3(0, 1, 0));

        angle += 0.05;
        if (angle > 360)
            angle = 0;

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(lockX, lockY, lockZ)) * mRot; // offset
        // mMat = glm::translate(glm::mat4(1.f), glm::vec3(lockX, lockY, lockZ));
        sMat = glm::scale(glm::mat4(1.f), glm::vec3(12.f, 12.f, 12.f));

        // mvMat = vMat * mMat * sMat;
        mvMat = vMat * mMat * sMat;
        // mvMat = vMat*sMat;

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

        // colision

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, bunnyNVertices * 3 * sizeof(float), &bunny[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // glPointSize(10.f);
        // glDrawArrays(GL_POINTS, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, bunnyNVertices);

        // Rotaciones Dragon
        mRot = mBack * glm::rotate(mOrg, angle, glm::vec3(0, 1, 0));

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(pyLockX, pyLockY, pyLockZ)) * mRot; // offset
        sMat = glm::scale(glm::mat4(1.f), glm::vec3(14.f, 14.f, 14.f));

        mvMat = vMat * mMat * sMat;

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, dragonNVertices * 3 * sizeof(float), &dragon[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, dragonNVertices);

        glfwSwapBuffers(window); // Buffers->OpenGL

        glfwPollEvents(); // User
    }

    glfwTerminate();

    std::cout << "Finished!" << std::endl;
    return 0;
}

glm::vec3 functionF(glm::vec3 x, float t)
{
    glm::vec3 gravity(0, -0.98, 0);
    return gravity;
}

void updatePrimitives(std::vector<glm::vec3> &vertices, std::vector<unsigned short> &indices, std::vector<glm::vec3> &primitives)
{
    unsigned short index;

    for (unsigned int i = 0; i < indices.size(); ++i)
    {
        index = indices[i];
        primitives[i] = vertices[index-1];
    }
}