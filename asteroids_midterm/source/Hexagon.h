/*
* Hexagon class
* Created by Michael Furst 10/2017
*/

#ifndef Hexagon_h
#define Hexagon_h

#include "common.h"

class Hexagon {
	vec2 hex_vert[13];
	vec3 hex_color[13];
	float width, height;
	float angle;
	vec2 location;
	vec2 move;
	vec2 point;
	float size;
	float originalSize;
	mat4 M;

	struct {
		GLuint vao;
		GLuint program;
		GLuint buffer;
		GLuint vertex_shader, fragment_shader;
		GLint vpos_location, vcolor_location;
		GLint M_location;
	} GLvars;
	mat2 RotateZ2D( const GLfloat theta ){
		GLfloat angle = DegreesToRadians * theta;
		mat2 c;
		c[0][0] = c[1][1] = cos(angle);
		c[1][0] = sin(angle);
		c[0][1] = -c[1][0];
		return c;
	}
public:
	Hexagon() {
		this->location = vec2(0,0);
		this->angle=0.0;
		this->point = RotateZ2D(angle) * vec2(0,-1);
		this->move = vec2(0,0);
		this->move += 10*point;
		this->size=30;
		this->gl_init();
	};
	Hexagon(vec2 location, float angle, float size=30, bool flatTopped=true) {
		this->location = location;
		this->angle=angle;
		this->point = RotateZ2D(angle) * vec2(0,-1);
		this->move = vec2(0,0);
		this->move += 10*point;
		this->size=size;
		this->originalSize=size;
		this->gl_init(flatTopped);
	};
	void gl_init(bool flatTopped=true) {

		for (int i=0;i<12;i++) {
			float angle = (60*i+ ((flatTopped) ? 0 : 30) )*M_PI/180;
			float x = size*cos(angle);
			float y = size*sin(angle);
			hex_vert[i] = vec2(x,y);
		}
		hex_vert[12]=vec2(0,0);

		for (int i=0;i<6;i++) {
			hex_color[i] = vec3(1.0,0.0,0.0);
		}
		for (int i=6;i<12;i++) {
			hex_color[i] = vec3(1.0,1.0,1.0);
		}
		hex_color[12]=vec3(0.0,1.0,0.0);

		std::string vshader = shader_path + "vshader_Ship.glsl";
		std::string fshader = shader_path + "fshader_Ship.glsl";

		GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
		GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

		GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
		glCompileShader(GLvars.vertex_shader);
		check_shader_compilation(vshader, GLvars.vertex_shader);

		GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
		glCompileShader(GLvars.fragment_shader);
		check_shader_compilation(fshader, GLvars.fragment_shader);

		GLvars.program = glCreateProgram();
		glAttachShader(GLvars.program, GLvars.vertex_shader);
		glAttachShader(GLvars.program, GLvars.fragment_shader);

		glLinkProgram(GLvars.program);
		check_program_link(GLvars.program);

		glBindFragDataLocation(GLvars.program, 0, "fragColor");
		GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
		GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor" );
		GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );

		// Create a vertex array object
		glGenVertexArrays( 1, &GLvars.vao );
		//Set GL state to use vertex array object
		glBindVertexArray( GLvars.vao );

		//Generate buffer to hold our vertex data
		glGenBuffers( 1, &GLvars.buffer );
		//Set GL state to use this buffer
		glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );

		//Create GPU buffer to hold vertices and color
		glBufferData( GL_ARRAY_BUFFER, sizeof(hex_vert) + sizeof(hex_color), NULL, GL_STATIC_DRAW );
		//First part of array holds vertices
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(hex_vert), hex_vert );
		//Second part of array hold colors (offset by sizeof(triangle))
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(hex_vert), sizeof(hex_color), hex_color );

		glEnableVertexAttribArray(GLvars.vpos_location);
		glEnableVertexAttribArray(GLvars.vcolor_location );

		glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(hex_vert)) );

		glBindVertexArray(0);
	};
	void draw(mat4 proj) {
		glUseProgram( GLvars.program );
		glBindVertexArray( GLvars.vao );

		mat4 m = Translate(location.x, location.y,0.0) * RotateZ(angle)*Scale((size/originalSize),(size/originalSize),1);
		this->M=m;
		//!!!!!!!!Create a modelview matrix and pass it
		glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*m);

		//!!!!!!!!Draw something
		glDrawArrays(GL_TRIANGLE_FAN,0,6);
		glDrawArrays(GL_LINE_LOOP,6,6);
		glDrawArrays(GL_POINT,12,1);

		glBindVertexArray(0);
		glUseProgram(0);
	};

	void set_extents(int w, int h){
		width  = w/2;
		height = h/2;
	};
	void update_state() {

	};
	float getSize() {
		return this->size;
	};
	void setSize(float size) {
		this->size=size;
	};
	float getAngle() {
		return this->angle;
	};
	vec2 getLocation() {
		return this->location;
	};
	std::vector<vec2> getPoints() {
		std::vector<vec2> points = std::vector<vec2>(0);
		for (int i=0;i<6;i++) {
			vec2 point;
			point=hex_vert[i];
			vec4 p = vec4(point.x,point.y,1,1);
			p=M*(p);
			point=vec2(p.x,p.y);
			points.push_back(point);
		}
		return points;
	};
	void rotate(float angle) {
		this->angle+=angle;
	};
};

#endif