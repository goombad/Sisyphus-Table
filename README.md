# Sisyphus-Table
Input SVG/PDF file that a Sisyphus table will draw out.

# Description
- Converts SVG file into XY coordinates using svgelements python lib
- r = sqrt(x^2 + y^2)
- theta = arctan2(y, x) ranges -pi/2 to pi/2

# Components
- ESP32S
- 2 NEMA17 Stepper Motor

# Errors Faced
- <img width="597" height="428" alt="image" src="https://github.com/user-attachments/assets/858ea21f-223c-4a8e-8d11-70945c5f21eb" />
- Standard svg considers top left as the origin. This is terrible for polar which is relative to origin
- A circle I thought was centered in the page was outputting theta the same as theta unwrapped although I'm expecting theta to have jumps but it was the same.
- 
