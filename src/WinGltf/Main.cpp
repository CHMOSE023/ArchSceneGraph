#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
 
 

int main()
{  
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
     
    // ╪стьнд╪Ч
    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, R"(C:\Users\BIM\Desktop\demo.gltf)");
     
    return 0;
}
