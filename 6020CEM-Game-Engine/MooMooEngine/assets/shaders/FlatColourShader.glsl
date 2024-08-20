#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;

uniform sampler2D colourID;
uniform int hasTexture;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform vec4 u_LightColour; 
uniform vec4 u_ObjectColour;

void main()
{
    // Ambient 
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * vec3(u_LightColour);

    // Diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(u_LightColour);

    // Specular 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(u_LightColour);


    // Adds to get the results
    vec3 result = (ambient + diffuse + specular) * vec3(u_ObjectColour);
    vec4 texColour;
    texColour = texture(colourID, v_TexCoord);
    if (hasTexture == 1) {
        result = result * vec3(texColour);
    }
    color = vec4(result, u_ObjectColour.a); //??? this works :shrug:

}