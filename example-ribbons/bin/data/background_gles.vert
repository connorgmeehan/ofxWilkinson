// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;

attribute vec4 position;     // Per-vertex position information we will pass in.
attribute vec2 texcoord;

varying vec2 texCoordVarying;

void main()
{
  texCoordVarying = texcoord; 
	gl_Position = modelViewProjectionMatrix * position;
}