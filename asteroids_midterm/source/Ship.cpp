//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//  Edited by Michael Furst 10/2017
//

#include "common.h"

Ship::Ship(){
	//!!!!!!!!Your initial state might be different depending on how you
	//pick your world coordinates
	state.cur_location = vec2(0.0,0.0);
	state.pointing = vec2(0.0,-1.0);
	state.move = vec2(0.0,0.0);
	state.thruster_on = false;
	state.angle = 0.0;
	state.hasDrag=false;
	state.reverse=false;
	state.left=false;
	state.right=false;
	integrity=100;
	level=1;
	experience=0;
};


void Ship::update_state(){
	if (experience>=(pow(10,level)+(10*level))) {
		experience-=(pow(10,level)+(10*level));
		level++;
	}
	if (state.left) {
		rotateLeft();
	}
	if (state.right) {
		rotateRight();
	}
	if(state.thruster_on && !state.reverse){
		//!!!!!!!!Accelerate if the thruster is on
		state.move+=_ACC*state.pointing;
		//!!!!!!!!Clamp acceleration at some maximum value
		if (length(state.move)>_MAX_SPEED) {
			state.move=normalize(state.move);
			state.move*=_MAX_SPEED;
		}
	}
	if (state.reverse) {
		state.move*=_DAMPING;
		round(state.move);		
	}
	
	//!!!!!!!!Dampen the velocity at every timestep to lessen intertia
	if (state.hasDrag) state.move*=_DAMPING;
	//!!!!!!!!Move the ship location
	state.cur_location+=state.move;

	//Wrap the ship position when at the boundary
	//!!!!!!!!This will change depending on the world coordinates you are using
	if(state.cur_location.x < -width || state.cur_location.x > width){
		state.cur_location.x = -state.cur_location.x + (10 * (state.cur_location.x/abs(state.cur_location.x)));
	}
	if(state.cur_location.y < -height ||state.cur_location.y > height){
		state.cur_location.y = -state.cur_location.y + (10 * (state.cur_location.y/abs(state.cur_location.y)));
	}
	this->gl_init();
}


void Ship::gl_init(){
	//Ship
	//!!!!!!!!Populate ship_vert and ship_color
	//ship
	ship_vert[0] = vec2(0,-40);
	ship_vert[1] = vec2(4,-36);
	ship_vert[2] = vec2(8,-29);
	ship_vert[3] = vec2(12,-21);
	ship_vert[4] = vec2(16,-6);
	ship_vert[5] = vec2(16,8);
	ship_vert[6] = vec2(12,12);
	ship_vert[7] = vec2(22,33);
	ship_vert[8] = vec2(21,39);
	ship_vert[9] = vec2(10,39);
	ship_vert[10] = vec2(10,34);
	ship_vert[11] = vec2(10,24);
	ship_vert[12] = vec2(10,34);
	ship_vert[13] = vec2(6,39);
	ship_vert[14] = vec2(-6,39);
	ship_vert[15] = vec2(-10,34);
	ship_vert[16] = vec2(-10,24);
	ship_vert[17] = vec2(-10,34);
	ship_vert[18] = vec2(-10,39);
	ship_vert[19] = vec2(-21,39);
	ship_vert[20] = vec2(-22,33);
	ship_vert[21] = vec2(-12,12);
	ship_vert[22] = vec2(-16,8);
	ship_vert[23] = vec2(-16,-6);
	ship_vert[24] = vec2(-12,-21);
	ship_vert[25] = vec2(-8,-29);
	ship_vert[26] = vec2(-4,-36);
	//flame 1
	ship_vert[27] = vec2(11,40); 
	ship_vert[28] = vec2(20,40);
	ship_vert[29] = vec2(4,50);
	//flame 2
	ship_vert[30] = vec2(-11,40);
	ship_vert[31] = vec2(-20,40);
	ship_vert[32] = vec2(-4,50);
	//flame 3
	ship_vert[33] = vec2(5,40);
	ship_vert[34] = vec2(-5,40);
	ship_vert[35] = vec2(0,55);
	//cockpit
	ship_vert[36] = vec2(0,-30);
	ship_vert[37] = vec2(3,-27);
	ship_vert[38] = vec2(4,-15);
	ship_vert[39] = vec2(5,-2);
	ship_vert[39] = vec2(5,5);
	ship_vert[40] = vec2(-5,5);
	ship_vert[41] = vec2(-5,-2);
	ship_vert[42] = vec2(-4,-15);
	ship_vert[43] = vec2(-3,-27);
	ship_vert[44] = vec2(0,-30);
	//ship health
	ship_vert[45] = vec2(-20,45);
	ship_vert[46] = vec2(20,45);
	ship_vert[47] = vec2(20,43);
	ship_vert[48] = vec2(-20,43);

	//ship color
	for (int i=0;i<27;i++) {
		ship_color[i] = vec3(1.0, 1.0, 1.0);
	}
	//flame color
	//flame 1
	ship_color[27] = vec3(1.0,0.0,0.0);
	ship_color[28] = vec3(1.0,0.0,0.0);
	ship_color[29] = vec3(1.0,0.33,0.0);
	//flame 2
	ship_color[30] = vec3(1.0,0.0,0.0);
	ship_color[31] = vec3(1.0,0.0,0.0);
	ship_color[32] = vec3(1.0,0.33,0.0);
	//flame 3
	ship_color[33] = vec3(1.0,0.0,0.0);
	ship_color[34] = vec3(1.0,0.0,0.0);
	ship_color[35] = vec3(1.0,0.33,0.0);

	for (int i=36;i<45;i++) {
		ship_color[i]=vec3(0.0,0.3,1.0);
	}
	ship_color[45] = vec3(1.0,0.0,0.0);
	ship_color[46] = vec3(0.0,0.0,1.0);
	ship_color[47] = vec3(0.0,0.0,1.0);
	ship_color[48] = vec3(1.0,0.0,0.0);
	
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
	glBufferData( GL_ARRAY_BUFFER, sizeof(ship_vert) + sizeof(ship_color), NULL, GL_STATIC_DRAW );
	//First part of array holds vertices
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ship_vert), ship_vert );
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(ship_vert), sizeof(ship_color), ship_color );

	glEnableVertexAttribArray(GLvars.vpos_location);
	glEnableVertexAttribArray(GLvars.vcolor_location );

	glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_vert)) );

	glBindVertexArray(0);

}


Bullet Ship::fire() {
	return Bullet(state.cur_location,state.pointing,state.angle,(width/1024*20));
};
