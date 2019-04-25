#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/Angel.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#ifndef pointsInACircle
#define pointsInACircle 4005
#endif

using namespace Angel;


//Globals there is a way to avoid this, but for now let's do the bad thing.
enum{_TRIANGLE, _SQUARE, _CIRCLE, _CIRCLEHSV, _HEXAGON, _NUM_OBJ};
int current_object;

//one for inside rest for outside
bool isAnimated=false;

//GLFW error callback
static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

//GLFW keypress callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    current_object = (current_object+1)%_NUM_OBJ;
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	  isAnimated = !isAnimated;
  if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	  current_object = _TRIANGLE;
  if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	  current_object=_SQUARE;
  if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	  current_object=_CIRCLE;
  if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	  current_object=_CIRCLEHSV;
  if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	  current_object=_HEXAGON;
}

void makeTriangle(GLuint &triVao, GLint vpos_location, GLint vcolor_location) {
	GLuint tri_buffer;
	vec2 triangle[3]          = { vec2(-1.0, -1.0),
                                vec2( 0.0,  1.0),
                                vec2( 1.0, -1.0)};
	vec3 triangle_colors[3]   = { vec3( 0.0, 0.0, 1.0),
                                vec3( 1.0, 0.0, 0.0),
                                vec3( 0.0, 1.0, 0.0)};
	// Create a vertex array object
	glGenVertexArrays( 1, &triVao );
	//Set GL state to use vertex array object
	glBindVertexArray( triVao );
  
	//Generate buffer to hold our vertex data
	glGenBuffers( 1, &tri_buffer );
	//Set GL state to use this buffer
	glBindBuffer( GL_ARRAY_BUFFER, tri_buffer );
  
	//Create GPU buffer to hold vertices and color
	glBufferData( GL_ARRAY_BUFFER, sizeof(triangle)+sizeof(triangle_colors), NULL, GL_STATIC_DRAW );
	//First part of array holds vertices
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(triangle), triangle );
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(triangle), sizeof(triangle_colors), triangle_colors );
  
	glEnableVertexAttribArray(vpos_location);
	glEnableVertexAttribArray( vcolor_location );
  
	glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(triangle)) );
  
	glBindVertexArray(0);
}
void makeSquare(GLuint &sqVao, GLint vpos_location, GLint vcolor_location) {
	GLuint sq_buffer;
	vec2 square[4]            = { vec2(-0.5, -0.5),
                                vec2(-0.5,  0.5),
                                vec2( 0.5, -0.5),
                                vec2( 0.5,  0.5)};
	vec3 square_colors[4]     = { vec3( 0.78, 0.85, 0.97),
                                vec3( 0.78, 0.85, 0.97),
                                vec3( 0.78, 0.85, 0.97),
                                vec3( 0.78, 0.85, 0.97)};
	// Create a vertex array object
  glGenVertexArrays( 1, &sqVao );
  //Set GL state to use vertex array object
  glBindVertexArray( sqVao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &sq_buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, sq_buffer );
  
  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, sizeof(square)+sizeof(square_colors), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(square), square );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(square), sizeof(square_colors), square_colors );
  
  glEnableVertexAttribArray(vpos_location);
  glEnableVertexAttribArray(vcolor_location);
  
  glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(square)) );
  
  glBindVertexArray(0);
}
void makeBackSquare(GLuint &backSqVao, GLint vpos_location, GLint vcolor_location) {
	GLuint back_sq_buffer;

	vec2 backSquare[4]        = { vec2(-0.51, -0.51),
								vec2(-0.51,  0.51),
								vec2( 0.51, -0.51),
								vec2( 0.51,  0.51)};

	vec3 back_square_colors[4]= { vec3( 0.0, 0.0, 0.0),
								vec3( 0.0, 0.0, 0.0),
								vec3( 0.0, 0.0, 0.0),
								vec3( 0.0, 0.0, 0.0)};
	//create back square object
	glGenVertexArrays(1, &backSqVao);
	glBindVertexArray(backSqVao);
	glGenBuffers(1, &back_sq_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, back_sq_buffer);
	glBufferData( GL_ARRAY_BUFFER, sizeof(backSquare)+sizeof(back_square_colors), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(backSquare), backSquare);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(backSquare), sizeof(back_square_colors), back_square_colors);

	glEnableVertexAttribArray(vpos_location);
	glEnableVertexAttribArray(vcolor_location);
  
	glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(backSquare)) );
  
	glBindVertexArray(0);

}
void makeHSVCircle(GLuint &circVao, GLint vpos_location, GLint vcolor_location) {
	//make circle
	GLuint circle_buffer;
	float posX=0.0;
	float posY=0.0;
	float radius=0.75;
	vec2 *circle = new vec2[pointsInACircle];
	circle[0] = vec2(posX,posY);
	float angleInterval = 360.0f/(pointsInACircle-2);
	for (int i=1;i<pointsInACircle;i++) {
		float angle = (float(i-1) * angleInterval);
		float x = posX + radius * cosf(float(angle*DegreesToRadians));
		float y = posY + radius * sinf(float(angle*DegreesToRadians));
		circle[i] = vec2(x,y);
	}
	int nColors = pointsInACircle;
	vec3* circleColors = new vec3[nColors];
	float intadjust = pointsInACircle/6;
	float interval = 1.0/intadjust;
	bool iR = false;
	bool dR = false;
	bool iG = true;
	bool dG = false;
	bool iB = false;
	bool dB = false;
	float r,g,b;
	r = 1.0;
	g = 0.0;
	b = 0.0;
	for (int i=0;i<nColors;i++) {
		r += interval*iR;
		r -= interval*dR;
		g += interval*iG;
		g -= interval*dG;
		b += interval*iB;
		b -= interval*dB;

		if (iB && b>=1) {//now red and blue
			iB = false;
			dG = true;
		}
		if (dR && r<=0) {//now blue
			dR = false;
			iB = true;
		}
		if (iG && g>=1) {//now blue and green
			iG = false;
			dR = true;
		}
		if (dB && b<=0) {//now green
			dB = false;
			iR = true;
		}
		if (iR && r>=1) {//now green and red
			iR = false;
			dB = true;
		}
		if (dG && g<=0) {//now red
			dG=false;
			iR = true;
		}

		circleColors[i] = vec3(r,g,b);
	}	
	circleColors[0]=vec3(1.0,1.0,1.0);
	unsigned long long int sizeCircle = sizeof(vec2) * pointsInACircle;
	unsigned long long int sizeColors = sizeof(vec3) * nColors;
	glGenVertexArrays(1, &circVao);
	glBindVertexArray(circVao);
	glGenBuffers(1, &circle_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, circle_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeCircle+sizeColors, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeCircle, circle );
	glBufferSubData( GL_ARRAY_BUFFER, sizeCircle, sizeColors, circleColors);

	glEnableVertexAttribArray(vpos_location);
	glEnableVertexAttribArray(vcolor_location);
  
	glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeCircle) );
	glBindVertexArray(0);
}
void makeRGBCircle(GLuint &circVao, GLint vpos_location, GLint vcolor_location) {
	//make circle
	GLuint circle_buffer;
	float posX=0.0;
	float posY=0.0;
	float radius=0.75;
	vec2 *circle = new vec2[pointsInACircle];
	circle[0] = vec2(posX,posY);
	float angleInterval = 360.0f/(pointsInACircle-2);
	for (int i=1;i<pointsInACircle;i++) {
		float angle = (float(i-1) * angleInterval) + 90.0f;
		float x = posX + radius * cosf(float(angle*DegreesToRadians));
		float y = posY + radius * sinf(float(angle*DegreesToRadians));
		circle[i] = vec2(x,y);
	}
	int nColors = pointsInACircle;
	vec3* circleColors = new vec3[nColors];
	float intadjust = pointsInACircle/6;
	float interval = 1.0/intadjust;
	bool iR = false;
	bool dR = false;
	bool iG = false;
	bool dG = false;
	bool iB = true;
	bool dB = false;
	float r,g,b;
	r = 1.0;
	g = 0.0;
	b = 0.0;
	for (int i=0;i<nColors;i++) {
		r += interval*iR;
		r -= interval*dR;
		g += interval*iG;
		g -= interval*dG;
		b += interval*iB;
		b -= interval*dB;

		if (iB && b>=1) {//now red and blue
			iB = false;
			dR = true;
		}
		if (dR && r<=0) {//now blue
			dR = false;
			iG = true;
		}
		if (iG && g>=1) {//now blue and green
			iG = false;
			dB = true;
		}
		if (dB && b<=0) {//now green
			dB = false;
			iR = true;
		}
		if (iR && r>=1) {//now green and red
			iR = false;
			dG = true;	
		}
		if (dG && g<=0) {//now red
			dG=false;
			iB = true;
		}

		circleColors[i] = vec3(r,g,b);
	}	
	circleColors[0]=vec3(1.0/3.0,1.0/3.0,1.0/3.0);
	unsigned long long int sizeCircle = sizeof(vec2) * pointsInACircle;
	unsigned long long int sizeColors = sizeof(vec3) * nColors;
	glGenVertexArrays(1, &circVao);
	glBindVertexArray(circVao);
	glGenBuffers(1, &circle_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, circle_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeCircle+sizeColors, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeCircle, circle );
	glBufferSubData( GL_ARRAY_BUFFER, sizeCircle, sizeColors, circleColors);

	glEnableVertexAttribArray(vpos_location);
	glEnableVertexAttribArray(vcolor_location);
  
	glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeCircle) );
	glBindVertexArray(0);
}
void makeRGBHexagon(GLuint &hexVao, GLint vpos_location, GLint vcolor_location) {
	//I thought the hex looked cool so I added it
	int points = 8;
	GLuint hexBuffer;
	float posX=0.0;
	float posY=0.0;
	float radius=0.75;
	vec2 *hex = new vec2[points];
	hex[0] = vec2(posX,posY);
	float angleInterval = 360.0f/(points-2);
	for (int i=1;i<points;i++) {
		float angle = (float(i-1) * angleInterval) + 90.0f;
		float x = posX + radius * cosf(float(angle*DegreesToRadians));
		float y = posY + radius * sinf(float(angle*DegreesToRadians));
		hex[i] = vec2(x,y);
	}
	int nColors = points;
	vec3* hexColors = new vec3[nColors];
	float intadjust = points/6;
	float interval = 1.0/intadjust;
	bool iR = false;
	bool dR = false;
	bool iG = false;
	bool dG = false;
	bool iB = true;
	bool dB = false;
	float r,g,b;
	r = 1.0;
	g = 0.0;
	b = 0.0;
	for (int i=0;i<nColors;i++) {
		r += interval*iR;
		r -= interval*dR;
		g += interval*iG;
		g -= interval*dG;
		b += interval*iB;
		b -= interval*dB;

		if (iB && b>=1) {//now red and blue
			iB = false;
			dR = true;
		}
		if (dR && r<=0) {//now blue
			dR = false;
			iG = true;
		}
		if (iG && g>=1) {//now blue and green
			iG = false;
			dB = true;
		}
		if (dB && b<=0) {//now green
			dB = false;
			iR = true;
		}
		if (iR && r>=1) {//now green and red
			iR = false;
			dG = true;
		}
		if (dG && g<=0) {//now red
			dG=false;
			iB = true;
		}

		hexColors[i] = vec3(r,g,b);
	}	
	unsigned long long int sizeHex = sizeof(vec2) * points;
	unsigned long long int sizeColors = sizeof(vec3) * nColors;
	glGenVertexArrays(1, &hexVao);
	glBindVertexArray(hexVao);
	glGenBuffers(1, &hexBuffer);
	glBindBuffer( GL_ARRAY_BUFFER, hexBuffer);
	glBufferData( GL_ARRAY_BUFFER, sizeHex+sizeColors, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeHex, hex );
	glBufferSubData( GL_ARRAY_BUFFER, sizeHex, sizeColors, hexColors);

	glEnableVertexAttribArray(vpos_location);
	glEnableVertexAttribArray(vcolor_location);
  
	glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeHex) );
	glBindVertexArray(0);
}

//Initialize everything
void init(GLuint &glShaderProgram, GLuint &triVao, GLuint &sqVao, GLuint &backSqVao, GLuint &circVao, GLuint &hsvVao, GLuint &hexVao){
  
  GLuint vertex_shader, fragment_shader;
  GLint vpos_location, vcolor_location;
  
  vec2 concave_poly[9]      = { vec2(0.44,  0.65),
                                vec2(-0.42, 0.65),
                                vec2(-0.86, -0.28),
                                vec2(-0.04, -0.70),
                                vec2(0.29,  -0.46),
                                vec2(-0.18, -0.14),
                                vec2(0.19, 0.28),
                                vec2(0.57, -0.18),
                                vec2(0.90, 0.12)};
 
  //String with absolute paths to our shaders, shader_path set by CMAKE hack
  std::string vshader = shader_path + "vshader.glsl";
  std::string fshader = shader_path + "fshader.glsl";

  //Read in shader code to a character array
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());
  
  //Create and compile vertex shader
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_shader_compilation(vshader, vertex_shader);//error lies here - nonascii code being added to file
  
  //Create and compile fragment shader
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_shader_compilation(fshader, fragment_shader);
  
  //Create shader program from the 2 shaders
  glShaderProgram = glCreateProgram();
  glAttachShader(glShaderProgram, vertex_shader);
  glAttachShader(glShaderProgram, fragment_shader);
  glLinkProgram(glShaderProgram);
  check_program_link(glShaderProgram);
  
  //bind or find the numerical location for shader variables
  glBindFragDataLocation(glShaderProgram, 0, "fragColor");
  vpos_location   = glGetAttribLocation(glShaderProgram, "vPos");
  vcolor_location = glGetAttribLocation(glShaderProgram, "vColor" );
  
  makeTriangle(triVao,vpos_location,vcolor_location);
  makeSquare(sqVao,vpos_location,vcolor_location);
  makeBackSquare(backSqVao,vpos_location,vcolor_location);
  makeRGBCircle(circVao,vpos_location,vcolor_location);
  makeHSVCircle(hsvVao,vpos_location,vcolor_location);
  makeRGBHexagon(hexVao,vpos_location,vcolor_location);

}

//Animation callback.  For now this does nothing
void animate(){
  //Do something every second
  if(glfwGetTime() > 1.0){
    glfwSetTime(0.0);
    //Do something Here	
	if (isAnimated) {
		current_object = (current_object+1)%_NUM_OBJ;
	}
  }
}

//Main
int main(void)
{
  GLFWwindow* window;
  GLuint glShaderProgram;
  GLuint triVao;
  GLuint sqVao;
  GLuint backSqVao;
  GLuint circleHSVVao;
  GLuint circle;
  GLuint hexVao;
  
  //Set the error callback defined above
  glfwSetErrorCallback(error_callback);
  
  //Init glfw
  if (!glfwInit()) {
	  exit(EXIT_FAILURE);
  }

  //Enforce OpenGL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  //Multisampling
  glfwWindowHint(GLFW_SAMPLES, 4);

  //Create window
  window = glfwCreateWindow(1024, 1024, "Rainbow triangle and friends", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  //Set key callback
  glfwSetKeyCallback(window, key_callback);
  //GLFW setup calls
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);
  //Init the geomerty and shaders
  init(glShaderProgram, triVao, sqVao, backSqVao, circle, circleHSVVao, hexVao);
  //set the background/clear color
  glClearColor(1.0, 1.0, 1.0, 1.0);
  //main loop, loop forever until user closes window
  while (!glfwWindowShouldClose(window)){
    
    //Grab window size in pixels
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
   
    //Set viewport
    glViewport(0, 0, width, height);
    
    //Call animation update
	animate();
    
    //Clear color buffer bit
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Enable our shader program
    glUseProgram(glShaderProgram);

    //Draw
    switch(current_object){
      case _TRIANGLE:
        glBindVertexArray( triVao );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
      case _SQUARE:
		glBindVertexArray( backSqVao );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray( sqVao );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        break;
	  case _CIRCLE:
		glBindVertexArray(circle);
		glDrawArrays(GL_TRIANGLE_FAN, 0, pointsInACircle);
		break;
	  case _CIRCLEHSV:
		glBindVertexArray(circleHSVVao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, pointsInACircle);
		break;
	  case _HEXAGON:
		glBindVertexArray(hexVao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
		break;
	}
    glBindVertexArray(0);
    
    //Swap our double buffers
    glfwSwapBuffers(window);
    
    //GLFW event handler
    glfwPollEvents();
  
  }
  //Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
