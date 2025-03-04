#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <limits>
#include <functional>
#include <iomanip>
#include <cstring>  // Added for std::strncmp

#include "Container/Include/base.hpp"
#include "Container/Include/qstack.hpp"
#include "Container/Include/qarray.hpp"
#include "Quake/Container/Include/qforward_list.hpp"
#include "Quake/Container/Include/qstring.h"

#include "Container/Include/iterator.hpp"

#include "Math/Include/math.h"
#include "Math/Include/vector2.hpp"

#include "Memory/Include/Allocator/pool_allocator.hpp"

#include "Auxiliary/Include/string_aux.h"
#include "Auxiliary/Include/algorithm.h"

#include "UI/Include/OpenGL/opengl_context.h"

#include <gtest/gtest.h>

#define GTEST

#define AMOUNT 40000000

int main(int argc, char* argv[])  // Fixed signature to accept command-line arguments
{
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

#ifdef GTEST
    ::testing::InitGoogleTest(&argc, argv);  // Fixed to use argc and argv
    return RUN_ALL_TESTS();  // Simplified return statement
#endif

    /*char* A = "Hello ";
    char* B = "World!";
    char* together = "Hello World!";
    char* cated = new char[12];
    Q_strCpy(A, cated);
    Q_strCat(B, cated);*/

    //char* cated = Q_strCat(A, B);
    //std::cout << cated << std::endl;
    //std::cout << Q_strCmp(together, cated) << std::endl;

    /*QString hello("Hello World!");
    QString::Iterator i = hello.findnth('z', 1);
    std::cout << hello.substring(i) << std::endl;*/

    //QString str("Hello");
    //QBool b = str < "hello";
    //QBool b = QString("Hello") == QString("Hello");
    //std::cout << b << std::endl;

    const char* str1 = "Hello World";
    const char* str2 = "Hello Worlz";
    std::cout << std::strncmp(str1, str2, 12) << std::endl;  // Qualified with std::

    std::cin.get();
    return 0;
}