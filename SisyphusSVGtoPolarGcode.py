import math
import numpy as np
from svgelements import SVG, Shape, Path, Group
#inkscape creates everything in pixel but scales it to mm when we save it as svg, so have to convert back to px.
scale = 3.7795275591 #pixels per mm, 96 dpi, 25.4 mm per inch, so 96/25.4 = 3.7795 pixels per mm

svg = SVG.parse('drawing.svg') #parses the svg 
documentSize = svg.viewbox #gets the size of the svg canvas in MILLIMETERS

        
count = 0
segment_res = 20 #number of points per segment, higher is smoother but more points to process
xypoints = []
r_array = [] #radius
theta_array = [] #raw radians
theta_unwrapped =[] #unwrapped radians, ranges 0 to 2pi, perfect for stepper motor, which is steps per 2pi aka revolution

#viewbox gives us MM ONLY, so we have to convert it back into pixels because svgelement uses PIXELS ONLY
cx = (documentSize.x + documentSize.width/2)*scale #center x of the canvas and then scaled from mm to pixels
cy = (documentSize.y + documentSize.height/2)*scale #center y of the canvas and then scaled from mm to pixels

canvas_rad = min(documentSize.width, documentSize.height)/2*scale #radius of the canvas, scaled from mm to pixels
#uses min of both width and height to get the smallest inscribed circle, and then divide by 2 to get radius

for element in svg.elements(): #goes through everything in the svg, all paths, circles, lines
    if isinstance(element, Shape): #filters only drawable shapes, ignores metadata, groups, etc
        count += 1
        path = Path(element) #no matter what the shape was, convert it to a path, so we can sample points along it. 
        # **returns pixels ONLY

        for segment in path: #a path is made of segments like lines, arcs, beziers
            for i in range(segment_res + 1): #samples points along the segment + endpoint, higher res = smoother but slower processing  
                t = i / segment_res #position along segment, ranges 0 to 1
                p = segment.point(t) #gets x y coords of point at position t along segment
                x = p.x - cx #translates points to be centered around document origin instead of top left
                y = p.y - cy 
                xypoints.append((x, y))
                


for x, y in xypoints:
    r = math.sqrt(x**2 + y**2) #hypotenuse of a triangle
    normalized_r = r/canvas_rad # 0 to 1, ratio between radius of path to radius of canvas
    r_array.append(normalized_r) #in pixels right now
    #for nema17, steps required = normalized r * steps for full travel
    #steps for full travel is something we have to measure
    
    theta = math.atan2(y, x) #arctan of y/x is -pi/2 to pi/2 but atan2 doubles it to -pi to pi
    theta_array.append(theta) #theta is in radians, -pi to pi
    theta_unwrapped = np.unwrap(theta_array) #unwraps the angles to account for -pi to pi discontinuity
    #unwrapping checks between prev theta and theta and if its greater than pi, it adds 2pi to current theta to correct
    print (f"{r_array[-1]:.2f}, {theta_array[-1]:.2f}, {theta_unwrapped[-1]:.2f}")
print("cx:", cx, "cy:", cy) #raw center of canvas in mm
print("Total shapes:", count) 

print("circle center from bounds:", 
      (min(p[0] for p in xypoints) + max(p[0] for p in xypoints)) / 2,
      (min(p[1] for p in xypoints) + max(p[1] for p in xypoints)) / 2)
print(f"min(theta_array): {min(theta_array):.2f}, max(theta_array): {max(theta_array):.2f}")
print(f"min(theta_unwrapped): {min(theta_unwrapped):.2f}, max(theta_unwrapped): {max(theta_unwrapped):.2f}")
