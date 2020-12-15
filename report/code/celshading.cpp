vec3 celShading(vec3 rgbCol) { 
     vec3 hsvCol = rgbtohsv(rgbCol)
     hsvCol.z = (round(rgbCol.z * 3) / 3);
     return hsv2rgb(hsvCol);
}