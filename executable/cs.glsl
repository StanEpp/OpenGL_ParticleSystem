#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct particle{
  vec4  currPos;
  vec4  prevPos;
};

layout(std430, binding=0) buffer particles{
  particle p[];
};

uniform vec4 attPos;
uniform float frameTimeDiff;
uniform uint maxParticles;

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main(){
  uint gid = gl_GlobalInvocationID.x;
  
  if(gid <= maxParticles){
    particle part = p[gid];
    vec4 a, tempCurrPos;
    
    if(attPos.w < 0.0f){
      // The direction of this vector corresponds to the direction of the gravity.
      // A zero vector will freeze the particles when not interacted with them.
      a = vec4(0,-0.125,0,0);
    } else {
      a = attPos - part.currPos - vec4(0,-1,0,0);
      a = normalize(a) * 5 * length(part.currPos.xyz);
    }
    
    tempCurrPos  = 1.99 * part.currPos - 0.99 * part.prevPos + a * frameTimeDiff * frameTimeDiff;
    part.prevPos = part.currPos;
    part.currPos = tempCurrPos;
    
    p[gid] = part;
  }
}