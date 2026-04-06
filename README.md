# Sisyphus-Table
Input SVG/PDF file that a Sisyphus table will draw out.

# Description
- Converts SVG file into XY coordinates using svgelements python lib
- r = sqrt(x^2 + y^2)
- theta = arctan2(y, x) ranges -pi/2 to pi/2

# Components
- ESP32S
- 2 NEMA17 Stepper Motor
# Limitations and Drawbacks
- Texts in svgs are a little more complex to break down into paths, they require font rendering, the work around to this is using Inkscape Object to Path
# Errors Faced
- <img width="597" height="428" alt="image" src="https://github.com/user-attachments/assets/858ea21f-223c-4a8e-8d11-70945c5f21eb" />
- Standard svg considers top left as the origin. This is terrible for polar which is relative to origin
- Inkscape creates objects in pixels, but when we make an svg from inkscape it prints the pixels into a mm canvas.
- Inkscape svg uses W3C standard: 96dpi, which is 96 px/inch multiplied by 1in/25.4 mm, 96/25.4 = 3.7795 px per mm
- I was running into multiple problems with a bounding box center debug not matching my document center and the fact that I was expecting theta_unwrapped to be -pi to pi for a circle at the origin but was getting random theta ranges that werent indicative of a circle centered at the origin.
- After parsing the group transform with
-     if isinstance(element, Group):
        t = element.transform 
        print("group transform:", t)
- <img width="809" height="57" alt="image" src="https://github.com/user-attachments/assets/fdf9a764-6cbc-4240-a952-9a5e41437982" />
- it confirmed that Inkscape was scaling their pixels to a mm canvas by exactly 3.77953
