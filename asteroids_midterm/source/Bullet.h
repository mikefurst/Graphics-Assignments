/*
* Bullet class
* Created by Michael Furst 10/2017
*/

#ifndef Bullet_h
#define Bullet_h

#define _ROTATE 15

#include "common.h"
class Bullet {
	vec2 bul_vert[12];
	vec3 bul_color[12];
	float width, height;

	//Bullet State
	struct {
		vec2 cur_location;
		vec2 pointing;
		vec2 move;
		float angle;
		bool bounce;
		int fuel;
		mat4 M;
	} state;

	//OpenGL vars
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

	Bullet() {
		state.cur_location = vec2(0,0);
		state.angle = 0.0;
		state.pointing = vec2(0.0,-1.0);
		state.move = vec2(0,0);
		state.bounce=false;
		state.fuel=0;
	};
	Bullet(vec2 loc, vec2 pointing, float angle=0.0, int fuel=20.0) {
		state.cur_location = loc;
		state.angle = angle;
		state.pointing = pointing;
		state.move = vec2(0,0);
		state.move += 3 * state.pointing;
		state.move*=50;
		state.bounce=false;
		state.fuel=fuel;
	};

	inline void rotateLeft() {  state.angle-=_ROTATE;   state.pointing =  RotateZ2D(state.angle) * vec2(0.0,-1.0);}
	inline void rotateRight(){  state.angle+=_ROTATE;   state.pointing =  RotateZ2D(state.angle) * vec2(0.0,-1.0);}

	void update_state() {

		if (state.fuel>0) {
			state.fuel--;
			state.move+=5*state.pointing;
			if (length(state.move)>55) {
				state.move=normalize(state.move)*55;
			}
		}

		//!!!!!!!!Dampen the velocity at every timestep to lessen intertia
		state.move*=0.85;
		//!!!!!!!!Move the bullet location
		state.cur_location+=state.move;

		//Wrap the bullet position when at the boundary
		//!!!!!!!!This will change depending on the world coordinates you are using
		if(state.cur_location.x < -width || state.cur_location.x > width){
			state.cur_location.x = -state.cur_location.x + (10 * (state.cur_location.x/abs(state.cur_location.x)));
		}
		if(state.cur_location.y < -height ||state.cur_location.y > height){
			state.cur_location.y = -state.cur_location.y + (10 * (state.cur_location.y/abs(state.cur_location.y)));
		}
	};

	void gl_init() {

		bul_vert[0] = vec2(0.0,-10.0);
		bul_vert[1] = vec2(-5,-5);
		bul_vert[2] = vec2(-3,2);
		bul_vert[3] = vec2(-3,7);
		bul_vert[4] = vec2(-7,10);
		bul_vert[5] = vec2(7,10);
		bul_vert[6] = vec2(3,7);
		bul_vert[7] = vec2(3,2);
		bul_vert[8] = vec2(5,-5);
		bul_vert[9] = vec2(-6,11);
		bul_vert[10] = vec2(6,11);
		bul_vert[11] = vec2(0,15);

		for (int i=0;i<9;i++) {
			bul_color[i] = vec3(1.0,1.0,1.0);
		}
		bul_color[9]=vec3(1.0,0.5,0.0);
		bul_color[10]=vec3(1.0,0.5,0.0);
		bul_color[11]=vec3(1.0,0.0,0.0);

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
		glBufferData( GL_ARRAY_BUFFER, sizeof(bul_vert) + sizeof(bul_color), NULL, GL_STATIC_DRAW );
		//First part of array holds vertices
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(bul_vert), bul_vert );
		//Second part of array hold colors (offset by sizeof(triangle))
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(bul_vert), sizeof(bul_color), bul_color );

		glEnableVertexAttribArray(GLvars.vpos_location);
		glEnableVertexAttribArray(GLvars.vcolor_location );

		glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(bul_vert)) );

		glBindVertexArray(0);
	};

	void draw(mat4 proj) {
		glUseProgram( GLvars.program );
		glBindVertexArray( GLvars.vao );

		mat4 m = Translate(state.cur_location.x, state.cur_location.y,0.0) * RotateZ(state.angle);
		state.M=m;
		//!!!!!!!!Create a modelview matrix and pass it
		glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj * m);

		//!!!!!!!!Draw something
		if (GameSettings.dMode==_OUTLINE) {
			glDrawArrays(GL_LINE_LOOP,0,9);
		}
		else if (GameSettings.dMode==_FILL) {
			glDrawArrays(GL_TRIANGLE_FAN,0,9);
		}
		if (state.fuel>0) {
			glDrawArrays(GL_TRIANGLES,9,3);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	};

	void set_extents(int w, int h){
		width  = w/2;
		height = h/2;
	};
	float getSpeed() {
		return length(state.move);
	};

	bool shouldDraw() {
		return length(state.move)>=1.0;
	};
	Explosion explode() {
		return Explosion(-state.cur_location,state.angle,(rand()%3+3));
	};
	Explosion explodeOnColide() {
		return Explosion(state.cur_location,state.angle,(rand()%3+3));
	};
	std::vector<vec2> getPoints() {
		std::vector<vec2> points = std::vector<vec2>(0);
		if (GameSettings.bMode==BoundsMode::_BOUNDINGBOX) {
			vec2 topLeftCorner = vec2(state.cur_location.x-11,state.cur_location.y-11);
			vec2 topRightCorner = vec2(state.cur_location.x+11,state.cur_location.y-11);
			vec2 bottomRightCorner = vec2(state.cur_location.x+11,state.cur_location.y+11);
			vec2 bottomLeftCorner = vec2(state.cur_location.x-11,state.cur_location.y+11);
			points.push_back(topLeftCorner);
			points.push_back(topRightCorner);
			points.push_back(bottomLeftCorner);
			points.push_back(bottomRightCorner);
		}
		else if (GameSettings.bMode==BoundsMode::_FULL_OBJECT) {
			for (int i=0;i<9;i++) {
				vec2 point;
				point=bul_vert[i];
				vec4 p = vec4(point.x,point.y,1,1);
				p=state.M*(p);
				point=vec2(p.x,p.y);
				points.push_back(point);
			}
		}
		return points;
	};
};

#endif