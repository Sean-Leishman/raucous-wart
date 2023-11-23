import json
import math
import subprocess

NUM_SPHERES = 10
BACKGROUND_COLOR = [0.25, 0.25, 0.25]
def load_shapes(index, positions, frames):
    shapes = []
    for i in range(NUM_SPHERES):
        positions[i] = index % len(positions)
        el = frames[positions[i]]
        sphere = {
            "type": "sphere",
            "center": [el[0], el[2], el[1]],
            "radius": 0.3,
            "material":
                {
                    "ks": 0.1,
                    "kd": 0.9,
                    "specularexponent": 20,
                    "diffusecolor": [0.8, 0.5, 0.5],
                    "specularcolor": [1.0, 0, 0],
                    "isreflective": True,
                    "reflectivity": 0.9,
                    "isrefractive": True,
                    "refractiveindex": 1.5
                }
        }
        shapes.append(sphere)
    return shapes

def load_camera(index, positions, rotations):
    camera = {
        "type":"pinhole",
        "width":1200,
        "height":800,
        "position":positions,
        "lookAt":rotations,
        "upVector": [0, 0, 1],
        "fov":45.0,
        "exposure":0.1
    }
    return camera

def load_light():
    positions = [
        light_positions[light_index[0]],
        light_positions[light_index[1]]
    ]

    positions[0][1] += 5
    positions[1][1] -= 5

    lights = [
        {
            "type": "arealight",
            "position": positions[0],
            "intensity": [30, 30, 30]
        },
        {
            "type": "arealight",
            "position": positions[1],
            "intensity": [30,30,30]
        },
    ]
    return lights

def lemniscate_position(a, t):
    x = a * math.cos(t)
    y = a * math.sin(t) * math.cos(t)
    z = a * math.sin(t)
    return x, y, z

def falling_position(initial_height, gravity, frame):
    # Simple physics formula: s = ut + 1/2 at^2
    # Assuming initial vertical velocity (u) is 0
    return initial_height - 0.5 * gravity * (frame ** 2)

def generate_positions(num_frames, a, initial_height):
    positions = []
    for frame in range(num_frames):
        t = -math.pi + 2 * math.pi * frame / num_frames
        x, y, z = lemniscate_position(a, t)
        positions.append((x, y, z))
    return positions


def camera_position(radius, frame, num_frames):
    angle = 2 * math.pi * frame / num_frames
    x = radius * math.cos(angle)
    y = radius * math.sin(angle)

    pos = [x, 0.5, y]
    return pos , [0 - pos[0], 0 - pos[1], 0 - pos[2]]



# Parameters
num_frames = 100
a = 5  # Scale of the lemniscate
initial_height = 50  # Initial height of the object

# Generate positions
figure_8 = generate_positions(num_frames, a, initial_height)

# Initial Position: Index of figure_8 is current position of index of array
positions = [i for i in range(len(figure_8)) if i % NUM_SPHERES == 0]

camera_positions = [-1.0, 0.5, -1.5]
camera_rotations = [0 - i for i in [-1, 0.5, -1.5]]

light_positions = [[x * 0.1, 0, 1] for x in range(-50, 50)]
light_index = [0, 50]
for frame in range(num_frames):
    curr_frame = {}
    curr_frame['nbounces'] = 6
    curr_frame['samples'] = 5000
    curr_frame['phong'] = 'pathtracer'
    position, look_at = camera_position(5, frame, 100)
    curr_frame['camera'] = load_camera(frame, position, look_at)

    curr_frame['scene'] = {}
    curr_frame['scene']['background_color'] = BACKGROUND_COLOR
    curr_frame['scene']['lightsources'] = load_light()

    curr_frame['scene']['shapes'] = load_shapes(frame, positions, figure_8)

    filename = f'./materials/Animation/Scene/frame_{frame}.json'
    with open(filename, "w") as f:
        json.dump(curr_frame, f)

    out = f'./materials/Animation/Images/frame_{frame}.json'
    result = subprocess.run(["/cmake-build-release/main", filename, out])


