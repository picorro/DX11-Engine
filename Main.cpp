//#include "EngineApplication.hpp"
//#include "HelloTriangleApplication.hpp"
//#include "TexturingApplication.hpp"
//#include "Setting3DApplication.hpp"
#include "3DRenderingApplication.hpp"

int main(int argc, char* argv[])
{
    Rendering3DApplication application{ "D3D11 Engine" };
    //Setting3DApplication application{ "D3D11 Engine" };

    application.Run();
}