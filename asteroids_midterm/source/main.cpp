#include "common.h"
#include "Explosion.h"

using namespace Angel;

std::vector<Hexagon*> hexGrid;
vec2 screenSize;
vec2 position(0,0);

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (action==GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			position.y-=1;
			break;
		case GLFW_KEY_S:
			position.y+=1;
			break;
		case GLFW_KEY_A:
			position.x-=1;
			break;
		case GLFW_KEY_D:
			position.x+=1;
			break;
		default:
			break;
		}
	}
}

void createHexGrid(float size=30, bool flatTopped = true) {
	if (flatTopped) {
		float width=size*2;
		float height = sqrt(3)/2*width;
		for (int y=-20;y<=20;y++) {
			for (int x=-20;x<=20;x++) {
				float posX = x * width*3/4;
				float posY = y * height;
				if (x%2==0) {
					posY+=height/2;
				}
				Hexagon* h = new Hexagon(vec2(posX,posY),0,size,flatTopped);
				hexGrid.push_back(h);
			}
		}
	}
	else {
		float height=size*2;
		float width = sqrt(3)/2*height;
		for (int x=-20;x<=20;x++) {
			for (int y=-20;y<=20;y++) {
				float posX = x*width;
				float posY = y * (height*3/4);
				if (y%2==0) {
					posX+=width/2;
				}
				Hexagon* h = new Hexagon(vec2(posX,posY),0,size,flatTopped);
				hexGrid.push_back(h);
			}
		}
	}
}

void init(){
	srand(time(NULL));
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	hexGrid = std::vector<Hexagon*>();
	createHexGrid();
}
void draw(mat4 proj) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i=0;i<hexGrid.size();i++) {
		hexGrid[i]->draw(proj);
	}
}

bool pointLiesInPolygonLeft(vec2 point, std::vector<vec2> polygon) {
    int intersections=0;
    for (int i=0;i<polygon.size();i++) {
        vec2 pointA = polygon.at(i);
        int a = (i<polygon.size()-1) ? (i+1) : (0);
        vec2 pointB = polygon.at(a);
        if (point.y<=max(pointA.y,pointB.y)&&point.y>=min(pointA.y,pointB.y)) {
            //point is in right altitude
			//find x of poly line at altitude
			//find slope
            float m = (pointA.y-pointB.y)/(pointA.x-pointB.x);
			//find intercept
            //y = mx + b => y-mx = b
            float b = pointA.y-(pointA.x*m);
			//find x
            // y = mx+b => y-b = mx => y-b/m = x
            float x = (point.y-b)/m;
			//add 1 to intersections if the point x is less than the point 
            if (point.x >= x) {
				intersections++;
			}
        }
    }
	if (intersections>0 && intersections%2==1) {
		return true;
	}
    return false;
}
bool pointLiesInPolygonRight(vec2 point, std::vector<vec2> polygon) {
    int intersections=0;
    for (int i=0;i<polygon.size();i++) {
        vec2 pointA = polygon.at(i);
        int a = (i<polygon.size()-1) ? (i+1) : (0);
        vec2 pointB = polygon.at(a);
        if (point.y<=max(pointA.y,pointB.y)&&point.y>=min(pointA.y,pointB.y)) {
            //point is in right altitude
			//find x of poly line at altitude
			//find slope
            float m = (pointA.y-pointB.y)/(pointA.x-pointB.x);
			//find intercept
            //y = mx + b => y-mx = b
            float b = pointA.y-(pointA.x*m);
			//find x
            // y = mx+b => y-b = mx => y-b/m = x
            float x = (point.y-b)/m;
			//add 1 to intersections if the point x is less than the point 
            if (point.x <= x) {
				intersections++;
			}
        }
    }
	if (intersections>0 && intersections%2==1) {
		return true;
	}
    return false;
}
bool pointLiesInPolygon(vec2 point, std::vector<vec2> polygon) {
	return (pointLiesInPolygonLeft(point,polygon)&&pointLiesInPolygonRight(point,polygon));
}

void checkColisions() {
}

void animate() {
	if(glfwGetTime() > 0.033){
		glfwSetTime(0.0);
		for (int i=0;i<hexGrid.size();i++) {
			hexGrid[i]->update_state();
		}
		checkColisions();
	}
}

void drawGame(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	screenSize=vec2(width,height);
	glViewport(position.x, position.y, width+position.x, height+position.y);
	for (int i=0;i<hexGrid.size();i++) {
		hexGrid[i]->set_extents(width,height);\
	}
	mat4 proj = Ortho2D(-width/2, width/2, height/2, -height/2);
		
	animate();

	draw(proj);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void run(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)){
		drawGame(window);
	}
	glfwDestroyWindow(window);
}

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY,true);

	glfwWindowHint(GLFW_SAMPLES, 10);


	//window = glfwCreateWindow(mode->width, mode->height, "Asteroids!", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(640, 480, "Asteroids!", NULL, NULL);
	screenSize=(640,480);
	
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	init();
	run(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
