#ifndef OBJFILE_H
#define OBJFILE_H

#include <fstream>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <iostream>
#include <iomanip>

//Marco Antonio Quiña Mamani

class OBJFile
{

public:
    OBJFile();
    static void loadOBJ(const std::string& filename, 
                    std::vector<glm::vec3>& out_vertices, 
                    std::vector<glm::vec2>& out_uvs, 
                    std::vector<glm::vec3>& out_normals);
    static void writeFramesInOBJ(const std::string& directory, 
                            unsigned index,
                            std::vector<glm::vec3>& out_vertices, 
                            std::vector<glm::vec2>& out_uvs, 
                            std::vector<glm::vec3>& out_normals);
    static void writeFramesInVTK(const std::string& directory, 
                                unsigned index,
                                std::vector<glm::vec3>& out_vertices, 
                                std::vector<glm::vec2>& out_uvs, 
                                std::vector<glm::vec3>& out_normals);
};

#endif // OBJFILE_H
