#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;
uniform float iTime;
uniform vec2 iResolution;

// Created by greenbird10
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0

float hash(vec2 p) {
	return 0.5*(
    sin(dot(p, vec2(271.319, 413.975)) + 1217.13*p.x*p.y)
    ) + 0.5;
}

float noise(vec2 p) {
  vec2 w = fract(p);
  w = w * w * (3.0 - 2.0*w);
  p = floor(p);
  return mix(
    mix(hash(p+vec2(0,0)), hash(p+vec2(1,0)), w.x),
    mix(hash(p+vec2(0,1)), hash(p+vec2(1,1)), w.x), w.y);
}

// wave octave inspiration
// Alexander Alekseev - Seascape
// https://www.shadertoy.com/view/Ms2SD1

//this is responsible for the wave pattern
float map_octave(vec2 uv) {
  uv = (uv + noise(uv)) / 2.1;
  uv = vec2(uv.x*0.6-uv.y*0.8, uv.x*0.8+uv.y*0.6);
  vec2 uvsin = 1.0 - abs(sin(uv));
  vec2 uvcos = abs(cos(uv));
  uv = mix(uvsin, uvcos, uvsin);
  float val = 1.0 - pow(uv.x * uv.y, 0.65);
  return val;
}

float map(vec3 p) {
  vec2 uv = p.xz + iTime/2.;
  float amp = 0.6, freq = 2.0, val = 0.0;
  for(int i = 0; i < 3; ++i) {
    val += map_octave(uv) * amp;
    amp *= 0.3;
    uv *= freq;
   // uv = vec2(uv.x*0.6-uv.y*0.8, uv.x*0.8+uv.y*0.6);
  }
  uv = p.xz - 1000. - iTime/2.;
  amp = 0.6, freq = 2.0;
  for(int i = 0; i < 3; ++i) {
    val += map_octave(uv) * amp;
    amp *= 0.3;
    uv *= freq;
   // uv = vec2(uv.x*0.6-uv.y*0.8, uv.x*0.8+uv.y*0.6);
  }
  return val + 3.0 - p.y;
}

vec3 getNormal(vec3 p) {
  float eps = 1./iResolution.x;
  vec3 px = p + vec3(eps, 0, 0);
  vec3 pz = p + vec3(0, 0, eps);
  return normalize(vec3(map(px),eps,map(pz)));
}

// raymarch inspiration
// Alexander Alekseev - Seascape
// https://www.shadertoy.com/view/Ms2SD1
float raymarch(vec3 ro, vec3 rd, out vec3 outP, out float outT) {
    float l = 0., r = 24.;
    int i = 0, steps = 26;
    float dist = 100000.;
    for(i = 0; i < steps; ++i) {
        float mid = (r+l)/2.;
        float mapmid = map(ro + rd*mid);
        dist = min(dist, abs(mapmid));
        if(mapmid > 0.) {
        	l = mid;
        }
        else {
        	r = mid;
        }
        if(r - l < 1./iResolution.x) break;
    }
    outP = ro + rd*l;
    outT = l;
    return dist;
}

float fbm(vec2 n) {
	float total = 0.0, amplitude = 1.3;
	for (int i = 0; i < 5; i++) {
		total += noise(n) * amplitude; 
		n += n;
		amplitude *= 0.4; 
	}
	return total;
}

float lightShafts(vec2 st) {
    float angle = -0.2;
    vec2 _st = st;
    float t = iTime / 16.;
    st = vec2(st.x * cos(angle) - st.y * sin(angle), 
              st.x * sin(angle) + st.y * cos(angle));
    float val = fbm(vec2(st.x*2. + 200. + t, st.y/4.));
    val += fbm(vec2(st.x*2. + 40. - t, st.y/4.));
    val = val / 3.;
    float mask = pow(clamp(1.0 - abs(_st.y-0.15), 0., 1.)*0.49 + 0.5, 2.0);
    mask *= clamp(1.0 - abs(_st.x+0.2), 0., 1.) * 0.49 + 0.5;
	return pow(val*mask, 2.0);
}

vec2 bubble(vec2 uv, float scale) {
    if(uv.y > 0.2) return vec2(0.);
    float t = iTime/4.;
    vec2 st = uv * scale;
    vec2 _st = floor(st);
    vec2 bias = vec2(0., 4. * sin(_st.x*128. + t));
    float mask = smoothstep(0.1, 0.2, -cos(_st.x*128. + t));
    st += bias;
    vec2 _st_ = floor(st);
    st = fract(st);
    float size = noise(_st_)*0.07+0.01;
    vec2 pos = vec2(noise(vec2(t, _st_.y*64.1)) * 0.8 + 0.1, 0.5);
    if(length(st.xy - pos) < size) {
       
        //return (st + pos) * vec2(.1, .2) * mask;
        return (st + pos) * vec2(0.1, 0.2) * mask;
    }
    return vec2(0.);
}

void main() {
    vec3 ro = vec3(0.0,0.0,2.0);
    vec3 lightPos = vec3(8, 3, -3);
    vec3 lightDir = normalize(lightPos - ro);

    // adjust uv
    vec2 uv = texCoord;
    uv = (-iResolution.xy + 2.0*uv) / iResolution.y;
    uv.y *= 0.5;
    uv.x *= 0.45;
    uv += bubble(uv, 6.0) + bubble(uv, 12.0); // add bubbles

    vec3 rd = normalize(vec3(uv, -1.));
    vec3 hitPos;
    float hitT;
    vec3 seaColor = vec3(11,82,142)/255.;
    vec3 color;
    
    // waves
    float dist = raymarch(ro, rd, hitPos, hitT);
    float diffuse = dot(getNormal(hitPos), rd) * 0.5 + 0.5;
    color = mix(seaColor, vec3(20,150,152)/255., diffuse);
    color += pow(diffuse, 6.0);
	// refraction
    vec3 ref = normalize(refract(hitPos-lightPos, getNormal(hitPos), 0.05));
    float refraction = clamp(dot(ref, rd), 0., 1.0);
    color += vec3(20,130,120)/255. * 0.6 * pow(refraction, 1.5);

    vec3 col = vec3(0.);
    col = mix(color, seaColor, pow(clamp(0., 1., dist), 0.2)); // glow edge
    col += vec3(130,100,200)/255. * lightShafts(uv); // light shafts

    // tone map
    col = (col*col + sin(col))/vec3(1.4, 1.8, 1.9);
    
    // vignette
    // inigo quilez - Stop Motion Fox 
    // https://www.shadertoy.com/view/3dXGWB
    vec2 q = texCoord / iResolution.xy;
    col *= 0.7+0.4*pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.2);

float amplitude = 0.03;        // Higher waves
float angularVelocity = 15.0;  // More waves
float frequency = 1.0;         // Slower movement
float offset = 0.45;
  
  float y = amplitude * sin((angularVelocity * uv.x) + (frequency * iTime)) + offset;
  
  vec4 topColor = vec4(0.0, 0.0, 0.0, 0.0);    
  vec4 bottomColor = vec4(col,1.0);  //actual water
  
  fragColor = uv.y > y ? topColor : bottomColor;
}