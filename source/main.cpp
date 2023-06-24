
#define STB_IMAGE_IMPLEMENTATION

#include <thirdparty.h>
#include <system/include/moduleSDL.h>
#include <system/include/moduleOpenGL.h>
#include <core/include/vmemory.h>

#include <testing/include/testing.h>

using namespace Vision;
static const char* grassPath = "texture\\grassPattern.jpg";

int main(int arc, char* argv[]) 
{
	return Testing::Run();
}
