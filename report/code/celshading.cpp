vec3 celShading(vec3 rgbCol) { 
     vec3 hsv = rgbtohsv(rgbCol);
     hsv.z = (round(rgbCol.z * 3) / 3.0);
     return hsv2rgb(hsv);
}