//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//  Edited by Michael Furst 10/2017
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "common.h"

#define _MAX_SPEED 20
#define _DAMPING 0.98
#define _ACC 3
#define _ROT 15

class Ship{

	vec2 ship_vert[49];
	vec3 ship_color[49];

	float width, height;


	//Ship State
	struct {
		vec2 cur_location;
		vec2 pointing;
		vec2 move;
		bool thruster_on;
		bool reverse;
		bool left;
		bool right;
		float angle;
		bool hasDrag;
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
	void round(vec2 &vector) {
		if (vector.x<0) {
			vector.x=ceilf(vector.x);
		}
		else {
			vector.x=floorf(vector.x);
		}
		if (vector.y<0) {
			vector.y=ceilf(vector.y);
		}
		else {
			vector.y=floorf(vector.y);
		}
	};

public:


	Ship();
	float integrity;
	int level;
	int experience;
	inline void start_thruster(){ state.thruster_on= true;}
	inline void stop_thruster() { state.thruster_on= false;}
	inline void start_reverse() { state.reverse= true;}
	inline void stop_reverse() { state.reverse= false;}
	inline void start_right() {state.right=true;}
	inline void stop_right() {state.right=false;}
	inline void start_left() {state.left=true;}
	inline void stop_left() {state.left=false;}

	inline void rotateLeft() {  state.angle-=_ROT;   state.pointing =  RotateZ2D(state.angle) * vec2(0.0,-1.0);}
	inline void rotateRight(){  state.angle+=_ROT;   state.pointing =  RotateZ2D(state.angle) * vec2(0.0,-1.0);}

	void update_state();

	void gl_init();  

	void draw(mat4 proj) {
		glUseProgram( GLvars.program );
		glBindVertexArray( GLvars.vao );

		mat4 m = Translate(state.cur_location.x, state.cur_location.y,0.0) * RotateZ(state.angle);
		state.M=m;
		//!!!!!!!!Create a modelview matrix and pass it
		glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj * m);

		//!!!!!!!!Draw something
		if (GameSettings.dMode==_OUTLINE) {
			glDrawArrays(GL_LINE_LOOP,0,27);
		}
		else if (GameSettings.dMode==_FILL) {
			glDrawArrays(GL_TRIANGLE_FAN,0,27);
			glDrawArrays(GL_TRIANGLE_FAN,36,9);
		}

		if(state.thruster_on){
			if ((!state.left&&!state.right) || (state.left&&state.right)) {
				glDrawArrays(GL_TRIANGLES,27,9);
			}
			else if (state.left) {
				glDrawArrays(GL_TRIANGLES,27,3);
				glDrawArrays(GL_TRIANGLES,33,3);
			}
			else if (state.right) {
				glDrawArrays(GL_TRIANGLES,30,3);
				glDrawArrays(GL_TRIANGLES,33,3);
			}
		}
		else {
			if ((state.left&&state.right)) {
				glDrawArrays(GL_TRIANGLES,27,6);
			}
			else if (state.left) {
				glDrawArrays(GL_TRIANGLES,27,3);
			}
			else if (state.right) {
				glDrawArrays(GL_TRIANGLES,30,3);
			}
		}

		m = Translate(state.cur_location.x, state.cur_location.y,0.0) * Scale(this->integrity/100,1,1);
		glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj * m);
		glDrawArrays(GL_TRIANGLE_FAN,45,4);

		glBindVertexArray(0);
		glUseProgram(0);
	};

	void set_extents(int w, int h){
		width  = w/2;
		height = h/2;
	};
	vec2 get_extents() {
		return vec2(width,height);
	};

	Bullet fire();

	void toggleDrag() {
		state.hasDrag=!state.hasDrag;
	};
	std::vector<vec2> getPoints() {
		std::vector<vec2> points = std::vector<vec2>(0);
		if (GameSettings.bMode==BoundsMode::_BOUNDINGBOX) {
			vec2 topLeftCorner = vec2(state.cur_location.x-40,state.cur_location.y-40);
			vec2 topRightCorner = vec2(state.cur_location.x+40,state.cur_location.y-40);
			vec2 bottomRightCorner = vec2(state.cur_location.x+40,state.cur_location.y+40);
			vec2 bottomLeftCorner = vec2(state.cur_location.x-40,state.cur_location.y+40);
			points.push_back(topLeftCorner);
			points.push_back(topRightCorner);
			points.push_back(bottomLeftCorner);
			points.push_back(bottomRightCorner);
		}
		else if (GameSettings.bMode==BoundsMode::_FULL_OBJECT) {
			//ignore the flame and cockpit
			for (int i=0;i<27;i++) {
				vec2 point;
				point=ship_vert[i];
				vec4 p = vec4(point.x,point.y,1,1);
				p=state.M*(p);
				point=vec2(p.x,p.y);
				points.push_back(point);
			}
		}
		return points;
	};
};


#endif /* defined(__Asteroids__Ship__) */
