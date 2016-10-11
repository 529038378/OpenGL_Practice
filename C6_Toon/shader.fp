
#version 130

uniform sampler2D textureSampler;
in vec2 uvCoor;
smooth in vec3 Normal;
smooth in vec3 LightDir;
smooth in vec3 pos;

out vec4 vFragColor;

void main()
{
    vec4 material = texture(textureSampler, uvCoor);
    vec4 Ambient = 0.2f*material;
    
    float diff = max(0.0f, dot(normalize(Normal), normalize(LightDir)) );
    vec4 Diffuse = diff*material;
    vFragColor = Ambient + Diffuse;
    
    vec3 Reflect = normalize( reflect(-normalize(Normal), normalize(LightDir)) );
    float fSpec = max(0, dot(normalize(Reflect), normalize(Normal) ));
    fSpec = pow(fSpec, 128);
    vec4 fSpecular = vec4(fSpec, fSpec, fSpec, 1.0f);
    if(diff != 0) vFragColor.rgb += fSpecular.rgb;

    //vFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
   
}
