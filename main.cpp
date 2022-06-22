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

void calculateDistance( Geometry::Vertices &vertices, Topology::Edges &edges, std::vector<float> &distances) 
{
    unsigned short vertexA, vertexB;
    for(unsigned i = 0; i < distances.size(); ++i)
    {
        vertexA = edges[i].GetVertexA();
        vertexB = edges[i].GetVertexB();
        distances[i] = glm::length(vertices[vertexA]-vertices[vertexB]);
    }
}

void calculateNormals(std::vector<glm::vec3> &primitives, std::vector<glm::vec3> &normals) 
{
    unsigned short index1, index2, index3;
    glm::vec3 p1, p2, p3;
    glm::vec3 normal;
    glm::vec3 A, B;

    for (unsigned int i = 0; i < primitives.size(); i+=3)
    {

        p1 = primitives[i];
        p2 = primitives[i+1];
        p3 = primitives[i+2];

        A = p2 - p1;
        B = p3 - p1;

        normal.x = (A.y * B.z) - (A.z * B.y);
        normal.y = (A.z * B.x) - (A.x * B.z);
        normal.x = (A.x * B.y) - (A.y * B.x);

        normals[i] = normal;
        normals[i+1] = normal;
        normals[i+2] = normal;
        //std::cout<<normal.x<<"\t"<<normal.y<<"\t"<<normal.z<<std::endl;
    }
}

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

    Geometry::Vertices verticesBunny{geometryBunny.GetVertices()};
    Geometry::Indices indicesBunny{geometryBunny.GetIndices()};
    // Se forman las primitivas para enviarlas a opengl
    std::vector<glm::vec3> bunny(indicesBunny.size());
    updatePrimitives(verticesBunny, indicesBunny, bunny);
    unsigned bunnyNVertices = bunny.size();
    // Vector de velocidad
    std::vector<glm::vec3> velocityBunny(bunnyNVertices, glm::vec3(0, 0, 0));
    //Se obtienen las aristas
    Topology::Edges edgesBunny(topologyBunny.GetEdges());
    //Se calcula la distancia entre puntos de cada arista
    std::vector<float> distancesBunny(edgesBunny.size());
    calculateDistance(verticesBunny, edgesBunny, distancesBunny);
    // Se calcula las normales
    std::vector<glm::vec3> normalsBunny(indicesBunny.size());
    calculateNormals(bunny, normalsBunny);

    /*for(glm::vec3 &n: normalsBunny) {
        std::cout<<n.x<<"\t"<<n.y<<"\t"<<n.z<<std::endl;
    }*/
   

    OBJFile objDragon("dragon1.obj");

    Geometry geometryDragon(objDragon);
    Topology topologyDragon(geometryDragon.GetIndices());

    Geometry::Vertices verticesDragon{geometryDragon.GetVertices()};
    Geometry::Indices indicesDragon{geometryDragon.GetIndices()};
    // Se forman las primitivas para enviarlas a opengl
    std::vector<glm::vec3> dragon(indicesDragon.size());
    updatePrimitives(verticesDragon, indicesDragon, dragon);
    unsigned dragonNVertices = dragon.size();
    // Vector de velocidad
    std::vector<glm::vec3> velocityDragon(dragonNVertices, glm::vec3(0, 0, 0));
    //Se obtienen las aristas
    Topology::Edges edgesDragon(topologyDragon.GetEdges());
    //Se calcula la distancia entre puntos de cada arista
    std::vector<float> distancesDragon(edgesDragon.size());
    calculateDistance(verticesDragon, edgesDragon, distancesDragon);




    GLuint vbo[4]; // vertex buffer object para cada objeto

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, bunnyNVertices * 3 * sizeof(float), &bunny[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo[2]); //normals bunny
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, normalsBunny.size()*3*sizeof(float), &normalsBunny[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, dragonNVertices * 3 * sizeof(float), &dragon[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



    const char* verter_shader =
            "#version 400\n"
            "uniform mat4 projMatrix;" //projection
            "uniform mat4 mvMatrix;" // model-view

            "layout (location=0) in vec3 vertPos;"
            "layout (location=1) in vec3 vertNormal;"

            "out vec4 varyingColor;"

            "struct PositionalLight"
            "{ vec4 ambient;"
            "vec4 diffuse;"
            "vec4 specular;"
            "vec3 position;"
            "};"

            "struct Material"
            "{ vec4 ambient;"
            "vec4 diffuse;"
            "vec4 specular;"
            "float shininess;"
            "};"


            "uniform vec4 globalAmbient;"
            "uniform PositionalLight light;"
            "uniform Material material;"
            "uniform mat4 norm_matrix;" // for transforming normals

            "void main()"
            "{"

            "vec4 P = mvMatrix * vec4(vertPos,1.0);"

            "vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);"
            "vec3 L = normalize(light.position - P.xyz);"
            "vec3 V = normalize(-P.xyz);"
            "vec3 R = reflect(-L,N);"

            "vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;"
            "vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0);"
            "vec3 specular= material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess);"
            "varyingColor = vec4((ambient + diffuse + specular), 1.0);"
            "gl_Position = projMatrix*mvMatrix*vec4(vertPos, 1.0);"

            "}";

    const char *fragment_shader =
        "#version 400\n"
        "out vec4 color;"
        "in vec4 varyingColor;"
        "uniform mat4 mvMatrix;"   // model-view
        "uniform mat4 projMatrix;" // projection
        "void main(){"
        "color = varyingColor;"
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

    //light


    // SILVER material - ambient, diffuse, specular, and shininess
    //material
    float matAmbSilver[4] = {0.1923f, 0.1923f, 0.1923f, 1}; //RGB-A
    float matDifSilver[4] = {0.5075f, 0.5075f, 0.5075f, 1};
    float matSpeSilver[4] = {0.5083f, 0.5083f, 0.5083f, 1};
    float matShiSilver = 51.2f;
    
    //light source
    // white light properties
    float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f };

    float lightAmbient[4]  = {0.0f, 0.0f, 0.0f, 1.0f };
    float lightDiffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
    float lightPos[3];



    while (glfwWindowShouldClose(window) == 0) // animation
    {

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.2, 1.0); // RGB/255

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
        //light pyramid

        GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
        globalAmbLoc = glGetUniformLocation(shaderProgram, "globalAmbient");
        ambLoc = glGetUniformLocation(shaderProgram, "light.ambient");
        diffLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
        specLoc = glGetUniformLocation(shaderProgram, "light.specular");
        posLoc = glGetUniformLocation(shaderProgram, "light.position");
        mAmbLoc = glGetUniformLocation(shaderProgram, "material.ambient");
        mDiffLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
        mSpecLoc = glGetUniformLocation(shaderProgram, "material.specular");
        mShiLoc = glGetUniformLocation(shaderProgram, "material.shininess");
        GLuint nLoc = glGetUniformLocation(shaderProgram, "norm_matrix");

        currentLightPos = glm::vec3(5.0f, 5.0f, 2.0f);
        lightPosV = glm::vec3(vMat * glm::vec4(currentLightPos, 1.0));

        lightPos[0] = lightPosV.x;
        lightPos[1] = lightPosV.y;
        lightPos[2] = lightPosV.z;

        glm::mat4 invTrMat = glm::transpose(glm::inverse(mvMat)); //model-view for normals

        glProgramUniform4fv(shaderProgram, globalAmbLoc, 1, globalAmbient);
        glProgramUniform4fv(shaderProgram, ambLoc, 1, lightAmbient);
        glProgramUniform4fv(shaderProgram, diffLoc, 1, lightDiffuse);
        glProgramUniform4fv(shaderProgram, specLoc, 1, lightSpecular);
        glProgramUniform3fv(shaderProgram, posLoc, 1, lightPos);
        glProgramUniform4fv(shaderProgram, mAmbLoc, 1, matAmbSilver);
        glProgramUniform4fv(shaderProgram, mDiffLoc, 1, matDifSilver);
        glProgramUniform4fv(shaderProgram, mSpecLoc, 1, matSpeSilver);
        glProgramUniform1f(shaderProgram, mShiLoc, matShiSilver);
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));




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
        updatePrimitives(verticesBunny, indicesBunny, bunny);

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

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); //pyramid normals
        glBufferData(GL_ARRAY_BUFFER, normalsBunny.size()*3 * sizeof(float)  , &normalsBunny[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

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


