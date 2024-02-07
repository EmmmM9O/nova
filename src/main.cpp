#include <GLFW/glfw3.h>
int main(void)
{
	    GLFWwindow* window;
	    if (!glfwInit())
		            return -1;
	    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	        if (!window)
			    {
				            glfwTerminate();
					            return -1;
						        }
		glfwTerminate();
		    return 0;
}
