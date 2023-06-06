#version 150

// Define the value of PI
#define PI 3.1415926538

// Define a scaling factor
#define FACTOR 1.5

// Define a constant reflection factor
const float reflection_factor = 1;

// Define a constant alpha value
const int alpha = 30;

// Uniform declarations
uniform float AmbientIntensity; // Define a uniform variable for ambient intensity
uniform float LightIntensity;   // Define a uniform variable for light intensity
uniform vec3 CameraPosition;    // Define a uniform variable for the camera position
uniform vec3 PlanetColor;       // Define a uniform variable for the planet color
uniform vec3 AmbientColor;      // Define a uniform variable for the ambient color
uniform vec3 LightColor;        // Define a uniform variable for the light color
uniform vec3 LightPosition;     // Define a uniform variable for the light position
uniform bool Cel; 		  // Define a uniform variable for the Cel shading flag

// Input variables
in vec3 Position;
in vec3 pass_Normal;

// Output variable
out vec4 out_Color;

void main() {

  // Calculate the direction and distance from the light source to the current position
  vec3 light_direction = LightPosition - Position;
  float distance = length(light_direction);

  // Calculate the contribution of the light source using the light color, intensity, and distance
  vec3 beta = (LightColor * LightIntensity) / (4 * PI * distance * distance);

  // Calculate the contribution of ambient light using the ambient color and intensity
  vec3 ambient = AmbientIntensity * AmbientColor;

  // Calculate the strength of the diffuse reflection using the light direction and surface normal
  float diffuse_strength = max(dot(normalize(light_direction), normalize(pass_Normal)), 0.0);

  // Calculate the diffuse reflection using the planet color and the diffuse strength
  vec3 diffuse = PlanetColor * diffuse_strength;

  // Calculate the direction from the current position to the camera
  vec3 view_direction = normalize(CameraPosition - Position);

  // Calculate the halfway vector between the light direction and the view direction
  vec3 l = normalize(light_direction);
  vec3 v = normalize(view_direction);
  vec3 h = (l + v) / length(l + v);

  // Calculate the strength of the specular reflection using the surface normal and halfway vector
  float specular_strength = pow(max(dot(h, normalize(pass_Normal)), 0.0), 4 * alpha);

  // Calculate the specular reflection using the reflection factor, ambient color, and specular strength
  vec3 specular = reflection_factor * AmbientColor * specular_strength;

  // Apply Cel shading if enabled
  if (Cel) {
    // Calculate the angle between the surface normal and view direction
    float angle = dot(normalize(pass_Normal), normalize(view_direction));

    // Check if the angle is within the desired range for cel shading
    if ((angle <= 0.3f) && (angle >= 0.0f)) {
      // If angle is within the range, output the planet color without shading
      out_Color = vec4(PlanetColor, 1.0f);
      return;
    } else {
      // If angle is outside the range, apply ceil shading to the diffuse and specular components
      diffuse = ceil(diffuse * FACTOR) / FACTOR;
      specular = ceil(specular * FACTOR) / FACTOR;
    }
  }

  // Combine the ambient, diffuse, and specular components to calculate the final color
  out_Color = vec4(ambient + beta * (diffuse + specular), 1.0);
}