#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OBJFile.h"
#include "Geometry.h"


int main()
{

    GLFWwindow* window;

    if(glfwInit()==0) //si todo esta ok con el SO
    {
        std::cerr<<"GLFW failed\n";
        return -1;
    }

    //dar memoria
    window = glfwCreateWindow(1000, 1000, "Hello triangle", nullptr, nullptr);

    if(window==nullptr)
    {
        std::cerr<<"GLFW failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if(glewInit() != GLEW_OK)
    {
        std::cerr<<"GLEW failed\n";
        glfwTerminate(); //delete
        return -1;

    }

    //Se realiza la lectura de los archivos obj
    OBJFile objBunny("conejo1.obj");
    Geometry geometryBunny(objBunny);

    OBJFile objDragon("dragon1.obj");
    Geometry geometryDragon(objDragon);

    std::vector<glm::vec3> bunny{geometryBunny.GetVertices()};
    std::vector<glm::vec3> dragon{geometryDragon.GetVertices()};

    std::cout<<bunny.size()<<std::endl;
    std::cout<<geometryBunny.GetIndices().size()<<std::endl;

    unsigned bunnyNVertices = bunny.size();
    unsigned dragonNVertices = dragon.size();
    std::vector<float> velocity(bunnyNVertices, 0.f);

    GLuint vbo[2]; //vertex buffer object para cada objeto

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, bunnyNVertices*3*sizeof(float), &bunny[0], GL_STATIC_DRAW);


    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, dragonNVertices*3*sizeof(float), &dragon[0], GL_STATIC_DRAW);


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



    const char * verter_shader =
            "#version 400\n"
            "in vec3 vp;"
            "uniform mat4 mvMatrix;" // model-view
            "uniform mat4 projMatrix;" //projection
            "void main(){"
            "gl_Position = projMatrix * mvMatrix * vec4(vp, 1.0);"
            "}";




    const char * fragment_shader =
            "#version 400\n"
            "out vec4 frag_color;"
            "uniform mat4 mvMatrix;" // model-view
            "uniform mat4 projMatrix;" //projection
            "void main(){"
            "frag_color = vec4(0.5, 0.5, 0.0, 0.5);"
            "}";


    GLuint vs  = glCreateShader(GL_VERTEX_SHADER);
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



    //all objects
    float cameraX = 0.f;
    float cameraY = 0.f;
    float cameraZ = 7.f;


    //bunny
    float lockX = 0.0;
    float lockY = -2.0;
    float lockZ = 0.0; //offset


    //dragon
    float pyLockX = -2.0;
    float pyLockY = 0.0;
    float pyLockZ = 0.0;



    float h = 0.00001;

    int width, height;
    glm::mat4 mMat, vMat, mvMat, pMat, sMat;

    float angle=0;
    while (glfwWindowShouldClose(window)==0) //animation
    {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2, 0.2, 0.5, 0.5); //RGB/255

        //get matrix locations
        GLuint mvLoc    = glGetUniformLocation(shaderProgram, "mvMatrix");
        GLuint projLoc  = glGetUniformLocation(shaderProgram, "projMatrix");


        //perstective
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = static_cast<float>(width)/static_cast<float>(height);
        pMat =  glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);



        //matrix model - view
        vMat = glm::translate(glm::mat4(1.f), glm::vec3(-cameraX, -cameraY, -cameraZ));


        glm::mat4 vRot  = glm::rotate(glm::mat4(1.f) , angle , glm::vec3(0, -1,0));

        vMat =  vMat*vRot;




        //aminate here
        /*for (unsigned int i = 0; i < bunnyNVertices; ++i)
        {
            velocity[i] = velocity[i] + (-0.98/1);
            bunny[i] = bunny[i] + h*velocity[i];
        }*/
        

        /*velocity[1] = velocity[1] + (-0.98/1);
        bunny[1] = bunny[1] + h*velocity[1];

        velocity[4] = velocity[4] + (-0.98/1);
        bunny[4] = bunny[4] + h*velocity[4];

        velocity[7] = velocity[7] + (-0.98/1);
        bunny[7] = bunny[7] + h*velocity[7];*/

        glm::mat4 mOrg = glm::translate(glm::mat4(1.f), glm::vec3(1.0f, -1.0f, 1.0f)); //offset
        glm::mat4 mBack = glm::translate(glm::mat4(1.f), glm::vec3(-1.0f, 1.0f, -1.0f)); //offset

        glm::mat4 mRot  = mBack*glm::rotate(mOrg, angle , glm::vec3(1  , 0, 0 ));


        angle+=0.05;
        if(angle>360) angle=0;

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(lockX, lockY, lockZ))*mRot; //offset
        sMat = glm::scale(glm::mat4(1.f), glm::vec3(15.f, 15.f, 15.f));

        mvMat = vMat * mMat * sMat;
        //mvMat = vMat*sMat;


        glUniformMatrix4fv(mvLoc, 1, GL_FALSE,  glm::value_ptr(mvMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE,  glm::value_ptr(pMat));





        //colision

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, bunnyNVertices*3*sizeof(float), &bunny[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        //glPointSize(10.f);
        //glDrawArrays(GL_POINTS, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, bunnyNVertices);

        //draw pyramid

        mMat = glm::translate(glm::mat4(1.f), glm::vec3(pyLockX, pyLockY, pyLockZ)); //offset
        sMat = glm::scale(glm::mat4(1.f), glm::vec3(10.f, 10.f, 10.f));

        mvMat = vMat * mMat * sMat;

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE,  glm::value_ptr(mvMat));


        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, dragonNVertices*3*sizeof(float), &dragon[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, dragonNVertices);



        glfwSwapBuffers(window); //Buffers->OpenGL

        glfwPollEvents(); //User

    }

    glfwTerminate();


    std::cout << "Finished!" << std::endl;
    return 0;
}