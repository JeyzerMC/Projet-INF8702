float vpDist = length(texCoords - 0.5);
if (vpDist > 0.45) {
   color *= 1 - remap(vpDist, 
                      0.45, sqrt(0.5),
                      0, 0.7);
}