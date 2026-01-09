#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

class Shader {
public:
    // Program ID
    unsigned int ID;

    // Default constructor
    Shader() : ID(0) {}
    
    // Constructor builds the shader from embedded source
    Shader(const char* vertexSource, const char* fragmentSource);
    
    // Use/activate the shader
    void use();
    
    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    // Checks if the different shaders have compiled properly
    void checkCompileErrors(GLuint shader, std::string type);
};