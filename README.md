
## Hand Writing Detection with Depth map

### :paperclip: **Use Opencv and Stereo Camera**

### :paperclip: **Preprocedure**
 - Calibrate the cameras :
    - To get an intrinsic and extrinsic matrix of camera.

 - Images' Retrification
    - To make the image on the epiplor plane.

 - Calculate the Depth Map of two import images

 - Using `WLS filter` to make good quality stereo map

 ### :paperclip: **Parzen Window Density Estimation**
 - Using `PWDS` for get the skin part in stereo image.
    - Depends on the brightness of the skin part in stereo image.
    - And use PWDS again to remove the noising background in the region of interest by gray image.

### :paperclip: **Particle Swarm Optimization, PSO**
 - Using this algorithm to track region of interest setting in the beginning.
